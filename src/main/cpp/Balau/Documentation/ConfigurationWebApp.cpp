// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ConfigurationWebApp.hpp"
#include "../Network/Http/Server/HttpSession.hpp"

using namespace Balau::Network;
using namespace Balau::Network::Http;

namespace Balau::Documentation {

void ConfigurationWebApp::handleGetRequest(HttpSession & session,
                                           const StringRequest & request,
                                           std::map<std::string, std::string> & variables) {
	session.sendResponse(createServerErrorResponse(session, request, "Get requests are not implemented yet."));
}

void ConfigurationWebApp::handleHeadRequest(HttpSession & session,
                                            const StringRequest & request,
                                            std::map<std::string, std::string> & variables) {
	session.sendResponse(createServerErrorResponse(session, request, "Head requests are not implemented yet."));
}

void ConfigurationWebApp::handlePostRequest(HttpSession & session,
                                            const StringRequest & request,
                                            std::map<std::string, std::string> & variables) {
	session.sendResponse(createServerErrorResponse(session, request, "Post requests are not implemented."));
}

} // namespace Balau::Documentation
