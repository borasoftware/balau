// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpsByteReadResourceTest.hpp"

#include <Balau/Resource/Https.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::startsWith;
using Testing::endsWith;

namespace Resource {

void HttpsByteReadResourceTest::emptyPath() {
	performTest("https://borasoftware.com");
}

void HttpsByteReadResourceTest::nonEmptyPath() {
	performTest("https://borasoftware.com/index.html");
}

void HttpsByteReadResourceTest::performTest(const std::string & url_) {
	Https url(url_);

	auto httpsReadResource = url.getByteReadResource();
	auto uriReadResource = url.byteReadResource();

	std::istream & httpsReadStream = httpsReadResource.readStream();
	std::istream & uriReadStream = uriReadResource->readStream();

	const std::string expectedStart = "<!DOCTYPE html";
	const std::string expectedEnd = "</html>\n";

	auto actualHttpsData = ::toString(httpsReadStream);
	auto actualUriData = ::toString(uriReadStream);

	assertThat(actualHttpsData, startsWith(expectedStart));
	assertThat(actualUriData, startsWith(expectedStart));

	assertThat(actualHttpsData, endsWith(expectedEnd));
	assertThat(actualUriData, endsWith(expectedEnd));
}

} // namespace Resource

} // namespace Balau
