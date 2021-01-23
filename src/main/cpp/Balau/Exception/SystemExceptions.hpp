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

	protected: ErrNoException(SourceCodeLocation location,
	                          const std::string & st,
	                          int errorNumber_,
	                          const std::string & name)
		: BalauException(location, st, name, " - error: " + ::toString(errorNumber_))
		, errorNumber(errorNumber_) {}

	protected: ErrNoException(const std::string & st,
	                          int errorNumber_,
	                          const std::string & name)
		: BalauException(st, name, " - error: " + ::toString(errorNumber_))
		, errorNumber(errorNumber_) {}

	protected: ErrNoException(SourceCodeLocation location,
	                          const std::string & st,
	                          int errorNumber_,
	                          const std::string & name,
	                          const std::string & message_)
		: BalauException(location, st, name, " - error: " + ::toString(errorNumber_) + " - " + message_)
		, errorNumber(errorNumber_) {}

	protected: ErrNoException(const std::string & st,
	                          int errorNumber_,
	                          const std::string & name,
	                          const std::string & message_)
		: BalauException(st, name, " - error: " + ::toString(errorNumber_) + " - " + message_)
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
	public: ForkException(SourceCodeLocation location,
	                      const std::string & st,
	                      int errorNumber_,
	                      const std::string & message_)
		: ErrNoException(location, st, errorNumber_, "Fork", message_) {}

	public: ForkException(const std::string & st,
	                      int errorNumber_,
	                      const std::string & message_)
		: ErrNoException(st, errorNumber_, "Fork", message_) {}
};

///
/// Thrown when a wait call fails.
///
class WaitException : public ErrNoException {
	public: WaitException(SourceCodeLocation location,
	                      const std::string & st,
	                      int errorNumber_,
	                      const std::string & message_)
		: ErrNoException(location, st, errorNumber_, "Wait", message_) {}

	public: WaitException(const std::string & st,
	                      int errorNumber_,
	                      const std::string & message_)
		: ErrNoException(st, errorNumber_, "Wait", message_) {}

	public: WaitException(SourceCodeLocation location, const std::string & st, int errorNumber_)
		: ErrNoException(location, st, errorNumber_, "Wait") {}

	public: WaitException(const std::string & st, int errorNumber_)
		: ErrNoException(st, errorNumber_, "Wait") {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__SYSTEM_EXCEPTIONS
