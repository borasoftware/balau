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
/// @file EmailSendingHttpWebApp.hpp
///
/// An HTTP web application handler that sends an email when a post request is made.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__ROUTING_EMAIL_SENDER_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__ROUTING_EMAIL_SENDER_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/Impl/CurlEmailSender.hpp>

// Avoid false positive (due to make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Network::Http::HttpWebApps {

///
/// An HTTP web application handler that sends an email when a post request is made.
///
/// The get and head request handler methods will result in bad requests.
///
/// Before using an email sending handler, a translator function that maps between
/// the request parameters and the email body to be sent by the email sender is
/// required. This function is supplied to the handler constructor, and may be
/// provided via a lambda expression.
///
/// The request parameters originate from the calling client, and their nature
/// is determined by the web application. These must be translated into a body
/// message that will be sent by the email sending handler.
///
class EmailSendingHttpWebApp : public HttpWebApp {
//	BalauInject(
//		  bodyGenerator_
//		, successHandler_
//		, failureHandler_
//		, host
//		, port
//		, user
//		, pw
//		, userAgent
//		, subject_
//		, from_
//		, to_
//		, cc_
//		, useTLS_
//	);

	///
	/// The map type that holds request body parameters.
	///
	/// These are extracted from the POST request body, and the body generator
	/// function supplied to the constructor is called to translate them into
	/// the email body to be sent.
	///
	public: using ParameterMap = std::unordered_map<std::string_view, std::string_view> ;

	///
	/// Construct an email sender handler.
	///
	/// The handler is created by supplying the destination details and two response
	/// handlers, one for successful emailings and another for failed emailings.
	///
	/// One of the get request methods of the two handlers will be called after email
	/// processing has succeeded or failed.
	///
	public: EmailSendingHttpWebApp(std::function<std::string (const ParameterMap &)> bodyGenerator_,
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
	                               std::vector<std::string> cc_ = {},
	                               bool useTLS_ = true);

	public: void handleGetRequest(HttpSession & session, const StringRequest & request) override;

	public: void handleHeadRequest(HttpSession & session, const StringRequest & request) override;

	public: void handlePostRequest(HttpSession & session, const StringRequest & request) override;

	///////////////////////// Private implementation //////////////////////////

	private: std::function<std::string (const ParameterMap &)> bodyGenerator;
	private: std::shared_ptr<HttpWebApp> successHandler;
	private: std::shared_ptr<HttpWebApp> failureHandler;
	private: const std::string subject;
	private: const std::string from;
	private: const std::string to;
	private: const std::vector<std::string> cc;
	private: const bool useTLS;
	private: Impl::CurlEmailSender emailSender;
};

} // namespace Balau::Network::Http::HttpWebApps

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__ROUTING_EMAIL_SENDER_WEB_APP
