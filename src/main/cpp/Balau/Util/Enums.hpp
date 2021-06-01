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
/// @file Enums.hpp
///
/// Utilities for enums.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__ENUMS
#define COM_BORA_SOFTWARE__BALAU_UTIL__ENUMS

#include <type_traits>

namespace Balau::Util {

///
/// Utilities for enums.
///
struct Enums final {
	///
	/// Convert the strongly typed enum to its underlying integer.
	///
	template <typename E> static constexpr auto toUnderlying(E e) noexcept -> typename std::underlying_type<E>::type {
		return static_cast<typename std::underlying_type<E>::type>(e);
	}

	///////////////////////////////////////////////////////////////////////////

	Enums() = delete;
	Enums(const Enums &) = delete;
	Enums & operator = (const Enums &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__ENUMS
