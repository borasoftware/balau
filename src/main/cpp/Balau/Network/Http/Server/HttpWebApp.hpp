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
/// @file HttpWebApp.hpp
///
/// Abstract base class of HTTP web application handlers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__HTTP_WEB_APPLICATION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__HTTP_WEB_APPLICATION

#include <Balau/Network/Http/Server/NetworkTypes.hpp>

namespace Balau::Network::Http {

class HttpSession;

///
/// Abstract base class of HTTP web application handlers.
///
/// This class also contains some canned responses for use in handler implementations.
///
class HttpWebApp {
	///
	/// Create an empty 200 response.
	///
	public: static StringResponse createOkResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create an empty 200 response for a head request.
	///
	public: static EmptyResponse createOkHeadResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create a bad request response.
	///
	public: static StringResponse createBadRequestResponse(HttpSession & session,
	                                                       const StringRequest & request,
	                                                       std::string_view errorMessage);

	///
	/// Create a bad request response for a head request.
	///
	public: static EmptyResponse createBadRequestHeadResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create a not found response.
	///
	static StringResponse createNotFoundStringResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create a not found response for a head request.
	///
	static EmptyResponse createNotFoundHeadResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create a server error response.
	///
	public: static StringResponse createServerErrorResponse(HttpSession & session,
	                                                        const StringRequest & request,
	                                                        std::string_view errorMessage);

	///
	/// Create a server error response for a head request.
	///
	public: static EmptyResponse createServerErrorHeadResponse(HttpSession & session, const StringRequest & request);

	///////////////////////////////////////////////////////////////////////////

	///
	/// Handle a GET request.
	///
	/// The supplied session provides shared server state and the sendResponse method.
	///
	public: virtual void handleGetRequest(HttpSession & session, const StringRequest & request) = 0;

	///
	/// Handle a HEAD request.
	///
	/// The supplied session provides shared server state and the sendResponse method.
	///
	public: virtual void handleHeadRequest(HttpSession & session, const StringRequest & request) = 0;

	///
	/// Handle a POST request.
	///
	/// The supplied session provides shared server state and the sendResponse method.
	///
	public: virtual void handlePostRequest(HttpSession & session, const StringRequest & request) = 0;

	///////////////////////////////////////////////////////////////////////////

	///
	/// Destroy the HTTP web application instance.
	///
	public: virtual ~HttpWebApp() = default;
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__HTTP_WEB_APPLICATION
