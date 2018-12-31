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
/// @file TestExceptions.hpp
///
/// %Balau exceptions for the test framework.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__TEST_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__TEST_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// The exception thrown by the test runner when an assertion fails.
///
class AssertionException : public BalauException {
	public: AssertionException(const char * file, int line, const std::string & st, const std::string & message_)
		: BalauException(file, line, st, "Assertion", message_) {}
};

///
/// Thrown by the test runner when there is a non test related error.
///
class TestRunnerException : public BalauException {
	public: TestRunnerException(const char * file, int line, const std::string & st, const std::string & message_)
		: BalauException(file, line, st, "TestRunner", message_) {}
};

} // namespace Balau::Exception

template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Balau::Exception::AssertionException & e) {
	return toString<AllocatorT>(e.what());
}

inline std::string toString(const Balau::Exception::AssertionException & e) {
	return e.what();
}

template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Balau::Exception::TestRunnerException & e) {
	return toString<AllocatorT>(e.what());
}

inline std::string toString(const Balau::Exception::TestRunnerException & e) {
	return e.what();
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__TEST_EXCEPTIONS
