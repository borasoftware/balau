// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_SYSTEM__DEFAULT_CLOCK_TEST
#define COM_BORA_SOFTWARE__BALAU_SYSTEM__DEFAULT_CLOCK_TEST

// Include before test runner for toString functions.
#include <Balau/Util/DateTime.hpp>

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace System {

struct SystemClockTest : public Testing::TestGroup<SystemClockTest> {
	explicit SystemClockTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&SystemClockTest::nanotime,  "nanotime");
		registerTest(&SystemClockTest::millitime, "millitime");
	}

	void nanotime();
	void millitime();
};

} // namespace System

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_SYSTEM__DEFAULT_CLOCK_TEST
