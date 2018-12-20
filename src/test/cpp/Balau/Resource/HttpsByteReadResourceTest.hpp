// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct HttpsByteReadResourceTest : public Testing::TestGroup<HttpsByteReadResourceTest> {
	explicit HttpsByteReadResourceTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&HttpsByteReadResourceTest::emptyPath, "emptyPath");
		registerTest(&HttpsByteReadResourceTest::nonEmptyPath, "nonEmptyPath");
	}

	void emptyPath();
	void nonEmptyPath();

	private: void performTest(const std::string & url_, const std::string & expectedStart);
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE_TEST
