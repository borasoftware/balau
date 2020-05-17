// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__BALAU_LOGGER
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__BALAU_LOGGER

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Logging/LoggingLevel.hpp>
#include <Balau/Logging/Impl/LoggerAllocator.hpp>
#include <Balau/Logging/Impl/LoggerForwardDeclarations.hpp>
#include <Balau/Type/SourceCodeLocation.hpp>

#include <functional>

namespace Balau {

class Logger;

//
// Logger class used in Balau code.
//
// This prevents the main Logger.hpp header from being included in any Balau library header.
//
// The public API of this class exactly matches the public API of the Logger class.
//
class BalauLogger {
	//
	// Construct a Balau library logger by specifying the logging namespace.
	//
	public: explicit BalauLogger(const char * loggingNamespace);

	//
	// Construct a Balau library logger by specifying the logging namespace.
	//
	public: explicit BalauLogger(const std::string & loggingNamespace);

	//
	// Construct a Balau library logger by providing the logger reference.
	//
	public: explicit BalauLogger(const Logger & logger_);

	//
	// Construct a Balau library logger by copying another balau Balau library logger.
	//
	public: BalauLogger(const BalauLogger & copy);

	//
	// Get the logging level of this logger.
	//
	public: LoggingLevel getLevel() const noexcept;

	/////////////////////////// Logging level checks //////////////////////////

	//
	// Is trace level logging enabled for this logger.
	//
	public: bool traceEnabled() const {
		return getLevel() >= LoggingLevel::TRACE;
	}

	//
	// Is debug level logging enabled for this logger.
	//
	public: bool debugEnabled() const {
		return getLevel() >= LoggingLevel::DEBUG;
	}

	//
	// Is info level logging enabled for this logger.
	//
	public: bool infoEnabled() const {
		return getLevel() >= LoggingLevel::INFO;
	}

	//
	// Is warn level logging enabled for this logger.
	//
	public: bool warnEnabled() const {
		return getLevel() >= LoggingLevel::WARN;
	}

	//
	// Is error level logging enabled for this logger.
	//
	public: bool errorEnabled() const {
		return getLevel() >= LoggingLevel::ERROR;
	}

	//
	// Is logging at the specified level enabled for this logger.
	//
	public: bool enabled(LoggingLevel level) const {
		return getLevel() >= level;
	}

	/////////////////////////// Trace level logging ///////////////////////////

	//
	// Log a trace message.
	//
	public: void trace(const char * message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message.
	//
	public: void trace(std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message with parameters.
	//
	public: template <typename ... ObjectT>
	void trace(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a trace message with parameters.
	//
	public: template <typename ... ObjectT>
	void trace(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a trace message via the supplied function.
	//
	public: void trace(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message and the source code location of the log message call site.
	//
	public: void trace(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message and the source code location of the log message call site.
	//
	public: void trace(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a trace message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::TRACE, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a trace message via the supplied function and the source code location of the log message call site.
	//
	public: void trace(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::TRACE, message);
		}
	}

	/////////////////////////// Debug level logging ///////////////////////////

	//
	// Log a debug message.
	//
	public: void debug(const char * message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message.
	//
	public: void debug(std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message with parameters.
	//
	public: template <typename ... ObjectT>
	void debug(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a debug message with parameters.
	//
	public: template <typename ... ObjectT>
	void debug(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a debug message via the supplied function.
	//
	public: void debug(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message and the source code location of the log message call site.
	//
	public: void debug(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message and the source code location of the log message call site.
	//
	public: void debug(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a debug message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::DEBUG, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a debug message via the supplied function and the source code location of the log message call site.
	//
	public: void debug(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::DEBUG, message);
		}
	}

	/////////////////////////// Info level logging ////////////////////////////

	//
	// Log an info message.
	//
	public: void info(const char * message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message.
	//
	public: void info(std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message with parameters.
	//
	public: template <typename ... ObjectT>
	void info(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an info message with parameters.
	//
	public: template <typename ... ObjectT>
	void info(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an info message via the supplied function.
	//
	public: void info(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message and the source code location of the log message call site.
	//
	public: void info(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message and the source code location of the log message call site.
	//
	public: void info(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an info message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::INFO, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an info message via the supplied function and the source code location of the log message call site.
	//
	public: void info(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::INFO, message);
		}
	}

	/////////////////////////// Warn level logging ////////////////////////////

	//
	// Log a warn message.
	//
	public: void warn(const char * message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message.
	//
	public: void warn(std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message with parameters.
	//
	public: template <typename ... ObjectT>
	void warn(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a warn message with parameters.
	//
	public: template <typename ... ObjectT>
	void warn(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a warn message via the supplied function.
	//
	public: void warn(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message and the source code location of the log message call site.
	//
	public: void warn(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message and the source code location of the log message call site.
	//
	public: void warn(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a warn message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::WARN, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a warn message via the supplied function and the source code location of the log message call site.
	//
	public: void warn(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::WARN, message);
		}
	}

	/////////////////////////// Error level logging ///////////////////////////

	//
	// Log an error message.
	//
	public: void error(const char * message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message.
	//
	public: void error(std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message with parameters.
	//
	public: template <typename ... ObjectT>
	void error(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an error message with parameters.
	//
	public: template <typename ... ObjectT>
	void error(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a error message via the supplied function.
	//
	public: void error(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message and the source code location of the log message call site.
	//
	public: void error(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message and the source code location of the log message call site.
	//
	public: void error(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an error message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			logMessage(location, LoggingLevel::ERROR, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an error message via the supplied function and the source code location of the log message call site.
	//
	public: void error(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, LoggingLevel::ERROR, message);
		}
	}

	///////////////////////// Specified level logging /////////////////////////

	//
	// Log a message.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: void log(LoggingLevel specifiedLevel, const char * message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel,  message);
		}
	}

	//
	// Log a message.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: void log(LoggingLevel specifiedLevel, std::string_view message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, message);
		}
	}

	//
	// Log a message with parameters.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a message with parameters.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(SourceCodeLocation(), specifiedLevel, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log a error message via the supplied function.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: void log(LoggingLevel specifiedLevel, const std::function<std::string ()> & function) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(SourceCodeLocation(), specifiedLevel, message);
		}
	}

	//
	// Log an error message and the source code location of the log message call site.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, message);
		}
	}

	//
	// Log an error message and the source code location of the log message call site.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, message);
		}
	}

	//
	// Log an error message with parameters and the source code location of the log message call site.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an error message with parameters and the source code location of the log message call site.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: template <typename ... ObjectT>
	void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			logMessage(location, specifiedLevel, message, LoggingSystem::makeStringVector(object ... ));
		}
	}

	//
	// Log an error message via the supplied function and the source code location of the log message call site.
	//
	// The error will be logged at the specified level if the level is enabled.
	//
	public: void log(LoggingLevel specifiedLevel, const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= specifiedLevel) {
			LoggingSystem::startLogAllocation();
			const std::string message = function();
			logMessage(location, specifiedLevel, message);
		}
	}

	////////////////////////// Private implementation /////////////////////////

	// The logger to which this class forms a proxy.
	private: const Logger & logger;

	// Non-parameter version.
	private: void logMessage(const SourceCodeLocation & location,
	                         LoggingLevel level,
	                         std::string_view message) const;

	// Parameter version.
	private: void logMessage(const SourceCodeLocation & location,
	                         LoggingLevel level,
	                         std::string_view message,
	                         const LoggingSystem::LoggerStringVector & parameters) const;
};

} // namespace Balau

///////////// Balau logger call macros which include line and file ////////////

//
// Log a trace message with the source code location of the logging statement.
//
#define BalauBalauLogTrace(LOGGER, ...) LOGGER.trace(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

//
// Log a debug message with the source code location of the logging statement.
//
#define BalauBalauLogDebug(LOGGER, ...) LOGGER.debug(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

//
// Log an info message with the source code location of the logging statement.
//
#define BalauBalauLogInfo(LOGGER, ...)  LOGGER.info(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

//
// Log a warn message with the source code location of the logging statement.
//
#define BalauBalauLogWarn(LOGGER, ...)  LOGGER.warn(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

//
// Log an error message with the source code location of the logging statement.
//
#define BalauBalauLogError(LOGGER, ...) LOGGER.error(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__BALAU_LOGGER
