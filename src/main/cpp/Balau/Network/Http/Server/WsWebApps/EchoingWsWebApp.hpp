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
/// @file EchoingWsWebApp.hpp
///
/// WebSocket web application that echos messages back to the client.
///

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS_ECHOING_WS_WEB_APP
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS_ECHOING_WS_WEB_APP

#include <Balau/Network/Http/Server/WsWebApp.hpp>

namespace Balau::Network::Http::WsWebApps {

///
/// WebSocket web application that echos messages back to the client.
///
/// This web app exists for testing purposes.
///
class EchoingWsWebApp : public WsWebApp {
	public: void handleTextMessage(WsSession & session, std::string_view path) override;

	public: void handleBinaryMessage(WsSession & session, std::string_view path) override;

	public: void handleClose(WsSession & session, std::string_view path) override;

	public: void handlePing(WsSession & session, std::string_view path) override;

	public: void handlePong(WsSession & session, std::string_view path) override;
};

} // namespace Balau::Network::Http::WsWebApps

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS_ECHOING_WS_WEB_APP
