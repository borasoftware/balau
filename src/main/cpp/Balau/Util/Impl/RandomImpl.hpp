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

#ifndef COM_BORA_SOFTWARE_BALAU_UTIL_IMPL_RANDOM_IMPL
#define COM_BORA_SOFTWARE_BALAU_UTIL_IMPL_RANDOM_IMPL

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

#endif // COM_BORA_SOFTWARE_BALAU_UTIL_IMPL_RANDOM_IMPL
