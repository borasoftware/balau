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
#include "ConfigurationWebApp.hpp"
#include "../Network/Http/Server/HttpSession.hpp"

using namespace Balau::Network;
using namespace Balau::Network::Http;

namespace Balau::Documentation {

void ConfigurationWebApp::handleGetRequest(HttpSession & session,
                                           const StringRequest & request,
                                           std::map<std::string, std::string> & variables) {
	session.sendResponse(createServerErrorResponse(session, request, "Get requests are not implemented yet."));
}

void ConfigurationWebApp::handleHeadRequest(HttpSession & session,
                                            const StringRequest & request,
                                            std::map<std::string, std::string> & variables) {
	session.sendResponse(createServerErrorResponse(session, request, "Head requests are not implemented yet."));
}

void ConfigurationWebApp::handlePostRequest(HttpSession & session,
                                            const StringRequest & request,
                                            std::map<std::string, std::string> & variables) {
	session.sendResponse(createServerErrorResponse(session, request, "Post requests are not implemented."));
}

} // namespace Balau::Documentation
