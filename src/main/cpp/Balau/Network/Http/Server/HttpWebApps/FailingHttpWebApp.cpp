// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FailingHttpWebApp.hpp"
#include "../HttpSession.hpp"
#include "../../../../Logging/Impl/BalauLogger.hpp"

namespace Balau::Network::Http::HttpWebApps {


void FailingHttpWebApp::handleGetRequest(HttpSession & session,
                                         const StringRequest & request,
                                         std::map<std::string, std::string> & ) {
	session.sendResponse(createNotFoundStringResponse(session, request));
}

void FailingHttpWebApp::handleHeadRequest(HttpSession & session,
                                          const StringRequest & request,
                                          std::map<std::string, std::string> & ) {
	session.sendResponse(createNotFoundStringResponse(session, request));
}

void FailingHttpWebApp::handlePostRequest(HttpSession & session,
                                          const StringRequest & request,
                                          std::map<std::string, std::string> & ) {
	session.sendResponse(createNotFoundStringResponse(session, request));
}

} // namespace Balau::Network::Http::HttpWebApps
