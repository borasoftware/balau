// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "RoutingHttpWebApp.hpp"

#include "../HttpSession.hpp"

namespace Balau::Network::Http::HttpWebApps {

void RoutingHttpWebApp::sendNotFoundResponse(HttpSession & session, const StringRequest & request) {
	if (request.method() == Method::head) {
		session.sendResponse(createNotFoundHeadResponse(session, request));
	} else {
		session.sendResponse(createNotFoundStringResponse(session, request));
	}
}

} // namespace Balau::Network::Http::HttpWebApps
