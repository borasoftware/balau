// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "BalauLogger.hpp"
#include "../Logger.hpp"

namespace Balau {

LoggingLevel BalauLogger::getLevel() const noexcept {
	return logger.getLevel();
}

BalauLogger::BalauLogger(const char * loggingNamespace)
	: logger(Logger::getLogger(loggingNamespace)) {}

BalauLogger::BalauLogger(const std::string & loggingNamespace)
	: logger(Logger::getLogger(loggingNamespace)) {}

BalauLogger::BalauLogger(const Logger & logger_)
	: logger(logger_) {}

BalauLogger::BalauLogger(const BalauLogger & copy)
	: logger(copy.logger) {}

void BalauLogger::logMessage(const SourceCodeLocation & location,
                             LoggingLevel level,
                             std::string_view message) const {
	Logger::logMessage(location, level, logger, message);
}

void BalauLogger::logMessage(const SourceCodeLocation & location,
                             LoggingLevel level,
                             std::string_view message,
                             const std::vector<std::string> & parameters) const {
	Logger::logMessage(location, level, logger, message, parameters);
}

} // namespace Balau
