// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct FileTest : public Testing::TestGroup<FileTest> {
	FileTest() {
		registerTest(&FileTest::iteration, "iteration");
		registerTest(&FileTest::resolve,   "resolve");
	}

	void iteration();
	void resolve();
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_TEST
