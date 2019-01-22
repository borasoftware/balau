// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "UrlDecodeTest.hpp"

#include <Balau/Network/Utilities/UrlDecode.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isGreaterThan;

namespace Network {

void UrlDecodeTest::decodeTest() {
	assertThat(UrlDecode::decode("no-special-characters"), is("no-special-characters"));
	assertThat(UrlDecode::decode("%23+beginning+and+space+and+other%5B%40%5Dcharacters"), is("# beginning and space and other[@]characters"));
	assertThat(UrlDecode::decode("all+%21%23%24%26%27%28%29%2A%2B%2C%2F%3A%3B%3D%3F%40%5B%5D+reserved"), is("all !#$&'()*+,/:;=?@[] reserved"));
	assertThat(UrlDecode::decode("utf-8+%c2%a9%c3%a7%e0%a6%88+characters"), is("utf-8 ©çঈ characters"));
	// TODO add tests for boolean arguments.
}

void UrlDecodeTest::splitAndDecodeTest() {
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

	assertThat(actual, is(expected));

	// TODO add tests for boolean arguments.
}

} // namespace Network

} // namespace Balau
