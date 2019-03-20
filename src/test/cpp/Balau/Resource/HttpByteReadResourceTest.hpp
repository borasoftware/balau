// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct HttpByteReadResourceTest : public Testing::TestGroup<HttpByteReadResourceTest> {
	HttpByteReadResourceTest() {
		registerTest(&HttpByteReadResourceTest::emptyPath, "emptyPath");
		registerTest(&HttpByteReadResourceTest::nonEmptyPath, "nonEmptyPath");
	}

	void emptyPath();
	void nonEmptyPath();

	private: void performTest(const std::string & url_);
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE_TEST
