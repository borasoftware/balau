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
	performTest("http://borasoftware.com", U"<html>\r\n<head><title>302 Found</title></head>");
}

void HttpsUtf8To32ReadResourceTest::nonEmptyPath() {
	performTest("https://borasoftware.com/en/index.html", U"<!DOCTYPE html");
}

void HttpsUtf8To32ReadResourceTest::performTest(const std::string & url_, const std::u32string & expectedStart) {
	Https url(url_);

	auto httpsReadResource = url.getUtf8To32ReadResource();
	auto uriReadResource = url.utf8To32ReadResource();

	std::u32istream & httpsReadStream = httpsReadResource.readStream();
	std::u32istream & uriReadStream = uriReadResource->readStream();

	auto actualHttpsData = ::toString32(httpsReadStream);
	auto actualUriData = ::toString32(uriReadStream);

	assertThat(actualHttpsData, startsWith(expectedStart));
	assertThat(actualUriData, startsWith(expectedStart));
}

} // namespace Resource

} // namespace Balau
