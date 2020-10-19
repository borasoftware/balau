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

struct StringByteReadResourceTest : public Testing::TestGroup<StringByteReadResourceTest> {
	StringByteReadResourceTest() {
		RegisterTest(test);
	}

	void test() {
		const std::string expected = "some random text";

		StringUri uri(expected);

		auto stringReadResource = uri.getByteReadResource();
		auto uriReadResource = uri.byteReadResource();

		std::istream & stringReadStream = stringReadResource.readStream();
		std::istream & uriReadStream = uriReadResource->readStream();

		auto actualStringData = ::toString(stringReadStream);
		auto actualUriData = ::toString(uriReadStream);

		AssertThat(actualStringData, is(expected));
		AssertThat(actualUriData, is(expected));
	}
};

} // namespace Resource

} // namespace Balau
