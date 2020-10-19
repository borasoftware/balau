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

namespace Util {

const std::vector<std::pair<std::chrono::milliseconds, std::string>> testData = { // NOLINT
	  std::make_pair(std::chrono::milliseconds(42),        "00:00:00.042")
	, std::make_pair(std::chrono::milliseconds(143123),    "00:02:23.123")
	, std::make_pair(std::chrono::milliseconds(43143123),  "11:59:03.123")
	, std::make_pair(std::chrono::milliseconds(215943123), "59:59:03.123")
};

const char * Format = "%H:%M:%S";

struct DateTimeTest : public Testing::TestGroup<DateTimeTest> {
	DateTimeTest() {
		RegisterTest(toStringString);
		RegisterTest(toStringStream);
		RegisterTest(toDuration);
	}

	void toStringString() {
		for (const auto & element : testData) {
			const std::string s = DateTime::toString(Format, element.first);
			AssertThat(s, is(std::string(element.second)));
		}
	}

	void toStringStream() {
		for (const auto & element : testData) {
			std::ostringstream stream;
			DateTime::toString(stream, Format, element.first);
			const std::string actual = stream.str();
			AssertThat(actual, is(std::string(element.second)));
		}
	}

	void toDuration() {
		for (const auto & element : testData) {
			const auto d = DateTime::toDuration<int64_t, std::nano>(Format, std::string(element.second));
			AssertThat(d, is(element.first));
		}
	}
};

} // namespace Util

} // namespace Balau
