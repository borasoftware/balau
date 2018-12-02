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
	                          int errorNumber_,
	                          const std::string & name)
		: BalauException(file, line, name, " - error: " + ::toString(errorNumber_))
		, errorNumber(errorNumber_) {}

	protected: ErrNoException(const char * file,
	                          int line,
	                          int errorNumber_,
	                          const std::string & name,
	                          const std::string & message_)
		: BalauException(file, line, name, " - error: " + ::toString(errorNumber_) + " - " + message_)
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
	public: ForkException(const char * file, int line, int errorNumber_, const std::string & message_)
		: ErrNoException(file, line, errorNumber_, "Fork", message_) {}
};

///
/// Thrown when a wait call fails.
///
class WaitException : public ErrNoException {
	public: WaitException(const char * file, int line, int errorNumber_, const std::string & message_)
		: ErrNoException(file, line, errorNumber_, "Wait", message_) {}

	public: WaitException(const char * file, int line, int errorNumber_)
		: ErrNoException(file, line, errorNumber_, "Wait") {}
};

} // namespace Balau::Exception

inline std::string toString(const Balau::Exception::ErrNoException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::ForkException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::WaitException & e) {
	return e.what();
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__SYSTEM_EXCEPTIONS
