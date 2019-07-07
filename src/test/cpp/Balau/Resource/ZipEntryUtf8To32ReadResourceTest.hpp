// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_UTF8_TO_32_READ_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_UTF8_TO_32_READ_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Resource {

struct ZipEntryUtf8To32ReadResourceTest : public Testing::TestGroup<ZipEntryUtf8To32ReadResourceTest> {
	explicit ZipEntryUtf8To32ReadResourceTest() {
		registerTest(&ZipEntryUtf8To32ReadResourceTest::test, "test");
	}

	void test();
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_UTF8_TO_32_READ_RESOURCE_TEST
