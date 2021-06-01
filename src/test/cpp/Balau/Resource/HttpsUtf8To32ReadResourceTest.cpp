// @formatter:off
//
// Balau core C++ library
//
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
using Testing::endsWith;
using Testing::is;

namespace Resource {

struct HttpsUtf8To32ReadResourceTest : public Testing::TestGroup<HttpsUtf8To32ReadResourceTest> {
	explicit HttpsUtf8To32ReadResourceTest() {
		RegisterTestCase(nonEmptyPath);
		RegisterTestCase(emptyPath);
	}

	void performTest(const std::string & url_, const std::u32string & expectedStart) {
		Https url(url_);

		auto httpsReadResource = url.getUtf8To32ReadResource();
		auto uriReadResource = url.utf8To32ReadResource();

		std::u32istream & httpsReadStream = httpsReadResource.readStream();
		std::u32istream & uriReadStream = uriReadResource->readStream();

		auto actualHttpsData = toString32(httpsReadStream);
		auto actualUriData = toString32(uriReadStream);

		AssertThat(actualHttpsData, startsWith(expectedStart));
		AssertThat(actualUriData, startsWith(expectedStart));
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
