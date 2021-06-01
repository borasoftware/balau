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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__TO_STRING_IMPL
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__TO_STRING_IMPL

#include <string>
#include <limits>

namespace Balau::Impl {

template <typename AllocatorT>
using U8String = std::basic_string<char, std::char_traits<char>, AllocatorT>;

template <typename AllocatorT>
using U16String = std::basic_string<char16_t, std::char_traits<char16_t>, AllocatorT>;

template <typename AllocatorT>
using U32String = std::basic_string<char32_t, std::char_traits<char32_t>, AllocatorT>;

template <typename T, unsigned long N = std::numeric_limits<T>::max()> struct CharacterCount {
	static constexpr size_t count = CharacterCount<T, N / 10>::count + 1;
};

template <typename T> struct CharacterCount<T, 0> {
	static constexpr size_t count = 1;
};

template <typename AllocatorT, typename T>
inline U8String<AllocatorT> toString(const char * fmt, T value) {
	char buffer[CharacterCount<T>::count];
	int len = std::sprintf(buffer, fmt, value);
	return U8String<AllocatorT>(buffer, buffer + len);
}

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__TO_STRING_IMPL
