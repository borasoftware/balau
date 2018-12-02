// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ITEM_PARAMETERS
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ITEM_PARAMETERS

#include <Balau/Logging/Logger.hpp>

#include <chrono>

namespace Balau::LoggingSystem {

// Parameters passed as single reference to log items during logging.
struct LoggerItemParameters {
	std::ostringstream & builder;
	const std::string & nameSpace; // NOLINT
	const std::string & ns; // NOLINT
	const LoggingLevel level;
	const char * filename;
	const unsigned int line;
	const std::string_view message;
	const std::chrono::system_clock::time_point & timePoint;

	LoggerItemParameters(std::ostringstream & builder_,
	                     const std::string & nameSpace_,
	                     const std::string & ns_,
	                     const LoggingLevel level_,
	                     const char * filename_,
	                     unsigned int line_,
	                     std::string_view message_,
	                     const std::chrono::system_clock::time_point & timePoint_)
		: builder(builder_)
		, nameSpace(nameSpace_)
		, ns(ns_)
		, level(level_)
		, filename(filename_)
		, line(line_)
		, message(message_)
		, timePoint(timePoint_) {}
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ITEM_PARAMETERS
