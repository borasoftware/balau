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

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__LISTENER
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__LISTENER

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Network/Http/Server/Impl/ClientSessions.hpp>
#include <Balau/Network/Http/Server/Impl/HttpSessions.hpp>
#include <Balau/Logging/Impl/BalauLogger.hpp>

// Avoid false positive (due to std::make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Network::Http::Impl {

// Listener for HttpServer.
class Listener final : public std::enable_shared_from_this<Listener> {
	//
	// @throw NetworkException if there was an issue constructing the listener
	//
	public: Listener(std::shared_ptr<HttpServerConfiguration> serverConfiguration_,
	                 boost::asio::io_context & context)
		: serverConfiguration(std::move(serverConfiguration_))
		, acceptor(context)
		, socket(context) {
		boost::system::error_code errorCode;

		acceptor.open(serverConfiguration->endpoint.protocol(), errorCode);
		checkError(errorCode, [] () { return "acceptor.open failed"; });

		acceptor.set_option(boost::asio::socket_base::reuse_address(true), errorCode);
		checkError(errorCode, [] () { return "acceptor.set_option(reuse_address(true)) failed"; });

		auto & endpoint = serverConfiguration->endpoint;
		acceptor.bind(endpoint, errorCode);
		checkError(errorCode, [&endpoint] () { return ::toString("acceptor.bind(", endpoint, ")"); });

		acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
		checkError(errorCode, [] () { return "acceptor.listen failed"; });
	}

	public: bool isOpen() {
		return acceptor.is_open();
	}

	public: void close() {
		acceptor.close();
		httpSessions.unregisterAllSessions(serverConfiguration->logger);
	}

	public: void doAccept() {
		acceptor.async_accept(socket, std::bind(&Listener::onAccept, shared_from_this(), std::placeholders::_1));
	}

	private: void onAccept(boost::system::error_code errorCode) {
		if (!errorCode) {
			// ASIO states that following the socket move, the moved-from socket is in the same
			// state as if constructed using basic_stream_socket<tcp>(io_context &) constructor.
			auto session = std::make_shared<HttpSession>(
				httpSessions, clientSessions, serverConfiguration, std::move(socket)
			);

			httpSessions.registerSession(session);
			session->doRead();
		} else if (errorCode == boost::system::errc::operation_canceled) {
			BalauBalauLogInfo(
				  serverConfiguration->logger
				, "Listener {}:{} stopping due to: {}"
				, serverConfiguration->endpoint.address()
				, serverConfiguration->endpoint.port()
				, errorCode.message()
			);

			return;
		} else {
			BalauBalauLogInfo(
				  serverConfiguration->logger
				, "Listener accept failure on {}:{}: {}. Restarting listener."
				, serverConfiguration->endpoint.address()
				, serverConfiguration->endpoint.port()
				, errorCode.message()
			);
		}

		doAccept();
	}

	private: template <typename ErrorMessageCreator>
	void checkError(const boost::system::error_code & errorCode, const ErrorMessageCreator & errorMessage) {
		if (errorCode) {
			ThrowBalauException(Exception::NetworkException, ::toString(errorCode.message(), " - ", errorMessage()));
		}
	}

	private: std::shared_ptr<ClientSession> getClientSession() {
		// TODO
		return std::shared_ptr<ClientSession>();
	}

	private: std::shared_ptr<HttpServerConfiguration> serverConfiguration;
	private: TCP::acceptor acceptor;
	private: TCP::socket socket;
	private: Impl::HttpSessions httpSessions;
	private: Impl::ClientSessions clientSessions;
};

} // namespace Balau::Network::Http::Impl

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__LISTENER
