// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_STREAMS_TEST
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_STREAMS_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace LoggingSystem {

struct LoggingStreamsTest : public Testing::TestGroup<LoggingStreamsTest> {
	LoggingStreamsTest() {
		// Work in progress.
		//registerTest(&LoggingStreamsTest::fileRotation, "fileRotation");
	}

	void fileRotation();
};

} // namespace LoggingSystem

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_STREAMS_TEST
