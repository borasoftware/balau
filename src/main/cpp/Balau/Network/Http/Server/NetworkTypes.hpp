// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__NETWORK_TYPES
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__NETWORK_TYPES

///
/// @file NetworkTypes.hpp
///
/// Low level types used in networking code.
///

#include <Balau/Type/ToString.hpp>
#include <Balau/Type/FromString.hpp>
#include <Balau/Util/Enums.hpp>
#include <Balau/ThirdParty/Boost/Beast/Http/BasicFileBody.hpp>

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/config.hpp>

namespace boost {

namespace asio::ip {

///
/// Print the IP address as a UTF-8 string.
///
/// @return a UTF-8 string representing the IP address
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const address & a) {
	// Currently not possible to avoid std::allocator.
	return ::toString<AllocatorT>(a.to_string());
}

///
/// Print the IP address as a UTF-8 string.
///
/// @return a UTF-8 string representing the IP address
///
inline std::string toString(const address & a) {
	return a.to_string();
}

///
/// Print the endpoint as a UTF-8 string.
///
/// @return a UTF-8 string representing the endpoint
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const tcp::endpoint & endpoint) {
	// Currently not possible to avoid std::allocator.
	return ::toString<AllocatorT>(endpoint.address().to_string(), ":", endpoint.port());
}

///
/// Print the endpoint as a UTF-8 string.
///
/// @return a UTF-8 string representing the endpoint
///
inline std::string toString(const tcp::endpoint & endpoint) {
	return ::toString(endpoint.address().to_string(), ":", endpoint.port());
}

///
/// Populate the endpoint by parsing the supplied string.
///
inline void fromString(tcp::endpoint & endpoint, std::string_view value) {
	// TODO improve implementation (ipv4 / ipv6).

	const auto v = std::string(value);
	static const std::regex numeric("[0-9]+");

	const bool isPortOnly = std::regex_match(v, numeric);

	if (isPortOnly) {
		unsigned short port;
		::fromString(port, value);
		endpoint = tcp::endpoint(make_address("127.0.0.1"), port);
		return;
	}

	auto lastColon = Balau::Util::Strings::lastIndexOf(value, ":");

	if (lastColon == std::string::npos) {
		// No port.
		endpoint = tcp::endpoint(make_address(v), 8080);
	} else {
		auto portStr = value.substr(lastColon + 1);

		if (Balau::Util::Strings::contains(portStr, "]")) {
			// No port.
			endpoint = tcp::endpoint(make_address(v), 8080);
		} else {
			auto ip = value.substr(0, lastColon);;
			unsigned short port;
			::fromString(port, portStr);
			endpoint = tcp::endpoint(make_address(std::string(ip)), port);
		}
	}
}

} // namespace asio::ip

namespace system {

///
/// Print the error code as a UTF-8 string.
///
/// @return a UTF-8 string representing the error code
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const error_code & errorCode) {
	// Currently not possible to avoid std::allocator.
	return Balau::U8String<AllocatorT>(errorCode.message());
}

///
/// Print the error code as a UTF-8 string.
///
/// @return a UTF-8 string representing the error code
///
inline std::string toString(const error_code & errorCode) {
	return errorCode.message();
}

} // namespace system

namespace beast::http {

///
/// Print the network status as a UTF-8 string.
///
/// @return a UTF-8 string representing the network status
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const status & s) {
	auto u = Balau::Util::Enums::toUnderlying(s);
	return Balau::U8String<AllocatorT>(detail::status_to_string<>((unsigned int) u));
}

///
/// Print the network status as a UTF-8 string.
///
/// @return a UTF-8 string representing the network status
///
inline std::string toString(const status & s) {
	auto u = Balau::Util::Enums::toUnderlying(s);
	return std::string(detail::status_to_string<>((unsigned int) u));
}

///
/// Print the network status as a UTF-8 string.
///
/// @return a UTF-8 string representing the network status
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const verb & method) {
	return Balau::U8String<AllocatorT>(to_string(method));
}

///
/// Print the network status as a UTF-8 string.
///
/// @return a UTF-8 string representing the network status
///
inline std::string toString(const verb & method) {
	return std::string(to_string(method));
}

///
/// Print the network status as a UTF-8 string.
///
/// @return a UTF-8 string representing the network status
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const field & f) {
	return Balau::U8String<AllocatorT>(to_string(f));
}

///
/// Print the network status as a UTF-8 string.
///
/// @return a UTF-8 string representing the network status
///
inline std::string toString(const field & f) {
	return std::string(to_string(f));
}

} // namespace beast::http

} // namespace boost

namespace Balau::Network {

///
/// An IP4 or IP6 address.
///
using Address = boost::asio::ip::address;

///
/// An IP address and port number.
///
using Endpoint = boost::asio::ip::tcp::endpoint;

///
/// A message body represented by a vector of 8 bit characters.
///
using CharVectorBody = boost::beast::http::vector_body<char>;

///
/// A message body represented by a UTF-8 string.
///
using StringBody = boost::beast::http::basic_string_body<char>;

///
/// A message body represented by a file on the filesystem.
///
using FileBody = Balau::Boost::Beast::Http::basic_file_body<boost::beast::file>;
//using FileBody = boost::beast::http::file_body;

///
/// The type of the message body represented by a file on the filesystem.
///
using FileBodyValue = FileBody::value_type;

///
/// A message body wth no content.
///
using EmptyBody = boost::beast::http::empty_body;

///
/// The request type.
///
template<class Body, class Fields = boost::beast::http::fields>
using Request = boost::beast::http::request<Body, Fields>;

///
/// A request with a string body.
///
using StringRequest = Request<StringBody>;

///
/// A request with a char vector body.
///
using CharVectorRequest = Request<CharVectorBody>;

///
/// The response type.
///
template<class Body, class Fields = boost::beast::http::fields>
using Response = boost::beast::http::response<Body, Fields>;

///
/// A response with a string body.
///
using StringResponse = Response<StringBody>;

///
/// A response with a char vector body.
///
using CharVectorResponse = Response<CharVectorBody>;

///
/// A response with an empty body.
///
using EmptyResponse = Response<EmptyBody>;

///
/// The HTTP method (GET, HEAD, POST).
///
using Method = boost::beast::http::verb;

///
/// An HTTP header field.
///
using Field = boost::beast::http::field;

///
/// A data buffer used in HTTP code.
///
using Buffer = boost::beast::flat_buffer;

///
/// Boost error codes returned from HTTP algorithms and operations.
///
using Error = boost::beast::http::error;

///
/// Boost system error.
///
using BoostSystemError = boost::system::system_error;

///
/// Boost system error codes.
///
using BoostSystemErrorCode = boost::system::error_code;

///
/// Boost HTTP file open modes.
///
using FileMode = boost::beast::file_mode;

///
/// Boost status code.
///
using Status = boost::beast::http::status;

///
/// The Boost SSL namespace.
///
namespace SSL = boost::asio::ssl;

///
/// The Boost TCP namespace.
///
using TCP = boost::asio::ip::tcp;

///
/// The Boost HTTP namespace.
///
namespace HTTP = boost::beast::http;

///
/// The Boost WebSocket namespace.
///
namespace WS = boost::beast::websocket;

///
/// WeSocket control frames.
///
using WsFrame = WS::frame_type;

///
/// Make an endpoint on the stack.
///
inline Endpoint makeEndpoint(const std::string & address, unsigned short port) {
	return Endpoint(boost::asio::ip::make_address(address), port);
}

///
/// Make an endpoint on the stack.
///
inline Endpoint makeEndpoint(const char * address, unsigned short port) {
	return Endpoint(boost::asio::ip::make_address(address), port);
}

///
/// Make an endpoint on the heap.
///
inline std::shared_ptr<Endpoint> newEndpoint(const std::string & address, unsigned short port) {
	return std::shared_ptr<Endpoint>(new Endpoint(boost::asio::ip::make_address(address), port));
}

///
/// Make an endpoint on the heap.
///
inline std::shared_ptr<Endpoint> newEndpoint(const char * address, unsigned short port) {
	return std::make_shared<Endpoint>(boost::asio::ip::make_address(address), port);
}

} // namespace Balau::Network

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__NETWORK_TYPES
