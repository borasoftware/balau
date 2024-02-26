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
/// @file LoggingLevel.hpp
///
/// The logging system's logging level.
///

#ifndef COM_BORA_SOFTWARE_BALAU_LOGGING_LOGGING_LEVEL
#define COM_BORA_SOFTWARE_BALAU_LOGGING_LOGGING_LEVEL

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
constexpr size_t BALAU_LoggingLevelCount = 5;

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

} // namespace Balau

#endif // COM_BORA_SOFTWARE_BALAU_LOGGING_LOGGING_LEVEL
