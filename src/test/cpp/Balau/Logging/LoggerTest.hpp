// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER_TEST
#define COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER_TEST

#include <Balau/Logging/Logger.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

// The logger test cases are only run if the execution model is single threaded or process per test.
// Otherwise, the logging results may be messed up by the other tests and the assertions will fail.
struct LoggerTest : public Testing::TestGroup<LoggerTest> {
	explicit LoggerTest(Testing::TestRunner & runner) : TestGroup(runner, Testing::SingleThreaded | Testing::ProcessPerTest) {
		registerTest(&LoggerTest::stringMessages,                   "stringMessages");
		registerTest(&LoggerTest::parameterisedMessages,            "parameterisedMessages");
		registerTest(&LoggerTest::loggerMacros,                     "loggerMacros");
		registerTest(&LoggerTest::getConfigurationCall,             "getConfigurationCall");
		registerTest(&LoggerTest::globalNamespace,                  "globalNamespace");
		registerTest(&LoggerTest::unconfiguredRootNamespace,        "unconfiguredRootNamespace");
		registerTest(&LoggerTest::configuredNamespaceWithoutFormat, "configuredNamespaceWithoutFormat");
		registerTest(&LoggerTest::loggingWithLineAndFileName,       "loggingWithLineAndFileName");
		registerTest(&LoggerTest::functionBasedLogging,             "functionBasedLogging");
		registerTest(&LoggerTest::customLoggingStream,              "customLoggingStream");
		registerTest(&LoggerTest::resetLoggingSystem,               "resetLoggingSystem");
	}

	void stringMessages();
	void parameterisedMessages();
	void loggerMacros();
	void getConfigurationCall();
	void globalNamespace();
	void unconfiguredRootNamespace();
	void configuredNamespaceWithoutFormat();
	void loggingWithLineAndFileName();
	void functionBasedLogging();
	void customLoggingStream();
	void resetLoggingSystem(); // Resets the logging system.

	Resource::File configureLoggerForTest(const std::string & testName);
	Resource::File configureLoggerForTest(const std::string & testName, const std::string & configurationText);
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER_TEST
