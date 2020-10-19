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

namespace Balau {

using Testing::is;

namespace Resource {

struct StringUriTest : public Testing::TestGroup<StringUriTest> {
	StringUriTest() {
		RegisterTest(cloning);
		RegisterTest(byteStream);
	}

	void cloning() {
		auto uri1 = std::unique_ptr<Uri>(new StringUri("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tem"));
		auto uri2 = uri1->clone();

		AssertThat(*uri1, is(*uri2));
	}

	void byteStream() {
		auto str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tem";
		auto uri1 = std::unique_ptr<Uri>(new StringUri(str));
		auto uri2 = uri1->clone();
		auto readResource = uri2->byteReadResource();
		auto & stream = readResource->readStream();
		auto s = ::toString(stream);

		AssertThat(s, is(str));
	}
};

} // namespace Resource

} // namespace Balau
