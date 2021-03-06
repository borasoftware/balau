// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

///
/// @file Logger.hpp
///
/// The logging system's main Logger class.
///
/// Always include this file last when there is a series of Balau include files,
/// in order to ensure that all toString functions are included before the logger.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER
#define COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Logging/LoggingLevel.hpp>
#include <Balau/Logging/Impl/LoggerAllocator.hpp>
#include <Balau/Logging/Impl/LoggerForwardDeclarations.hpp>

#include <atomic>
#include <functional>

namespace Balau {

class EnvironmentProperties;

namespace LoggingSystem {

class LoggerConfigurationVisitor;
class LoggerPropertyVisitor;

} // namespace LoggingSystem

/////////////////////////// Logging stream plugins ////////////////////////////

///
/// Base class of logging stream plugins.
///
/// The built in schemes and pseudo schemes natively handled by the logging system are:
///  - file;
///  - stdout;
///  - stderr.
///
/// In order to handle other schemes, this class should be extended, a factory function
/// should be created, and the factory function should be registered at application
/// startup by calling Logger::registerLoggingStreamFactory.
///
class LoggingStream {
	///
	/// Write the supplied string to the logging stream.
	///
	/// This method must be thread safe.
	///
	public: virtual void write(const LoggingSystem::LoggerString & str) = 0;

	///
	/// Flush the logging stream.
	///
	/// This method must be thread safe.
	///
	public: virtual void flush() = 0;

	public: virtual ~LoggingStream() = default;
};

///
/// The logging stream factory function type.
///
/// The URI passed to the factory function will contain scheme specific information
/// for the creation of the logging stream.
///
using LoggingStreamFactory = LoggingStream * (*)(std::string_view uri);

//////////////////////////////// Logger class /////////////////////////////////

///
/// The main logger class.
///
/// All interaction with the logging system occurs through static methods in
/// this class or instance methods called on Logger references obtained from
/// the static Logger::getLogger method.
///
class Logger {
	/////////////////////// Logging system configuration //////////////////////

	///
	/// Get a reference to the instance of the logger identified by the supplied namespace.
	///
	/// If the logger instance cannot be obtained, the null logger will be returned
	/// and an error message will be logged to stderr.
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	/// @param loggingNamespace the logger's logging namespace
	///
	public: static Logger & getLogger(std::string_view loggingNamespace) noexcept;

	///
	/// Flush all logging streams.
	///
	/// This call will obtain a lock on the logging mutex.
	///
	public: static void flushAll();

	///
	/// A predefined reference to the global logger.
	///
	public: static Logger & globalLogger();

	///
	/// Flush all current logging streams and configure the logging system with the supplied configuration text.
	///
	/// Any placeholders supplied in the placeholders map will be used to expand any macros found in the configuration text.
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	/// @param configurationText the unparsed logging configuration
	/// @param placeholders zero or more placeholders to be used to expand any macros present in the string value properties
	/// @throw LoggingConfigurationException if logging reconfiguration has previously been locked.
	///
	public: static void configure(const std::string & configurationText,
	                              const std::map<std::string, std::string> & placeholders = std::map<std::string, std::string>());

	///
	/// Flush all current logging streams and configure the logging system with the supplied configuration.
	///
	/// Any placeholders supplied in the placeholders map will be used to expand any macros found in the configuration.
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	/// @param configuration the pre-parsed logging configuration (normally from the injector)
	/// @param placeholders zero or more placeholders to be used to expand any macros present in the string value properties
	/// @throw LoggingConfigurationException if logging reconfiguration has previously been locked.
	///
	public: static void configure(const std::shared_ptr<EnvironmentProperties> & configuration,
	                              const std::map<std::string, std::string> & placeholders = std::map<std::string, std::string>());

	///
	/// Flush all current logging streams and reconfigure the logging system to the default configuration.
	///
	/// The default configuration will be the default configuration plus the
	/// cascade of the configuration obtained from the balau-logging.hconf
	/// file if the file exists.
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	/// @throw LoggingConfigurationException if logging reconfiguration has previously been locked.
	///
	public: static void resetConfiguration();

	///
	/// Lock the configuration in order to prevent future reconfiguration.
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	/// @param throwOnReconfigure determines whether a subsequent call to the
	/// configure method will result in an exception (LoggingConfigurationException)
	/// being thrown or whether such a call will be silently ignored.
	///
	public: static void lockConfiguration(bool throwOnReconfigure = false);

	///
	/// Register a logging stream factory.
	///
	/// See LoggingStream for information on creating logging stream plugins.
	///
	public: static void registerLoggingStreamFactory(const std::string & scheme, LoggingStreamFactory factory);

	////////////////////////// Logger utility methods /////////////////////////

	///
	/// Get the logging level of this logger.
	///
	public: LoggingLevel getLevel() const noexcept {
		return level.load(std::memory_order_relaxed);
	}

	///
	/// Returns true if the logger is auto-flushing.
	///
	public: bool flushes() const noexcept {
		return shouldFlush.load(std::memory_order_relaxed);
	}

	///
	/// Flush the streams associated with the logger.
	///
	/// The logger may already be configured to flush after every message.
	///
	/// If the logging system is reconfigured during a call to flush, it
	/// is not guaranteed that all or any of the streams will be flushed.
	///
	public: void flush() const;

	///
	/// Get the logging namespace of this logger.
	///
	public: std::string getNamespace() const {
		return nameSpace;
	}

	///
	/// get the logger's configuration as a string (namespace and properties).
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	public: std::string getConfiguration() const;

	///
	/// Compare two loggers for equality.
	///
	public: bool operator == (const Logger & logger) const {
		return nameSpace == logger.nameSpace;
	}

	/////////////////////////// Logging level checks //////////////////////////

	///
	/// Is trace level logging enabled for this logger.
	///
	public: bool traceEnabled() const {
		return getLevel() >= LoggingLevel::TRACE;
	}

	///
	/// Is debug level logging enabled for this logger.
	///
	public: bool debugEnabled() const {
		return getLevel() >= LoggingLevel::DEBUG;
	}

	///
	/// Is info level logging enabled for this logger.
	///
	public: bool infoEnabled() const {
		return getLevel() >= LoggingLevel::INFO;
	}

	///
	/// Is warn level logging enabled for this logger.
	///
	public: bool warnEnabled() const {
		return getLevel() >= LoggingLevel::WARN;
	}

	///
	/// Is error level logging enabled for this logger.
	///
	public: bool errorEnabled() const {
		return getLevel() >= LoggingLevel::ERROR;
	}

	///
	/// Is logging at the specified level enabled for this logger.
	///
	public: bool enabled(LoggingLevel level) const {
		return getLevel() >= level;
	}

	/////////////////////////// Trace level logging ///////////////////////////

	///
	/// Log a trace message.
	///
	public: void trace(const char * message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message.
	///
	public: void trace(std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message with parameters.
	///
	public: template <typename ... ObjectT>
	void trace(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a trace message with parameters.
	///
	public: template <typename ... ObjectT>
	void trace(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a trace message via the supplied function.
	///
	public: void trace(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message and the source code location of the log message call site.
	///
	public: void trace(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message and the source code location of the log message call site.
	///
	public: void trace(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a trace message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a trace message via the supplied function and the source code location of the log message call site.
	///
	public: void trace(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::TRACE, *this, message);
		}
	}

	/////////////////////////// Debug level logging ///////////////////////////

	///
	/// Log a debug message.
	///
	public: void debug(const char * message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message.
	///
	public: void debug(std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message with parameters.
	///
	public: template <typename ... ObjectT>
	void debug(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a debug message with parameters.
	///
	public: template <typename ... ObjectT>
	void debug(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a debug message via the supplied function.
	///
	public: void debug(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message and the source code location of the log message call site.
	///
	public: void debug(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message and the source code location of the log message call site.
	///
	public: void debug(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a debug message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a debug message via the supplied function and the source code location of the log message call site.
	///
	public: void debug(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::DEBUG, *this, message);
		}
	}

	/////////////////////////// Info level logging ////////////////////////////

	///
	/// Log an info message.
	///
	public: void info(const char * message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message.
	///
	public: void info(std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message with parameters.
	///
	public: template <typename ... ObjectT>
	void info(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an info message with parameters.
	///
	public: template <typename ... ObjectT>
	void info(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an info message via the supplied function.
	///
	public: void info(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message and the source code location of the log message call site.
	///
	public: void info(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message and the source code location of the log message call site.
	///
	public: void info(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an info message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an info message via the supplied function and the source code location of the log message call site.
	///
	public: void info(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::INFO, *this, message);
		}
	}

	/////////////////////////// Warn level logging ////////////////////////////

	///
	/// Log a warn message.
	///
	public: void warn(const char * message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message.
	///
	public: void warn(std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message with parameters.
	///
	public: template <typename ... ObjectT>
	void warn(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a warn message with parameters.
	///
	public: template <typename ... ObjectT>
	void warn(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a warn message via the supplied function.
	///
	public: void warn(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message and the source code location of the log message call site.
	///
	public: void warn(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message and the source code location of the log message call site.
	///
	public: void warn(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a warn message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a warn message via the supplied function and the source code location of the log message call site.
	///
	public: void warn(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::WARN, *this, message);
		}
	}

	/////////////////////////// Error level logging ///////////////////////////

	///
	/// Log an error message.
	///
	public: void error(const char * message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message.
	///
	public: void error(std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message with parameters.
	///
	public: template <typename ... ObjectT>
	void error(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an error message with parameters.
	///
	public: template <typename ... ObjectT>
	void error(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a error message via the supplied function.
	///
	public: void error(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message and the source code location of the log message call site.
	///
	public: void error(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message and the source code location of the log message call site.
	///
	public: void error(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an error message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an error message via the supplied function and the source code location of the log message call site.
	///
	public: void error(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::ERROR, *this, message);
		}
	}

	///////////////////////// Specified level logging /////////////////////////

	///
	/// Log a message.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: void log(LoggingLevel specifiedLevel, const char * message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, *this, message);
		}
	}

	///
	/// Log a message.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: void log(LoggingLevel specifiedLevel, std::string_view message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, *this, message);
		}
	}

	///
	/// Log a message with parameters.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a message with parameters.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log a error message via the supplied function.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: void log(LoggingLevel specifiedLevel, const std::function<std::string ()> & function) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), specifiedLevel, *this, message);
		}
	}

	///
	/// Log an error message and the source code location of the log message call site.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, *this, message);
		}
	}

	///
	/// Log an error message and the source code location of the log message call site.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, *this, message);
		}
	}

	///
	/// Log an error message with parameters and the source code location of the log message call site.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an error message with parameters and the source code location of the log message call site.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, *this, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	///
	/// Log an error message via the supplied function and the source code location of the log message call site.
	///
	/// The error will be logged at the specified level if the level is enabled.
	///
	public: void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, specifiedLevel, *this, message);
		}
	}

	////////////////////////// Private implementation /////////////////////////

	// Local identifier of this logger.
	// Not const in order to support move semantics only.
	private: std::string identifier;

	// Full namespace of this logger.
	// Not const in order to support move semantics only.
	private: std::string nameSpace;

	// Abbreviated namespace of this logger.
	// Not const in order to support move semantics only.
	private: std::string ns;

	//
	// The logging level of this logger.
	//
	// Reads on this atomic are free on x86/x64.
	//
	private: std::atomic<LoggingLevel> level = { LoggingLevel::NONE };

	//
	// Indicates whether messages from this logger should flush the stream after each message.
	//
	// Reads on this atomic are free on x86/x64.
	//
	private: std::atomic_bool shouldFlush { true };

	//
	// The log items for which this logger is configured.
	//
	// The log item vector is a pointer in order to allow reconfiguration to occur
	// during logging, without the need for synchronisation.
	//
	// Log item vector instances are kept in the global logging system state and
	// are not deleted until exit.
	//
	// Apart from compiler ordering restrictions, reads on these atomics are
	// free on x86/x64.
	//
	private: std::atomic<std::vector<std::shared_ptr<LoggingSystem::LogItem>> *> logItems {};

	//
	// The streams to which this logger writes.
	// The array contains one stream pointer for each logging level (0 = error, 4 = trace).
	//
	// The streams are pointer in order to allow reconfiguration to occur during logging,
	// without the need for synchronisation.
	//
	// Stream instances are kept in the global logging system state and are not
	// deleted until exit.
	//
	// Apart from compiler ordering restrictions, reads on these atomics are
	// free on x86/x64.
	//
	private: std::array<std::atomic<LoggingStream *>, BALAU_LoggingLevelCount> streams {};

	friend class LoggingSystem::LoggerConfigurationVisitor;
	friend class LoggingSystem::LoggerPropertyVisitor;

	//
	// This logger's configuration properties.
	//
	// This is the complete set of configuration properties for this logger,
	// including the ones inherited from the parent logger.
	//
	// This field is not used during logging. It is only used at configuration
	// time, protected by the global logging mutex.
	//
	private: std::map<std::string, std::string> properties;

	private: Logger(std::string identifier_,
	                std::string nameSpace_,
	                std::string ns_,
	                const std::initializer_list<std::string> & propertyPairs);

	private: Logger() = default;
	private: Logger(Logger && rhs) noexcept;
	private: Logger(std::string && identifier_, std::string && nameSpace_, std::string && ns_) noexcept;

	public: Logger(const Logger &) = delete;
	public: Logger & operator = (const Logger &) = delete;

	private: void inheritConfiguration(const Logger & copy);

	// Non-parameter version.
	private: static void logMessage(const SourceCodeLocation & location,
	                                LoggingLevel level,
	                                const Logger & logger,
	                                std::string_view message);

	// Parameter version.
	private: static void logMessage(const SourceCodeLocation & location,
	                                LoggingLevel level,
	                                const Logger & logger,
	                                std::string_view message,
	                                const LoggingSystem::LoggerStringVector & parameters);

	friend class LoggingSystem::LoggerHolder;
	friend class LoggingSystem::LoggingState;
	friend class BalauLogger;
};

} // namespace Balau

//////////////// Logger call macros which include line and file ///////////////

///
/// Log a trace message with the source code location of the logging statement.
///
#define BalauLogTrace(LOGGER, ...) LOGGER.trace(BalauSourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log a debug message with the source code location of the logging statement.
///
#define BalauLogDebug(LOGGER, ...) LOGGER.debug(BalauSourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log an info message with the source code location of the logging statement.
///
#define BalauLogInfo(LOGGER, ...)  LOGGER.info(BalauSourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log a warn message with the source code location of the logging statement.
///
#define BalauLogWarn(LOGGER, ...)  LOGGER.warn(BalauSourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log an error message with the source code location of the logging statement.
///
#define BalauLogError(LOGGER, ...) LOGGER.error(BalauSourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log a message at the specified level and with the source code location of the logging statement.
///
#define BalauLogLog(LOGGER, LEVEL, ...) LOGGER.log(LoggingLevel::LEVEL, BalauSourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER
