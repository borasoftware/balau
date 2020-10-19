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
/// @file CannedHttpWebApp.hpp
///
/// An HTTP web application handler that serves a fixed response for each request method.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__CANNED_HTTP_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__CANNED_HTTP_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau {

class BalauLogger;
class EnvironmentProperties;

namespace Network::Http::HttpWebApps {

///
/// An HTTP web application handler that serves a fixed response for each request method.
///
class CannedHttpWebApp : public HttpWebApp {
	///
	/// Create a canned handler that has get and post method bodies.
	///
	/// If a get only or post only canned handler is required, pass the empty string to the
	/// get or post response body string.
	///
	/// Any empty bodied get/post body strings will result in bad requests for the respective methods.
	///
	/// @param mimeType_ the mime type to be returned in responses
	/// @param getResponseBody_ the response body to send or get requests
	/// @param postResponseBody_ the response body to send or post requests
	///
	public: CannedHttpWebApp(std::string mimeType_, std::string getResponseBody_, std::string postResponseBody_);

	///
	/// Constructor used by the HTTP server.
	///
	public: CannedHttpWebApp(const EnvironmentProperties & configuration, const BalauLogger & logger);

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

	private: void handle(HttpSession & session, const StringRequest & request, const std::string & body);

	private: const std::string mimeType;
	private: const std::string getResponseBody;
	private: const std::string postResponseBody;
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__CANNED_HTTP_WEB_APP
