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
/// @file HttpServerConfiguration.hpp
///
/// Shared state between HTTP sessions.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_CONFIGURATION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_CONFIGURATION

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Network/Utilities/MimeTypes.hpp>

namespace Balau {

class Injector;
class Logger;

namespace System {

class Clock;

} // namespace System

namespace Network {

namespace Http {

class HttpWebApp;
class WsWebApp;

///
/// Shared state between HTTP sessions.
///
struct HttpServerConfiguration {
	///
	/// The injector instance passed to the HTTP server.
	///
	/// If this is null, no injector was supplied to the server.
	///
	const std::weak_ptr<Injector> injector;

	///
	/// The clock used by the server.
	///
	const std::shared_ptr<System::Clock> clock;

	///
	/// The logger into which the server will log.
	///
	const Logger & logger;

	///
	/// The server identification string.
	///
	const std::string serverId;

	///
	/// The IP address and port on which the server is listening.
	///
	const TCP::endpoint endpoint;

	///
	/// The handler implementation used to handle HTTP messages.
	///
	const std::shared_ptr<HttpWebApp> httpHandler;

	///
	/// The handler implementation used to handle WebSocket messages.
	///
	const std::shared_ptr<WsWebApp> wsHandler;

	///
	/// The map of registered mime types.
	///
	const std::shared_ptr<MimeTypes> mimeTypes;

	///////////////////////// Private implementation //////////////////////////

	HttpServerConfiguration(std::weak_ptr<Injector> injector_,
	                        std::shared_ptr<System::Clock> clock_,
	                        Logger & logger_,
	                        std::string serverIdentification_,
	                        TCP::endpoint endpoint_,
	                        std::shared_ptr<HttpWebApp> httpHandler_,
	                        std::shared_ptr<WsWebApp> wsHandler_,
	                        std::shared_ptr<MimeTypes> mimeTypes_)
		: injector(std::move(injector_))
		, clock(clock_)
		, logger(logger_)
		, serverId(std::move(serverIdentification_))
		, endpoint(std::move(endpoint_))
		, httpHandler(std::move(httpHandler_))
		, wsHandler(std::move(wsHandler_))
		, mimeTypes(std::move(mimeTypes_)) {}
};

} // namespace Http

} // namespace Network

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_CONFIGURATION
