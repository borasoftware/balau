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
/// @file Assertions.hpp
///
/// Test assertion functions.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS
#define COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS

#include <Balau/Testing/Matchers.hpp>
#include <Balau/Type/SourceCodeLocation.hpp>

namespace Balau::Testing {

///
/// Cause an assertion failure with a message.
///
inline void assertFail(const SourceCodeLocation & location, const std::string & failMessage) {
	ThrowBalauException(Exception::AssertionException, toString(location) + " - " + failMessage + "\n");
}

///
/// Cause an assertion failure with a message.
///
inline void assertFail(const std::string & failMessage) {
	ThrowBalauException(Exception::AssertionException, failMessage + "\n");
}

///
/// Assert that the actual value supplied matches that in the matcher.
///
/// Print the supplied failure message on assertion failure.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThat(const SourceCodeLocation & location, const std::string & failMessage, const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	MatcherFunction<A, E, V, C> matcherFunction(expected.value());

	if (!matcherFunction.matches(actual, expected.expected)) {
		ThrowBalauException(
			  Exception::AssertionException
			, toString(location) + " - " + failMessage + "\n" + TestRenderers::render(actual, expected.expected)
		);
	}
}

///
/// Assert that the actual value supplied matches that in the matcher.
///
/// Print the supplied failure message on assertion failure.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThat(const std::string & failMessage, const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	MatcherFunction<A, E, V, C> matcherFunction(expected.value());

	if (!matcherFunction.matches(actual, expected.expected)) {
		ThrowBalauException(
			  Exception::AssertionException
			, failMessage + "\n" + TestRenderers::render(actual, expected.expected)
		);
	}
}

///
/// Assert that the actual value supplied matches that in the matcher.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThat(const SourceCodeLocation & location, const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	assertThat(location, "", actual, expected);
}

///
/// Assert that the actual value supplied matches that in the matcher.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThat(const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	assertThat("", actual, expected);
}

///
/// Assertion without printing a failure rendering.
///
/// Print the supplied failure message on assertion failure.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThatNP(const SourceCodeLocation & location, const std::string & failMessage, const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	MatcherFunction<A, E, V, C> matcherFunction(expected.value());

	if (!matcherFunction.matches(actual, expected.expected)) {
		ThrowBalauException(Exception::AssertionException, toString(location) + " - " + failMessage + "\n");
	}
}

///
/// Assertion without printing a failure rendering.
///
/// Print the supplied failure message on assertion failure.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThatNP(const std::string & failMessage, const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	MatcherFunction<A, E, V, C> matcherFunction(expected.value());

	if (!matcherFunction.matches(actual, expected.expected)) {
		ThrowBalauException(Exception::AssertionException, failMessage + "\n");
	}
}

///
/// Assertion without printing a failure rendering.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThatNP(const SourceCodeLocation & location, const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	assertThatNP(location, "", actual, expected);
}

///
/// Assertion without printing a failure rendering.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename E, typename C, typename V, typename F>
inline void assertThatNP(const A & actual, const ExpectedValue<E, C, V, F> & expected) {
	assertThatNP("", actual, expected);
}

///
/// Assert that the actual value's type is castable to the supplied one.
///
/// Print the supplied failure message on assertion failure.
///
template <typename A, typename T>
inline void assertThat(const SourceCodeLocation & location, const std::string & failMessage, const A & actual, const IsAExpectation<T> & ) {
	if (!dynamic_cast<const T *>(&actual)) {
		ThrowBalauException(
			  Exception::AssertionException
			, toString(location) + " - " + failMessage + "\nIncompatible types: " + boost::core::demangle(typeid(actual).name()) + " is not a " + boost::core::demangle(typeid(T).name())
		);
	}
}

///
/// Assert that the actual value's type is castable to the supplied one.
///
/// Print the supplied failure message on assertion failure.
///
template <typename A, typename T>
inline void assertThat(const std::string & failMessage, const A & actual, const IsAExpectation<T> & ) {
	if (!dynamic_cast<const T *>(&actual)) {
		ThrowBalauException(
			  Exception::AssertionException
			, failMessage + "\nIncompatible types: " + boost::core::demangle(typeid(actual).name()) + " is not a " + boost::core::demangle(typeid(T).name())
		);
	}
}

///
/// Assert that the actual value's type is castable to the supplied one.
///
template <typename A, typename T>
inline void assertThat(const SourceCodeLocation & location, const A & actual, const IsAExpectation<T> & expected) {
	assertThat(location, "", actual, expected);
}

///
/// Assert that the actual value's type is castable to the supplied one.
///
template <typename A, typename T>
inline void assertThat(const A & actual, const IsAExpectation<T> & expected) {
	assertThat("", actual, expected);
}

///
/// Assertion without printing a failure rendering.
///
/// Print the supplied failure message on assertion failure.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename T>
inline void assertThatNP(const SourceCodeLocation & location, const std::string & failMessage, const A & actual, const IsAExpectation<T> & ) {
	if (!dynamic_cast<const T *>(&actual)) {
		ThrowBalauException(Exception::AssertionException, toString(location) + " - " + failMessage + "\n");
	}
}

///
/// Assertion without printing a failure rendering.
///
/// Print the supplied failure message on assertion failure.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename T>
inline void assertThatNP(const std::string & failMessage, const A & actual, const IsAExpectation<T> & ) {
	if (!dynamic_cast<const T *>(&actual)) {
		ThrowBalauException(Exception::AssertionException, failMessage + "\n");
	}
}

///
/// Assertion without printing a failure rendering.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename T>
inline void assertThatNP(const SourceCodeLocation & location, const A & actual, const IsAExpectation<T> & expected) {
	assertThatNP(location, "", actual, expected);
}

///
/// Assertion without printing a failure rendering.
///
/// Best to use this one if the data types are huge, otherwise a great deal of
/// logging will be generated on assertion failure.
///
template <typename A, typename T>
inline void assertThatNP(const A & actual, const IsAExpectation<T> & expected) {
	assertThatNP("", actual, expected);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The standard equality assertion is used for comparison.
/// Print the supplied failure message on assertion failure.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(const SourceCodeLocation & location, const std::string & failMessage, F function, const ThrowExpectation<E> & e) {
	try {
		function();
	} catch (const E & actualException) {
		assertThat(location, failMessage, actualException, is(e.e));
		return;
	} catch (const std::exception & actualException) {
		ThrowBalauException(
			  Exception::AssertionException
			, toString(location) + " - " + failMessage + "\nA different standard exception was thrown: " + actualException.what()
		);
	}

	ThrowBalauException(
		  Exception::AssertionException
		, toString(location) + " - " + failMessage + "\nThe expected exception was not thrown."
	);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The standard equality assertion is used for comparison.
/// Print the supplied failure message on assertion failure.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(const std::string & failMessage, F function, const ThrowExpectation<E> & e) {
	try {
		function();
	} catch (const E & actualException) {
		assertThat(failMessage, actualException, is(e.e));
		return;
	} catch (const std::exception & actualException) {
		ThrowBalauException(
			  Exception::AssertionException
			, failMessage + "\nA different standard exception was thrown: " + actualException.what()
		);
	}

	ThrowBalauException(
		  Exception::AssertionException
		, failMessage + "\nThe expected exception was not thrown."
	);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The supplied function is used for comparison.
/// Print the supplied failure message on assertion failure.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E, typename C>
inline void assertThat(const SourceCodeLocation & location, const std::string & failMessage, F function, const ThrowExpectationWithFunction<E, C> & e) {
	try {
		function();
	} catch (const E & actualException) {
		if (!e.compare(actualException)) {
			ThrowBalauException(
				  Exception::AssertionException
				, ::toString(
					  location
					, " - "
					, failMessage
					, "\nThe exception's state was not identical to the supplied exception: "
					, e
					, " != "
					, actualException
				)
			);
		} else {
			return;
		}
	} catch (const std::exception & actualException) {
		ThrowBalauException(
			  Exception::AssertionException
			, toString(location) + " - " + failMessage + "\nA different standard exception was thrown: " + actualException.what()
		);
	}

	ThrowBalauException(
		  Exception::AssertionException
		, toString(location) + " - " + failMessage + "\nThe expected exception was not thrown."
	);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The supplied function is used for comparison.
/// Print the supplied failure message on assertion failure.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E, typename C>
inline void assertThat(const std::string & failMessage, F function, const ThrowExpectationWithFunction<E, C> & e) {
	try {
		function();
	} catch (const E & actualException) {
		if (!e.compare(actualException)) {
			ThrowBalauException(
				  Exception::AssertionException
				, ::toString(
					  failMessage
					, "\nThe exception's state was not identical to the supplied exception: "
					, e
					, " != "
					, actualException
				)
			);
		} else {
			return;
		}
	} catch (const std::exception & actualException) {
		ThrowBalauException(
			  Exception::AssertionException
			, failMessage + "\nA different standard exception was thrown: " + actualException.what()
		);
	}

	ThrowBalauException(
		  Exception::AssertionException
		, failMessage + "\nThe expected exception was not thrown."
	);
}

///
/// Assert that the supplied function throws the supplied exception type.
///
/// Print the supplied failure message on assertion failure.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(const SourceCodeLocation & location, const std::string & failMessage, F function, const ThrowTypeExpectation<E> &) {
	try {
		function();
	} catch (const E & actualException) {
		// Assertion passed.
		return;
	} catch (const std::exception & actualException) {
		ThrowBalauException(
			  Exception::AssertionException
			, toString(location) + " - " + failMessage + "\nA different standard exception was thrown: " + actualException.what()
		);
	}

	ThrowBalauException(
		  Exception::AssertionException
		, toString(location) + " - " + failMessage + "\nThe expected exception was not thrown."
	);
}

///
/// Assert that the supplied function throws the supplied exception type.
///
/// Print the supplied failure message on assertion failure.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(const std::string & failMessage, F function, const ThrowTypeExpectation<E> &) {
	try {
		function();
	} catch (const E & actualException) {
		// Assertion passed.
		return;
	} catch (const std::exception & actualException) {
		ThrowBalauException(
			  Exception::AssertionException
			, failMessage + "\nA different standard exception was thrown: " + actualException.what()
		);
	}

	ThrowBalauException(
		  Exception::AssertionException
		, failMessage + "\nThe expected exception was not thrown."
	);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The standard equality assertion is used for comparison.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(const SourceCodeLocation & location, F function, const ThrowExpectation<E> & expectedException) {
	assertThat(location, "", function, expectedException);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The standard equality assertion is used for comparison.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(F function, const ThrowExpectation<E> & expectedException) {
	assertThat("", function, expectedException);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The supplied function is used for comparison.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E, typename C>
inline void assertThat(const SourceCodeLocation & location, F function, const ThrowExpectationWithFunction<E, C> & expectedException) {
	assertThat(location, "", function, expectedException);
}

///
/// Assert that the supplied function throws the supplied exception example instance.
///
/// The supplied function is used for comparison.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E, typename C>
inline void assertThat(F function, const ThrowExpectationWithFunction<E, C> & expectedException) {
	assertThat("", function, expectedException);
}

///
/// Assert that the supplied function throws the supplied exception type.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(const SourceCodeLocation & location, F function, const ThrowTypeExpectation<E> & expectedExceptionType) {
	assertThat(location, "", function, expectedExceptionType);
}

///
/// Assert that the supplied function throws the supplied exception type.
///
/// If any exception is throw which does not derive from std::exception,
/// the test application will fail.
///
template <typename F, typename E>
inline void assertThat(F function, const ThrowTypeExpectation<E> & expectedExceptionType) {
	assertThat("", function, expectedExceptionType);
}

} // namespace Balau::Testing

///
/// Assertion macro, providing the source code location of the assertion.
///
#define AssertFail(...) ::Balau::Testing::assertFail(::Balau::SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Assertion macro, providing the source code location of the assertion.
///
#define AssertThat(...) ::Balau::Testing::assertThat(::Balau::SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

///
/// Assertion macro, providing the source code location of the assertion.
///
#define AssertThatNP(...) ::Balau::Testing::assertThatNP(::Balau::SourceCodeLocation(__FILE__, __LINE__), __VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS
