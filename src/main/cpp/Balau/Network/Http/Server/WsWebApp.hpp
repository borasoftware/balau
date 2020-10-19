// @formatter:off
//
// Balau core C++ library
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
/// @file WsWebApp.hpp
///
/// Abstract base class of WebSocket web application handlers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__WS_WEB_APPLICATION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__WS_WEB_APPLICATION

#include <Balau/Network/Http/Server/NetworkTypes.hpp>

namespace Balau::Network::Http {

class WsSession;

///
/// Abstract base class of WebSocket web application handlers.
///
/// All concrete HTTP web applications must have a constructor of the following signature:
///
///  <pre>const EnvironmentProperties & configuration</pre>
///
/// This constructor will be used to instantiate the web application during initialisation
/// of the HTTP server.
///
class WsWebApp {
	///
	/// Handle received text data.
	///
	public: virtual void handleTextMessage(WsSession & session, std::string_view path) = 0;

	///
	/// Handle received binary data.
	///
	public: virtual void handleBinaryMessage(WsSession & session, std::string_view path) = 0;

	///
	/// Handle a received close control message.
	///
	public: virtual void handleClose(WsSession & session, std::string_view path) = 0;

	///
	/// Handle a received ping control message.
	///
	public: virtual void handlePing(WsSession & session, std::string_view path) = 0;

	///
	/// Handle a received pong control message.
	///
	public: virtual void handlePong(WsSession & session, std::string_view path) = 0;

	///////////////////////////////////////////////////////////////////////////

	///
	/// Destroy the WebSocket web application instance.
	///
	public: virtual ~WsWebApp() = default;
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__WS_WEB_APPLICATION
