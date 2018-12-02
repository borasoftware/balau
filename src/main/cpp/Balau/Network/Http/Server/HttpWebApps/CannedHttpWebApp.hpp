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
/// @file CannedHttpWebApp.hpp
///
/// An HTTP web application handler that serves a fixed response for each request method.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__CANNED_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__CANNED_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau::Network::Http::HttpWebApps {

///
/// An HTTP web application handler that serves a fixed response for each request method.
///
class CannedHttpWebApp : public HttpWebApp {
	///
	/// Create a canned handler that has get and post method bodies.
	///
	/// If a get only or post only canned handler is required, pass the empty string to the
	/// get or post response body string.
	///
	/// Any empty bodied get/post body strings will result in bad requests for the respective methods.
	///
	/// @param mimeType_ the mime type to be returned in responses
	/// @param getResponseBody_ the response body to send or get requests
	/// @param postResponseBody_ the response body to send or post requests
	///
	public: CannedHttpWebApp(std::string mimeType_, std::string getResponseBody_, std::string postResponseBody_)
		: mimeType(std::move(mimeType_))
		, getResponseBody(std::move(getResponseBody_))
		, postResponseBody(std::move(postResponseBody_)) {}

	public: void handleGetRequest(HttpSession & session, const StringRequest & request) override;

	public: void handleHeadRequest(HttpSession & session, const StringRequest & request) override;

	public: void handlePostRequest(HttpSession & session, const StringRequest & request) override;

	///////////////////////// Private implementation //////////////////////////

	private: void handle(HttpSession & session, const StringRequest & request, const std::string & body);

	private: const std::string mimeType;
	private: const std::string getResponseBody;
	private: const std::string postResponseBody;
};

} // namespace Balau::Network::Http::HttpWebApps

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__CANNED_WEB_APP
