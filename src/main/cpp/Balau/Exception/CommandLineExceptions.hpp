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
	public: CommandlineException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "Commandline", text) {}

	public: CommandlineException(const std::string & st, const std::string & text)
		: BalauException(st, "Commandline", text) {}

	public: CommandlineException(SourceCodeLocation location, const std::string & st, const std::string & name, const std::string & text)
		: BalauException(location, st, name, text) {}

	public: CommandlineException(const std::string & st, const std::string & name, const std::string & text)
		: BalauException(st, name, text) {}
};

///
/// Thrown when an invalid command line parameter is specified.
///
class OptionNotFoundException : public CommandlineException {
	public: OptionNotFoundException(SourceCodeLocation location, const std::string & st, const std::string & key)
		: CommandlineException(location, st, "OptionNotFound", key) {}

	public: OptionNotFoundException(const std::string & st, const std::string & key)
		: CommandlineException(SourceCodeLocation(), st, "OptionNotFound", key) {}
};

///
/// Thrown when there is no option value available and one is required.
///
class MissingOptionValueException : public CommandlineException {
	public: MissingOptionValueException(SourceCodeLocation location, const std::string & st, const std::string & )
		: CommandlineException(location, st, "MissingOptionValue", "") {}

	public: MissingOptionValueException(const std::string & st, const std::string & )
		: CommandlineException(st, "MissingOptionValue", "") {}
};

///
/// Thrown when there is no final value available and one is required.
///
class MissingFinalValueException : public CommandlineException {
	public: MissingFinalValueException(SourceCodeLocation location, const std::string & st, const std::string & )
		: CommandlineException(location, st, "MissingFinalValue", "") {}

	public: MissingFinalValueException(const std::string & st, const std::string & )
		: CommandlineException(st, "MissingFinalValue", "") {}
};

///
/// Thrown when a requested option value cannot be coerced into the required type.
///
class OptionValueException : public CommandlineException {
	public: OptionValueException(SourceCodeLocation location, const std::string & st, const std::string & key)
		: CommandlineException(location, st, "OptionValue", key) {}

	public: OptionValueException(const std::string & st, const std::string & key)
		: CommandlineException(st, "OptionValue", key) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__COMMAND_LINE_EXCEPTIONS
