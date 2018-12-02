// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Impl/ClientSession.hpp"
#include "Impl/HttpSessions.hpp"
#include "../../../Logging/Logger.hpp"

namespace Balau::Network::Http {

HttpSession::HttpSession(Impl::HttpSessions & owner_,
                         std::shared_ptr<Impl::ClientSession> clientSession_,
                         std::shared_ptr<HttpServerConfiguration> serverConfiguration_,
                         TCP::socket socket_)
	: owner(owner_)
	, log(serverConfiguration_->logger)
	, clientSession(std::move(clientSession_))
	, serverConfiguration(std::move(serverConfiguration_))
	, socket(std::move(socket_))
	, strand(socket.get_executor()) {
	BalauBalauLogTrace(
		  log
		, "HttpSession created (local = {}, remote = {})"
		, socket.local_endpoint()
		, socket.remote_endpoint()
	);
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

void HttpSession::onRead(boost::system::error_code errorCode, std::size_t bytesTransferred) {
	boost::ignore_unused(bytesTransferred);

	if (!validateRequest(errorCode, request)) {
		return;
	}

	// Check for WebSocket upgrade.
	if (WS::is_upgrade(request)) {
		std::make_shared<WsSession>(clientSession, serverConfiguration, std::move(socket))->doAccept(std::move(request));
		return;
	}

	handleRequest(request);
}

bool HttpSession::validateRequest(boost::system::error_code errorCode, const StringRequest & request) {
	if (errorCode == Error::end_of_stream) {
		doClose(); // The client closed the connection.
		return false;
	} else if (errorCode) {
		BalauBalauLogWarn(log, "HttpSession request error: {}", errorCode);
		return false;
	}

	const auto target = request.target();

	if (target.empty() || target[0] != '/' || target.find("..") != boost::string_view::npos) {
		sendResponse(HttpWebApp::createBadRequestResponse(*this, request, "Illegal path in request."));
		return false;
	}

	return true;
}

void HttpSession::handleRequest(const StringRequest & request) {
	switch (request.method()) {
		case Method::get: {
			serverConfiguration->httpHandler->handleGetRequest(*this, request);
			break;
		}

		case Method::head: {
			serverConfiguration->httpHandler->handleHeadRequest(*this, request);
			break;
		}

		case Method::post: {
			serverConfiguration->httpHandler->handlePostRequest(*this, request);
			break;
		}

		default: {
			sendResponse(HttpWebApp::createBadRequestResponse(*this, request, "Unsupported HTTP method."));
			break;
		}
	}
}

void HttpSession::onWrite(boost::system::error_code errorCode, std::size_t bytesTransferred, bool close) {
	boost::ignore_unused(bytesTransferred);

	if (errorCode) {
		BalauBalauLogWarn(log, "HttpSession error: {}", errorCode);
		doClose();
	} else if (close) {
		doClose();
	} else {
		cachedResponse = nullptr;
		doRead();
	}
}

void HttpSession::stop() {
	BalauBalauLogTrace(log, "HttpSession::stop");
	doClose();
	owner.stop(shared_from_this());
}

void HttpSession::doClose() {
	boost::system::error_code errorCode;
	BalauBalauLogTrace(log, "HttpSession::doClose shutting down");
	socket.shutdown(TCP::socket::shutdown_send, errorCode);
	BalauBalauLogTrace(log, "HttpSession::doClose completed shut down");
	socket.close(errorCode);
	BalauBalauLogTrace(log, "HttpSession::doClose closed");
}

} // namespace Balau::Network::Http
