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
/// @file Matchers.hpp
///
/// Matcher functions for test assertions.
///

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_MATCHERS
#define COM_BORA_SOFTWARE_BALAU_TESTING_MATCHERS

#include <Balau/Testing/Impl/MatcherClasses.hpp>

namespace Balau::Testing {

///
/// Is the actual value equal to the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareEquals, EvNotUsed, EvNotUsed> is(const E & expected) {
	return ExpectedValue<E, MatcherCompareEquals, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the actual value not equal to the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareNotEqual, EvNotUsed, EvNotUsed> isNot(const E & expected) {
	return ExpectedValue<E, MatcherCompareNotEqual, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the actual value not equal to the supplied expected value?
///
inline ExpectedValue<void *, MatcherCompareNotEqual, EvNotUsed, EvNotUsed> isNotNull() {
	return ExpectedValue<void *, MatcherCompareNotEqual, EvNotUsed, EvNotUsed>(nullptr);
}

///
/// Is the actual value greater than the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareGreaterThan, EvNotUsed, EvNotUsed> isGreaterThan(const E & expected) {
	return ExpectedValue<E, MatcherCompareGreaterThan, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the actual value greater than or equal to the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareGreaterThanOrEqualTo, EvNotUsed, EvNotUsed> isGreaterThanOrEqualTo(const E & expected) {
	return ExpectedValue<E, MatcherCompareGreaterThanOrEqualTo, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the actual value less than the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareLessThan, EvNotUsed, EvNotUsed> isLessThan(const E & expected) {
	return ExpectedValue<E, MatcherCompareLessThan, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the actual value less than or equal to the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareLessThanOrEqualTo, EvNotUsed, EvNotUsed> isLessThanOrEqualTo(const E & expected) {
	return ExpectedValue<E, MatcherCompareLessThanOrEqualTo, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the actual value equal to within the specified error limit to the supplied expected value?
///
template <typename E, typename V>
inline ExpectedValue<E, MatcherCompareAlmostEqual, V, EvNotUsed>  isAlmostEqual(const E & expected, const V & errorDelta) {
	return ExpectedValue<E, MatcherCompareAlmostEqual, V, EvNotUsed>(expected, errorDelta);
}

///
/// Does the actual value start with the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareStartsWith, EvNotUsed, EvNotUsed> startsWith(const E & expected) {
	return ExpectedValue<E, MatcherCompareStartsWith, EvNotUsed, EvNotUsed>(expected);
}

///
/// Does the actual value end with the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareEndsWith, EvNotUsed, EvNotUsed> endsWith(const E & expected) {
	return ExpectedValue<E, MatcherCompareEndsWith, EvNotUsed, EvNotUsed>(expected);
}

///
/// Does the actual value contains the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareContains, EvNotUsed, EvNotUsed> contains(const E & expected) {
	return ExpectedValue<E, MatcherCompareContains, EvNotUsed, EvNotUsed>(expected);
}

///
/// Does the actual value not contain the supplied expected value?
///
template <typename E>
inline ExpectedValue<E, MatcherCompareDoesNotContain, EvNotUsed, EvNotUsed> doesNotContain(const E & expected) {
	return ExpectedValue<E, MatcherCompareDoesNotContain, EvNotUsed, EvNotUsed>(expected);
}

///
/// Is the type of the actual value castable to the specified type?
///
template <typename T> inline const IsAExpectation<T> isA() {
	return IsAExpectation<T>();
}

///
/// Does the code block throw the supplied expected exception type?
///
template <typename E> inline const ThrowTypeExpectation<E> throws() {
	return ThrowTypeExpectation<E>();
}

///
/// Does the code block throw the supplied exception (including the exception's data)?
///
template <typename E> inline const ThrowExpectation<E> throws(const E & expected) {
	return ThrowExpectation<E>(expected);
}

///
/// Does the code block throw the supplied exception type?
///
/// Examine the exception with the supplied predicate function.
///
template <typename E>
inline const ThrowExpectationWithPredicate<E> throws(const std::function<bool (const E & )> & predicate) {
	return ThrowExpectationWithPredicate<E>(predicate);
}

///
/// Does the code block throw the supplied exception (including the exception's data)?
///
/// Perform the comparison with the supplied function.
///
template <typename E, typename C>
inline const ThrowExpectationWithFunction<E, C> throws(const E & expected, C comparisonFunction) {
	return ThrowExpectationWithFunction<E, C>(expected, comparisonFunction);
}

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_MATCHERS
