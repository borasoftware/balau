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

#include "../../../../main/cpp/Balau/Resource/Http.hpp"

namespace Balau {

using Testing::is;

namespace Resource {

struct HttpTest : public Testing::TestGroup<HttpTest> {
	HttpTest() {
		RegisterTestCase(uriAppend);
	}

	void uriAppend() {
		auto uri = std::unique_ptr<Uri>(new Http("http://example.com/a/b/c?query=1&part=2#fragment-part"));
		auto actual = uri / "d" / "e" / "f";
		auto expected = Http("http://example.com/a/b/c/d/e/f?query=1&part=2#fragment-part");

		AssertThat(*actual, is(expected));
	}
};

} // namespace Resource

} // namespace Balau
