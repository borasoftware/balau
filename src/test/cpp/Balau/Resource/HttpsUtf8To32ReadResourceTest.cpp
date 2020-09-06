// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>
#include <Balau/Resource/Https.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::startsWith;
using Testing::endsWith;
using Testing::is;

namespace Resource {

struct HttpsUtf8To32ReadResourceTest : public Testing::TestGroup<HttpsUtf8To32ReadResourceTest> {
	explicit HttpsUtf8To32ReadResourceTest() {
		RegisterTestCase(nonEmptyPath);
		RegisterTestCase(emptyPath);
	}

	void performTest(const std::string & url_, const std::u32string & expectedStart) {
		try {
			Https url(url_);

			auto httpsReadResource = url.getUtf8To32ReadResource();
			auto uriReadResource = url.utf8To32ReadResource();

			std::u32istream & httpsReadStream = httpsReadResource.readStream();
			std::u32istream & uriReadStream = uriReadResource->readStream();

			auto actualHttpsData = toString32(httpsReadStream);
			auto actualUriData = toString32(uriReadStream);

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

	void emptyPath() {
		performTest("http://borasoftware.com", U"<html>\r\n<head><title>302 Found</title></head>");
	}

	void nonEmptyPath() {
		performTest("https://borasoftware.com/en/index.html", U"<!DOCTYPE html");
	}
};

} // namespace Resource

} // namespace Balau
