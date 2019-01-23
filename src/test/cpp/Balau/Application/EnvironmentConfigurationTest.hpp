// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_CONFIGURATION_TEST
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_CONFIGURATION_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

struct EnvironmentConfigurationTest : public Testing::TestGroup<EnvironmentConfigurationTest> {
	explicit EnvironmentConfigurationTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&EnvironmentConfigurationTest::derivedEmpty,                 "derivedEmpty");
		registerTest(&EnvironmentConfigurationTest::derivedSimple,                "derivedSimple");
		registerTest(&EnvironmentConfigurationTest::derivedComposite,             "derivedComposite");

		registerTest(&EnvironmentConfigurationTest::directEmpty,                  "directEmpty");
		registerTest(&EnvironmentConfigurationTest::directSimple,                 "directSimple");
		registerTest(&EnvironmentConfigurationTest::directComposite,              "directComposite");
		registerTest(&EnvironmentConfigurationTest::directMultiple,               "directMultiple");

		registerTest(&EnvironmentConfigurationTest::mixedEmpty,                   "mixedEmpty");
		registerTest(&EnvironmentConfigurationTest::mixedSimple,                  "mixedSimple");
		registerTest(&EnvironmentConfigurationTest::mixedComposite,               "mixedComposite");
		registerTest(&EnvironmentConfigurationTest::mixedMultiple,                "mixedMultiple");

		registerTest(&EnvironmentConfigurationTest::derivedSimpleWithDefaults,    "derivedSimpleWithDefaults");
		registerTest(&EnvironmentConfigurationTest::derivedCompositeWithDefaults, "derivedCompositeWithDefaults");

		registerTest(&EnvironmentConfigurationTest::directSimpleWithDefaults,     "directSimpleWithDefaults");
		registerTest(&EnvironmentConfigurationTest::directCompositeWithDefaults,  "directCompositeWithDefaults");
		registerTest(&EnvironmentConfigurationTest::directMultipleWithDefaults,   "directMultipleWithDefaults");

		registerTest(&EnvironmentConfigurationTest::mixedSimpleWithDefaults,      "mixedSimpleWithDefaults");
		registerTest(&EnvironmentConfigurationTest::mixedCompositeWithDefaults,   "mixedCompositeWithDefaults");
		registerTest(&EnvironmentConfigurationTest::mixedMultipleWithDefaults,    "mixedMultipleWithDefaults");

		registerTest(&EnvironmentConfigurationTest::includedFilesConfig,          "includedFilesConfig");

		registerTest(&EnvironmentConfigurationTest::balauConfig,                  "balauConfig");
	}

	void derivedEmpty();
	void derivedSimple();
	void derivedComposite();

	void directEmpty();
	void directSimple();
	void directComposite();
	void directMultiple();

	void mixedEmpty();
	void mixedSimple();
	void mixedComposite();
	void mixedMultiple();

	void derivedSimpleWithDefaults();
	void derivedCompositeWithDefaults();

	void directSimpleWithDefaults();
	void directCompositeWithDefaults();
	void directMultipleWithDefaults();

	void mixedSimpleWithDefaults();
	void mixedCompositeWithDefaults();
	void mixedMultipleWithDefaults();

	void includedFilesConfig();

	void balauConfig();
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_CONFIGURATION_TEST
