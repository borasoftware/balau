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

#include <Balau/Resource/StringUri.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::is;

namespace Resource {

struct StringUtf8To32ReadResourceTest : public Testing::TestGroup<StringUtf8To32ReadResourceTest> {
	explicit StringUtf8To32ReadResourceTest() {
		RegisterTestCase(test);
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
