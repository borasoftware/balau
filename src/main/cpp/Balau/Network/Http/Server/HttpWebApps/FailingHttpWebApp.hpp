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
/// @file FailingHttpWebApp.hpp
///
/// An HTTP web application handler that returns 404 for post, get, and head requests.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__FAILING_HTTP_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__FAILING_HTTP_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau{

class BalauLogger;

namespace Network::Http::HttpWebApps {

class FailingHttpWebApp : public HttpWebApp {
	public: void handleGetRequest(HttpSession & session,
	                              const StringRequest & request,
	                              std::map<std::string, std::string> & variables) override;

	public: void handleHeadRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	public: void handlePostRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__FAILING_HTTP_WEB_APP
