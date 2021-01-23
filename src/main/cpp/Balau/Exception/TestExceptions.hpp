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
	public: AssertionException(SourceCodeLocation location, const std::string & st, const std::string & message_)
		: BalauException(location, st, "Assertion", message_) {}

	public: AssertionException(const std::string & st, const std::string & message_)
		: BalauException(st, "Assertion", message_) {}
};

///
/// Thrown by the test runner when there is a non test related error.
///
class TestRunnerException : public BalauException {
	public: TestRunnerException(SourceCodeLocation location, const std::string & st, const std::string & message_)
		: BalauException(location, st, "TestRunner", message_) {}

	public: TestRunnerException(const std::string & st, const std::string & message_)
		: BalauException(st, "TestRunner", message_) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__TEST_EXCEPTIONS
