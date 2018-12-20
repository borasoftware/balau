// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2018 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__HTTP_WEB_APP_FACTORY
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__HTTP_WEB_APP_FACTORY

#include <Balau/Application/EnvironmentProperties.hpp>

#include <mutex>

namespace Balau::Network::Http {

class HttpServer;
class HttpWebApp;

namespace Impl {

//
// HTTP web applications are instantiated by this class for the HTTP server.
//
// All HTTP web applications need to be registered via the HttpsServer::registerHttpWebApp
// method (which calls HttpWebAppFactory::registerHttpWebApp) before the application injector
// is created. This is done automatically for Balau HTTP web applications.
//
class HttpWebAppFactory final {
	//
	// Register an HTTP web application.
	//
	// The name is the name of the composite property that provides the web app's configuration.
	//
	public: template <typename WebAppT> static void registerHttpWebApp(const std::string & name) {
		auto & map = getFactoryMap();
		auto & mutex = getFactoryMutex();

		std::lock_guard<std::mutex> lock(mutex);
		map[name] = [] (const EnvironmentProperties & configuration, const BalauLogger & logger) {
			return std::shared_ptr<HttpWebApp>(new WebAppT(configuration, logger));
		};
	}

	friend class ::Balau::Network::Http::HttpServer;

	//
	// Get a new instance of the HTTP web application with the specified name.
	//
	// The web application created with the supplied configuration.
	//
	private: static std::shared_ptr<HttpWebApp> getInstance(const std::string & name,
	                                                        const EnvironmentProperties & configuration,
	                                                        const BalauLogger & logger) {
		auto & map = getFactoryMap();
		auto & mutex = getFactoryMutex();

		std::lock_guard<std::mutex> lock(mutex);
		auto iter = map.find(name);

		if (iter == map.end()) {
			ThrowBalauException(
				Exception::NetworkException, "No HTTP web application with name \"" + name + "\" has been registered."
			);
		}

		return iter->second(configuration, logger);
	}

	private: HttpWebAppFactory() = delete;
	private: HttpWebAppFactory(const HttpWebAppFactory &) = delete;
	private: HttpWebAppFactory & operator = (const HttpWebAppFactory &) = delete;

	private: using Function = std::function<std::shared_ptr<HttpWebApp> (const EnvironmentProperties &, const BalauLogger & logger)>;

	private: static std::map<std::string, Function> & getFactoryMap() {
		static std::map<std::string, Function> factories;
		return factories;
	}

	private: static std::mutex & getFactoryMutex() {
		static std::mutex mutex;
		return mutex;
	}
};

} // namespace Impl

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__HTTP_WEB_APP_FACTORY
