// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CannedHttpWebApp.hpp"
#include "../HttpSession.hpp"

namespace Balau::Network::Http::HttpWebApps {

void CannedHttpWebApp::handleGetRequest(HttpSession & session, const StringRequest & request) {
	handle(session, request, getResponseBody);
}

void CannedHttpWebApp::handleHeadRequest(HttpSession & session, const StringRequest & request) {
	Response <StringBody> response {Status::ok, request.version()};
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, mimeType);
	response.set(Field::content_length, getResponseBody.size()); // TODO This is causing a deadlock.
	response.keep_alive(request.keep_alive());
	session.sendResponse(std::move(response));
}

void CannedHttpWebApp::handlePostRequest(HttpSession & session, const StringRequest & request) {
	handle(session, request, postResponseBody);
}

void CannedHttpWebApp::handle(HttpSession & session, const StringRequest & request, const std::string & body) {
	if (body.empty()) {
		session.sendResponse(createBadRequestResponse(session, request, "Not supported"));
	} else {
		Response <StringBody> response {Status::ok, request.version()};
		response.set(Field::server, session.configuration().serverId);
		response.set(Field::content_type, mimeType);
		response.body() = body;
		response.prepare_payload();
		response.keep_alive(request.keep_alive());
		session.sendResponse(std::move(response));
	}
}

} // namespace Balau::Network::Http::HttpWebApps
