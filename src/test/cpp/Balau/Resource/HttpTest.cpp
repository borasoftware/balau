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
