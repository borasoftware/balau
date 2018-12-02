// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SESSION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SESSION

///
/// @file HttpSession.hpp
///
/// Manages the handling of HTTP messages and WebSocket upgrade requests in an HTTP connection.
///

#include <Balau/Network/Http/Server/WsSession.hpp>
#include <Balau/Network/Http/Server/HttpWebApp.hpp>
#include <Balau/Util/DateTime.hpp>
#include <Balau/Logging/Impl/BalauLogger.hpp>

// Avoid false positive (due to std::make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Network::Http {

namespace Impl {

class ClientSession;
class HttpSessions;

} // namespace Impl

///
/// Manages the handling of HTTP messages and WebSocket upgrade requests in an HTTP connection.
///
/// Holds a pointer to the private session information object for the client.
///
/// Multiple HTTP sessions may occur within the lifetime of a single client session.
///
class HttpSession : public std::enable_shared_from_this<HttpSession> {
	///
	/// Create an HTTP session object with the supplied data.
	///
	/// @param clientSession_ the client session associated with the HTTP session
	/// @param serverConfiguration_ the configuration of the HTTP server that created this session
	/// @param socket_ the session socket
	///
	public: HttpSession(Impl::HttpSessions & owner_,
	                    std::shared_ptr<Impl::ClientSession> clientSession_,
	                    std::shared_ptr<HttpServerConfiguration> serverConfiguration_,
	                    TCP::socket socket_);

	///
	/// Get the shared state of the http server.
	///
	public: HttpServerConfiguration & configuration() const {
		return *serverConfiguration;
	}

	///
	/// Send the response back to the client.
	///
	/// Called by handlers.
	///
	public: template <typename BodyT> void sendResponse(Response<BodyT> && response) {
		BalauBalauLogInfo(
			  log
			, "{} - {} {} - {} {} {} bytes - \"{}\" [{}]"
			, socket.remote_endpoint().address().to_string()
			, request.method()
			, response.version() == 11 ? "HTTP/1.1" : "HTTP/1.0"
			, response.result_int()
			, response[Field::content_type]
			, response[Field::content_length]
			, request.target() // path
			, request[Field::user_agent]
		);

		// Transfer ownership of the response in preparation for the asynchronous call.
		auto sharedResponse = std::make_shared<Response<BodyT>>(std::move(response));
		auto sharedVoidResponse = std::shared_ptr<void>(sharedResponse);
		cachedResponse = sharedVoidResponse;

		HTTP::async_write(
			  socket
			, *sharedResponse
			, boost::asio::bind_executor(
				strand
				, std::bind(
					&HttpSession::onWrite
					, shared_from_this()
					, std::placeholders::_1
					, std::placeholders::_2
					, sharedResponse->need_eof()
				)
			)
		);
	}

	////////////////////////// Private implementation /////////////////////////

	friend class Listener;
	friend class ::Balau::Network::Http::Impl::HttpSessions;

	///
	/// @throw NetworkException if there was an issue reading
	///
	public: void doRead();

	// Callback from context.
	private: void onRead(boost::system::error_code errorCode, std::size_t bytesTransferred);

	// Validate the request path.
	// TODO Add other required validations.
	private: bool validateRequest(boost::system::error_code errorCode, const StringRequest & request);

	// Dispatch the request to the appropriate handler method.
	private: void handleRequest(const StringRequest & request);

	private: void onWrite(boost::system::error_code errorCode, std::size_t bytesTransferred, bool close);

	private: void stop();

	private: void doClose();

	private: Impl::HttpSessions & owner;
	private: BalauLogger log;
	private: std::shared_ptr<Impl::ClientSession> clientSession;
	private: std::shared_ptr<HttpServerConfiguration> serverConfiguration;
	private: TCP::socket socket;
	private: boost::asio::strand<boost::asio::io_context::executor_type> strand;
	private: Buffer buffer;
	private: StringRequest request;
	private: std::shared_ptr<void> cachedResponse; // Used to keep the response alive.
};

} // namespace Balau::Network::Http

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SESSION
