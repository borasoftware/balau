// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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
		return std::make_unique<HttpClient>(std::string(host), port, userAgent, version);
	} else if (scheme == "https") {
		const auto & host = components.host();
		const unsigned short port = components.hasPort() ? components.port() : (unsigned short) 443;
		return std::unique_ptr<HttpClient>(new HttpsClient(std::string(host), port, userAgent, version));
	} else {
		ThrowBalauException(Exception::NetworkException, "The scheme must be HTTP or HTTPS.");
	}
}

} // namespace Balau::Network::Http
