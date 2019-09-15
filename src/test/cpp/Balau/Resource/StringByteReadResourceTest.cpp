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

struct StringByteReadResourceTest : public Testing::TestGroup<StringByteReadResourceTest> {
	StringByteReadResourceTest() {
		registerTest(&StringByteReadResourceTest::test, "test");
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
