// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__RANDOM_TEST
#define COM_BORA_SOFTWARE__BALAU_UTIL__RANDOM_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Util {

struct RandomTest : public Testing::TestGroup<RandomTest> {
	RandomTest() {
		registerTest(&RandomTest::test, "test");
	}

	void test();
};

} // namespace Util

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__RANDOM_TEST
