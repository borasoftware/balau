// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF32_TO_8_WRITE_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF32_TO_8_WRITE_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Resource {

struct FileUtf32To8WriteResourceTest : public Testing::TestGroup<FileUtf32To8WriteResourceTest> {
	explicit FileUtf32To8WriteResourceTest() {
		registerTest(&FileUtf32To8WriteResourceTest::test, "test");
	}

	void test();

	private: File prepWritePath(const std::string & testName, const std::string & text);
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF32_TO_8_WRITE_RESOURCE_TEST
