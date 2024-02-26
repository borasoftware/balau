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

///
/// @file FailingHttpWebApp.hpp
///
/// An HTTP web application handler that returns 404 for post, get, and head requests.
///

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS_FAILING_HTTP_WEB_APP
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS_FAILING_HTTP_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau{

class BalauLogger;

namespace Network::Http::HttpWebApps {

class FailingHttpWebApp : public HttpWebApp {
	public: void handleGetRequest(HttpSession & session,
	                              const StringRequest & request,
	                              std::map<std::string, std::string> & variables) override;

	public: void handleHeadRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	public: void handlePostRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS_FAILING_HTTP_WEB_APP
