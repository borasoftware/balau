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
/// @file SystemExceptions.hpp
///
/// %Balau exceptions for system utilities.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__SYSTEM_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__SYSTEM_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// Abstract base class of exceptions that report errno.
///
class ErrNoException : public BalauException {
	public: int errorNumber;

	protected: ErrNoException(const char * file,
	                          int line,
	                          const std::string & st,
	                          int errorNumber_,
	                          const std::string & name)
		: BalauException(file, line, st, name, " - error: " + ::toString(errorNumber_))
		, errorNumber(errorNumber_) {}

	protected: ErrNoException(const char * file,
	                          int line,
	                          const std::string & st,
	                          int errorNumber_,
	                          const std::string & name,
	                          const std::string & message_)
		: BalauException(file, line, st, name, " - error: " + ::toString(errorNumber_) + " - " + message_)
		, errorNumber(errorNumber_) {}
};

inline bool operator == (const ErrNoException & lhs, const ErrNoException & rhs) {
	return typeid(lhs) == typeid(rhs) && lhs.message == rhs.message && lhs.errorNumber == rhs.errorNumber;
}

///
/// Thrown when a fork call fails.
///
/// EAGAIN: no resources to create another process
/// ENOMEM: insufficient storage space is available
///
class ForkException : public ErrNoException {
	public: ForkException(const char * file,
	                      int line,
	                      const std::string & st,
	                      int errorNumber_,
	                      const std::string & message_)
		: ErrNoException(file, line, st, errorNumber_, "Fork", message_) {}
};

///
/// Thrown when a wait call fails.
///
class WaitException : public ErrNoException {
	public: WaitException(const char * file,
	                      int line,
	                      const std::string & st,
	                      int errorNumber_,
	                      const std::string & message_)
		: ErrNoException(file, line, st, errorNumber_, "Wait", message_) {}

	public: WaitException(const char * file, int line, const std::string & st, int errorNumber_)
		: ErrNoException(file, line, st, errorNumber_, "Wait") {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__SYSTEM_EXCEPTIONS
