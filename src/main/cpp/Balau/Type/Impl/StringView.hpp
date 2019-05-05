//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__STRING_VIEW
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__STRING_VIEW

#include <boost/utility/string_view.hpp>
#include <string>

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

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__STRING_VIEW
