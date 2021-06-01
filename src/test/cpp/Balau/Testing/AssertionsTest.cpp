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

#include "AssertionsTestData.hpp"
#include <TestResources.hpp>

namespace Balau {

using Testing::is;
using Testing::isNot;
using Testing::isGreaterThan;
using Testing::isGreaterThanOrEqualTo;
using Testing::isLessThan;
using Testing::isLessThanOrEqualTo;
using Testing::isAlmostEqual;
using Testing::startsWith;
using Testing::endsWith;
using Testing::contains;
using Testing::doesNotContain;
using Testing::throws;

// This test group tests the assertions of the test runner.
struct AssertionsTest : public Testing::TestGroup<AssertionsTest> {
	AssertionsTest() {
		RegisterTestCase(test);
	}

	void test() {
		AssertThat(true,  is(true));
		AssertThat(false, is(false));

		AssertThat(true,  isNot(false));
		AssertThat(false, isNot(true));

		AssertThat(A{1}, is(A{1}));
		AssertThat(A{1}, is(B{1}));
		AssertThat(B{1}, is(A{1}));
		AssertThat(B{1}, is(B{1}));

		AssertThat(A{1}, isNot(A{2}));
		AssertThat(A{1}, isNot(B{3}));
		AssertThat(B{1}, isNot(A{4}));
		AssertThat(B{1}, isNot(B{5}));

		AssertThat(A{2}, isGreaterThan(A{1}));
		AssertThat(A{3}, isGreaterThan(B{1}));
		AssertThat(B{4}, isGreaterThan(A{1}));
		AssertThat(B{5}, isGreaterThan(B{1}));

		AssertThat(A{1}, isGreaterThanOrEqualTo(A{1}));
		AssertThat(A{2}, isGreaterThanOrEqualTo(B{1}));
		AssertThat(B{3}, isGreaterThanOrEqualTo(A{1}));
		AssertThat(B{4}, isGreaterThanOrEqualTo(B{1}));

		AssertThat(A{1}, isLessThan(A{2}));
		AssertThat(A{1}, isLessThan(B{3}));
		AssertThat(B{1}, isLessThan(A{4}));
		AssertThat(B{1}, isLessThan(B{5}));

		AssertThat(A{1}, isLessThanOrEqualTo(A{1}));
		AssertThat(A{1}, isLessThanOrEqualTo(B{2}));
		AssertThat(B{1}, isLessThanOrEqualTo(A{3}));
		AssertThat(B{1}, isLessThanOrEqualTo(B{4}));

		AssertThat(A{1}, isAlmostEqual(A{1}, 0));
		AssertThat(A{1}, isAlmostEqual(B{2}, 1));
		AssertThat(B{1}, isAlmostEqual(A{3}, 2));
		AssertThat(B{1}, isAlmostEqual(B{4}, 3));

		AssertThat(C{"test"}, startsWith(C{"te"}));
		AssertThat(C{"test"}, startsWith(D{"te"}));
		AssertThat(D{"test"}, startsWith(C{"te"}));
		AssertThat(D{"test"}, startsWith(D{"te"}));

		AssertThat(C{"test"}, endsWith(C{"est"}));
		AssertThat(C{"test"}, endsWith(D{"est"}));
		AssertThat(D{"test"}, endsWith(C{"est"}));
		AssertThat(D{"test"}, endsWith(D{"est"}));

		AssertThat(C{"test"}, contains(C{"est"}));
		AssertThat(C{"test"}, contains(D{"est"}));
		AssertThat(D{"test"}, contains(C{"est"}));
		AssertThat(D{"test"}, contains(D{"est"}));

		AssertThat(C{"test"}, doesNotContain(C{"ets"}));
		AssertThat(C{"test"}, doesNotContain(D{"ets"}));
		AssertThat(D{"test"}, doesNotContain(C{"ets"}));
		AssertThat(D{"test"}, doesNotContain(D{"ets"}));

		AssertThat([&] () { foo(); }, throws<Exception::SizeException>());
		AssertThat([&] () { foo(); }, throws(Exception::SizeException("", "testing")));

		AssertThat(
			  [&] () { foo(); }
			, throws(
				  Exception::SizeException("", "testing")
				, [] (auto a, auto e) { return std::string(a.message) == e.message; }
			)
		);

		AssertThat(
			  [&] () { foo(); }
			, throws<Exception::SizeException>(
				[] (const Exception::SizeException & e) {
					return Util::Strings::contains(e.what(), "testing");
				}
			)
		);
	}
};

} // namespace Balau
