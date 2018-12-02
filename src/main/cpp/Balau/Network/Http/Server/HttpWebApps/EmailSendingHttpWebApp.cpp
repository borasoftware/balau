// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "EmailSendingHttpWebApp.hpp"
#include "../HttpSession.hpp"
#include "../../../Utilities/UrlEncodeSplit.hpp"

namespace Balau::Network::Http::HttpWebApps {

EmailSendingHttpWebApp::EmailSendingHttpWebApp(std::function<std::string (const ParameterMap &)> bodyGenerator_,
                                               std::shared_ptr<HttpWebApp> successHandler_,
                                               std::shared_ptr<HttpWebApp> failureHandler_,
                                               std::string host,
                                               unsigned short port,
                                               std::string user,
                                               std::string pw,
                                               std::string userAgent,
                                               std::string subject_,
                                               std::string from_,
                                               std::string to_,
                                               std::vector<std::string> cc_,
                                               bool useTLS_)
	: bodyGenerator(std::move(bodyGenerator_))
	, successHandler(std::move(successHandler_))
	, failureHandler(std::move(failureHandler_))
	, subject(std::move(subject_))
	, from(std::move(from_))
	, to(std::move(to_))
	, cc(std::move(cc_))
	, useTLS(useTLS_)
	, emailSender(std::move(host), port, std::move(user), std::move(pw), std::move(userAgent), false) {} // TODO verify cert option

void EmailSendingHttpWebApp::handleGetRequest(HttpSession & session, const StringRequest & request) {
	session.sendResponse(
		createServerErrorResponse(session, request, "Get requests are not implemented.")
	);
}

void EmailSendingHttpWebApp::handleHeadRequest(HttpSession & session, const StringRequest & request) {
	session.sendResponse(
		createServerErrorResponse(session, request, "Head requests are not implemented.")
	);
}

void EmailSendingHttpWebApp::handlePostRequest(HttpSession & session, const StringRequest & request) {
	try {
		const ParameterMap parameters = UrlEncodeSplit::split(request.body());
		const std::string body = bodyGenerator(parameters);
		emailSender.sendEmail(from, to, cc, subject, body);
		successHandler->handlePostRequest(session, request);
	} catch (...) {
		failureHandler->handlePostRequest(session, request);
	}
}

} // namespace Balau::Network::Http::HttpWebApps
