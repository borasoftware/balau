// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Impl/ClientSessions.hpp"
#include "Impl/HttpSessions.hpp"
#include "../../../Logging/Logger.hpp"

namespace Balau::Network::Http {

HttpSession::HttpSession(Impl::HttpSessions & httpSessions_,
                         Impl::ClientSessions & clientSessions_,
                         std::shared_ptr<HttpServerConfiguration> serverConfiguration_,
                         TCP::socket && socket_)
	: httpSessions(httpSessions_)
	, clientSessions(clientSessions_)
	, serverConfiguration(std::move(serverConfiguration_))
	, strand(socket_.get_executor())
	, socket(std::move(socket_)) {
}

void HttpSession::doRead() {
	request = {};

	HTTP::async_read(
		  socket
		, buffer
		, request
		, boost::asio::bind_executor(
			  strand
			, std::bind(
				  &HttpSession::onRead
				, shared_from_this()
				, std::placeholders::_1
				, std::placeholders::_2
			)
		)
	);
}

void HttpSession::close() {
	strand.post(std::bind(&HttpSession::doClose, this), allocator);
}

void HttpSession::onRead(boost::system::error_code errorCode, std::size_t bytesTransferred) {
	boost::ignore_unused(bytesTransferred);

	parseCookies();

	if (!validateRequest(errorCode, request)) {
		return;
	}

	setClientSession();

	// Check for WebSocket upgrade.
	if (WS::is_upgrade(request)) {
		// ASIO states that following the socket move, the moved-from socket is in the same
		// state as if constructed using basic_stream_socket<tcp>(io_context &) constructor.
		std::make_shared<WsSession>(
			serverConfiguration, std::move(socket), std::string(request.target())
		)->doAccept(
			std::move(request)
		);

		return;
	} else {
		handleRequest(request);
	}
}

bool HttpSession::validateRequest(boost::system::error_code errorCode, const StringRequest & request) {
	if (errorCode == Error::end_of_stream) {
		doClose(); // The client closed the connection.
		return false;
	} else if (errorCode) {
		BalauBalauLogWarn(serverConfiguration->logger, "HttpSession request error: {}", errorCode);
		return false;
	}

	const auto target = request.target();

	if (target.empty() || target[0] != '/' || target.find("..") != boost::string_view::npos) {
		sendResponse(HttpWebApp::createBadRequestResponse(*this, request, "Illegal path in request."));
		return false;
	}

	return true;
}

void HttpSession::onWrite(boost::system::error_code errorCode, std::size_t bytesTransferred, bool close) {
	boost::ignore_unused(bytesTransferred);

	if (errorCode) {
		BalauBalauLogWarn(serverConfiguration->logger, "HttpSession error: {}", errorCode);
		doClose();
	} else if (close) {
		doClose();
	} else {
		cachedResponse = nullptr;
		doRead();
	}
}

void HttpSession::doClose() {
	// Executed via the strand.

	if (!socket.is_open()) {
		BalauBalauLogTrace(serverConfiguration->logger, "HttpSession::doClose ignoring duplicate call.");
		return;
	}

	boost::system::error_code errorCode;
	BalauBalauLogTrace(serverConfiguration->logger, "HttpSession::doClose shutting down");
	socket.shutdown(TCP::socket::shutdown_send, errorCode);
	BalauBalauLogTrace(serverConfiguration->logger, "HttpSession::doClose completed shut down");
	socket.close(errorCode);
	BalauBalauLogTrace(serverConfiguration->logger, "HttpSession::doClose closed");
	httpSessions.unregisterSession(shared_from_this());
}

void HttpSession::parseCookies() {
	cookies.clear();
	cookieString = ::toString(request[Field::cookie]);

	// According to RFC-6265, cookies must be split by the exact "; " string.
	auto cookieViews = Util::Strings::split(cookieString, "; ");

	for (auto cookieView : cookieViews) {
		auto cookieNameAndValue = Util::Strings::split(cookieView, "=");

		if (cookieNameAndValue.size() != 2) {
			// Invalid cookie sent by user agent.. ignore.
			continue;
		}

		cookies.insert(std::make_pair(cookieNameAndValue[0], cookieNameAndValue[1]));
	}
}

void HttpSession::setClientSession() {
	auto iter = cookies.find(std::string_view(serverConfiguration->sessionCookieName));

	if (iter != cookies.end()) {
		// Existing client session?
		auto session = clientSessions.get(std::string(iter->second));

		if (session) {
			clientSession = session;
			return;
		}
	}

	// New client session.
	clientSession = clientSessions.create(*serverConfiguration->clock);
}

} // namespace Balau::Network::Http
