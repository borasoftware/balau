// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <Balau/Logging/Logger.hpp>
#include <TestResources.hpp>

#include <Balau/Exception/LoggingExceptions.hpp>
#include <Balau/Exception/ParsingExceptions.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Type/OnScopeExit.hpp>

namespace Balau {

using namespace Util;

using Testing::is;
using Testing::startsWith;
using Testing::endsWith;
using Testing::contains;

namespace {

const std::string defaultConfigurationTextPrefix = 1 + R"RR(
. {
	level  = warn
    format = %Y-%m-%d %H:%M:%S [%thread] %LEVEL - %ns - %message
)RR";

const std::string defaultConfigurationTextSuffix = 1 + R"RR(
}

com.borasoftware {
	level = info
}

com.borasoftware.misc {
	level = trace
}
)RR";

const std::string STREAM_ENTRY = "_STREAM_ENTRY_";

std::ostringstream customLoggingStreamStream;

} // namespace

// The logger test cases are only run if the execution model is single threaded or process per test.
// Otherwise, the logging results may be messed up by the other tests and the assertions will fail.
struct LoggerTest : public Testing::TestGroup<LoggerTest> {
	explicit LoggerTest() : TestGroup(Testing::SingleThreaded | Testing::ProcessPerTest) {
		registerTest(&LoggerTest::stringMessages,                   "stringMessages");
		registerTest(&LoggerTest::parameterisedMessages,            "parameterisedMessages");
		registerTest(&LoggerTest::loggerMacros,                     "loggerMacros");
		registerTest(&LoggerTest::getConfigurationCall,             "getConfigurationCall");
		registerTest(&LoggerTest::globalNamespace,                  "globalNamespace");
		registerTest(&LoggerTest::unconfiguredRootNamespace,        "unconfiguredRootNamespace");
		registerTest(&LoggerTest::configuredNamespaceWithoutFormat, "configuredNamespaceWithoutFormat");
		registerTest(&LoggerTest::loggingWithLineAndFileName,       "loggingWithLineAndFileName");
		registerTest(&LoggerTest::functionBasedLogging,             "functionBasedLogging");
		registerTest(&LoggerTest::flushing,                         "flushing");
		registerTest(&LoggerTest::customLoggingStream,              "customLoggingStream");
		registerTest(&LoggerTest::resetLoggingSystem,               "resetLoggingSystem");
	}

	static void assertLines(const std::string & actual, const std::vector<std::string> & expected) {
		const auto lines = Strings::split(actual, "\n");
	
		AssertThat(lines.size(), is(expected.size()));
	
		for (size_t m = 0; m < lines.size(); m++) {
			AssertThat(std::string(lines[m]), endsWith(expected[m]));
		}
	}
	
	static Resource::File configureLoggerForTest(const std::string & testName) {
		const std::string filename = std::string("LoggerTest-") + testName + ".log";
		Resource::File logFile = TestResources::TestResultsFolder / filename;
		const std::string logFileUriStr = logFile.toUriString();
	
		logFile.getParentDirectory().createDirectories();
		AssertThat(logFile.getParentDirectory().exists(), is(true));
		logFile.removeFile();
		AssertThat(logFile.exists(), is(false));
	
		const std::string configurationText = defaultConfigurationTextPrefix
			+ "              stream = " + logFileUriStr + "\n"
			+ defaultConfigurationTextSuffix;
	
		Logger::configure(configurationText);
	
		return logFile;
	}
	
	// The string configurationText contains a placeholder _STREAM_ENTRY_
	// which is replace with the file stream string.
	static Resource::File configureLoggerForTest(const std::string & testName, const std::string & configurationText) {
		const std::string filename = std::string("LoggerTest-") + testName + ".log";
		Resource::File logFile = TestResources::TestResultsFolder / filename;
		const std::string logFileUriStr = logFile.toUriString();
	
		logFile.getParentDirectory().createDirectories();
		AssertThat(logFile.getParentDirectory().exists(), is(true));
		logFile.removeFile();
		AssertThat(logFile.exists(), is(false));
	
		const std::string conf = Strings::replaceAll(configurationText, STREAM_ENTRY, logFileUriStr);
	
		Logger::configure(conf);
	
		return logFile;
	}
	
	void stringMessages() {
		Resource::File logFile1 = configureLoggerForTest("stringMessages");
		OnScopeExit removeLogFile([=] () mutable { logFile1.removeFile(); });
	
		Logger::getLogger("com.borasoftware").info("Hello World");
		Logger::getLogger("com.borasoftware.a").trace("Hello trace");
		Logger::getLogger("com.borasoftware.a").debug("Hello debug");
		Logger::getLogger("com.borasoftware.a").info("Hello info");
		Logger::getLogger("com.borasoftware.a").warn("Hello warn");
		Logger::getLogger("com.borasoftware.a").error("Hello error");
		Logger::getLogger("com.borasoftware.misc").info("Hello unconfigured namespace");
	
		Resource::File logFile2 = configureLoggerForTest("stringMessages2");
		OnScopeExit removeLogFile2([=] () mutable { logFile2.removeFile(); });
	
		Logger::getLogger("com.borasoftware.misc").info("Hello newly configured namespace");
		Logger::getLogger("com.borasoftware").info("Hello World");
		Logger::getLogger("com.borasoftware.a").info("Hello again");
		Logger::getLogger("com.borasoftware.misc.deps.foo.zoo.bob.abc").info("A big unconfigured namespace");
		Logger::getLogger("com.borasoftware.misc").warn("This should be logged in a file.");
		Logger::getLogger("com.borasoftware.misc").error("This should be logged in a file.");
		Logger::getLogger("com.borasoftware.misc").info("This should be logged in a file.");
		Logger::getLogger("com.borasoftware.misc").debug("This should be logged in a file.");
		Logger::getLogger("com.borasoftware.misc").trace("This should be logged in a file.");
	
		Logger::flushAll();
	
		const std::string actual1 = Files::readToString(logFile1);
		const std::string actual2 = Files::readToString(logFile2);
	
		const std::vector<std::string> expectedContains1 = {
			  " [LoggerTest::stringMessages] INFO - c.borasoftware - Hello World"
			, " [LoggerTest::stringMessages] INFO - c.b.a - Hello info"
			, " [LoggerTest::stringMessages] WARN - c.b.a - Hello warn"
			, " [LoggerTest::stringMessages] ERROR - c.b.a - Hello error"
			, " [LoggerTest::stringMessages] INFO - c.b.misc - Hello unconfigured namespace"
		};
	
		const std::vector<std::string> expectedContains2 = {
			  " [LoggerTest::stringMessages] INFO - c.b.misc - Hello newly configured namespace"
			, " [LoggerTest::stringMessages] INFO - c.borasoftware - Hello World"
			, " [LoggerTest::stringMessages] INFO - c.b.a - Hello again"
			, " [LoggerTest::stringMessages] INFO - c.b.m.d.f.z.b.abc - A big unconfigured namespace"
			, " [LoggerTest::stringMessages] WARN - c.b.misc - This should be logged in a file."
			, " [LoggerTest::stringMessages] ERROR - c.b.misc - This should be logged in a file."
			, " [LoggerTest::stringMessages] INFO - c.b.misc - This should be logged in a file."
			, " [LoggerTest::stringMessages] DEBUG - c.b.misc - This should be logged in a file."
			, " [LoggerTest::stringMessages] TRACE - c.b.misc - This should be logged in a file."
		};
	
		assertLines(actual1, expectedContains1);
		assertLines(actual2, expectedContains2);
	}
	
	void parameterisedMessages() {
		const std::string configurationText = 1 + R"RR(
	. {
		level = info
		format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
		stream = _STREAM_ENTRY_
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("parameterisedMessages", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		Logger & log = Logger::getLogger("com.borasoftware");
	
		log.info("A single {} parameter message", 1);
		log.info("A two {} parameter {} message", 1, 2);
		log.info("A three {} parameter {} message {}", 1, 2.0 ,3);
		log.info("{}, {}, {}, {}", 1, 2.0 ,3, 4.0);
	
		Logger::flushAll();
	
		const std::string actual = Files::readToString(logFile);
	
		const std::vector<std::string> expectedContains = {
			  ": - [LoggerTest::parameterisedMessages] INFO - com.borasoftware - A single 1 parameter message"
			, ": - [LoggerTest::parameterisedMessages] INFO - com.borasoftware - A two 1 parameter 2 message"
			, ": - [LoggerTest::parameterisedMessages] INFO - com.borasoftware - A three 1 parameter 2 message 3"
			, ": - [LoggerTest::parameterisedMessages] INFO - com.borasoftware - 1, 2, 3, 4"
		};
	
		assertLines(actual, expectedContains);
	}
	
	void loggerMacros() {
		const std::string configurationText = 1 + R"RR(
	. {
		level = trace
		format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
		stream = _STREAM_ENTRY_
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("loggerMacros", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		AssertThat(Logger::getLogger(".").getLevel(), is(LoggingLevel::TRACE));
		AssertThat(Logger::getLogger("com.borasoftware").getLevel(), is(LoggingLevel::TRACE));
		AssertThat(Logger::getLogger(".").getNamespace(), is(std::string("")));
		AssertThat(Logger::getLogger("com.borasoftware").getNamespace(), is(std::string("com.borasoftware")));
	
		Logger & log = Logger::getLogger("com.borasoftware");
	
		log.info("A single {} parameter message", 1);
		log.info("A two {} parameter {} message", 1, 2);
		log.info("A three {} parameter {} message {}", 1, 2.0 ,3);
		log.info("{}, {}, {}, {}", 1, 2.0 ,3, 4.0);
	
		BalauLogTrace(log, "Test log trace macro");
		BalauLogDebug(log, "Test log debug macro");
		BalauLogInfo(log, "Test log info macro");
		BalauLogWarn(log, "Test log warn macro");
		BalauLogError(log, "Test log error macro");
	
		BalauLogTrace(log, "Test log trace macro with {} parameter", 1);
		BalauLogDebug(log, "Test log debug macro with {} parameter", 2);
		BalauLogInfo(log, "Test log info macro with {} parameter", 3);
		BalauLogWarn(log, "Test log warn macro with {} parameter", 4);
		BalauLogError(log, "Test log error macro with {} parameter", 5);
	
		BalauLogTrace(log, "Test log trace macro with {}, {} parameters", 0, 5);
		BalauLogDebug(log, "Test log debug macro with {}, {} parameters", 1, 6);
		BalauLogInfo(log, "Test log info macro with {}, {} parameters", 2, 7);
		BalauLogWarn(log, "Test log warn macro with {}, {} parameters", 3, 8);
		BalauLogError(log, "Test log error macro with {}, {} parameters", 4, 9);
	
		Logger::flushAll();
		const std::string actual = Files::readToString(logFile);
	
		const std::vector<std::string> expectedContains = {
			  " - [LoggerTest::loggerMacros] INFO - com.borasoftware - A single 1 parameter message"
			, " - [LoggerTest::loggerMacros] INFO - com.borasoftware - A two 1 parameter 2 message"
			, " - [LoggerTest::loggerMacros] INFO - com.borasoftware - A three 1 parameter 2 message 3"
			, " - [LoggerTest::loggerMacros] INFO - com.borasoftware - 1, 2, 3, 4"
			, " - [LoggerTest::loggerMacros] TRACE - com.borasoftware - Test log trace macro"
			, " - [LoggerTest::loggerMacros] DEBUG - com.borasoftware - Test log debug macro"
			, " - [LoggerTest::loggerMacros] INFO - com.borasoftware - Test log info macro"
			, " - [LoggerTest::loggerMacros] WARN - com.borasoftware - Test log warn macro"
			, " - [LoggerTest::loggerMacros] ERROR - com.borasoftware - Test log error macro"
			, " - [LoggerTest::loggerMacros] TRACE - com.borasoftware - Test log trace macro with 1 parameter"
			, " - [LoggerTest::loggerMacros] DEBUG - com.borasoftware - Test log debug macro with 2 parameter"
			, " - [LoggerTest::loggerMacros] INFO - com.borasoftware - Test log info macro with 3 parameter"
			, " - [LoggerTest::loggerMacros] WARN - com.borasoftware - Test log warn macro with 4 parameter"
			, " - [LoggerTest::loggerMacros] ERROR - com.borasoftware - Test log error macro with 5 parameter"
			, " - [LoggerTest::loggerMacros] TRACE - com.borasoftware - Test log trace macro with 0, 5 parameters"
			, " - [LoggerTest::loggerMacros] DEBUG - com.borasoftware - Test log debug macro with 1, 6 parameters"
			, " - [LoggerTest::loggerMacros] INFO - com.borasoftware - Test log info macro with 2, 7 parameters"
			, " - [LoggerTest::loggerMacros] WARN - com.borasoftware - Test log warn macro with 3, 8 parameters"
			, " - [LoggerTest::loggerMacros] ERROR - com.borasoftware - Test log error macro with 4, 9 parameters"
		};
	
		assertLines(actual, expectedContains);
	}
	
	void getConfigurationCall() {
		const std::string configurationText = 1 + R"RR(
. {
	level  = trace
	format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
	stream = _STREAM_ENTRY_
}
)RR";
	
		Resource::File logFile = configureLoggerForTest("getConfigurationCall", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		Logger & log = Logger::getLogger("com.borasoftware");
	
		const std::string actual = log.getConfiguration();
	
		const std::string expectedConfigurationTextStart = 1 + R"RR(
com.borasoftware {
    format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
    level = trace
    stream = )RR";

	AssertThat(actual, startsWith(expectedConfigurationTextStart));
}
	
	void globalNamespace() {
		const std::string configurationText = 1 + R"RR(
	. {
		level  = info
		format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
		stream = _STREAM_ENTRY_
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("globalNamespace", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		Logger::globalLogger().warn("This is a test message with parameter {}", 2);
	
		Logger::flushAll();
		const std::string actual = Files::readToString(logFile);
		const std::string expectedContains = " : - [LoggerTest::globalNamespace] WARN -  - This is a test message with parameter 2";
	
		AssertThat(actual, contains(expectedContains));
	}
	
	void unconfiguredRootNamespace() {
		const std::string configurationText = 1 + R"RR(
	. {
		stream = _STREAM_ENTRY_
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("unconfiguredRootNamespace", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		Balau::Logger & log = Balau::Logger::getLogger("qwerty");
		Balau::Logger & log2 = Balau::Logger::getLogger("zxcv.asdf.wer");
	
		log.warn("This is a message with parameter {} on an unconfigured root namespace.", 2);
		log2.warn("This is a message with parameter {} on another deep hierarchy unconfigured root namespace.", 2);
	
		Logger::flushAll();
		const std::string actual = Files::readToString(logFile);
	
		const std::vector<std::string> expectedContains =  {
			  "[LoggerTest::unconfiguredRootNamespace] WARN - qwerty - This is a message with parameter 2 on an unconfigured root namespace."
			, "[LoggerTest::unconfiguredRootNamespace] WARN - zxcv.asdf.wer - This is a message with parameter 2 on another deep hierarchy unconfigured root namespace."
		};
	
		assertLines(actual, expectedContains);
	}
	
	void configuredNamespaceWithoutFormat() {
		const std::string configurationText = 1 + R"RR(
	.    {
		stream = _STREAM_ENTRY_
	}
	
	abcd {
		level = debug
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("configuredNamespaceWithoutFormat", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		auto & log = Logger::getLogger("abcd");
	
		log.info("Hello newly configured namespace {}", "abcd");
	
		Logger::flushAll();
		const std::string actual = Files::readToString(logFile);
		const std::string expectedContains = " [LoggerTest::configuredNamespaceWithoutFormat] INFO - abcd - Hello newly configured namespace abcd";
	
		AssertThat(actual, contains(expectedContains));
	}
	
	void loggingWithLineAndFileName() {
		const std::string configurationText = 1 + R"RR(
	. {
		level = info
		format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
		stream = _STREAM_ENTRY_
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("loggingWithLineAndFileName", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
		Logger & log1 = Logger::globalLogger();
		Logger & log2 = Logger::getLogger("com.borasoftware");
	
		log1.info(SourceCodeLocation(__FILE__, __LINE__), "hello {}", "mike");
		log1.info("hello {} {}", 8U, "john");
	
		BalauLogInfo(log1, "hello {}", "bob");
		BalauLogInfo(log2, "hello {}", "C++");
	
		Logger::flushAll();
		const std::string actual = Files::readToString(logFile);
	
		const std::vector<std::string> expectedContains = {
			  " - [LoggerTest::loggingWithLineAndFileName] INFO -  - hello mike"
			, " - [LoggerTest::loggingWithLineAndFileName] INFO -  - hello 8 john"
			, " - [LoggerTest::loggingWithLineAndFileName] INFO -  - hello bob"
			, " - [LoggerTest::loggingWithLineAndFileName] INFO - com.borasoftware - hello C++"
		};
	
		assertLines(actual, expectedContains);
	}
	
	void functionBasedLogging() {
		const std::string configurationText = 1 + R"RR(
	. {
		level  = info
		format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
		stream = _STREAM_ENTRY_
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("functionBasedLogging", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
		Logger & log1 = Logger::globalLogger();
	
		log1.info([] () { return "hello bob"; });
	
		// Default flushing = true.
		AssertThat(log1.flushes(), is(true));
	
		int i = 2;
		int j = 3;
	
		log1.trace([&i, &j] () { return ::toString("TR i + j = ", i * j); });
		log1.debug([&i, &j] () { return ::toString("DE i + j = ", i * j); });
		log1.info([&i, &j] ()  { return ::toString("IN i + j = ", i * j); });
		log1.warn([&i, &j] ()  { return ::toString("WA i + j = ", i * j); });
		log1.error([&i, &j] () { return ::toString("ER i + j = ", i * j); });
	
		Logger::flushAll();
		const std::string actual = Files::readToString(logFile);
	
		const std::vector<std::string> expectedContains = {
			  "[LoggerTest::functionBasedLogging] INFO -  - hello bob"
			, "[LoggerTest::functionBasedLogging] INFO -  - IN i + j = 6"
			, "[LoggerTest::functionBasedLogging] WARN -  - WA i + j = 6"
			, "[LoggerTest::functionBasedLogging] ERROR -  - ER i + j = 6"
		};
	
		assertLines(actual, expectedContains);
	}
	
	void flushing() {
		const std::string configurationText = 1 + R"RR(
	. {
		level = info
		flush = false
	}
	)RR";
	
		Resource::File logFile = configureLoggerForTest("flushing", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
		Logger & log1 = Logger::globalLogger();
	
		log1.info([] () { return "hello bob"; });
	
		AssertThat(log1.getLevel(), is(LoggingLevel::INFO));
		AssertThat(log1.flushes(), is(false));
	}

	// A custom logging stream which logs to the test string stream.
	class CustomLoggingStream : public LoggingStream {
		public: void write(const LoggingSystem::LoggerString & str) override {
			customLoggingStreamStream << str;
		}
	
		public: void flush() override {
			customLoggingStreamStream.flush();
		}
	};
	
	// The factory function for the custom logging stream.
	static LoggingStream * customLoggingStreamNewInstance(std::string_view) {
		return new CustomLoggingStream;
	}
	
	void customLoggingStream() {
		const std::string configurationText = 1 + R"RR(
	. {
		level  = info
		format = %Y-%m-%d %H:%M:%S %filename:%line - [%thread] %LEVEL - %namespace - %message
		stream = custom
	}
	)RR";
	
		Logger::registerLoggingStreamFactory("custom", customLoggingStreamNewInstance);
	
		Resource::File logFile = configureLoggerForTest("customLoggingStream", configurationText);
		OnScopeExit removeLogFile([=] () mutable { logFile.removeFile(); });
	
		Logger & log1 = Logger::globalLogger();
		Logger & log2 = Logger::getLogger("com.borasoftware");
	
		BalauLogInfo(log1, "hello {}", "bob");
		BalauLogInfo(log2, "hello {}", "C++");
	
		Logger::flushAll();
	
		const auto actual = customLoggingStreamStream.str();
		const std::vector<std::string> expectedContains = {
			  " - [LoggerTest::customLoggingStream] INFO -  - hello bob"
			, " - [LoggerTest::customLoggingStream] INFO - com.borasoftware - hello C++"
		};
	
		assertLines(actual, expectedContains);
	}
	
	void resetLoggingSystem() {
		Logger::resetConfiguration();
		Logger::lockConfiguration();
	}
};

} // namespace Balau
