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

using Testing::startsWith;
using Testing::endsWith;

namespace Resource {

void HttpsByteReadResourceTest::emptyPath() {
	performTest("http://borasoftware.com", "<html>\r\n<head><title>302 Found</title></head>");
}

void HttpsByteReadResourceTest::nonEmptyPath() {
	performTest("https://borasoftware.com/en/index.html", "<!DOCTYPE html");
}

void HttpsByteReadResourceTest::performTest(const std::string & url_, const std::string & expectedStart) {
	try {
		Https url(url_);

		auto httpsReadResource = url.getByteReadResource();
		auto uriReadResource = url.byteReadResource();

		std::istream & httpsReadStream = httpsReadResource.readStream();
		std::istream & uriReadStream = uriReadResource->readStream();

		auto actualHttpsData = ::toString(httpsReadStream);
		auto actualUriData = ::toString(uriReadStream);

		AssertThat(actualHttpsData, startsWith(expectedStart));
		AssertThat(actualUriData, startsWith(expectedStart));
	} catch (const boost::system::system_error & e) {
		if (e.code() == boost::system::errc::device_or_resource_busy) {
			// Ignore due to no network available.
			ignore();
		} else {
			throw;
		}
	}
}

} // namespace Resource

} // namespace Balau
