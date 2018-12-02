// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpWebApp.hpp"
#include "HttpSession.hpp"

namespace Balau::Network::Http {

StringResponse HttpWebApp::createOkResponse(HttpSession & session, const StringRequest & request) {
	Response<StringBody> response { Status::ok, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());

	response.body() = "";
	response.prepare_payload();

	return response;
}

EmptyResponse HttpWebApp::createOkHeadResponse(HttpSession & session, const StringRequest & request) {
	Response<EmptyBody> response { Status::ok, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());
	return response;
}

StringResponse HttpWebApp::createBadRequestResponse(HttpSession & session,
                                                    const StringRequest & request,
                                                    std::string_view errorMessage) {
	Response<StringBody> response { Status::bad_request, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());

	response.body() = std::string(errorMessage);
	response.prepare_payload();

	return response;
}

EmptyResponse HttpWebApp::createBadRequestHeadResponse(HttpSession & session, const StringRequest & request) {
	Response<EmptyBody> response { Status::bad_request, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());
	return response;
}

StringResponse HttpWebApp::createNotFoundStringResponse(HttpSession & session, const StringRequest & request) {
	Response<StringBody> response { Status::not_found, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());
	response.body() = "The resource '" + std::string(request.target()) + "' was not found.";
	response.prepare_payload();
	return response;
}

EmptyResponse HttpWebApp::createNotFoundHeadResponse(HttpSession & session, const StringRequest & request) {
	Response<EmptyBody> response { Status::not_found, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());
	return response;
}

StringResponse HttpWebApp::createServerErrorResponse(HttpSession & session,
                                                     const StringRequest & request,
                                                     std::string_view errorMessage) {
	Response<StringBody> response { Status::internal_server_error, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());
	response.body() = "An error occurred: '" + std::string(errorMessage) + "'";
	response.prepare_payload();
	return response;
}

EmptyResponse HttpWebApp::createServerErrorHeadResponse(HttpSession & session, const StringRequest & request) {
	Response<EmptyBody> response { Status::internal_server_error, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, "text/html");
	response.keep_alive(request.keep_alive());
	return response;
}

} // namespace Balau::Network::Http
