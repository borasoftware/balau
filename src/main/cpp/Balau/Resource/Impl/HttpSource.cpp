// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpSource.hpp"
#include "../Http.hpp"
#include "../../Exception/ResourceExceptions.hpp"

namespace Balau::Resource::Impl {

HttpSource::HttpSource(const Http & url_)
	: http(std::make_unique<Http>(url_))
	, url(url_)
	, client(createClient())
	, responseBody(getBody())
	, currentPosition(0) {}

HttpSource::~HttpSource() {}

std::streamsize HttpSource::read(char * s, std::streamsize n) {
	if (currentPosition >= (std::streamsize) responseBody.size()) {
		return 0;
	}

	const auto bytesAvailable = (std::streamsize) responseBody.size() - currentPosition;
	const auto bytesToProvide = bytesAvailable < n ? bytesAvailable : n;

	std::memcpy(s, responseBody.data() + currentPosition, (size_t) bytesToProvide);
	currentPosition += bytesToProvide;
	return bytesToProvide;
}

const Http & HttpSource::getUrl() const {
	return *http;
}

Network::Http::HttpClient HttpSource::createClient() {
	if (!url.hasHost()) {
		ThrowBalauException(
			Exception::InvalidUriException, "A URL with host information is required: " + toString(url)
		);
	}

	if (!url.hasPort()) {
		// Default to port 80.
		return Network::Http::HttpClient(std::string(url.host()), 80);
	} else {
		return Network::Http::HttpClient(std::string(url.host()), url.port());
	}
}

std::vector<char> HttpSource::getBody() {
	auto p = url.pathQueryFragment();
	auto response = client.get(p);
	return response.body();
}

} // namespace Balau::Resource::Impl
