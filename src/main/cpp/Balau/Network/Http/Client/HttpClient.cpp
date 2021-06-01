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

#include "HttpsClient.hpp"

#include "../../../Resource/UriComponents.hpp"

namespace Balau::Network::Http {

std::unique_ptr<HttpClient> HttpClient::newClient(const Resource::Url & url,
                                                  std::string userAgent,
                                                  const char * version) {
	return newClient(url.toUriString(), userAgent, version);
}

std::unique_ptr<HttpClient> HttpClient::newClient(const std::string & url,
                                                  std::string userAgent,
                                                  const char * version) {
	Resource::UriComponents components(url);

	if (!components.hasHost()) {
		ThrowBalauException(Exception::NetworkException, "The host is required in the supplied URL.");
	}

	const auto & scheme = components.scheme();

	if (scheme == "http") {
		const auto & host = components.host();
		const unsigned short port = components.hasPort() ? components.port() : (unsigned short) 80;
		return std::make_unique<HttpClient>(std::string(host), userAgent, port, version);
	} else if (scheme == "https") {
		const auto & host = components.host();
		const unsigned short port = components.hasPort() ? components.port() : (unsigned short) 443;
		return std::unique_ptr<HttpClient>(new HttpsClient(std::string(host), userAgent, port, version));
	} else {
		ThrowBalauException(Exception::NetworkException, "The scheme must be HTTP or HTTPS.");

		// Prevent compiler warning.
		return std::unique_ptr<HttpClient>();
	}
}

} // namespace Balau::Network::Http
