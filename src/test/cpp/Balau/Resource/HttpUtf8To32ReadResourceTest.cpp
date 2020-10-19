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
#include <Balau/Resource/Http.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::startsWith;

namespace Resource {

struct HttpUtf8To32ReadResourceTest : public Testing::TestGroup<HttpUtf8To32ReadResourceTest> {
	explicit HttpUtf8To32ReadResourceTest() {
		RegisterTest(emptyPath);
		RegisterTest(nonEmptyPath);
	}

	void performTest(const std::string & url_) {
		try {
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
		performTest("http://borasoftware.com");
	}

	void nonEmptyPath() {
		performTest("http://borasoftware.com/blah");
	}
};

} // namespace Resource

} // namespace Balau
