// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file HttpServer.hpp
///
/// An asynchronous HTTP/WebSocket server.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER

#include <Balau/Network/Http/Server/HttpServerConfiguration.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/WsWebApps/RoutingWsWebApp.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/System/Clock.hpp>
#include <Balau/Application/Injectable.hpp>

#include <boost/asio/signal_set.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

// Avoid false positive (due to make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau {

class Logger;

namespace Network::Http {

namespace Impl {

class Listener;

}

///
/// An asynchronous HTTP/WebSocket server.
///
/// HTTP and WebSocket messages are handled by the web application handlers
/// supplied to the server's constructor. Complex handler infrastructure may be
/// created by using HTTP and WebSocket routing web application instances,
/// either in code or via text based server configuration.
///
/// As WebSocket connections are initiated via an HTTP upgrade, the HttpServer
/// class also manages WebSocket connections via upgrades initiated from created
/// HttpSessions.
///
/// In order to use automatic thread naming, leave the thread name prefix empty.
/// In order to use one worker thread per CPU core, set the worker count to zero.
///
/// The HTTP server registers signal handlers via a Boost Asio signal set in order
/// to ensure a graceful shut down. If the application subsequently calls signal()
/// or sigaction(), these handlers will be overridden. In order to avoid this,
/// use Boost Asio signal sets for all signal handler registrations in the application.
///
/// The HTTP server and web application framework supports the use of injectors.
/// The server shared state that is supplied to the web application handlers contains
/// a shared pointer to an injector. If an injector is supplied to the constructor
/// of the HTTP server, the injector will be available to handlers.
///
/// Application developers are responsible for ensuring that a suitable injector
/// is supplied to the HTTP server instance when one or more of the handlers require
/// one. Otherwise, the application will be defective and will cause a segmentation
/// fault when the injector nullptr is accessed.
///
class HttpServer {
	BalauInjectNamedTypes(
		  HttpServer
		, std::shared_ptr<System::Clock>,         ""
		, std::shared_ptr<EnvironmentProperties>, "http.server"
		, bool,                                   "http.server.register.signal.handler"
	);

	////////////////// Constructors with injector parameter ///////////////////

	///
	/// Create an HTTP server configured with the HTTP server configuration pointed to by the URI.
	///
	/// The supplied injector will be used by the handlers during requests to
	/// obtain injected instances.
	///
	/// This constructor can be used within an injector configuration in order
	/// to bind an HTTP server that is configured via complex configuration.
	///
	/// By default, the HTTP server installs its own signal handler in order to provide graceful
	/// shutdown in SIGINT and SIGTERM. If this is not required, the registerSignalHandler
	/// parameter can be set to false. This is set by default to true in the Balau environment
	/// configuration specification.
	///
	/// @param injector the injector to use in the web apps
	/// @param clock the clock to be used in the server
	/// @param configuration the server configuration
	/// @param registerSignalHandler (default = true) set to false in order to prevent signal handler installation
	///
	public: HttpServer(std::shared_ptr<System::Clock> clock,
	                   const std::shared_ptr<EnvironmentProperties>& configuration,
	                   bool registerSignalHandler = true);

	///
	/// Create an HTTP server with HTTP and optional WebSocket handlers.
	///
	/// In addition to direct instantiation, this constructor can be used
	/// within an injector provider.
	///
	/// @param clock the clock to be used in the server
	/// @param serverIdentification the server string to return
	/// @param endpoint the network address and port to listen on
	/// @param threadNamePrefix_ the prefix of the worker thread names (used for logging)
	/// @param workerCount_ the number of worker threads to use
	/// @param httpHandler the HTTP request handler to use
	/// @param wsHandler the WebSocket message handler to use (default is no WebSocket handler)
	/// @param loggingNamespace the logging namespace to use (default = "balau.network.server")
	/// @param sessionCookieName the name of the cookie in which the session id is stored (default = "session")
	/// @param mimeTypes the mime type map to use
	/// @param registerSignalHandler (default = true) set to false in order to prevent signal handler installation
	///
	public: HttpServer(std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   std::shared_ptr<HttpWebApp> httpHandler,
	                   std::shared_ptr<WsWebApp> wsHandler = std::shared_ptr<WsWebApp>(nullptr),
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::string sessionCookieName = "session",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes,
	                   bool registerSignalHandler = true);

	///
	/// Create an HTTP server using the file serving HTTP handler.
	///
	/// The mime type map can be specified or left as the default mime type map.
	///
	/// @param clock the clock to be used in the server
	/// @param serverIdentification the server string to return
	/// @param endpoint the network address and port to listen on
	/// @param threadNamePrefix_ the prefix of the worker thread names (used for logging)
	/// @param workerCount_ the number of worker threads to use
	/// @param mimeTypes the mime type map to use
	/// @param documentRoot the root file system path for serving files
	/// @param loggingNamespace the logging namespace to use (default = "balau.network.server")
	/// @param sessionCookieName the name of the cookie in which the session id is stored (default = "session")
	/// @param defaultFile the default file to return if no file is specified in the request
	/// @param registerSignalHandler (default = true) set to false in order to prevent signal handler installation
	///
	public: HttpServer(std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   const Resource::File & documentRoot,
	                   const std::string & defaultFile = "index.html",
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::string sessionCookieName = "session",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes,
	                   bool registerSignalHandler = true);

	///
	/// Destroy the HTTP server, stopping it if it is running.
	///
	public: ~HttpServer();

	//////////////////////////////// Public API ///////////////////////////////

	///
	/// Start the HTTP server and return once the worker threads are running.
	///
	/// This call should be used to start the server when the calling thread
	/// should not be used as one of the workers.
	///
	/// If the server is already running, this call is a NOP.
	///
	/// @throw NetworkException if there was an issue initialising the listener
	///
	public: void startAsync();

	///
	/// Start the HTTP server and block until the server is stopped.
	///
	/// This call should be used to start the server when the calling thread
	/// should be used as one of the workers.
	///
	/// @throw NetworkException if there was an issue initialising the listener
	///
	public: void startSync();

	///
	/// Returns true if the server is running.
	///
	/// @return true if the server is running
	///
	public: bool isRunning();

	///
	/// Stop the HTTP server.
	///
	/// If the server is already stopped, this call is a NOP.
	///
	/// @param warn warn if the server is already stopped (default = false)
	///
	public: void stop(bool warn = false);

	///
	/// Get the address being listened on.
	///
	public: std::string getAddress() const {
		return toString(state->endpoint.address());
	}

	///
	/// Get the port being listened on.
	///
	public: unsigned short getPort() const {
		return state->endpoint.port();
	}

	////////////////////////// Private implementation /////////////////////////

	// Used for injection for compilers without guaranteed copy elision.
	private: HttpServer(HttpServer && rhs) noexcept
		: state(std::move(rhs.state))
		, threadNamePrefix(rhs.threadNamePrefix)
		, workerCount(rhs.workerCount)
		, workers(std::move(rhs.workers))
		, launched(std::move(rhs.launched))
		, listener(std::move(rhs.listener))
		, ioContext(std::move(rhs.ioContext))
		, mutex(std::move(rhs.mutex))
		, signalSet(std::move(rhs.signalSet)) {}

	//
	// Create the HTTP server configuration object.
	//
	private: static std::shared_ptr<HttpServerConfiguration> createState(std::shared_ptr<System::Clock> clock,
	                                                                     const std::shared_ptr<EnvironmentProperties> & configuration);

	//
	// Create a mime type map from the environment configuration if one is
	// supplied, otherwise use the default mime type map that is hard coded.
	//
	private: static std::shared_ptr<MimeTypes> createMimeTypes(const std::shared_ptr<EnvironmentProperties> & configuration,
	                                                           BalauLogger & logger);

	//
	// Create the HTTP handler, consisting of a HTTP routing handle at the base
	// and other HTTP handlers at the leaves.
	//
	private: static std::shared_ptr<HttpWebApp> createHttpHandler(const std::shared_ptr<EnvironmentProperties> & configuration,
	                                                              BalauLogger & logger);

	//
	// Create the WebSocket handler, consisting of a WebSocket routing handle at the base
	// and other WebSocket handlers at the leaves.
	//
	private: static std::shared_ptr<WsWebApp> createWsHandler(const std::shared_ptr<EnvironmentProperties> & configuration,
	                                                          BalauLogger & logger);

	//
	// Helper function that adds the HTTP web application to the supplied routing
	// trie in the location(s) specified in the location string.
	//
	// An exception will be thrown a location is specified that already has a web
	// application defined for it.
	//
	private: static void addToHttpRoutingTrie(HttpWebApps::RoutingHttpWebApp::Routing & routing,
	                                          const std::string & locationStr,
	                                          std::shared_ptr<HttpWebApp> & webApp);

	private: void launchListener();
	private: void workerThreadFunction(size_t workerIndex);
	private: void doRegisterSignalHandler();
	private: void handleSignal(const boost::system::error_code & error, int sig);

	private: std::shared_ptr<HttpServerConfiguration> state;
	private: const std::string threadNamePrefix;
	private: const size_t workerCount;
	private: std::vector<std::thread> workers;
	private: std::unique_ptr<std::atomic_uint> launched;
	private: std::shared_ptr<Impl::Listener> listener;
	private: std::unique_ptr<boost::asio::io_context> ioContext;
	private: std::unique_ptr<std::mutex> mutex;
	private: std::unique_ptr<boost::asio::signal_set> signalSet;
};

} // namespace Network::Http

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER
