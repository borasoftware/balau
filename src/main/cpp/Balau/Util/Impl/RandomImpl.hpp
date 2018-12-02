// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__RANDOM_IMPL
#define COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__RANDOM_IMPL

#include <random>

namespace Balau::Util::Impl {

inline double inclusivise(double upper) {
	return std::nextafter(upper, upper > 0.0 ? upper + 1.0 : upper - 1.0);
}

inline float inclusivise(float upper) {
	return std::nextafter(upper, upper > 0.0f ? upper + 1.0f : upper - 1.0f);
}

template <typename IntegerT> inline IntegerT inclusivise(IntegerT upper) {
	return upper;
}

} // namespace Balau::Util::Impl

#endif // COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__RANDOM_IMPL
