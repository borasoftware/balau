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

namespace Balau::Network::Http {

class WsSession;

///
/// Abstract base class of WebSocket web application handlers.
///
class WsWebApp {
	///
	/// Handle received text data.
	///
	public: virtual void handleTextMessage(WsSession & session) = 0;

	///
	/// Handle received binary data.
	///
	public: virtual void handleBinaryMessage(WsSession & session) = 0;

	///
	/// Handle a received close control message.
	///
	public: virtual void handleClose(WsSession & session) = 0;

	///
	/// Handle a received ping control message.
	///
	public: virtual void handlePing(WsSession & session) = 0;

	///
	/// Handle a received pong control message.
	///
	public: virtual void handlePong(WsSession & session) = 0;

	///////////////////////////////////////////////////////////////////////////

	///
	/// Destroy the WebSockets web application instance.
	///
	public: virtual ~WsWebApp() = default;
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__WS_WEB_APPLICATION
