// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file ConfigurationWebApp.hpp
///
/// An HTTP web application that merges environment configuration documentation for serving.
///

#ifndef COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau::Documentation {

///
/// An HTTP web application that merges environment configuration documentation for serving.
///
/// @todo implement this web app
///
class ConfigurationWebApp : public Network::Http::HttpWebApp {
	public: void handleGetRequest(Network::Http::HttpSession & session,
	                              const Network::StringRequest & request,
	                              std::map<std::string, std::string> & variables) override;

	public: void handleHeadRequest(Network::Http::HttpSession & session,
	                               const Network::StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	public: void handlePostRequest(Network::Http::HttpSession & session,
	                               const Network::StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;
};

} // namespace Balau::Documentation

#endif // COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP
