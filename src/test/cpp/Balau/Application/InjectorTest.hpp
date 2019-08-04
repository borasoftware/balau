// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_TEST
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_TEST

#include <Balau/Exception/InjectorExceptions.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

struct InjectorTest : public Testing::TestGroup<InjectorTest> {
	InjectorTest() {
		registerTest(&InjectorTest::creationAndDeletion,       "creationAndDeletion");
		registerTest(&InjectorTest::singletonCreation,         "singletonCreation");
		registerTest(&InjectorTest::simpleTypeInjection,       "simpleTypeInjection");
		registerTest(&InjectorTest::multipleCustomScopes,      "multipleCustomScopes");
		registerTest(&InjectorTest::namedDependency,           "namedDependency");
		registerTest(&InjectorTest::eagerSingletons,           "eagerSingletons");
		registerTest(&InjectorTest::runtimeCustomScopes,       "runtimeCustomScopes");
		registerTest(&InjectorTest::threadLocalScopeUsage,     "threadLocalScopeUsage");
		registerTest(&InjectorTest::allBindings,               "allBindings");
		registerTest(&InjectorTest::docTest,                   "docTest");
		registerTest(&InjectorTest::injectTheInjector,         "injectTheInjector");
		registerTest(&InjectorTest::sharedCycleChecks,         "sharedCycleChecks");
		registerTest(&InjectorTest::mixedCycleChecks,          "mixedCycleChecks");
		registerTest(&InjectorTest::injectorCycleAvoidance,    "injectorCycleAvoidance");
		registerTest(&InjectorTest::singletonProvider,         "singletonProvider");
		registerTest(&InjectorTest::providedSingletonProvider, "providedSingletonProvider");
		registerTest(&InjectorTest::headerBodyMacros,          "headerBodyMacros");
	}

	void creationAndDeletion();
	void singletonCreation();
	void simpleTypeInjection();
	void multipleCustomScopes();
	void namedDependency();
	void eagerSingletons();
	void runtimeCustomScopes();
	void threadLocalScopeUsage();
	void allBindings();
	void docTest();
	void injectTheInjector();
	void sharedCycleChecks();
	void mixedCycleChecks();
	void injectorCycleAvoidance();
	void singletonProvider();
	void providedSingletonProvider();
	void headerBodyMacros();
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_TEST
