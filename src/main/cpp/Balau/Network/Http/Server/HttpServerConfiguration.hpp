// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Network/Utilities/BalauLogger.hpp>

namespace Balau {

class EnvironmentProperties;
class Injector;

namespace System {

class Clock;

} // namespace System

namespace Network::Http {

class HttpWebApp;
class WsWebApp;

///
/// Shared state between HTTP sessions.
///
struct HttpServerConfiguration {
	///
	/// The clock used by the server.
	///
	const std::shared_ptr<const System::Clock> clock;

	///
	/// The Http server's environment configuration.
	///
	/// This is available to web applications if they require it after the
	/// initial construction time configuration.
	///
	const std::shared_ptr<EnvironmentProperties> configuration;

	///
	/// The main logger into which the server will log.
	///
	const BalauLogger logger;

	///
	/// The server identification string.
	///
	const std::string serverId;

	///
	/// The IP address and port on which the server is listening.
	///
	const TCP::endpoint endpoint;

	///
	/// The name of the cookie where the client session id is stored.
	///
	const std::string sessionCookieName;

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

	HttpServerConfiguration(std::shared_ptr<const System::Clock> clock_,
	                        const BalauLogger & logger_,
	                        std::string serverIdentification_,
	                        TCP::endpoint endpoint_,
	                        std::string sessionCookieName_,
	                        std::shared_ptr<HttpWebApp> httpHandler_,
	                        std::shared_ptr<WsWebApp> wsHandler_,
	                        std::shared_ptr<MimeTypes> mimeTypes_)
		: clock(std::move(clock_))
		, logger(logger_)
		, serverId(std::move(serverIdentification_))
		, endpoint(std::move(endpoint_))
		, sessionCookieName(std::move(sessionCookieName_))
		, httpHandler(std::move(httpHandler_))
		, wsHandler(std::move(wsHandler_))
		, mimeTypes(std::move(mimeTypes_)) {}
};

} // namespace Network::Http

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_CONFIGURATION
