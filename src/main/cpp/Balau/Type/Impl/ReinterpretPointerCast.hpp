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

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_REINTERPRET_POINTER_CAST
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_REINTERPRET_POINTER_CAST

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

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_REINTERPRET_POINTER_CAST
