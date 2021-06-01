// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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

EmptyResponse HttpWebApp::createRedirectResponse(HttpSession & session,
                                                 const StringRequest & request,
                                                 std::string_view location) {
	Response<EmptyBody> response { Status::found, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::location, location);
	response.keep_alive(request.keep_alive());
	return response;
}

EmptyResponse HttpWebApp::createPermanentRedirectResponse(HttpSession & session,
                                                          const StringRequest & request,
                                                          std::string_view location) {
	Response<EmptyBody> response { Status::moved_permanently, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::location, location);
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
