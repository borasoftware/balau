// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
		RegisterTest(nanotime);
		RegisterTest(millitime);
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
