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

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_MATCHER_COMPARE_PLACEHOLDERS
#define COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_MATCHER_COMPARE_PLACEHOLDERS

#include <Balau/Exception/TestExceptions.hpp>
#include <Balau/Testing/Impl/StandardRenderers.hpp>

#include <iomanip>

namespace Balau::Testing {

////////////////////////// Compare type placeholders //////////////////////////

class MatcherCompareEquals {};
class MatcherCompareNotEqual {};
class MatcherCompareGreaterThan {};
class MatcherCompareGreaterThanOrEqualTo {};
class MatcherCompareLessThan {};
class MatcherCompareLessThanOrEqualTo {};
class MatcherCompareAlmostEqual {};
class MatcherCompareStartsWith {};
class MatcherCompareEndsWith {};
class MatcherCompareContains {};
class MatcherCompareDoesNotContain {};

// Type used when no type is required.
class EvNotUsed {};

//////////////////////// ExpectedValue specializations ////////////////////////

//
// An expected value which is compared to the actual value.
// This may involve an additional value/function.
//
template <typename E, typename C, typename V, typename F> class ExpectedValue;

template <typename E, typename C> class ExpectedValue<E, C, EvNotUsed, EvNotUsed> {
	public: const E & expected;

	public: explicit ExpectedValue(const E & expected_)
		: expected(std::move(expected_)) {}

	public: ExpectedValue(const ExpectedValue<E, C, EvNotUsed, EvNotUsed> & copy)
		: expected(copy.expected) {}

	public: const EvNotUsed & value() const { return v; }
	private: EvNotUsed v;
};

template <typename E, typename C, typename V> class ExpectedValue<E, C, V, EvNotUsed> {
	public: const E & expected;
	public: const V & v;

	public: explicit ExpectedValue(const E & expected_, const V & value_)
		: expected(std::move(expected_))
		, v(value_) {}

	public: ExpectedValue(const ExpectedValue<E, C, V, EvNotUsed> & copy)
		: expected(copy.expected)
		, v(copy.v) {}

	public: const V & value() const { return v; }
};

template <typename E, typename C, typename F> class ExpectedValue<E, C, EvNotUsed, F> {
	public: const E & expected;
	public: F compareFunction;

	public: ExpectedValue(const E & expected_, F compareFunction_)
		: expected(std::move(expected_))
		, compareFunction(compareFunction_) {}

	public: ExpectedValue(const ExpectedValue<E, C, EvNotUsed, F> & copy)
		: expected(copy.expected)
		, compareFunction(copy.compareFunction) {}

	public: const EvNotUsed & value() const { return v; }
	private: EvNotUsed v;
};

////////////////////////////// Is-a expectations //////////////////////////////

template <typename T> class IsAExpectation {};

//////////////////////////// Throwing expectations ////////////////////////////

// An expected exception thrown.
template <typename E> class ThrowExpectation {
	public: const E & e; // NOLINT

	public: explicit ThrowExpectation(const E & e_) : e(e_) {}
};

// An expected exception thrown which includes a compare function.
template <typename E, typename F> class ThrowExpectationWithFunction {
	public: const E & expectedException; // NOLINT
	public: F compareFunction;

	public: explicit ThrowExpectationWithFunction(const E & expectedException_, F compareFunction_)
		: expectedException(expectedException_)
		, compareFunction(compareFunction_) {}

	public: bool compare(const E & thrownException) const {
		return compareFunction(thrownException, expectedException);
	}
};

// An expected exception thrown which includes a compare function.
template <typename E> class ThrowExpectationWithPredicate {
	public: const std::function<bool (const E &)> predicate;

	public: explicit ThrowExpectationWithPredicate(const std::function<bool (const E &)> & predicate_)
		: predicate(predicate_) {}

	public: bool compare(const E & thrownException) const {
		return predicate(thrownException);
	}
};

template <typename AllocatorT, typename E, typename F>
inline Balau::U8String<AllocatorT> toString(const ThrowExpectationWithFunction<E, F> & e) {
	using ::toString;

	return toString<AllocatorT>(e.expectedException);
}

template <typename E, typename F>
inline std::string toString(const ThrowExpectationWithFunction<E, F> & e) {
	using ::toString;

	return toString(e.expectedException);
}

// Holds an exception type for the Hamcrest like API.
template <typename E> class ThrowTypeExpectation {};

/////////////////////// Matcher function specialisations //////////////////////

template <typename A, typename E, typename V, typename CompareT> class MatcherFunction {};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareEquals> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) {
		// Compilation error? Do the types have an operator == in scope?
		return actual == expected;
	}
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareNotEqual> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have an operator == in scope?
		return !(actual == expected);
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareGreaterThan> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have an operator > in scope?
		return actual > expected;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareGreaterThanOrEqualTo> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have an operator >= in scope?
		return actual >= expected;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareLessThan> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have an operator < in scope?
		return actual < expected;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareLessThanOrEqualTo> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have an operator <= in scope?
		return actual <= expected;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E, typename V> class MatcherFunction<A, E, V, MatcherCompareAlmostEqual> {
	const V & errorDelta;

	public: explicit MatcherFunction(const V & errorDelta_) : errorDelta(errorDelta_) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have the operator functions in scope?
		return actual >= expected - errorDelta && actual <= expected + errorDelta;
	}
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareStartsWith> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have the operator functions in scope?
		return !(actual.length() < expected.length()) && actual.substr(0, expected.length()) == expected;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename AE> class MatcherFunction<std::vector<AE>, std::vector<AE>, EvNotUsed, MatcherCompareStartsWith> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const std::vector<AE> & actual, const std::vector<AE> & expected) const {
		// Compilation error? Do the types have the operator functions in scope?
		return actual.size() >= expected.size() && std::memcmp(actual.data(), expected.data(), expected.size() * sizeof(AE)) == 0;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareEndsWith> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		// Compilation error? Do the types have the operator functions in scope?
		return !(actual.length() < expected.length())
			&& actual.substr(actual.length() - expected.length(), expected.length()) == expected;
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

// Predefined contains functions.

inline bool contains(const std::string & actual, const std::string & expected) {
	return Balau::Util::Strings::contains(actual, expected);
}

inline bool contains(const std::u16string & actual, const std::u16string & expected) {
	return Balau::Util::Strings::contains(actual, expected);
}

inline bool contains(const std::u32string & actual, const std::u32string & expected) {
	return Balau::Util::Strings::contains(actual, expected);
}

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareContains> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		return contains(actual, expected);
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

template <typename A, typename E> class MatcherFunction<A, E, EvNotUsed, MatcherCompareDoesNotContain> {
	public: explicit MatcherFunction(const EvNotUsed &) {}

	public: bool matches(const A & actual, const E & expected) const {
		return !contains(actual, expected);
	}

	public: EvNotUsed value() { return EvNotUsed(); }
};

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_MATCHER_COMPARE_PLACEHOLDERS
