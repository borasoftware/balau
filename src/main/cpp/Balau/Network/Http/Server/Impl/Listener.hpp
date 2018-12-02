// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__LISTENER
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__LISTENER

#include <Balau/Network/Http/Server/Impl/HttpSessions.hpp>
#include <Balau/Logging/Impl/BalauLogger.hpp>

// Avoid false positive (due to std::make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Network::Http::Impl {

// Listener for HttpServer.
class Listener : public std::enable_shared_from_this<Listener> {
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
		checkError(errorCode);

		acceptor.set_option(boost::asio::socket_base::reuse_address(true), errorCode);
		checkError(errorCode);

		acceptor.bind(serverConfiguration->endpoint, errorCode);
		checkError(errorCode);

		acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
		checkError(errorCode);
	}

	public: bool isOpen() {
		return acceptor.is_open();
	}

	public: void close() {
		acceptor.close();
		httpSessions.stopAll();
	}

	public: void doAccept() {
		acceptor.async_accept(
			socket, std::bind(&Listener::onAccept, shared_from_this(), std::placeholders::_1)
		);
	}

	private: void onAccept(boost::system::error_code errorCode);

	private: void checkError(const boost::system::error_code & errorCode) {
		if (errorCode) {
			ThrowBalauException(Exception::NetworkException, errorCode.message());
		}
	}

	private: std::shared_ptr<Impl::ClientSession> getClientSession() {
		// TODO
		return std::shared_ptr<Impl::ClientSession>();
	}

	private: std::shared_ptr<HttpServerConfiguration> serverConfiguration;
	private: TCP::acceptor acceptor;
	private: TCP::socket socket;
	private: Impl::HttpSessions httpSessions;
};

} // namespace Balau::Network::Http::Impl

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__LISTENER
