// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP_TEST
#define COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP_TEST

#include <Balau/Documentation/ConfigurationWebApp.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Documentation {

struct ConfigurationWebAppTest : public Testing::TestGroup<ConfigurationWebAppTest> {
	ConfigurationWebAppTest() {
		registerTest(&ConfigurationWebAppTest::test, "test");
	}

	void test();
};

} // namespace Balau::Documentation

#endif // COM_BORA_SOFTWARE__BALAU_DOCUMENTATION__CONFIGURATION_WEB_APP_TEST
