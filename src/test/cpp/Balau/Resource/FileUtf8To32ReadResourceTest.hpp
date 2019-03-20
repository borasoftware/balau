// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct FileUtf8To32ReadResourceTest : public Testing::TestGroup<FileUtf8To32ReadResourceTest> {
	explicit FileUtf8To32ReadResourceTest() {
		registerTest(&FileUtf8To32ReadResourceTest::test, "test");
	}

	void test();
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE_TEST
