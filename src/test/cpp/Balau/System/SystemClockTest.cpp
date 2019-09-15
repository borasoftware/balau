// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

// Include before test runner for toString functions.
#include <Balau/Util/DateTime.hpp>
#include <TestResources.hpp>
#include <Balau/System/SystemClock.hpp>
#include <Balau/System/Sleep.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;

namespace System {

struct SystemClockTest : public Testing::TestGroup<SystemClockTest> {
	SystemClockTest() {
		registerTest(&SystemClockTest::nanotime,  "nanotime");
		registerTest(&SystemClockTest::millitime, "millitime");
	}

	void nanotime() {
		std::chrono::nanoseconds n1 = SystemClock().nanotime();
		Sleep::nanoSleep(100);
		std::chrono::nanoseconds n2 = SystemClock().nanotime();

		AssertThat(n2, isGreaterThan(n1));
	}

	void millitime() {
		std::chrono::milliseconds m1 = SystemClock().millitime();
		Sleep::milliSleep(10);
		std::chrono::milliseconds m2 = SystemClock().millitime();

		AssertThat(m2, isGreaterThan(m1));

		std::chrono::milliseconds m3 = SystemClock().millitime();
		Sleep::milliSleep(10);
		std::chrono::nanoseconds n = SystemClock().nanotime();
		Sleep::milliSleep(10);
		std::chrono::milliseconds m4 = SystemClock().millitime();

		AssertThat(n, isGreaterThan(m3));
		AssertThat(m4, isGreaterThan(n));
		AssertThat(n, isGreaterThan(m3));
	}
};

} // namespace System

} // namespace Balau
