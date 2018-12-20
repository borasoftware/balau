// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "AssertionsTest.hpp"

namespace Balau {

using Testing::assertThat;
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

void AssertionsTest::test() {
	assertThat(true,  is(true));
	assertThat(false, is(false));

	assertThat(true,  isNot(false));
	assertThat(false, isNot(true));

	assertThat(A{1}, is(A{1}));
	assertThat(A{1}, is(B{1}));
	assertThat(B{1}, is(A{1}));
	assertThat(B{1}, is(B{1}));

	assertThat(A{1}, isNot(A{2}));
	assertThat(A{1}, isNot(B{3}));
	assertThat(B{1}, isNot(A{4}));
	assertThat(B{1}, isNot(B{5}));

	assertThat(A{2}, isGreaterThan(A{1}));
	assertThat(A{3}, isGreaterThan(B{1}));
	assertThat(B{4}, isGreaterThan(A{1}));
	assertThat(B{5}, isGreaterThan(B{1}));

	assertThat(A{1}, isGreaterThanOrEqualTo(A{1}));
	assertThat(A{2}, isGreaterThanOrEqualTo(B{1}));
	assertThat(B{3}, isGreaterThanOrEqualTo(A{1}));
	assertThat(B{4}, isGreaterThanOrEqualTo(B{1}));

	assertThat(A{1}, isLessThan(A{2}));
	assertThat(A{1}, isLessThan(B{3}));
	assertThat(B{1}, isLessThan(A{4}));
	assertThat(B{1}, isLessThan(B{5}));

	assertThat(A{1}, isLessThanOrEqualTo(A{1}));
	assertThat(A{1}, isLessThanOrEqualTo(B{2}));
	assertThat(B{1}, isLessThanOrEqualTo(A{3}));
	assertThat(B{1}, isLessThanOrEqualTo(B{4}));

	assertThat(A{1}, isAlmostEqual(A{1}, 0));
	assertThat(A{1}, isAlmostEqual(B{2}, 1));
	assertThat(B{1}, isAlmostEqual(A{3}, 2));
	assertThat(B{1}, isAlmostEqual(B{4}, 3));

	assertThat(C{"test"}, startsWith(C{"te"}));
	assertThat(C{"test"}, startsWith(D{"te"}));
	assertThat(D{"test"}, startsWith(C{"te"}));
	assertThat(D{"test"}, startsWith(D{"te"}));

	assertThat(C{"test"}, endsWith(C{"est"}));
	assertThat(C{"test"}, endsWith(D{"est"}));
	assertThat(D{"test"}, endsWith(C{"est"}));
	assertThat(D{"test"}, endsWith(D{"est"}));

	assertThat(C{"test"}, contains(C{"est"}));
	assertThat(C{"test"}, contains(D{"est"}));
	assertThat(D{"test"}, contains(C{"est"}));
	assertThat(D{"test"}, contains(D{"est"}));

	assertThat(C{"test"}, doesNotContain(C{"ets"}));
	assertThat(C{"test"}, doesNotContain(D{"ets"}));
	assertThat(D{"test"}, doesNotContain(C{"ets"}));
	assertThat(D{"test"}, doesNotContain(D{"ets"}));

	assertThat([&] () { foo(); }, throws<Exception::SizeException>());
	assertThat([&] () { foo(); }, throws(Exception::SizeException("", 0, "", "testing")));

	assertThat(
		  [&] () { foo(); }
		, throws(
			Exception::SizeException("", 0, "", "testing")
			, [] (auto a, auto e) { return std::string(a.message) == e.message; }
		)
	);
}

} // namespace Balau
