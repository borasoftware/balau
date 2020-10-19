// @formatter:off
//
// Balau core C++ library
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

///
/// @file EmailSendingHttpWebApp.hpp
///
/// An HTTP web application handler that sends an email when a post request is made.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__EMAIL_SENDING_HTTP_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__EMAIL_SENDING_HTTP_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/Impl/CurlEmailSender.hpp>

// Avoid false positive (due to make_shared).
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau{

class BalauLogger;
class EnvironmentProperties;

namespace Network::Http::HttpWebApps {

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
	///
	/// The map type that holds request body parameters.
	///
	/// These are extracted from the POST request body, and the body generator
	/// function supplied to the constructor is called to translate them into
	/// the email body to be sent.
	///
	public: using ParameterMap = std::unordered_map<std::string, std::string> ;

	///
	/// The type of the body generator function.
	///
	public: using BodyGenerator = std::function<std::string (HttpSession & session,
	                                                         const StringRequest & request,
	                                                         std::map<std::string, std::string> & variables,
	                                                         const ParameterMap &)>;

	///
	/// Construct an email sender handler with success/failure handlers.
	///
	/// The email handler is created by supplying the destination details and two response
	/// handlers, one for successful emailings and another for failed emailings.
	///
	/// One of the get request methods of the two handlers will be called after email
	/// processing has succeeded or failed.
	///
	public: EmailSendingHttpWebApp(BodyGenerator bodyGenerator_,
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

	///
	/// Construct an email sender handler with success/failure redirects.
	///
	/// The email handler is created by supplying the destination details and two response
	/// handlers, one for successful emailings and another for failed emailings.
	///
	/// One of the get request methods of the two handlers will be called after email
	/// processing has succeeded or failed.
	///
	public: EmailSendingHttpWebApp(BodyGenerator bodyGenerator_,
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
	                               std::vector<std::string> cc_ = {},
	                               bool useTLS_ = true);

	///
	/// Constructor called by the HTTP server during construction.
	///
	/// @param configuration The environment configuration for the email sender.
	///
	public: EmailSendingHttpWebApp(const EnvironmentProperties & configuration, const BalauLogger & logger);

	public: void handleGetRequest(HttpSession & session,
	                              const StringRequest & request,
	                              std::map<std::string, std::string> & variables) override;

	public: void handleHeadRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	public: void handlePostRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	///////////////////////// Private implementation //////////////////////////

	private: static unsigned short verifyPort(int port);

	private: static BodyGenerator createBodyGenerator(const EnvironmentProperties & configuration);

	private: BodyGenerator bodyGenerator;
	private: const std::shared_ptr<HttpWebApp> successHandler;
	private: const std::shared_ptr<HttpWebApp> failureHandler;
	private: const std::string successRedirectLocation;
	private: const std::string failureRedirectLocation;
	private: const std::string subject;
	private: const std::string from;
	private: const std::string to;
	private: const std::vector<std::string> cc;
	private: const bool useTLS;
	private: Impl::CurlEmailSender emailSender;
	private: std::unique_ptr<BalauLogger> log;
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__EMAIL_SENDING_HTTP_WEB_APP
