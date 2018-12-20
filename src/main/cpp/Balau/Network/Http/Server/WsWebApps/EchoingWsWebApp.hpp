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
/// @file EchoingWsWebApp.hpp
///
/// WebSocket web application that echos messages back to the client.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__ECHOING_WS_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__ECHOING_WS_WEB_APP

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

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__ECHOING_WS_WEB_APP
