// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER_TEST
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Impl {

struct EnvironmentConfigurationBuilderTest : public Testing::TestGroup<EnvironmentConfigurationBuilderTest> {
	EnvironmentConfigurationBuilderTest() {
		registerTest(&EnvironmentConfigurationBuilderTest::empty,             "empty");
		registerTest(&EnvironmentConfigurationBuilderTest::mixed,             "mixed");
		registerTest(&EnvironmentConfigurationBuilderTest::mixedWithDefaults, "mixedWithDefaults");
	}

	void empty();
	void mixed();
	void mixedWithDefaults();
};

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER_TEST
