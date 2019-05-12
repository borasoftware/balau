// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__PRETTY_PRINT_TEST
#define COM_BORA_SOFTWARE__BALAU_UTIL__PRETTY_PRINT_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Util {

struct PrettyPrintTest : public Testing::TestGroup<PrettyPrintTest> {
	PrettyPrintTest() {
		registerTest(&PrettyPrintTest::fixed,        "fixed");
		registerTest(&PrettyPrintTest::scientific,   "scientific");
		registerTest(&PrettyPrintTest::metricPrefix, "metricPrefix");
		registerTest(&PrettyPrintTest::binaryPrefix, "binaryPrefix");
		registerTest(&PrettyPrintTest::byteValue,    "byteValue");
		registerTest(&PrettyPrintTest::duration,     "duration");
	}

	void fixed();
	void scientific();
	void metricPrefix();
	void binaryPrefix();
	void byteValue();
	void duration();
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__PRETTY_PRINT_TEST
