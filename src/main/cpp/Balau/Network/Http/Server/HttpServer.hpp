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
#include <Balau/Network/Http/Server/HttpWebApp.hpp>
#include <Balau/Network/Http/Server/WsWebApp.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/System/Clock.hpp>
#include <Balau/Application/Injectable.hpp>
#include <Balau/Application/Injector.hpp>

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
	BalauInjectNamed(
		  HttpServer
		, state->injector,    ""
		, state->clock,       ""
		, state->serverId,    "httpServerIdentification"
		, state->endpoint,    "httpServerEndpoint"
		, threadNamePrefix,   "httpServerThreadName"
		, workerCount,        "httpServerWorkerCount"
		, state->httpHandler, "httpHandler"
		, state->wsHandler,   "webSocketHandler"
		, threadNamePrefix,   "httpServerLoggingNamespace"
		, state->mimeTypes,   "mimeTypes"
	);

//	BalauInjectNamedTypes(
//		  HttpServer
//		, std::weak_ptr<Injector>,        ""
//		, std::shared_ptr<System::Clock>, ""
//		, std::string,                    "http.server.configuration"
//	);

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
	/// @param injector the injector to use in the web apps
	/// @param clock the clock to be used in the server
	/// @param configurationUri the server configuration
	///
	public: HttpServer(std::weak_ptr<Injector> injector,
	                   std::shared_ptr<System::Clock> clock,
	                   const std::string & configurationUri);

	///
	/// Create an HTTP server with HTTP and WebSocket handlers.
	///
	/// The supplied injector will be used by the handlers during requests to
	/// obtain injected instances.
	///
	/// This constructor can be used within an injector configuration in order
	/// to bind a custom configured HTTP server. If the default configuration is
	/// sufficient, the standard injector constructor can be used instead.
	///
	/// The mime type map can be specified or left as the default mime type map.
	///
	/// @param injector the injector to use in the web apps
	/// @param clock the clock to be used in the server
	/// @param serverIdentification the server string to return
	/// @param endpoint the network address and port to listen on
	/// @param threadNamePrefix_ the prefix of the worker thread names (used for logging)
	/// @param workerCount_ the number of worker threads to use
	/// @param httpHandler the HTTP request handler to use
	/// @param wsHandler the WebSocket message handler to use
	/// @param loggingNamespace the logging namespace to use (default = "balau.network.server")
	/// @param mimeTypes the mime type map to use
	///
	public: HttpServer(std::weak_ptr<Injector> injector,
	                   std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   std::shared_ptr<HttpWebApp> httpHandler,
	                   std::shared_ptr<WsWebApp> wsHandler,
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes);

	///
	/// Create an HTTP server with an HTTP handler.
	///
	/// The supplied injector will be used by the handlers during requests to
	/// obtain injected instances.
	///
	/// This constructor can be used within an injector configuration in order
	/// to bind a custom configured HTTP server. If the default configuration is
	/// sufficient, the standard injector constructor can be used instead.
	///
	/// The mime type map can be specified or left as the default mime type map.
	///
	/// @param injector the injector to use in the web apps
	/// @param clock the clock to be used in the server
	/// @param serverIdentification the server string to return
	/// @param endpoint the network address and port to listen on
	/// @param threadNamePrefix_ the prefix of the worker thread names (used for logging)
	/// @param workerCount_ the number of worker threads to use
	/// @param httpHandler the HTTP request handler to use
	/// @param loggingNamespace the logging namespace to use (default = "balau.network.server")
	/// @param mimeTypes the mime type map to use
	///
	public: HttpServer(std::weak_ptr<Injector> injector,
	                   std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   std::shared_ptr<HttpWebApp> httpHandler,
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes);

	////////////////////// Constructors without injector //////////////////////

	///
	/// Create an HTTP server with HTTP and WebSocket handlers.
	///
	/// The mime type map can be specified or left as the default mime type map.
	///
	/// @param clock the clock to be used in the server
	/// @param serverIdentification the server string to return
	/// @param endpoint the network address and port to listen on
	/// @param threadNamePrefix_ the prefix of the worker thread names (used for logging)
	/// @param workerCount_ the number of worker threads to use
	/// @param httpHandler the HTTP request handler to use
	/// @param wsHandler the WebSocket message handler to use
	/// @param loggingNamespace the logging namespace to use (default = "balau.network.server")
	/// @param mimeTypes the mime type map to use
	///
	public: HttpServer(std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   std::shared_ptr<HttpWebApp> httpHandler,
	                   std::shared_ptr<WsWebApp> wsHandler,
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes);

	///
	/// Create an HTTP server with an HTTP handler.
	///
	/// The mime type map can be specified or left as the default mime type map.
	///
	/// @param clock the clock to be used in the server
	/// @param serverIdentification the server string to return
	/// @param endpoint the network address and port to listen on
	/// @param threadNamePrefix_ the prefix of the worker thread names (used for logging)
	/// @param workerCount_ the number of worker threads to use
	/// @param httpHandler the HTTP request handler to use
	/// @param loggingNamespace the logging namespace to use (default = "balau.network.server")
	/// @param mimeTypes the mime type map to use
	///
	public: HttpServer(std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   std::shared_ptr<HttpWebApp> httpHandler,
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes);

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
	/// @param defaultFile the default file to return if no file is specified in the request
	///
	public: HttpServer(std::shared_ptr<System::Clock> clock,
	                   const std::string & serverIdentification,
	                   const TCP::endpoint & endpoint,
	                   std::string threadNamePrefix_,
	                   size_t workerCount_,
	                   const Resource::File & documentRoot,
	                   const std::string & defaultFile = "index.html",
	                   const std::string & loggingNamespace = "balau.network.server",
	                   std::shared_ptr<MimeTypes> mimeTypes = MimeTypes::defaultMimeTypes);

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
	public: void start();

	///
	/// Start the HTTP server and block until the server is stopped.
	///
	/// This call should be used to start the server when the calling thread
	/// should be used as one of the workers.
	///
	/// @throw NetworkException if there was an issue initialising the listener
	///
	public: void run();

	///
	/// Returns true if the server is running.
	///
	/// @return true if the server is running
	///
	public: bool isRunning();

	///
	/// Blocks until isRunning() returns false.
	///
	/// @param period the sleep period in milliseconds
	///
	public: void blockUntilStopped(size_t period);

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
		return ::toString(state->endpoint.address());
	}

	///
	/// Get the port being listened on.
	///
	public: unsigned short getPort() const {
		return state->endpoint.port();
	}

	////////////////////////// Private implementation /////////////////////////

	private: void startWorkerThreads(size_t thisWorkerCount);
	private: void workerThreadFunction(size_t workerIndex, bool blocking);
	private: void registerSignalHandler();
	private: void handleSignal(const boost::system::error_code & error, int sig);

	private: std::shared_ptr<HttpServerConfiguration> state;
	private: const std::string threadNamePrefix;
	private: size_t workerCount;
	private: boost::asio::io_context ioContext;
	private: std::shared_ptr<Impl::Listener> listener;
	private: std::vector<std::thread> workers;
	private: std::atomic_uint runningWorkerCount;
	private: std::mutex mutex;
	private: boost::asio::signal_set signalSet;
};

} // namespace Network::Http

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER
