// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST
#define COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST

#include "AssertionsTestData.hpp"
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

// This test group tests the assertions of the test runner.
struct AssertionsTest : public Testing::TestGroup<AssertionsTest> {
	explicit AssertionsTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&AssertionsTest::test, "test");
	}

	void test();
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST
