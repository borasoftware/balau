// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "SystemClockTest.hpp"

#include <Balau/System/SystemClock.hpp>
#include <Balau/System/Sleep.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isGreaterThan;

namespace System {

void SystemClockTest::nanotime() {
	std::chrono::nanoseconds n1 = SystemClock().nanotime();
	Sleep::nanoSleep(100);
	std::chrono::nanoseconds n2 = SystemClock().nanotime();

	assertThat(n2, isGreaterThan(n1));
}

void SystemClockTest::millitime() {
	std::chrono::milliseconds m1 = SystemClock().millitime();
	Sleep::milliSleep(10);
	std::chrono::milliseconds m2 = SystemClock().millitime();

	assertThat(m2, isGreaterThan(m1));

	std::chrono::milliseconds m3 = SystemClock().millitime();
	Sleep::milliSleep(10);
	std::chrono::nanoseconds n = SystemClock().nanotime();
	Sleep::milliSleep(10);
	std::chrono::milliseconds m4 = SystemClock().millitime();

	assertThat(n, isGreaterThan(m3));
	assertThat(m4, isGreaterThan(n));
	assertThat(n, isGreaterThan(m3));
}

} // namespace System

} // namespace Balau
