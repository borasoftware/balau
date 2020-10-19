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

#include <Balau/Resource/StringUri.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::is;

namespace Resource {

struct StringUtf8To32ReadResourceTest : public Testing::TestGroup<StringUtf8To32ReadResourceTest> {
	explicit StringUtf8To32ReadResourceTest() {
		RegisterTest(test);
	}

	void test() {
		const std::string text = "some random text";
		const std::u32string expected = toString32("some random text");

		StringUri uri(text);

		auto stringReadResource = uri.getUtf8To32ReadResource();
		auto uriReadResource = uri.utf8To32ReadResource();

		std::u32istream & stringReadStream = stringReadResource.readStream();
		std::u32istream & uriReadStream = uriReadResource->readStream();

		auto actualStringData = toString32(stringReadStream);
		auto actualUriData = toString32(uriReadStream);

		AssertThat(actualStringData, is(expected));
		AssertThat(actualUriData, is(expected));
	}
};

} // namespace Resource

} // namespace Balau
