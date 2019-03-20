// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpByteReadResourceTest.hpp"

#include <Balau/Resource/Http.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::startsWith;

namespace Resource {

void HttpByteReadResourceTest::emptyPath() {
	performTest("http://borasoftware.com");
}

void HttpByteReadResourceTest::nonEmptyPath() {
	performTest("http://borasoftware.com/blah");
}

void HttpByteReadResourceTest::performTest(const std::string & url_) {
	Http url(url_);

	auto httpReadResource = url.getByteReadResource();
	auto uriReadResource = url.byteReadResource();

	std::istream & httpReadStream = httpReadResource.readStream();
	std::istream & uriReadStream = uriReadResource->readStream();

	const std::string expectedStart = "<html>\r\n<head><title>301 Moved Permanently</title></head>";

	auto actualHttpData = ::toString(httpReadStream);
	auto actualUriData = ::toString(uriReadStream);

	AssertThat(actualHttpData, startsWith(expectedStart));
	AssertThat(actualUriData, startsWith(expectedStart));
}

} // namespace Resource

} // namespace Balau
