// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__COMMAND_LINE_TEST
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__COMMAND_LINE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

struct CommandLineTest : public Testing::TestGroup<CommandLineTest> {
	CommandLineTest() {
		registerTest(&CommandLineTest::basicSsv,           "basicSsv");
		registerTest(&CommandLineTest::finalValueSsv,      "finalValueSsv");
		registerTest(&CommandLineTest::numericValueSsv,    "numericValueSsv");
		registerTest(&CommandLineTest::docOutputSsv,       "docOutputSsv");

		registerTest(&CommandLineTest::basicSev,           "basicSev");
		registerTest(&CommandLineTest::finalValueSev,      "finalValueSev");
		registerTest(&CommandLineTest::numericValueSev,    "numericValueSev");
		registerTest(&CommandLineTest::docOutputSev,       "docOutputSev");

		registerTest(&CommandLineTest::basicDetect,        "basicDetect");
		registerTest(&CommandLineTest::finalValueDetect,   "finalValueDetect");
		registerTest(&CommandLineTest::numericValueDetect, "numericValueDetect");
		registerTest(&CommandLineTest::docOutputDetect,    "docOutputDetect");
	}

	void basicSsv();
	void finalValueSsv();
	void numericValueSsv();
	void docOutputSsv();

	void basicSev();
	void finalValueSev();
	void numericValueSev();
	void docOutputSev();

	void basicDetect();
	void finalValueDetect();
	void numericValueDetect();
	void docOutputDetect();
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__COMMAND_LINE_TEST
