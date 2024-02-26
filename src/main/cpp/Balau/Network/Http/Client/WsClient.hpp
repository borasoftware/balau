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
/// @file WsClient.hpp
///
/// A simple WebSocket client.
///

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_CLIENT_WS_CLIENT
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_CLIENT_WS_CLIENT

#include <Balau/Type/ToString.hpp>

#include <Balau/Exception/NetworkExceptions.hpp>
#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Resource/Url.hpp>
#include <Balau/Type/StdTypes.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>

namespace Balau::Network::Http {

///
/// A simple WebSocket client.
///
/// This is currently just used for testing. Future development will extend this
/// class into a full WebSocket client.
///
class WsClient {
	///
	/// Create a WebSocket client instance.
	///
	/// @param host_ the host name to connect to
	/// @param port_ the port number to connect to
	///
	public: explicit WsClient(std::string host_, unsigned short port_ = 80)
		: host(std::move(host_))
		, port(port_) {}

	///
	/// Create an HTTPS client by copying the supplied instance.
	///
	public: WsClient(const WsClient &) = default;

	///
	/// Create an HTTPS client by moving the contents of the supplied instance.
	///
	public: WsClient(WsClient &&) = default;

	///
	/// Assign an HTTPS client by copying the supplied instance.
	///
	public: WsClient & operator = (const WsClient &) = default;

	///
	/// Assign an HTTPS client by moving the contents of the supplied instance.
	///
	public: WsClient & operator = (WsClient &&) = default;

	public: template <typename ResponseBufferT, typename RequestBufferT>
	ResponseBufferT send(const std::string_view & path, const RequestBufferT & requestData) {
		/////////////// Connect ///////////////

		boost::asio::io_context ioc;
		TCP::resolver resolver { ioc };
		TCP::socket socket { ioc };

		auto resolverResults = resolver.resolve(host.c_str(), ::toString(port).c_str());
		boost::asio::connect(socket, resolverResults.begin(), resolverResults.end());

		WS::stream<TCP::socket> ws { ioc };

		ws.handshake(host, path.empty() ? "/" : std::string(path));
		ws.write(requestData);

		ResponseBufferT buffer {};

		ws.read(buffer);
		ws.close(WS::close_code::normal);

		////////////// Disconnect /////////////

		boost::system::error_code errorCode {};
		socket.shutdown(TCP::socket::shutdown_both, errorCode);

		// TODO clarify the following.
		if (errorCode && errorCode != boost::system::errc::not_connected) {
			throw boost::system::system_error { errorCode }; // TODO
		}

		///////////////////////////////////////

		return buffer;
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::string host;
	private: unsigned short port;
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_CLIENT_HTTPS_CLIENT
