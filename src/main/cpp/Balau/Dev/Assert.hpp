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
/// @file Assert.hpp
///
/// Assertion utilities for development purposes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_DEV__ASSERT
#define COM_BORA_SOFTWARE__BALAU_DEV__ASSERT

#include <boost/core/ignore_unused.hpp>
#include <string>

#ifdef BALAU_ENABLE_STACKTRACES
	#include <boost/stacktrace.hpp>
#endif

#include <iostream>
#include <sstream>

namespace Balau {

///
/// An assertion class for development purposes.
///
class Assert {
	///
	/// Abort after logging the message.
	///
	public: static void fail(const char * fatalMessage) {
		performAssertion(false, fatalMessage, "Fail called");
	}

	///////////////////////////////////////////////////////////////////////////

	///
	/// If the bug test assertion fails, abort after logging the message supplied by the function.
	///
	public: template <typename StringFunctionT>
	static void assertion(bool test, StringFunctionT function) {
		performAssertion(test, function, "Bug found");
	}

	///
	/// If the bug test assertion fails, abort after logging the message.
	///
	public: static void assertion(bool test, const char * fatalMessage) {
		performAssertion(test, fatalMessage, "Bug found");
	}

	///
	/// Abort if the bug test fails.
	///
	public: static void assertion(bool test) {
		performAssertion(test, "", "Bug found");
	}

	///
	/// If the bug test assertion predicate function fails, abort after logging the message supplied by the function.
	///
	public: template <typename TestFunctionT, typename StringFunctionT>
	static void assertion(TestFunctionT test, StringFunctionT function) {
		performAssertion(test, function, "Bug found");
	}

	///
	/// If the bug test assertion predicate function fails, abort after logging the message.
	///
	public: template <typename TestFunctionT>
	static void assertion(TestFunctionT test, const char * fatalMessage) {
		performAssertion(test, fatalMessage, "Bug found");
	}

	///
	/// Abort if the bug test assertion predicate function fails.
	///
	public: template <typename TestFunctionT>
	static void assertion(TestFunctionT test) {
		performAssertion(test, "", "Bug found");
	}

	///////////////////////////////////////////////////////////////////////////

	private: template <typename FunctionT>
	static void performAssertion(bool test, FunctionT function, const char * testType) {
		boost::ignore_unused(test);
		boost::ignore_unused(function);
		boost::ignore_unused(testType);

		#ifdef BALAU_DEBUG
			if (!test) {
				const std::string message = function();
				abortProcess(message.c_str(), testType);
			}
		#endif
	}

	private: template <typename TestT, typename FunctionT>
	static void performAssertion(TestT test, FunctionT function, const char * testType) {
		boost::ignore_unused(test);
		boost::ignore_unused(function);
		boost::ignore_unused(testType);

		#ifdef BALAU_DEBUG
			if (!test()) {
				const std::string message = function();
				abortProcess(message.c_str(), testType);
			}
		#endif
	}

	private: static void performAssertion(bool test, const char * fatalMessage, const char * testType) {
		boost::ignore_unused(test);
		boost::ignore_unused(fatalMessage);
		boost::ignore_unused(testType);

		#ifdef BALAU_DEBUG
			if (!test) {
				abortProcess(fatalMessage, testType);
			}
		#endif
	}

	private: template <typename TestT>
	static void performAssertion(TestT test, const char * fatalMessage, const char * testType) {
		boost::ignore_unused(test);
		boost::ignore_unused(fatalMessage);
		boost::ignore_unused(testType);

		#ifdef BALAU_DEBUG
			if (!test()) {
				abortProcess(fatalMessage, testType);
			}
		#endif
	}

	private: static void abortProcess(const char * fatalMessage, const char * testType) {
		std::ostringstream str;

		if (fatalMessage != nullptr) {
			str << testType << ": " << fatalMessage << "\n";
		} else {
			str << testType << "\n";
		}

		#ifdef BALAU_ENABLE_STACKTRACES
			str << "Stack trace: " << boost::stacktrace::stacktrace() << "\n";
		#endif

		str << "Program will abort now\n";
		std::cerr << str.str();
		abort();
	}
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_DEV__ASSERT
