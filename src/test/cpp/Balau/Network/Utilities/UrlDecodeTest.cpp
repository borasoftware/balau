// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <TestResources.hpp>

#include <Balau/Network/Utilities/UrlDecode.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;

namespace Network {

struct UrlDecodeTest : public Testing::TestGroup<UrlDecodeTest> {
	UrlDecodeTest() {
		RegisterTestCase(decodeTest);
		RegisterTestCase(splitAndDecodeTest);
	}

	void decodeTest() {
		AssertThat(UrlDecode::decode("no-special-characters"), is("no-special-characters"));
		AssertThat(UrlDecode::decode("%23+beginning+and+space+and+other%5B%40%5Dcharacters"), is("# beginning and space and other[@]characters"));
		AssertThat(UrlDecode::decode("all+%21%23%24%26%27%28%29%2A%2B%2C%2F%3A%3B%3D%3F%40%5B%5D+reserved"), is("all !#$&'()*+,/:;=?@[] reserved"));
		AssertThat(UrlDecode::decode("utf-8+%c2%a9%c3%a7%e0%a6%88+characters"), is("utf-8 ©çঈ characters"));
		// TODO add tests for boolean arguments.
	}

	void splitAndDecodeTest() {
		auto actual = UrlDecode::splitAndDecode(
			"no-special-characters"
			"=%23+beginning+and+space+and+other%5B%40%5Dcharacters"
			"&all+%21%23%24%26%27%28%29%2A%2B%2C%2F%3A%3B%3D%3F%40%5B%5D+reserved"
			"=utf-8+%c2%a9%c3%a7%e0%a6%88+characters"
		);

		std::unordered_map<std::string, std::string> expected = {
			  std::make_pair("no-special-characters", "# beginning and space and other[@]characters")
			, std::make_pair("all !#$&'()*+,/:;=?@[] reserved", "utf-8 ©çঈ characters")
		};

		AssertThat(actual, is(expected));

		// TODO add tests for boolean arguments.
	}
};

} // namespace Network

} // namespace Balau
