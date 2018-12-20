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
/// @file WsWebApp.hpp
///
/// Abstract base class of WebSocket web application handlers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__WS_WEB_APPLICATION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__WS_WEB_APPLICATION

#include <Balau/Network/Http/Server/NetworkTypes.hpp>

#include <string_view>

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
