// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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
/// @file ConfigurationWebApp.hpp
///
/// An HTTP web application that merges environment configuration documentation for serving.
///

#ifndef COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau::Documentation {

///
/// An HTTP web application that merges environment configuration documentation for serving.
///
/// @todo implement this web app
///
class ConfigurationWebApp : public Network::Http::HttpWebApp {
	public: void handleGetRequest(Network::Http::HttpSession & session,
	                              const Network::StringRequest & request,
	                              std::map<std::string, std::string> & variables) override;

	public: void handleHeadRequest(Network::Http::HttpSession & session,
	                               const Network::StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	public: void handlePostRequest(Network::Http::HttpSession & session,
	                               const Network::StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;
};

} // namespace Balau::Documentation

#endif // COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP
