// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpServer.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebApp.hpp"
#include "HttpWebApps/RoutingHttpWebApp.hpp"
#include "Impl/HttpWebAppFactory.hpp"
#include "Impl/Listener.hpp"
#include "WsWebApps/NullWsWebApp.hpp"
#include "../../Utilities/MimeTypes.hpp"
#include "../../../System/ThreadName.hpp"
#include "../../../System/Sleep.hpp"
#include "../../../Logging/Logger.hpp"

#include <boost/bind.hpp>

// For built in web app initialiser.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Network::Http {

// The built-in web applications are registered here.
struct HttpServerRegisterBuiltInWeApps {
	HttpServerRegisterBuiltInWeApps() {
		Impl::HttpWebAppFactory::registerHttpWebApp<HttpWebApps::EmailSendingHttpWebApp>("email.sender");
		Impl::HttpWebAppFactory::registerHttpWebApp<HttpWebApps::FileServingHttpWebApp>("files");
		Impl::HttpWebAppFactory::registerHttpWebApp<HttpWebApps::RedirectingHttpWebApp>("redirections");
	}
};

HttpServerRegisterBuiltInWeApps httpServerRegisterBuiltInWeApps;

//////////////////// Constructors with injector parameter /////////////////////

HttpServer::HttpServer(std::shared_ptr<System::Clock> clock,
                       const std::string & serverId,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       std::shared_ptr<HttpWebApp> httpHandler,
                       std::shared_ptr<WsWebApp> wsHandler,
                       const std::string & loggingNamespace,
                       std::string sessionCookieName,
                       std::shared_ptr<MimeTypes> mimeTypes,
                       bool registerSignalHandler)
	: state(
		std::make_shared<HttpServerConfiguration>(
			  std::move(clock)
			, BalauLogger(loggingNamespace)
			, serverId
			, endpoint
			, sessionCookieName
			, std::move(httpHandler)
			, std::move(wsHandler)
			, std::move(mimeTypes)
		)
	)
	, threadNamePrefix(std::move(threadNamePrefix_))
	, workerCount(workerCount_)
	, ioContext((int) workerCount)
	, signalSet(ioContext) {
	if (registerSignalHandler) {
		doRegisterSignalHandler();
	}
}

HttpServer::HttpServer(std::shared_ptr<System::Clock> clock,
                       const std::string & serverId,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       const Resource::File & documentRoot,
                       const std::string & defaultFile,
                       const std::string & loggingNamespace,
                       std::string sessionCookieName,
                       std::shared_ptr<MimeTypes> mimeTypes,
                       bool registerSignalHandler)
	: HttpServer(
		  clock
		, serverId
		, endpoint
		, std::move(threadNamePrefix_)
		, workerCount_
		, std::make_shared<HttpWebApps::FileServingHttpWebApp>(documentRoot, defaultFile)
		, std::shared_ptr<WsWebApp>(nullptr)
		, loggingNamespace
		, sessionCookieName
		, std::move(mimeTypes)
		, registerSignalHandler
	) {}

HttpServer::~HttpServer() {
	stop();
}

///////////////////////////////// Public API //////////////////////////////////

void HttpServer::start() {
	std::unique_lock<std::mutex> lock(mutex);

	if (!workers.empty()) {
		BalauBalauLogInfo(
			state->logger, "HTTP server {}:{} already started", state->endpoint.address(), state->endpoint.port()
		);
		return;
	}

	BalauBalauLogInfo(state->logger, "Starting HTTP server {}:{}", state->endpoint.address(), state->endpoint.port());

	startWorkerThreads(workerCount);

	BalauBalauLogInfo(
		  state->logger
		, "HTTP server {}:{} started with {} workers"
		, state->endpoint.address()
		, state->endpoint.port()
		, runningWorkerCount
	);
}

void HttpServer::run() {
	{
		std::unique_lock<std::mutex> lock(mutex);

		if (!workers.empty()) {
			BalauBalauLogWarn(
				state->logger, "HTTP server {}:{} already started", state->endpoint.address(), state->endpoint.port()
			);
			return;
		}

		BalauBalauLogInfo(
			state->logger, "Starting HTTP server {}:{}", state->endpoint.address(), state->endpoint.port()
		);

		startWorkerThreads(workerCount - 1);
	}

	// Last worker is on this thread.
	const std::string previousThreadName = System::ThreadName::getName();

	workerThreadFunction(workerCount - 1, true);

	stop(false);

	if (!previousThreadName.empty()) {
		System::ThreadName::setName(previousThreadName);
	}
}

bool HttpServer::isRunning() {
	std::lock_guard<std::mutex> lock(mutex);
	return !workers.empty();
}

void HttpServer::stop(bool warn) {
	std::lock_guard<std::mutex> lock(mutex);

	if (workers.empty()) {
		if (warn) {
			BalauBalauLogWarn(
				state->logger, "HTTP server {}:{} already stopped", state->endpoint.address(), state->endpoint.port()
			);
		}

		return;
	}

	BalauBalauLogInfo(state->logger, "Stopping HTTP server {}:{}", state->endpoint.address(), state->endpoint.port());

	ioContext.stop();

	while (!ioContext.stopped()) {
		System::Sleep::milliSleep(10);
	}

	for (size_t m = 0; m < workers.size(); m++) {
		workers[m].join();
	}

	workers.clear();

	BalauBalauLogInfo(state->logger, "HTTP server {}:{} stopped", state->endpoint.address(), state->endpoint.port());
}

std::shared_ptr<HttpServerConfiguration> HttpServer::createState(std::shared_ptr<System::Clock> clock,
                                                                 std::shared_ptr<EnvironmentProperties> configuration) {
	// Root logging configuration
	auto loggingNamespace = configuration->getValue<std::string>("logging.namespace", "http.server");
	auto accessLog = configuration->getValue<std::string>("access.log", "stream: stdout");
	auto errorLog = configuration->getValue<std::string>("error.log", "stream: stderr");

	// TODO configure logging namespace.. this requires the fine grained reconfiguration implementation to be created.
	BalauLogger logger(loggingNamespace);

	auto serverId = configuration->getValue<std::string>("server.id", "Balau server");
	auto endpoint = configuration->getValue<Network::Endpoint>("listen");
	auto sessionCookieName = configuration->getValue<std::string>("session-cookie-name", "session");
	auto mimeTypes = createMimeTypes(configuration, logger);
	std::shared_ptr<HttpWebApp> httpHandler = createHttpHandler(configuration, logger);
	std::shared_ptr<WsWebApp> wsHandler = createWsHandler(configuration, logger);

	return std::make_shared<HttpServerConfiguration>(
		clock, logger, serverId, endpoint, sessionCookieName, httpHandler, wsHandler, mimeTypes
	);
}

std::shared_ptr<MimeTypes> HttpServer::createMimeTypes(std::shared_ptr<EnvironmentProperties> configuration, BalauLogger & logger) {
	auto mimeTypesProperties = configuration->getCompositeOrNull("mime.types");

	if (!mimeTypesProperties) {
		return MimeTypes::defaultMimeTypes;
	}

	std::unordered_map<std::string, std::string> data;

	for (const auto & mimeTypesProperty : *mimeTypesProperties) {
		if (mimeTypesProperty.isValue<std::string>()) {
			auto mimeType = std::string(mimeTypesProperty.getName());
			auto extensions = mimeTypesProperty.getValue<std::string>();
			auto extensionList = Util::Strings::split(extensions, ",");
			auto mt = Util::Strings::trim(mimeType);

			for (auto extension : extensionList) {
				auto ext = Util::Strings::trim(extension);
				data.insert(std::make_pair(ext, mt));
			}
		} else {
			BalauBalauLogWarn(
				  logger
				, "Ignoring mime type property entry {} because the property is not of type string."
				, mimeTypesProperty.getName()
			);
		}
	}

	return std::make_shared<MimeTypes>(std::move(data));
}

std::shared_ptr<HttpWebApp> HttpServer::createHttpHandler(std::shared_ptr<EnvironmentProperties> configuration,
                                                          BalauLogger & logger) {
	auto webAppConfigurations = configuration->hasComposite("http")
		? configuration->getComposite("http")
		: std::make_shared<EnvironmentProperties>();

	HttpWebApps::RoutingHttpWebApp::Routing routing;

	for (const auto & webAppConfiguration : *webAppConfigurations) {
		if (webAppConfiguration.isComposite()) {
			const auto name = std::string(webAppConfiguration.getName());
			auto config = webAppConfiguration.getComposite();

			//
			// HTTP web application configurations without a location property are
			// unconfigured and thus are not instantiated. This also eliminates
			// default config brought in from the specs.
			//
			if (!config->hasValue<std::string>("location")) {
				continue;
			}

			auto location = config->getValue<std::string>("location");
			auto webApp = Impl::HttpWebAppFactory::getInstance(name, *config, logger);
			addToHttpRoutingTrie(routing, location, webApp);
		} else {
			BalauBalauLogWarn(
				  logger
				, "Ignoring simple property \"{}\" found in the http web application configuration."
				, webAppConfiguration.getName()
			);
		}
	}

	return std::shared_ptr<HttpWebApp>(new HttpWebApps::RoutingHttpWebApp(std::move(routing)));
}

std::shared_ptr<WsWebApp> HttpServer::createWsHandler(std::shared_ptr<EnvironmentProperties> configuration,
                                                      BalauLogger & logger) {
	auto webAppConfigurations = configuration->hasComposite("ws")
		? configuration->getComposite("ws")
		: std::make_shared<EnvironmentProperties>();

	// TODO complete implementation
	//WsWebApps::RoutingWsWebApp::Routing routing;

	if (webAppConfigurations) {
		for (const auto & webAppConfiguration : *webAppConfigurations) {
			if (webAppConfiguration.isComposite()) {
				const auto name = std::string(webAppConfiguration.getName());
				auto config = webAppConfiguration.getComposite();

				//
				// WebSocket web application configurations without a location property
				// are unconfigured and thus are not instantiated. This also eliminates
				// default config brought in from the specs.
				//
				if (!config->hasValue<std::string>("location")) {
					continue;
				}

				auto webApp = Impl::HttpWebAppFactory::getInstance(name, *config, logger);
			} else {
				BalauBalauLogWarn(
					  logger
					, "Ignoring simple property \"{}\" found in the WebSocket web application configuration."
					, webAppConfiguration.getName()
				);
			}
		}
	}

	// TODO complete implementation
	//return std::shared_ptr<WsWebApp>(new WsWebApps::RoutingWsWebApp(std::move(routing)));
	return std::shared_ptr<WsWebApp>(new WsWebApps::NullWsWebApp());
}

void HttpServer::addToHttpRoutingTrie(HttpWebApps::RoutingHttpWebApp::Routing & routing,
                                      const std::string & locationStr,
                                      std::shared_ptr<HttpWebApp> & webApp) {
	// Blank delimited list of locations.
	static std::regex delimiter("[ \t]+");

	auto locations = Util::Strings::splitAndTrim(locationStr, delimiter);

	for (auto & path : locations) {
		// Split the path or make it empty for the root path.
		auto components = path == "/" ? std::vector<std::string_view>() : Util::Strings::split(path, "/");

		HttpWebApps::RoutingHttpWebApp::Node & node = routing.findOrAdd(
			  components
			, [] (auto & lhs, auto & rhs) { return std::get<HttpWebApps::RoutingHttpWebApp::KeyIndex>(lhs) == rhs; }
			, [] (auto & component) {
				return HttpWebApps::RoutingHttpWebApp::Value(
					  component
					, std::shared_ptr<HttpWebApp>()
					, std::shared_ptr<HttpWebApp>()
					, std::shared_ptr<HttpWebApp>()
				);
			}
		);

		auto & handler = std::get<HttpWebApps::RoutingHttpWebApp::GetHandlerIndex>(node.value);

		// The pointer container should be empty, otherwise the http webapp config is invalid.
		if (handler) {
			ThrowBalauException(
				  Exception::NetworkException
				, ::toString("Duplicate HTTP web application handler found for location \"", path, "\".")
			);
		}

		std::get<HttpWebApps::RoutingHttpWebApp::GetHandlerIndex>(node.value) = webApp;
		std::get<HttpWebApps::RoutingHttpWebApp::HeadHandlerIndex>(node.value) = webApp;
		std::get<HttpWebApps::RoutingHttpWebApp::PostHandlerIndex>(node.value) = webApp;
	}
}

void HttpServer::startWorkerThreads(size_t thisWorkerCount) {
	listener = std::make_unique<Impl::Listener>(state, ioContext);

	if (!listener->isOpen()) {
		ThrowBalauException(
			  Exception::NetworkException
			, "Listener on " + ::toString(state->endpoint.address()) + ":"
			+ ::toString(state->endpoint.port()) + " did not initialise correctly."
		);
	}

	listener->doAccept();

	runningWorkerCount = 0;
	workers.reserve(thisWorkerCount);

	for (size_t workerIndex = 0; workerIndex < thisWorkerCount; workerIndex++) {
		workers.emplace_back([workerIndex, this] { workerThreadFunction(workerIndex, false); });
	}

	// TODO add thread creation timeout failure handling
	while (runningWorkerCount != thisWorkerCount) {
		System::Sleep::milliSleep(10);
	}
}

void HttpServer::workerThreadFunction(size_t workerIndex, bool blocking) {
	if (!threadNamePrefix.empty()) {
		System::ThreadName::setName(threadNamePrefix + "-" + ::toString(workerIndex));
	}

	BalauBalauLogInfo(
		  state->logger
		, "Starting HTTP server {}:{} worker {}"
		, state->endpoint.address()
		, state->endpoint.port()
		, workerIndex
	);

	++runningWorkerCount;

	// Log started message if this is the main thread blocking.
	if (blocking) {
		BalauBalauLogInfo(
			  state->logger
			, "HTTP server {}:{} started with {} workers"
			, state->endpoint.address()
			, state->endpoint.port()
			, runningWorkerCount
		);
	}

	while (true) {
		try {
			ioContext.run();
			break;
		} catch (const std::exception & e) {
			BalauBalauLogError(
				  state->logger
				, "Exception encountered in HTTP server {}:{} worker {}: {}. Restarting worker."
				, state->endpoint.address()
				, state->endpoint.port()
				, workerIndex
				, e.what()
			);

			ioContext.restart();
		} catch (...) {
			BalauBalauLogError(
				  state->logger
				, "Unknown exception encountered in HTTP server {}:{} worker {}. Restarting worker."
				, state->endpoint.address()
				, state->endpoint.port()
				, workerIndex
			);

			ioContext.restart();
		}
	}

	--runningWorkerCount;

	BalauBalauLogInfo(
		  state->logger
		, "Stopped HTTP server {}:{} worker {}"
		, state->endpoint.address()
		, state->endpoint.port()
		, workerIndex
	);
}

void HttpServer::doRegisterSignalHandler() {
	signalSet.add(SIGINT);
	signalSet.add(SIGTERM);

	#if defined(SIGQUIT)
		signalSet.add(SIGQUIT);
	#endif

	signalSet.async_wait(boost::bind(&HttpServer::handleSignal, this, _1, _2));
}

void HttpServer::handleSignal(const boost::system::error_code & error, int sig) {
	switch (sig) {
		case SIGINT: {
			write(STDERR_FILENO, "Shutting down HTTP server due to signal SIGINT.\n", 48);
			break;
		}

		case SIGTERM: {
			write(STDERR_FILENO, "Shutting down HTTP server due to signal SIGTERM.\n", 49);
			break;
		}

		#if defined(SIGQUIT)
		case SIGQUIT: {
			write(STDERR_FILENO, "Shutting down HTTP server due to signal SIGQUIT.\n", 49);
			break;
		}
		#endif

		default: {
			write(STDERR_FILENO, "Shutting down HTTP server due to unknown signal.\n", 49);
			break;
		}
	}

	listener->close();
}

} // namespace Balau::Network::Http

#pragma clang diagnostic pop
