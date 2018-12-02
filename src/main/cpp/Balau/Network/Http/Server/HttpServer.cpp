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
#include "Impl/Listener.hpp"
#include "HttpWebApps/FileServingHttpWebApp.hpp"
#include "../../Utilities/MimeTypes.hpp"
#include "../../../System/ThreadName.hpp"
#include "../../../System/Sleep.hpp"
#include "../../../Logging/Logger.hpp"

#include <boost/bind.hpp>

namespace Balau::Network::Http {

//////////////////// Constructors with injector parameter /////////////////////

HttpServer::HttpServer(std::weak_ptr<Injector> injector,
                       std::shared_ptr<System::Clock> clock,
                       const std::string & configurationUri)
	: state(
		std::make_shared<HttpServerConfiguration>(
			std::move(injector)
			, std::move(clock)
			, Logger::getLogger("")
			, ""
			, TCP::endpoint()
			, std::shared_ptr<HttpWebApp>()
			, std::shared_ptr<WsWebApp>()
			, MimeTypes::defaultMimeTypes
		)
	)
	, threadNamePrefix("")
	, workerCount(1)
	, ioContext(1)
	, signalSet(ioContext) {
	registerSignalHandler();
}


HttpServer::HttpServer(std::weak_ptr<Injector> injector,
                       std::shared_ptr<System::Clock> clock,
                       const std::string & serverIdentification,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       std::shared_ptr<HttpWebApp> httpHandler,
                       std::shared_ptr<WsWebApp> wsHandler,
                       const std::string & loggingNamespace,
                       std::shared_ptr<MimeTypes> mimeTypes)
	: state(
		std::make_shared<HttpServerConfiguration>(
			  std::move(injector)
			, std::move(clock)
			, Logger::getLogger(loggingNamespace)
			, serverIdentification
			, endpoint
			, std::move(httpHandler)
			, std::move(wsHandler)
			, std::move(mimeTypes)
		)
	)
	, threadNamePrefix(std::move(threadNamePrefix_))
	, workerCount(workerCount_)
	, ioContext((int) workerCount)
	, signalSet(ioContext) {
	registerSignalHandler();
}

HttpServer::HttpServer(std::weak_ptr<Injector> injector,
                       std::shared_ptr<System::Clock> clock,
                       const std::string & serverIdentification,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       std::shared_ptr<HttpWebApp> httpHandler,
                       const std::string & loggingNamespace,
                       std::shared_ptr<MimeTypes> mimeTypes)
	: HttpServer(
		  std::move(injector)
		, std::move(clock)
		, serverIdentification
		, endpoint
		, std::move(threadNamePrefix_)
		, workerCount_
		, std::move(httpHandler)
		, std::shared_ptr<WsWebApp>(nullptr)
		, loggingNamespace
		, std::move(mimeTypes)
	) {}

//////////////////////// Constructors without injector ////////////////////////

HttpServer::HttpServer(std::shared_ptr<System::Clock> clock,
                       const std::string & serverIdentification,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       std::shared_ptr<HttpWebApp> httpHandler,
                       std::shared_ptr<WsWebApp> wsHandler,
                       const std::string & loggingNamespace,
                       std::shared_ptr<MimeTypes> mimeTypes)
	: state(
		std::make_shared<HttpServerConfiguration>(
			  std::weak_ptr<Injector>()
			, std::move(clock)
			, Logger::getLogger(loggingNamespace)
			, serverIdentification
			, endpoint
			, std::move(httpHandler)
			, std::move(wsHandler)
			, std::move(mimeTypes)
		)
	)
	, threadNamePrefix(std::move(threadNamePrefix_))
	, workerCount(workerCount_)
	, ioContext((int) workerCount)
	, signalSet(ioContext) {
	registerSignalHandler();
}

HttpServer::HttpServer(std::shared_ptr<System::Clock> clock,
                       const std::string & serverIdentification,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       std::shared_ptr<HttpWebApp> httpHandler,
                       const std::string & loggingNamespace,
                       std::shared_ptr<MimeTypes> mimeTypes)
	: HttpServer(
		  clock
		, serverIdentification
		, endpoint
		, std::move(threadNamePrefix_)
		, workerCount_
		, std::move(httpHandler)
		, std::shared_ptr<WsWebApp>(nullptr)
		, loggingNamespace
		, std::move(mimeTypes)
	) {}

HttpServer::HttpServer(std::shared_ptr<System::Clock> clock,
                       const std::string & serverIdentification,
                       const TCP::endpoint & endpoint,
                       std::string threadNamePrefix_,
                       size_t workerCount_,
                       const Resource::File & documentRoot,
                       const std::string & defaultFile,
                       const std::string & loggingNamespace,
                       std::shared_ptr<MimeTypes> mimeTypes)
	: HttpServer(
		  clock
		, serverIdentification
		, endpoint
		, std::move(threadNamePrefix_)
		, workerCount_
		, std::make_shared<HttpWebApps::FileServingHttpWebApp>(documentRoot, defaultFile)
		, std::shared_ptr<WsWebApp>(nullptr)
		, loggingNamespace
		, std::move(mimeTypes)
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
			BalauBalauLogInfo(
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
	workerThreadFunction(workerCount - 1, true);

	stop(false);
}

bool HttpServer::isRunning() {
	std::lock_guard<std::mutex> lock(mutex);
	return !workers.empty();
}

void HttpServer::blockUntilStopped(size_t period) {
	while (isRunning()) {
		System::Sleep::milliSleep(period);
	}
}

void HttpServer::stop(bool warn) {
	std::lock_guard<std::mutex> lock(mutex);

	if (workers.empty()) {
		if (warn) {
			BalauBalauLogInfo(
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
	System::ThreadName::setName(threadNamePrefix + "-" + ::toString(workerIndex));

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

void HttpServer::registerSignalHandler() {
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
