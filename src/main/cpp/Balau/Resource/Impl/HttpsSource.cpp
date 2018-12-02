// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpsSource.hpp"
#include "../Https.hpp"
#include "../../Exception/ResourceExceptions.hpp"

namespace Balau::Resource::Impl {

HttpsSource::HttpsSource(const Https & url_)
	: https(std::make_unique<Https>(url_))
	, url(url_)
	, client(createClient())
	, responseBody(getBody())
	, currentPosition(0) {}

HttpsSource::~HttpsSource() {}

std::streamsize HttpsSource::read(char * s, std::streamsize n) {
	if (currentPosition >= (std::streamsize) responseBody.size()) {
		return 0;
	}

	const auto bytesAvailable = (std::streamsize) responseBody.size() - currentPosition;
	const auto bytesToProvide = bytesAvailable < n ? bytesAvailable : n;

	std::memcpy(s, responseBody.data() + currentPosition, (size_t) bytesToProvide);
	currentPosition += bytesToProvide;
	return bytesToProvide;
}

const Https & HttpsSource::getUrl() const {
	return *https;
}

Network::Http::HttpsClient HttpsSource::createClient() {
	if (!url.hasHost()) {
		ThrowBalauException(
			Exception::InvalidUriException, "A URL with host information is required: " + toString(url)
		);
	}

	if (!url.hasPort()) {
		// Default to port 443.
		return Network::Http::HttpsClient(std::string(url.host()), 443);
	} else {
		return Network::Http::HttpsClient(std::string(url.host()), url.port());
	}
}

std::vector<char> HttpsSource::getBody() {
	auto response = client.get(url.pathQueryFragment());
	return response.body();
}

} // namespace Balau::Resource::Impl
