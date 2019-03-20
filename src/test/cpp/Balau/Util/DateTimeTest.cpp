// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "DateTimeTest.hpp"

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

void DateTimeTest::toStringString() {
	for (const auto & element : testData) {
		const std::string s = DateTime::toString(Format, element.first);
		AssertThat(s, is(std::string(element.second)));
	}
}

void DateTimeTest::toStringStream() {
	for (const auto & element : testData) {
		std::ostringstream stream;
		DateTime::toString(stream, Format, element.first);
		const std::string actual = stream.str();
		AssertThat(actual, is(std::string(element.second)));
	}
}

void DateTimeTest::toDuration() {
	for (const auto & element : testData) {
		const auto d = DateTime::toDuration<int64_t, std::nano>(Format, std::string(element.second));
		AssertThat(d, is(element.first));
	}
}

} // namespace Util

} // namespace Balau
