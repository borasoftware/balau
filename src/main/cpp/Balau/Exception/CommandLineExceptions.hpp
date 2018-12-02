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
/// @file CommandLineExceptions.hpp
///
/// %Balau exceptions for the command line parser.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__COMMAND_LINE_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__COMMAND_LINE_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// Base class of command line exception classes.
///
class CommandlineException : public BalauException {
	public: CommandlineException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "Commandline", text) {}

	public: CommandlineException(const char * file, int line, const std::string & name, const std::string & text)
		: BalauException(file, line, name, text) {}
};

///
/// Thrown when an invalid command line parameter is specified.
///
class OptionNotFoundException : public CommandlineException {
	public: OptionNotFoundException(const char * file, int line, const std::string & key)
		: CommandlineException(file, line, "OptionNotFound", key) {}
};

///
/// Thrown when there is no option value available and one is required.
///
class MissingOptionValueException : public CommandlineException {
	public: MissingOptionValueException(const char * file, int line, const std::string & )
		: CommandlineException(file, line, "MissingOptionValue", "") {}
};

///
/// Thrown when there is no final value available and one is required.
///
class MissingFinalValueException : public CommandlineException {
	public: MissingFinalValueException(const char * file, int line, const std::string & )
		: CommandlineException(file, line, "MissingFinalValue", "") {}
};

///
/// Thrown when a requested option value cannot be coerced into the required type.
///
class OptionValueException : public CommandlineException {
	public: OptionValueException(const char * file, int line, const std::string & key)
		: CommandlineException(file, line, "OptionValue", key) {}
};

inline std::string toString(const OptionValueException & e) {
	return e.what();
}

} // namespace Balau::Exception

inline std::string toString(const Balau::Exception::CommandlineException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::OptionNotFoundException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::MissingOptionValueException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::MissingFinalValueException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::OptionValueException & e) {
	return e.what();
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__COMMAND_LINE_EXCEPTIONS
