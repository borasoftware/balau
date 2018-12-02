// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_UTF8_TO_32_READ_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_UTF8_TO_32_READ_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct StringUtf8To32ReadResourceTest : public Testing::TestGroup<StringUtf8To32ReadResourceTest> {
	explicit StringUtf8To32ReadResourceTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&StringUtf8To32ReadResourceTest::test, "test");
	}

	void test();
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_UTF8_TO_32_READ_RESOURCE_TEST
