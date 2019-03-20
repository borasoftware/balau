// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__DATE_TIME_TEST
#define COM_BORA_SOFTWARE__BALAU_UTIL__DATE_TIME_TEST

// Include before test runner for toString functions.
#include <Balau/Util/DateTime.hpp>

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Util {

struct DateTimeTest : public Testing::TestGroup<DateTimeTest> {
	DateTimeTest() {
		registerTest(&DateTimeTest::toStringString, "toStringString");
		registerTest(&DateTimeTest::toStringStream, "toStringStream");
		registerTest(&DateTimeTest::toDuration,     "toTimePoint");
	}

	void toStringString();
	void toStringStream();
	void toDuration();
};

} // namespace Util

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__DATE_TIME_TEST
