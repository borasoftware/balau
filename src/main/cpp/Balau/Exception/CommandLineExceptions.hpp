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
	public: CommandlineException(const char * file, int line, const std::string & st, const std::string & text)
		: BalauException(file, line, st, "Commandline", text) {}

	public: CommandlineException(const char * file, int line, const std::string & st, const std::string & name, const std::string & text)
		: BalauException(file, line, st, name, text) {}
};

///
/// Thrown when an invalid command line parameter is specified.
///
class OptionNotFoundException : public CommandlineException {
	public: OptionNotFoundException(const char * file, int line, const std::string & st, const std::string & key)
		: CommandlineException(file, line, st, "OptionNotFound", key) {}
};

///
/// Thrown when there is no option value available and one is required.
///
class MissingOptionValueException : public CommandlineException {
	public: MissingOptionValueException(const char * file, int line, const std::string & st, const std::string & )
		: CommandlineException(file, line, st, "MissingOptionValue", "") {}
};

///
/// Thrown when there is no final value available and one is required.
///
class MissingFinalValueException : public CommandlineException {
	public: MissingFinalValueException(const char * file, int line, const std::string & st, const std::string & )
		: CommandlineException(file, line, st, "MissingFinalValue", "") {}
};

///
/// Thrown when a requested option value cannot be coerced into the required type.
///
class OptionValueException : public CommandlineException {
	public: OptionValueException(const char * file, int line, const std::string & st, const std::string & key)
		: CommandlineException(file, line, st, "OptionValue", key) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__COMMAND_LINE_EXCEPTIONS
