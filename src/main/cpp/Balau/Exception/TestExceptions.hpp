// @formatter:off
//
// Balau core C++ library
//
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
/// @file TestExceptions.hpp
///
/// %Balau exceptions for the test framework.
///

#ifndef COM_BORA_SOFTWARE_BALAU_EXCEPTION_TEST_EXCEPTIONS
#define COM_BORA_SOFTWARE_BALAU_EXCEPTION_TEST_EXCEPTIONS

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

#endif // COM_BORA_SOFTWARE_BALAU_EXCEPTION_TEST_EXCEPTIONS
