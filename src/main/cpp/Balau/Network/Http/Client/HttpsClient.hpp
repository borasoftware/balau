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
/// @file HttpsClient.hpp
///
/// A simple HTTPS client.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_CLIENT__HTTPS_CLIENT
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_CLIENT__HTTPS_CLIENT

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Utilities/root_certificates.hpp>

namespace Balau::Network::Http {

///
/// A simple HTTPS client.
///
/// The HTTPS client derives from the HTTP client in order to allow polymorphic
/// usage via a pointer container.
///
/// @todo Add chunked transfer.
///
class HttpsClient : public HttpClient {
	///
	/// Create an HTTPS client instance.
	///
	/// @param host_ the host name to connect to
	/// @param port_ the port number to connect to
	/// @param version_ either "1.0" or "1.1" (the default)
	/// @param userAgent_ the user agent string to send
	///
	public: explicit HttpsClient(std::string host_,
	                             unsigned short port_ = 443,
	                             std::string userAgent_ = "Balau " + BalauVersion,
	                             const char * version_ = "1.1")
		: HttpClient(std::move(host_), port_, std::move(userAgent_), version_) {}

	///
	/// Create an HTTPS client by copying the supplied instance.
	///
	public: HttpsClient(const HttpsClient &) = default;

	///
	/// Create an HTTPS client by moving the contents of the supplied instance.
	///
	public: HttpsClient(HttpsClient &&) = default;

	///
	/// Assign an HTTPS client by copying the supplied instance.
	///
	public: HttpsClient & operator = (const HttpsClient &) = default;

	///
	/// Assign an HTTPS client by moving the contents of the supplied instance.
	///
	public: HttpsClient & operator = (HttpsClient &&) = default;

	///
	/// Perform a GET request.
	///
	public: CharVectorResponse get(const std::string_view & path) override {
		return sendRequest<CharVectorResponse>(Method::get, path, "");
	}

	///
	/// Perform a HEAD request.
	///
	public: EmptyResponse head(const std::string_view & path) override {
		return sendRequest<EmptyResponse>(Method::head, path, "");
	}

	///
	/// Perform a POST request.
	///
	public: CharVectorResponse post(const std::string_view & path, const std::string_view & body) override {
		return sendRequest<CharVectorResponse>(Method::post, path, body);
	}

	////////////////////////// Private implementation /////////////////////////

	private: template <typename ResponseT>
	ResponseT sendRequest(Method verb, const std::string_view & path, const std::string_view & body) {
		/////////////// Connect ///////////////

		boost::asio::io_context ioc;
		SSL::context ctx { SSL::context::tls_client };
		load_root_certificates(ctx);
		TCP::resolver resolver { ioc };
		SSL::stream<TCP::socket> stream { ioc, ctx };

		if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
			boost::system::error_code errorCode { static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
			throw boost::system::system_error{errorCode}; // TODO
		}

		auto resolverResults = resolver.resolve(host.c_str(), ::toString(port).c_str());
		boost::asio::connect(stream.next_layer(), resolverResults.begin(), resolverResults.end());
		stream.handshake(SSL::stream_base::client);

		///////////// Send message /////////////

		StringRequest request { verb, path.empty() ? "/" : std::string(path), version };
		request.set(Field::host, host);
		request.set(Field::user_agent, userAgent);
		request.set(Field::accept, "text/html");

		if (!body.empty()) {
			request.body() = std::string(body);
			request.prepare_payload();
		}

		HTTP::write(stream, request);

		Buffer buffer {};
		ResponseT response;
		HTTP::read(stream, buffer, response);
		boost::system::error_code errorCode {};
		stream.shutdown(errorCode);

		////////////// Disconnect /////////////

		if (errorCode == boost::asio::error::eof) {
			errorCode.assign(0, errorCode.category());
		} else if (errorCode == boost::asio::ssl::error::stream_truncated) {
			// NOP?
		} else if (errorCode) {
			throw boost::system::system_error { errorCode }; // TODO
		}

		///////////////////////////////////////

		return response;
	}
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_CLIENT__HTTPS_CLIENT
