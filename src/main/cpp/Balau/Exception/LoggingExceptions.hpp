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
/// @file LoggingExceptions.hpp
///
/// %Balau exceptions for the logging framework.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__LOGGING_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__LOGGING_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// %Exception thrown by the logging system configuration.
///
/// Thrown during logging configuration when the configuration is invalid (unknown
/// stream scheme, attempt to reconfigure after locking the configuration, etc.).
///
class LoggingConfigurationException : public BalauException {
	public: LoggingConfigurationException(const char * file, int line, const std::string & st, const std::string & text)
		: BalauException(file, line, st, "LoggingConfiguration", text) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__LOGGING_EXCEPTIONS
