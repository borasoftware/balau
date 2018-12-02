// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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

#include <Balau/Logging/LoggingLevel.hpp>
#include <Balau/Logging/Impl/LoggerForwardDeclarations.hpp>
#include <Balau/Util/Vectors.hpp>

#include <functional>

namespace Balau {

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
	public: virtual void write(const std::string & str) = 0;

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
	/// Flush all current logging streams and configure the logging system with the supplied configuration.
	///
	/// This call will obtain a lock on the logging system mutex.
	///
	/// @throw LoggingConfigurationException if logging reconfiguration has previously been locked.
	///
	public: static void configure(const std::string & configurationText);

	///
	/// Flush all current logging streams and reconfigure the logging system to the default configuration.
	///
	/// The default configuration will be the default configuration plus the
	/// cascade of the configuration obtained from the balau-logging.conf
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
	/// Flush the streams associated with the logger.
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
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message.
	///
	public: void trace(std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message with parameters.
	///
	public: template <typename ... ObjectT>
	void trace(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a trace message with parameters.
	///
	public: template <typename ... ObjectT>
	void trace(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a trace message via the supplied function.
	///
	public: void trace(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message and the source code location of the log message call site.
	///
	public: void trace(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message and the source code location of the log message call site.
	///
	public: void trace(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, *this, message);
		}
	}

	///
	/// Log a trace message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a trace message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void trace(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::TRACE) {
			logMessage(location, LoggingLevel::TRACE, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a trace message via the supplied function and the source code location of the log message call site.
	///
	public: void trace(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::TRACE) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message.
	///
	public: void debug(std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message with parameters.
	///
	public: template <typename ... ObjectT>
	void debug(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a debug message with parameters.
	///
	public: template <typename ... ObjectT>
	void debug(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a debug message via the supplied function.
	///
	public: void debug(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message and the source code location of the log message call site.
	///
	public: void debug(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message and the source code location of the log message call site.
	///
	public: void debug(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, *this, message);
		}
	}

	///
	/// Log a debug message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a debug message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void debug(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
			logMessage(location, LoggingLevel::DEBUG, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a debug message via the supplied function and the source code location of the log message call site.
	///
	public: void debug(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::DEBUG) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message.
	///
	public: void info(std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message with parameters.
	///
	public: template <typename ... ObjectT>
	void info(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an info message with parameters.
	///
	public: template <typename ... ObjectT>
	void info(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an info message via the supplied function.
	///
	public: void info(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message and the source code location of the log message call site.
	///
	public: void info(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message and the source code location of the log message call site.
	///
	public: void info(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, *this, message);
		}
	}

	///
	/// Log an info message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an info message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void info(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::INFO) {
			logMessage(location, LoggingLevel::INFO, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an info message via the supplied function and the source code location of the log message call site.
	///
	public: void info(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::INFO) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message.
	///
	public: void warn(std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message with parameters.
	///
	public: template <typename ... ObjectT>
	void warn(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a warn message with parameters.
	///
	public: template <typename ... ObjectT>
	void warn(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a warn message via the supplied function.
	///
	public: void warn(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message and the source code location of the log message call site.
	///
	public: void warn(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message and the source code location of the log message call site.
	///
	public: void warn(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, *this, message);
		}
	}

	///
	/// Log a warn message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a warn message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void warn(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::WARN) {
			logMessage(location, LoggingLevel::WARN, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a warn message via the supplied function and the source code location of the log message call site.
	///
	public: void warn(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::WARN) {
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
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message.
	///
	public: void error(std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message with parameters.
	///
	public: template <typename ... ObjectT>
	void error(const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an error message with parameters.
	///
	public: template <typename ... ObjectT>
	void error(std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log a error message via the supplied function.
	///
	public: void error(const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			const std::string message = function();
			logMessage(SourceCodeLocation(), LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message and the source code location of the log message call site.
	///
	public: void error(const SourceCodeLocation & location, const char * message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message and the source code location of the log message call site.
	///
	public: void error(const SourceCodeLocation & location, std::string_view message) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, *this, message);
		}
	}

	///
	/// Log an error message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, const char * message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an error message with parameters and the source code location of the log message call site.
	///
	public: template <typename ... ObjectT>
	void error(const SourceCodeLocation & location, std::string_view message, const ObjectT & ... object) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			logMessage(location, LoggingLevel::ERROR, *this, message, ebm(::toString(object) ... ));
		}
	}

	///
	/// Log an error message via the supplied function and the source code location of the log message call site.
	///
	public: void error(const SourceCodeLocation & location, const std::function<std::string ()> & function) const {
		if (getLevel() >= LoggingLevel::ERROR) {
			const std::string message = function();
			logMessage(location, LoggingLevel::ERROR, *this, message);
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
	private: std::atomic<LoggingLevel> level = LoggingLevel::NONE;

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
	private: std::array<std::atomic<LoggingStream *>, _BalauLoggingLevelCount> streams {};

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

	// Shortened version of emplace back multiple.
	private: template <typename ... StringT>
	static std::vector<std::string> ebm(std::string && first, StringT && ... remaining) {
		return Util::Vectors::pushBack(std::move(first), std::forward<StringT>(remaining) ...);
	}

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
	                                const std::vector<std::string> & parameters);

	friend class LoggingSystem::LoggerHolder;
	friend class LoggingSystem::LoggingState;
	friend class BalauLogger;
};

///
/// A predefined reference to the global logger.
///
extern Logger & globalLogger;

} // namespace Balau

//////////////// Logger call macros which include line and file ///////////////

///
/// Log a trace message with the source code location of the logging statement.
///
#define BalauLogTrace(LOGGER, ...) LOGGER.trace(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log a debug message with the source code location of the logging statement.
///
#define BalauLogDebug(LOGGER, ...) LOGGER.debug(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log an info message with the source code location of the logging statement.
///
#define BalauLogInfo(LOGGER, ...)  LOGGER.info(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log a warn message with the source code location of the logging statement.
///
#define BalauLogWarn(LOGGER, ...)  LOGGER.warn(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Log an error message with the source code location of the logging statement.
///
#define BalauLogError(LOGGER, ...) LOGGER.error(SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGER
