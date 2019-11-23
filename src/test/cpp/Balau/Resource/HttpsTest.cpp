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

#include "../../../../main/cpp/Balau/Resource/Https.hpp"

namespace Balau {

using Testing::is;

namespace Resource {

struct HttpsTest : public Testing::TestGroup<HttpsTest> {
	HttpsTest() {
		registerTest(&HttpsTest::uriAppend,  "uriAppend");
	}

	void uriAppend() {
		auto uri = std::unique_ptr<Uri>(new Https("https://example.com/a/b/c?query=1&part=2#fragment-part"));
		auto actual = uri / "d" / "e" / "f";
		auto expected = Https("https://example.com/a/b/c/d/e/f?query=1&part=2#fragment-part");

		AssertThat(*actual, is(expected));
	}
};

} // namespace Resource

} // namespace Balau
