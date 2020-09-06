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

namespace Balau {

using Testing::is;

namespace Resource {

struct StringUriTest : public Testing::TestGroup<StringUriTest> {
	StringUriTest() {
		RegisterTestCase(cloning);
		RegisterTestCase(byteStream);
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
