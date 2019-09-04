// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "LogWriter.hpp"

#include "Balau/Logging/Logger.hpp"

namespace Balau::Testing {

LogWriter::LogWriter(const std::string & loggingNamespace)
	: logger(Logger::getLogger(loggingNamespace)) {}

void LogWriter::writeString(const std::string & str) {
	logger.info(str.c_str());
}

} // namespace Balau::Testing
