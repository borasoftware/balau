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
/// @file NullWsWebApp.hpp
///
/// A WebSocket web application that silently consumes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__NULL_WS_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__NULL_WS_WEB_APP

#include <Balau/Network/Http/Server/WsWebApp.hpp>

namespace Balau::Network::Http::WsWebApps {

///
/// A WebSocket web application that silently consumes.
///
/// This web app exists as a default when no WebSocket web application is required.
///
class NullWsWebApp : public WsWebApp {
	public: void handleTextMessage(WsSession & session, std::string_view path) override {}

	public: void handleBinaryMessage(WsSession & session, std::string_view path) override {}

	public: void handleClose(WsSession & session, std::string_view path) override {}

	public: void handlePing(WsSession & session, std::string_view path) override {}

	public: void handlePong(WsSession & session, std::string_view path) override {}
};

} // namespace Balau::Network::Http::WsWebApps

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__NULL_WS_WEB_APP
