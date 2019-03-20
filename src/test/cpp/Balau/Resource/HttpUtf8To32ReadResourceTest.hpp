// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_UTF8_TO_32_READ_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_UTF8_TO_32_READ_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct HttpUtf8To32ReadResourceTest : public Testing::TestGroup<HttpUtf8To32ReadResourceTest> {
	explicit HttpUtf8To32ReadResourceTest() {
		registerTest(&HttpUtf8To32ReadResourceTest::emptyPath, "emptyPath");
		registerTest(&HttpUtf8To32ReadResourceTest::nonEmptyPath, "nonEmptyPath");
	}

	void emptyPath();
	void nonEmptyPath();

	private: void performTest(const std::string & url_);
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_UTF8_TO_32_READ_RESOURCE_TEST
