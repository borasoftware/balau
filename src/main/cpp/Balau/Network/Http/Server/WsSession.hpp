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

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WS_SESSION
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WS_SESSION

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
class WsSession final : public std::enable_shared_from_this<WsSession> {
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
	                  TCP::socket && socket_,
	                  std::string path_)
		: serverConfiguration(std::move(serverConfiguration_))
		, strand(socket_.get_executor())
		, socket(std::move(socket_))
		, path(std::move(path_)) {}

	///
	/// Get the shared state of the http server.
	///
	/// Called by handler implementations.
	///
	public: HttpServerConfiguration & configuration() const {
		return *serverConfiguration;
	}

	public: template <typename Body, typename AllocatorT>
	void doAccept(HTTP::request<Body, HTTP::basic_fields<AllocatorT>> req) {


		// TODO
//		ws.control_callback(
//			std::bind(
//				&WsSession::onControl,
//				this,
//				std::placeholders::_1,
//				std::placeholders::_2));

		socket.async_accept(
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
		socket.async_accept(
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
		socket.async_read(
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
		socket.text(socket.got_text());

		socket.async_write(
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
	private: boost::asio::strand<boost::asio::io_context::executor_type> strand;
	private: WS::stream<TCP::socket> socket;
	private: const std::string path;
	private: boost::beast::multi_buffer buffer;
};

} // namespace Balau::Network::Http

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WS_SESSION
