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
#include "Balau/Network/Utilities/UrlDecode.hpp"
#include "../../../../Application/EnvironmentProperties.hpp"
#include "../../../../Logging/Impl/BalauLogger.hpp"

namespace Balau::Network::Http::HttpWebApps {

EmailSendingHttpWebApp::EmailSendingHttpWebApp(BodyGenerator bodyGenerator_,
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
	, successRedirectLocation()
	, failureRedirectLocation()
	, subject(std::move(subject_))
	, from(std::move(from_))
	, to(std::move(to_))
	, cc(std::move(cc_))
	, useTLS(useTLS_) // TODO verify cert option
	, emailSender(std::move(host), port, std::move(user), std::move(pw), std::move(userAgent), false)
	, log(new BalauLogger("balau.email.sender")) {}

EmailSendingHttpWebApp::EmailSendingHttpWebApp(BodyGenerator bodyGenerator_,
                                               std::string successRedirectLocation_,
                                               std::string failureRedirectLocation_,
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
	, successHandler(std::shared_ptr<HttpWebApp>())
	, failureHandler(std::shared_ptr<HttpWebApp>())
	, successRedirectLocation(std::move(successRedirectLocation_))
	, failureRedirectLocation(std::move(failureRedirectLocation_))
	, subject(std::move(subject_))
	, from(std::move(from_))
	, to(std::move(to_))
	, cc(std::move(cc_))
	, useTLS(useTLS_) // TODO verify cert option
	, emailSender(std::move(host), port, std::move(user), std::move(pw), std::move(userAgent), false)
	, log(new BalauLogger("balau.email.sender")) {}

EmailSendingHttpWebApp::EmailSendingHttpWebApp(const EnvironmentProperties & configuration, const BalauLogger & )
	: bodyGenerator(createBodyGenerator(configuration))
	, successHandler(std::shared_ptr<HttpWebApp>())
	, failureHandler(std::shared_ptr<HttpWebApp>())
	, successRedirectLocation(configuration.getValue<std::string>("success"))
	, failureRedirectLocation(configuration.getValue<std::string>("failure"))
	, subject(configuration.getValue<std::string>("subject"))
	, from(configuration.getValue<std::string>("from"))
	, to(configuration.getValue<std::string>("to"))
	, cc() // TODO
	, useTLS(true) // TODO config
	, emailSender(
		  configuration.getValue<std::string>("host")
		, verifyPort(configuration.getValue<int>("port"))
		, configuration.getValue<std::string>("user")
		, configuration.getValue<std::string>("password")
		, configuration.getValue<std::string>("user-agent")
		, false
	)
	, log(new BalauLogger("balau.email.sender")) {}

void EmailSendingHttpWebApp::handleGetRequest(HttpSession & session,
                                              const StringRequest & request,
                                              std::map<std::string, std::string> & ) {
	session.sendResponse(createBadRequestHeadResponse(session, request));
}

void EmailSendingHttpWebApp::handleHeadRequest(HttpSession & session,
                                               const StringRequest & request,
                                               std::map<std::string, std::string> & ) {
	session.sendResponse(createBadRequestHeadResponse(session, request));
}

void EmailSendingHttpWebApp::handlePostRequest(HttpSession & session,
                                               const StringRequest & request,
                                               std::map<std::string, std::string> & variables) {
	try {
		const ParameterMap parameters = UrlDecode::splitAndDecode(request.body());
		const std::string body = bodyGenerator(session, request, variables, parameters);
		emailSender.sendEmail(from, to, cc, subject, body);

		if (successHandler) {
			successHandler->handlePostRequest(session, request, variables);
		} else {
			session.sendResponse(createRedirectResponse(session, request, successRedirectLocation), successRedirectLocation);
		}

		return;
	} catch (const Exception::BalauException & e) {
		log->error("Error sending email: BalauException thrown: {}", e.what());
	} catch (const std::exception & e) {
		log->error("Error sending email: std::exception thrown: {}", e.what());
	} catch (...) {
		log->error("Error sending email: Unknown exception thrown.");
	}

	if (failureHandler) {
		failureHandler->handlePostRequest(session, request, variables);
	} else {
		session.sendResponse(createRedirectResponse(session, request, failureRedirectLocation), failureRedirectLocation);
	}
}

unsigned short EmailSendingHttpWebApp::verifyPort(int port) {
	if (port < 0 || port > USHRT_MAX) {
		ThrowBalauException(
			Exception::NetworkException, ::toString("Email sender specified port (", port, ") is out of range.")
		);
	}

	return (unsigned short) port;
}

EmailSendingHttpWebApp::BodyGenerator EmailSendingHttpWebApp::createBodyGenerator(const EnvironmentProperties & configuration) {
	if (!configuration.hasComposite("parameters")) {
		ThrowBalauException(
			Exception::NetworkException, "Missing \"parameters\" configuration composite property for email sender."
		);
	}

	auto parameterProperties = configuration.getComposite("parameters");
	std::map<int, std::string> parameterMap;

	for (const auto & parameterProperty : *parameterProperties) {
		if (parameterProperty.isComposite()) {
			// TODO
//			BalauBalauLogWarn(
//				  logger
//				, "Ignoring simple property \"{}\" found in the http web application configuration."
//				, webAppConfiguration.getName()
//			);
		} else {
			const auto name = std::string(parameterProperty.getName());

			if (!parameterProperty.isValue<std::string>()) {
				ThrowBalauException(
					Exception::NetworkException, "Invalid email sender parameter property value (should be a string)."
				);
			}

			auto positionStr = parameterProperty.getValue<std::string>();

			try {
				int position;
				fromString(position, positionStr);
				parameterMap[position] = name;
			} catch (...) {
				ThrowBalauException(
					  Exception::NetworkException
					, "Invalid email sender parameter property value (failed to parse string as int)."
				);
			}

		}
	}

	// Create a vector of the parameter names from the map in the natural order.
	std::vector<std::string> parameterNames;

	for (auto entry : parameterMap) {
		parameterNames.push_back(std::move(entry.second));
	}

	return [parameterNames] (HttpSession & session,
	                         const StringRequest & request,
	                         std::map<std::string, std::string> & variables,
	                         const ParameterMap & parameters) -> std::string {
		std::ostringstream message;

		message << "\r\n";

		// Use X-Real-IP if available, otherwise the session client IP.
		const std::string realIp1 = ::toString(request["X-Real-IP"]);
		const std::string realIp2 = ::toString(request["HTTP_X_REAL_IP"]);

		if (!realIp1.empty()) {
			message << "IP: " << realIp1 << "\r\n";
		} else if (!realIp2.empty()) {
			message << "IP: " << realIp2 << "\r\n";
		} else {
			message << "IP: " << session.remoteIpAddress().to_string() << "\r\n";
		}

		for (auto parameterName : parameterNames) {
			auto iter = parameters.find(parameterName);
			auto value = iter != parameters.end() ? iter->second : "";

			message << parameterName << ":\r\n" << value << "\r\n\r\n";
		}

		return message.str();
	};
}

} // namespace Balau::Network::Http::HttpWebApps
