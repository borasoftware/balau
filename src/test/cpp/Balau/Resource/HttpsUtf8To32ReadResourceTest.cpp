// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpsUtf8To32ReadResourceTest.hpp"

#include <Balau/Resource/Https.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::startsWith;
using Testing::endsWith;

namespace Resource {

void HttpsUtf8To32ReadResourceTest::emptyPath() {
	performTest("https://borasoftware.com");
}

void HttpsUtf8To32ReadResourceTest::nonEmptyPath() {
	performTest("https://borasoftware.com/index.html");
}

void HttpsUtf8To32ReadResourceTest::performTest(const std::string & url_) {
	Https url(url_);

	auto httpsReadResource = url.getUtf8To32ReadResource();
	auto uriReadResource = url.utf8To32ReadResource();

	std::u32istream & httpsReadStream = httpsReadResource.readStream();
	std::u32istream & uriReadStream = uriReadResource->readStream();

	const std::u32string expectedStart = U"<!DOCTYPE html";

	auto actualHttpsData = ::toString32(httpsReadStream);
	auto actualUriData = ::toString32(uriReadStream);

	assertThat(actualHttpsData, startsWith(expectedStart));
	assertThat(actualUriData, startsWith(expectedStart));
}

} // namespace Resource

} // namespace Balau
