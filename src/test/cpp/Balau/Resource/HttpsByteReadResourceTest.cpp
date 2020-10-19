// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <TestResources.hpp>
#include <Balau/Resource/Https.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::startsWith;
using Testing::endsWith;

namespace Resource {

struct HttpsByteReadResourceTest : public Testing::TestGroup<HttpsByteReadResourceTest> {
	HttpsByteReadResourceTest() {
		RegisterTest(emptyPath);
		RegisterTest(nonEmptyPath);
	}

	void performTest(const std::string & url_, const std::string & expectedStart) {
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

	void emptyPath() {
		performTest("http://borasoftware.com", "<html>\r\n<head><title>302 Found</title></head>");
	}

	void nonEmptyPath() {
		performTest("https://borasoftware.com/en/index.html", "<!DOCTYPE html");
	}
};

} // namespace Resource

} // namespace Balau
