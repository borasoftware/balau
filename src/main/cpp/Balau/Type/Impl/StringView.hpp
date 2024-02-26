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

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_STRING_VIEW
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_STRING_VIEW

#include <string>
#include <boost/utility/string_view.hpp>

namespace std { // NOLINT

template <typename CharT, typename Traits = std::char_traits<CharT>>
using basic_string_view = boost::basic_string_view<CharT, Traits>;

using string_view    = boost::string_view;
using u16string_view = boost::u16string_view;
using u32string_view = boost::u32string_view;

template <typename CharT, typename Traits>
struct hash<basic_string_view<CharT, Traits>> {
	typedef basic_string_view<CharT, Traits> argument_type;
	typedef std::size_t result_type;

	result_type operator()(argument_type const & s) const noexcept
	{
		// TODO string view hash must be the same as string hash.
		// TODO optimise this (avoid allocation).
		return hash<basic_string<CharT, Traits>>()(basic_string<CharT, Traits>(s.data(), s.length()));
	}
};

} // namespace std

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_STRING_VIEW
