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
	template <typename E> static auto toUnderlying(E e) noexcept -> typename std::underlying_type<E>::type
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	}

	///////////////////////////////////////////////////////////////////////////

	Enums() = delete;
	Enums(const Enums &) = delete;
	Enums & operator = (const Enums &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__ENUMS
