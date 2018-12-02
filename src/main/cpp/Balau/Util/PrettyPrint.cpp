// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PrettyPrint.hpp"

namespace Balau::Util {

const char * PrettyPrint::PREFIXES[] = {
	// atto    a    0.000000000000000001    10^−18
	  "a" // ..., -18, -17, -16

	// femto    f    0.000000000000001    10^−15
	, "f" // -15, -14, -13

	// pico    p    0.000000000001    10^−12
	, "p" // -12, -11, -10

	// nano    n    0.000000001    10^−9
	, "n" // -9, -8, -7

	// micro    μ    0.000001    10^−6
	, "μ" // -6, -5, -4

	// milli    m    0.001    10^−3
	, "m" // -3, -2, -1

	// none    none    1    10^0
	, "" // 0, 1, 2

	// kilo    k    1000    10^3
	, "k" // 3, 4, 5

	// mega    M    1000000    10^6
	, "M" // 6, 7, 8

	// giga    G    1000000000    10^9
	, "G" // 9, 10, 11

	// tera    T    1000000000000    10^12
	, "T" // 12, 13, 14

	// peta    P    1000000000000000    10^15
	, "P" // 15, 16, 17

	// exa    E    1000000000000000000    10^18
	, "E" // 18, ...
};

} // namespace Balau::Util
