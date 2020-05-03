//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__REINTERPRET_POINTER_CAST
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__REINTERPRET_POINTER_CAST

#include <memory>

namespace std { // NOLINT

//
// Reinterpret cast for GCC 6.
//
template <typename T, typename U>
inline std::shared_ptr<T> reinterpret_pointer_cast(const std::shared_ptr<U> & r) noexcept {
	return std::shared_ptr<T>(r, reinterpret_cast<typename std::shared_ptr<T>::element_type *>(r.get()));
}

} // namespace std

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__REINTERPRET_POINTER_CAST
