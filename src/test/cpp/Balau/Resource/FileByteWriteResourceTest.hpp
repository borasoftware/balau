// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct FileByteWriteResourceTest : public Testing::TestGroup<FileByteWriteResourceTest> {
	FileByteWriteResourceTest() {
		registerTest(&FileByteWriteResourceTest::test, "test");
	}

	void test();

	private: File prepWritePath(const std::string & testName, const std::string & text);
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE_TEST
