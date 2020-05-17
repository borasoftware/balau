// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__STRINGS_IMPL
#define COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__STRINGS_IMPL

#include <string>

namespace Balau::Impl {

template <typename StringT> inline size_t length(const StringT & str) {
	return str.length();
}

template <typename CharT> inline size_t length(const CharT * str) {
	return std::char_traits<CharT>::length(str);
}

// Generic implementation of non-regex replace all methods.
template <typename CharT, typename ... T, template <typename ...> class StringT, typename MatchT, typename ReplacementT>
void replaceAllImpl(StringT<CharT, T ...> & input, const MatchT & match, const ReplacementT & replacement, size_t * count) {
	const size_t matchLength = length(match);
	const size_t replacementLength = length(replacement);
	size_t pos = 0;

	if (count) {
		*count = 0;
	}

	while (true) {
		pos = input.find(match, pos);

		if (pos == StringT<CharT, T ...>::npos) {
			break;
		}

		input.erase(pos, matchLength);
		input.insert(pos, replacement);

		if (count) {
			++*count;
		}

		pos += replacementLength;
	}
}

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__STRINGS_IMPL
