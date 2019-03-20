// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpUtf8To32ReadResourceTest.hpp"

#include <Balau/Resource/Http.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::startsWith;

namespace Resource {

void HttpUtf8To32ReadResourceTest::emptyPath() {
	performTest("http://borasoftware.com");
}

void HttpUtf8To32ReadResourceTest::nonEmptyPath() {
	performTest("http://borasoftware.com/blah");
}

void HttpUtf8To32ReadResourceTest::performTest(const std::string & url_) {
	Http url(url_);

	auto httpReadResource = url.getUtf8To32ReadResource();
	auto uriReadResource = url.utf8To32ReadResource();

	std::u32istream & httpReadStream = httpReadResource.readStream();
	std::u32istream & uriReadStream = uriReadResource->readStream();

	const std::u32string expectedStart = U"<html>\r\n<head><title>301 Moved Permanently</title></head>";

	auto actualHttpData = toString32(httpReadStream);
	auto actualUriData = toString32(uriReadStream);

	AssertThat(actualHttpData, startsWith(expectedStart));
	AssertThat(actualUriData, startsWith(expectedStart));
}

} // namespace Resource

} // namespace Balau
