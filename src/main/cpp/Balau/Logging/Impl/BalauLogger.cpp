// @formatter:off
//
// Balau core C++ library
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
                             const LoggingSystem::LoggerStringVector & parameters) const {
	Logger::logMessage(location, level, logger, message, parameters);
}

} // namespace Balau
