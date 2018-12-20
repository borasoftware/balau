// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__WS_SESSION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__WS_SESSION

///
/// @file WsSession.hpp
///
/// Manages the handling of WebSocket messages in a WebSocket connection.
///

#include <Balau/Network/Http/Server/WsWebApp.hpp>
#include <Balau/Network/Http/Server/HttpServerConfiguration.hpp>
#include <Balau/Util/DateTime.hpp>

// Avoid false positive (due to std::make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Network::Http {

///
/// Manages the handling of WebSocket messages in a WebSocket connection.
///
/// Holds a pointer to the private session information object for the client.
///
class WsSession : public std::enable_shared_from_this<WsSession> {
	///
	/// Create a WebSocket session object with the supplied data.
	///
	/// @param clientSession_ the client session associated with the WebSocket session
	/// @param serverConfiguration_ the configuration of the HTTP server that created this session
	/// @param socket_ the session socket
	///
	/// TODO A single WebSocket web application will handle all the session.
	/// TODO This can be selected from the routing trie in advance.
	///
	public: WsSession(std::shared_ptr<HttpServerConfiguration> serverConfiguration_,
	                  TCP::socket socket_,
	                  std::string path_)
		: serverConfiguration(std::move(serverConfiguration_))
		, ws(std::move(socket_))
		, strand(ws.get_executor())
		, path(std::move(path_)) {}

	///
	/// Get the shared state of the http server.
	///
	/// Called by handler implementations.
	///
	public: HttpServerConfiguration & configuration() const {
		return *serverConfiguration;
	}

	public: template <class Body, class Allocator>
	void doAccept(HTTP::request<Body, HTTP::basic_fields<Allocator>> req) {


		// TODO
//		ws.control_callback(
//			std::bind(
//				&WsSession::onControl,
//				this,
//				std::placeholders::_1,
//				std::placeholders::_2));

		ws.async_accept(
			req,
			boost::asio::bind_executor(
				strand,
				std::bind(
					&WsSession::onAccept,
					shared_from_this(),
					std::placeholders::_1)));

		// TODO
	}

	public: void run() {
		ws.async_accept(
			boost::asio::bind_executor(
				strand, std::bind(&WsSession::onAccept, shared_from_this(), std::placeholders::_1)
			)
		);
	}

	public: void onControl(WsFrame frameType, boost::beast::string_view payload) {
		boost::ignore_unused(payload);

		switch (frameType) {
			case WsFrame::close: {
				serverConfiguration->wsHandler->handleClose(*this, path); // TODO
				break;
			}

			case WsFrame::ping: {
				serverConfiguration->wsHandler->handlePing(*this, path); // TODO
				break;
			}

			case WsFrame::pong: {
				serverConfiguration->wsHandler->handlePong(*this, path); // TODO
				break;
			}
		}
	}

	///
	/// @throw NetworkException if there was an issue accepting
	///
	public: void onAccept(boost::system::error_code ec) {
		checkError(ec);
		doRead();
	}

	public: void doRead() {
		ws.async_read(
			buffer,
			boost::asio::bind_executor(
				strand,
				std::bind(
					&WsSession::onRead,
					shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2)));
	}

	///
	/// @throw NetworkException if there was an issue reading
	///
	public: void onRead(boost::system::error_code ec, std::size_t bytes_transferred) {
		boost::ignore_unused(bytes_transferred);

		if (ec == WS::error::closed) {
			return;
		}

		checkError(ec);

		// Echo the message
		ws.text(ws.got_text());

		ws.async_write(
			  buffer.data()
			, boost::asio::bind_executor(
				strand, std::bind(&WsSession::onWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2)
			)
		);
	}

	///
	/// @throw NetworkException if there was an issue writing
	///
	public: void onWrite(boost::system::error_code ec, std::size_t bytes_transferred) {
		boost::ignore_unused(bytes_transferred);

		checkError(ec);
		buffer.consume(buffer.size());
		doRead();
	}

	////////////////////////// Private implementation /////////////////////////

	private: void checkError(const boost::system::error_code & errorCode) {
		if (errorCode) {
			ThrowBalauException(Exception::NetworkException, errorCode.message());
		}
	}

	private: std::shared_ptr<HttpServerConfiguration> serverConfiguration;
	private: WS::stream<TCP::socket> ws;
	private: boost::asio::strand<boost::asio::io_context::executor_type> strand;
	private: const std::string path;
	private: boost::beast::multi_buffer buffer;
};

} // namespace Balau::Network::Http

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__WS_SESSION
