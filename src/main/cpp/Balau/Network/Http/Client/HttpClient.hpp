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
/// @file HttpClient.hpp
///
/// A simple HTTP client.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_CLIENT__HTTP_CLIENT
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_CLIENT__HTTP_CLIENT

#include <Balau/Type/ToString.hpp>
#include <Balau/Exception/NetworkExceptions.hpp>
#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Resource/Url.hpp>
#include <Balau/Type/StdTypes.hpp>

namespace Balau::Network::Http {

///
/// A simple HTTP client.
///
/// @todo Add chunked transfer.
///
class HttpClient {
	///
	/// Create a new HTTP or HTTPS client, according to the supplied URL.
	///
	/// @param url the url containing the scheme, host, and port information
	/// @param userAgent the user agent string to send
	/// @param version either "1.0" or "1.1" (the default)
	/// @throw InvalidUriException if the URI is not well formed
	/// @throw NetworkException if host information is not provided in the URI or the scheme is not "http" or "https"
	///
	public: static std::unique_ptr<HttpClient> newClient(const Resource::Url & url,
	                                                     std::string userAgent = "Balau " + BalauVersion,
	                                                     const char * version = "1.1");

	///
	/// Create a new HTTP or HTTPS client, according to the supplied URL string.
	///
	/// @param url the url string containing the scheme, host, and port information
	/// @param userAgent the user agent string to send
	/// @param version either "1.0" or "1.1" (the default)
	/// @throw InvalidUriException if the URI is not well formed
	/// @throw NetworkException if host information is not provided in the URI or the scheme is not "http" or "https"
	///
	public: static std::unique_ptr<HttpClient> newClient(const std::string & url,
	                                                     std::string userAgent = "Balau " + BalauVersion,
	                                                     const char * version = "1.1");

	///
	/// Create an HTTP client instance.
	///
	/// @param host_ the host name to connect to
	/// @param port_ the port number to connect to
	/// @param userAgent_ the user agent string to send
	/// @param version_ either "1.0" or "1.1" (the default)
	///
	public: explicit HttpClient(std::string host_,
	                            unsigned short port_ = 80,
	                            std::string userAgent_ = "Balau " + BalauVersion,
	                            const char * version_ = "1.1")
		: host(std::move(host_))
		, port(port_)
		, userAgent(std::move(userAgent_))
		, version(parseVersion(version_)) {}

	///
	/// Create an HTTP client by copying the supplied instance.
	///
	public: HttpClient(const HttpClient &) = default;

	///
	/// Create an HTTP client by moving the contents of the supplied instance.
	///
	public: HttpClient(HttpClient &&) = default;

	///
	/// Assign an HTTP client by copying the supplied instance.
	///
	public: HttpClient & operator = (const HttpClient &) = default;

	///
	/// Assign an HTTP client by moving the contents of the supplied instance.
	///
	public: HttpClient & operator = (HttpClient &&) = default;

	///
	/// Destroy the client instance.
	///
	public: virtual ~HttpClient() = default;

	///
	/// Perform a GET request.
	///
	/// @throw NetworkException if an invalid HTTP version was supplied to the client
	///
	public: virtual CharVectorResponse get(const std::string_view & path) {
		return sendRequest<CharVectorResponse>(Method::get, path, "");
	}

	///
	/// Perform a HEAD request.
	///
	/// @throw NetworkException if an invalid HTTP version was supplied to the client
	///
	public: virtual EmptyResponse head(const std::string_view & path) {
		return sendRequest<EmptyResponse>(Method::head, path, "");
	}

	///
	/// Perform a POST request.
	///
	/// @throw NetworkException if an invalid HTTP version was supplied to the client
	///
	public: virtual CharVectorResponse post(const std::string_view & path, const std::string_view & body) {
		return sendRequest<CharVectorResponse>(Method::post, path, body);
	}

	////////////////////////// Private implementation /////////////////////////

	protected: std::string host;
	protected: unsigned short port;
	protected: std::string userAgent;
	protected: int version;

	private: template <typename ResponseT>
	ResponseT sendRequest(Method verb, const std::string_view & path, const std::string_view & body);

	private: int parseVersion(const char * versionString) {
		if (std::strcmp(versionString, "1.0") == 0) {
			return 10;
		} else if (std::strcmp(versionString, "1.1") == 0) {
			return 11;
		} else {
			ThrowBalauException(
				Exception::NetworkException, "Invalid HTTP version supplied to client: " + ::toString(versionString)
			);
		}
	}
};

template <typename ResponseT>
ResponseT HttpClient::sendRequest(Method verb, const std::string_view & path, const std::string_view & body) {
	/////////////// Connect ///////////////

	boost::asio::io_context ioc;
	AsioTCP::resolver resolver {ioc };
	AsioTCP::socket socket {ioc };

	auto resolverResults = resolver.resolve(host.c_str(), ::toString(port).c_str());
	boost::asio::connect(socket, resolverResults.begin(), resolverResults.end());

	///////////// Send message /////////////

	StringRequest request { verb, path.empty() ? "/" : std::string(path), version };
	request.set(Field::host, host);
	request.set(Field::user_agent, userAgent);
	request.set(Field::accept, "text/html");

	if (!body.empty()) {
		request.body() = std::string(body);
		request.prepare_payload();
	}

	HTTP::write(socket, request);

	Buffer buffer {};
	ResponseT response;
	HTTP::read(socket, buffer, response);

	////////////// Disconnect /////////////

	boost::system::error_code errorCode {};
	socket.shutdown(AsioTCP::socket::shutdown_both, errorCode);

	// TODO clarify the following.
	if (errorCode && errorCode != boost::system::errc::not_connected) {
		throw boost::system::system_error { errorCode }; // TODO
	}

	///////////////////////////////////////

	return response;
}

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_CLIENT__HTTP_CLIENT
