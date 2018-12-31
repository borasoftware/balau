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
/// @file LoggingLevel.hpp
///
/// The logging system's logging level.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGING_LEVEL
#define COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGING_LEVEL

#include <Balau/Exception/BalauException.hpp>

namespace Balau {

//////////////////////////////// Logging level ////////////////////////////////

///
/// The logging level.
///
/// A logger will log a message to the level specified in the logging call if
/// the logger is configured to log to the specified level or below.
///
enum class LoggingLevel {
	///
	/// Log to the logger's trace stream if the logger is trace enabled.
	///
	TRACE =  4,

	///
	/// Log to the logger's debug stream if the logger is debug enabled.
	///
	DEBUG =  3,

	///
	/// Log to the logger's info stream if the logger is info enabled.
	///
	INFO  =  2,

	///
	/// Log to the logger's warn stream if the logger is warn enabled.
	///
	WARN  =  1,

	///
	/// Log to the logger's error stream if the logger is error enabled.
	///
	ERROR =  0,

	///
	/// Used to indicate that a logger is not enabled.
	///
	NONE  = -1
};

// The number of logging levels not including NONE (which never results in logging).
constexpr size_t _BalauLoggingLevelCount = 5;

///
/// Print the logging level as a UTF-8 string.
///
/// @return a UTF-8 string representing the logging level
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(LoggingLevel level) {
	switch (level) {
		case LoggingLevel::TRACE:    return "TRACE";
		case LoggingLevel::DEBUG:    return "DEBUG";
		case LoggingLevel::INFO:     return "INFO";
		case LoggingLevel::WARN:     return "WARN";
		case LoggingLevel::ERROR:    return "ERROR";
		case LoggingLevel::NONE:     return "NONE";
		default: ThrowBalauException(Exception::BugException, "Unhandled LoggingLevel in switch statement.");
	}
}

///
/// Print the logging level as a UTF-8 string.
///
/// @return a UTF-8 string representing the logging level
///
inline std::string toString(LoggingLevel level) {
	switch (level) {
		case LoggingLevel::TRACE:    return "TRACE";
		case LoggingLevel::DEBUG:    return "DEBUG";
		case LoggingLevel::INFO:     return "INFO";
		case LoggingLevel::WARN:     return "WARN";
		case LoggingLevel::ERROR:    return "ERROR";
		case LoggingLevel::NONE:     return "NONE";
		default: ThrowBalauException(Exception::BugException, "Unhandled LoggingLevel in switch statement.");
	}
}

///
/// A distinct type used for source code file and line number.
///
/// This prevents function overload issues that would occur were the logging
/// methods to use const char * file, unsigned int line instead.
///
struct SourceCodeLocation {
	const char * file;
	unsigned int line;

	SourceCodeLocation()
		: file(nullptr)
		, line(0) {}

	SourceCodeLocation(const char * file_, unsigned int line_)
		: file(file_)
		, line(line_) {}
};

} // namespace Balau

template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(Balau::LoggingLevel level) {
	return Balau::toString<AllocatorT>(level);
}

inline std::string toString(Balau::LoggingLevel level) {
	return Balau::toString(level);
}

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING__LOGGING_LEVEL
