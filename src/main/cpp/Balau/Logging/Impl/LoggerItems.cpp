// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "LoggerItems.hpp"

namespace Balau::LoggingSystem {

const char * LowercaseLevelLoggerItem::lowercaseLevelText[] = {
	"error", "warn", "info", "debug", "trace"
};

const char * UppercaseLevelLoggerItem::uppercaseLevelText[] = {
	"ERROR", "WARN", "INFO", "DEBUG", "TRACE"
};

} // namespace Balau::LoggingSystem
