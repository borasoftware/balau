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

#include <Balau/Logging/LoggingLevel.hpp>
#include <Balau/Logging/Impl/LoggerForwardDeclarations.hpp>
#include <Balau/Util/Vectors.hpp>

#include <functional>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

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
	public: BalauLogger(const char * loggingNamespace);

	//
	// Construct a Balau library logger by providing the logger reference.
	//
	public: BalauLogger(const Logger & logger_);

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
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message.
	//
	public: void trace(std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message with parameters.
	//
	public: template <typename ... ObjectT>
	void trace(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a trace message with parameters.
	//
	public: template <typename ... ObjectT>
	void trace(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a trace message via the supplied function.
	//
	public: void trace(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message and the source code location of the log message call site.
	//
	public: void trace(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message and the source code location of the log message call site.
	//
	public: void trace(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, message);
		}
	}

	//
	// Log a trace message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a trace message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a trace message via the supplied function and the source code location of the log message call site.
	//
	public: void trace(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message.
	//
	public: void debug(std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message with parameters.
	//
	public: template <typename ... ObjectT>
	void debug(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a debug message with parameters.
	//
	public: template <typename ... ObjectT>
	void debug(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a debug message via the supplied function.
	//
	public: void debug(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message and the source code location of the log message call site.
	//
	public: void debug(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message and the source code location of the log message call site.
	//
	public: void debug(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, message);
		}
	}

	//
	// Log a debug message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a debug message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a debug message via the supplied function and the source code location of the log message call site.
	//
	public: void debug(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message.
	//
	public: void info(std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message with parameters.
	//
	public: template <typename ... ObjectT>
	void info(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an info message with parameters.
	//
	public: template <typename ... ObjectT>
	void info(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an info message via the supplied function.
	//
	public: void info(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message and the source code location of the log message call site.
	//
	public: void info(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message and the source code location of the log message call site.
	//
	public: void info(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, message);
		}
	}

	//
	// Log an info message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an info message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an info message via the supplied function and the source code location of the log message call site.
	//
	public: void info(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message.
	//
	public: void warn(std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message with parameters.
	//
	public: template <typename ... ObjectT>
	void warn(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a warn message with parameters.
	//
	public: template <typename ... ObjectT>
	void warn(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a warn message via the supplied function.
	//
	public: void warn(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message and the source code location of the log message call site.
	//
	public: void warn(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message and the source code location of the log message call site.
	//
	public: void warn(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, message);
		}
	}

	//
	// Log a warn message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a warn message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a warn message via the supplied function and the source code location of the log message call site.
	//
	public: void warn(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message.
	//
	public: void error(std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message with parameters.
	//
	public: template <typename ... ObjectT>
	void error(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an error message with parameters.
	//
	public: template <typename ... ObjectT>
	void error(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log a error message via the supplied function.
	//
	public: void error(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message and the source code location of the log message call site.
	//
	public: void error(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message and the source code location of the log message call site.
	//
	public: void error(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, message);
		}
	}

	//
	// Log an error message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an error message with parameters and the source code location of the log message call site.
	//
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, message, ebm(::toString(object) ... ));
		}
	}

	//
	// Log an error message via the supplied function and the source code location of the log message call site.
	//
	public: void error(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			const std::string message = function();
			logMessage(location, LoggingLevel::ERROR, message);
		}
	}

	////////////////////////// Private implementation /////////////////////////

	// The logger to which this class forms a proxy.
	private: const Logger & logger;

	// Shortened version of emplace back multiple.
	private: template <typename ... StringT>
	static std::vector<std::string> ebm(std::string && first, StringT && ... remaining) {
		return Util::Vectors::pushBack(std::move(first), std::forward<StringT>(remaining) ...);
	}

	// Non-parameter version.
	private: void logMessage(const SourceCodeLocation & location,
	                         LoggingLevel level,
	                         std::string_view message) const;

	// Parameter version.
	private: void logMessage(const SourceCodeLocation & location,
	                         LoggingLevel level,
	                         std::string_view message,
	                         const std::vector<std::string> & parameters) const;
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

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__BALAU_LOGGER
