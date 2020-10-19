// @formatter:off
//
// Balau core C++ library
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
// ////////////////////////////////////////////////////////////////////////////
//
// The source code in this file was adapted from the ICU project.
//
// Copyright © 1991-2020 Unicode, Inc. All rights reserved.
// Distributed under the Terms of Use in https://www.unicode.org/copyright.html.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of the Unicode data files and any associated documentation
// (the "Data Files") or Unicode software and any associated documentation
// (the "Software") to deal in the Data Files or Software
// without restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, and/or sell copies of
// the Data Files or Software, and to permit persons to whom the Data Files
// or Software are furnished to do so, provided that either
// (a) this copyright and permission notice appear with all copies
// of the Data Files or Software, or
// (b) this copyright and permission notice appear in associated
// Documentation.
//
// THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT OF THIRD PARTY RIGHTS.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
// NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
// DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
// DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THE DATA FILES OR SOFTWARE.
//
// Except as contained in this notice, the name of a copyright holder
// shall not be used in advertising or otherwise to promote the sale,
// use or other dealings in these Data Files or Software without prior
// written authorization of the copyright holder.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF16
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF16

#include <Balau/Unicode/UTF32.hpp>

namespace Balau::Unicode {

///
/// Does this code unit alone encode a code point (BMP, not a surrogate)?
///
/// @param c 16-bit code unit
/// @return true or false
///
inline bool isSingle(const char16_t c) noexcept {
	return !isSurrogate((char32_t) c);
}

///
/// Is this code unit a lead surrogate (U+d800..U+dbff)?
///
/// @param c 16-bit code unit
/// @return true or false
///
inline bool isLead(const char16_t c) noexcept {
	return (c & 0xfffffc00) == 0xd800;
}

///
/// Is this code unit a trail surrogate (U+dc00..U+dfff)?
///
/// @param c 16-bit code unit
/// @return true or false
///
inline bool isTrail(const char16_t c) noexcept {
	return (c & 0xfffffc00) == 0xdc00;
}

///
/// Is this code unit a surrogate (U+d800..U+dfff)?
/// @param c 16-bit code unit
/// @return true or false
///
inline bool isSurrogate(const char16_t c) noexcept {
	return isSurrogate((char32_t) c);
}

///
/// Assuming c is a surrogate code point (isSurrogate(c)),
/// is it a lead surrogate?
/// @param c 16-bit code unit
/// @return true or false
///
inline bool isSurrogateLead(const char16_t c) noexcept {
	return (c & 0x400) == 0;
}

///
/// Assuming c is a surrogate code point (isSurrogate(c)),
/// is it a trail surrogate?
/// @param c 16-bit code unit
/// @return true or false
///
inline bool isSurrogateTrail(const char16_t c) noexcept {
	return (c & 0x400) != 0;
}

///
/// Helper constant for getSupplementary.
/// @internal
///
constexpr unsigned int utf16SurrogateOffset = (0xd800 << 10UL) + 0xdc00 - 0x10000;

///
/// Get a supplementary code point value (U+10000..U+10ffff)
/// from its lead and trail surrogates.
/// The result is undefined if the input values are not
/// lead and trail surrogates.
///
/// @param lead lead surrogate (U+d800..U+dbff)
/// @param trail trail surrogate (U+dc00..U+dfff)
/// @return supplementary code point (U+10000..U+10ffff)
///
inline char16_t getSupplementary(const char16_t lead, const char16_t trail) noexcept {
	return ((char32_t) lead << 10UL) + (char32_t) trail - utf16SurrogateOffset;
}

///
/// Get the lead surrogate (0xd800..0xdbff) for a
/// supplementary code point (0x10000..0x10ffff).
/// @param supplementary 32-bit code point (U+10000..U+10ffff)
/// @return lead surrogate (U+d800..U+dbff) for supplementary
///
inline char16_t utf16Lead(const char32_t supplementary) noexcept {
	return (char16_t) ((supplementary >> 10) + 0xd7c0);
}

///
/// Get the trail surrogate (0xdc00..0xdfff) for a
/// supplementary code point (0x10000..0x10ffff).
/// @param supplementary 32-bit code point (U+10000..U+10ffff)
/// @return trail surrogate (U+dc00..U+dfff) for supplementary
///
inline char16_t utf16Trail(const char32_t supplementary) noexcept {
	return (char16_t) ((supplementary & 0x3ff) | 0xdc00);
}

///
/// How many 16-bit code units are used to encode this Unicode code point? (1 or 2)
/// The result is not defined if c is not a Unicode code point (U+0000..U+10ffff).
/// @param c 32-bit code point
/// @return 1 or 2
///
inline unsigned int utf16Length(const char32_t c) noexcept {
	return ((uint32_t) c <= 0xffff ? 1 : 2);
}

///
/// The maximum number of 16-bit code units per Unicode code point (U+0000..U+10ffff).
/// @return 2
///
constexpr unsigned int utf16MaxLength = 2;

///
/// Get a code point from a string at a random-access offset,
/// without changing the offset.
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// The offset may point to either the lead or trail surrogate unit
/// for a supplementary code point, in which case the macro will read
/// the adjacent matching surrogate as well.
/// The result is undefined if the offset points to a single, unpaired surrogate.
/// Iteration through a string is more efficient with nextUnsafe or next.
///
/// @param s const char16_t * string
/// @param i string offset
/// @param c output char32_t variable
/// @see get
///
inline void getUnsafe(const char16_t * s, const uint32_t i, char32_t & c) noexcept {
	c = s[i];

	if (isSurrogate(c)) {
		if (isSurrogateLead(c)) {
			c = getSupplementary(c, s[i + 1]);
		} else {
			c = getSupplementary(s[i - 1], c);
		}
	}
}

///
/// Get a code point from a string at a random-access offset,
/// without changing the offset.
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// The offset may point to either the lead or trail surrogate unit
/// for a supplementary code point, in which case the macro will read
/// the adjacent matching surrogate as well.
///
/// The length can be negative for a NUL-terminated string.
///
/// If the offset points to a single, unpaired surrogate, then
/// c is set to that unpaired surrogate.
/// Iteration through a string is more efficient with nextUnsafe or next.
///
/// @param s const char16_t * string
/// @param start starting string offset (usually 0)
/// @param i string offset, must be start<=i<length
/// @param length string length
/// @param c output char32_t variable
/// @see getUnsafe
///
inline void get(const char16_t * s, const uint32_t start, const uint32_t i, const uint32_t length, char32_t & c) noexcept {
	c = s[i];

	if (isSurrogate(c)) {
		uint16_t c2;

		if (isSurrogateLead(c)) {
			if (i + 1 != length && isTrail((char16_t) (c2 = s[i + 1]))) {
				c = getSupplementary(c, c2);
			}
		} else {
			if (i > start && isLead((char16_t) (c2 = s[i - 1]))) {
				c = getSupplementary(c2, c);
			}
		}
	}
}

///
/// Get a code point from a string at a code point boundary offset,
/// and advance the offset to the next code point boundary.
/// (Post-incrementing forward iteration.)
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// The offset may point to the lead surrogate unit
/// for a supplementary code point, in which case the macro will read
/// the following trail surrogate as well.
/// If the offset points to a trail surrogate, then that itself
/// will be returned as the code point.
/// The result is undefined if the offset points to a single, unpaired lead surrogate.
///
/// @param s const char16_t * string
/// @param i string offset
/// @param c output char32_t variable
/// @see next
///
inline void nextUnsafe(const char16_t * s, int32_t & i, char32_t & c) noexcept {
	c = s[i++];

	if (isLead((char16_t) (c))) {
		c = getSupplementary(c, s[i++]);
	}
}

///
/// Get a code point from a string at a code point boundary offset,
/// and advance the offset to the next code point boundary.
/// (Post-incrementing forward iteration.)
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// The offset may point to the lead surrogate unit
/// for a supplementary code point, in which case the macro will read
/// the following trail surrogate as well.
/// If the offset points to a trail surrogate or
/// to a single, unpaired lead surrogate, then c is set to that unpaired surrogate.
///
/// @param s const char16_t * string
/// @param i string offset, must be i<length
/// @param length string length
/// @param c output char32_t variable
/// @see nextUnsafe
///
inline void next(const char16_t * s, int32_t & i, const int32_t length, char32_t & c) noexcept {
	c = s[i++];

	if (isLead((char16_t) (c))) {
		uint16_t c2;

		if (i != length && isTrail((char16_t) (c2 = s[i]))) {
			++i;
			c = getSupplementary(c, c2);
		}
	}
}

///
/// Append a code point to a string, overwriting 1 or 2 code units.
/// The offset points to the current end of the string contents
/// and is advanced (post-increment).
/// "Unsafe" macro, assumes a valid code point and sufficient space in the string.
/// Otherwise, the result is undefined.
///
/// @param s const char16_t * string buffer
/// @param i string offset
/// @param c code point to append
/// @see append
///
inline void appendUnsafe(char16_t * s, int32_t & i, const char32_t c) noexcept {
	if ((uint32_t) c <= 0xffff) {
		s[i++] = (uint16_t) c;
	} else {
		s[i++] = (uint16_t) ((c >> 10) + 0xd7c0);
		s[i++] = (uint16_t) ((c & 0x3ff) | 0xdc00);
	}
}

///
/// Append a code point to a string, overwriting 1 or 2 code units.
/// The offset points to the current end of the string contents
/// and is advanced (post-increment).
/// "Safe" macro, checks for a valid code point.
/// If a surrogate pair is written, checks for sufficient space in the string.
/// If the code point is not valid or a trail surrogate does not fit,
/// then isError is set to true.
///
/// @param s const char16_t * string buffer
/// @param i string offset, must be i<capacity
/// @param capacity size of the string buffer
/// @param c code point to append
/// @rturn true if successful, otherwise false
/// @see appendUnsafe
///
inline bool append(char16_t * s, int32_t & i, const int32_t capacity, const char32_t c) noexcept {
	if ((uint32_t) c <= 0xffff) {
		s[i++] = (uint16_t) c;
	} else if ((uint32_t) c <= 0x10ffff && i + 1 < (capacity)) {
		s[i++] = (uint16_t) ((c >> 10) + 0xd7c0);
		s[i++] = (uint16_t) ((c & 0x3ff) | 0xdc00);
	} else {
		// c>0x10ffff or not enough space
		return false;
	}

	return true;
}

///
/// Advance the string offset from one code point boundary to the next.
/// (Post-incrementing iteration.)
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// @param s const char16_t * string
/// @param i string offset
/// @see forwardOne
///
inline void forwardOneUnsafe(const char16_t * s, int32_t & i) noexcept {
	if (isLead(s[i++])) {
		++i;
	}
}

///
/// Advance the string offset from one code point boundary to the next.
/// (Post-incrementing iteration.)
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// @param s const char16_t * string
/// @param i string offset, must be i<length
/// @param length string length
/// @see forwardOneUnsafe
///
inline void forwardOne(const char16_t * s, int32_t & i, const int32_t length) noexcept {
	if (isLead(s[i++]) && i != length && isTrail(s[i])) {
		++i;
	}
}

///
/// Advance the string offset from one code point boundary to the n-th next one,
/// i.e., move forward by n code points.
/// (Post-incrementing iteration.)
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// @param s const char16_t * string
/// @param i string offset
/// @param n number of code points to skip
/// @see forwardN
///
inline void forwardNUnsafe(const char16_t * s, int32_t & i, const int32_t n) noexcept {
	int32_t N = n;
	while (N > 0) {
		forwardOneUnsafe(s, i);
		--N;
	}
}

///
/// Advance the string offset from one code point boundary to the n-th next one,
/// i.e., move forward by n code points.
/// (Post-incrementing iteration.)
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// @param s const char16_t * string
/// @param i int32_t string offset, must be i<length
/// @param length int32_t string length
/// @param n number of code points to skip
/// @see forwardNUnsafe
///
inline void forwardN(const char16_t * s, int32_t & i, const int32_t length, const int32_t n) noexcept {
	int32_t N = n;

	while (N > 0 && (i < length || (length < 0 && s[i] != 0))) {
		forwardOne(s, i, length);
		--N;
	}
}

///
/// Adjust a random-access offset to a code point boundary
/// at the start of a code point.
/// If the offset points to the trail surrogate of a surrogate pair,
/// then the offset is decremented.
/// Otherwise, it is not modified.
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// @param s const char16_t * string
/// @param i string offset
/// @see setCpStart
///
inline void setCpStartUnsafe(const char16_t * s, int32_t & i) noexcept {
	if (isTrail(s[i])) {
		--i;
	}
}

///
/// Adjust a random-access offset to a code point boundary
/// at the start of a code point.
/// If the offset points to the trail surrogate of a surrogate pair,
/// then the offset is decremented.
/// Otherwise, it is not modified.
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// @param s const char16_t * string
/// @param start starting string offset (usually 0)
/// @param i string offset, must be start<=i
/// @see setCpStartUnsafe
///
inline void setCpStart(const char16_t * s, const int32_t start, int32_t & i) noexcept {
	if (isTrail(s[i]) && i > start && isLead(s[i - 1])) {
		--i;
	}
}

///
/// Move the string offset from one code point boundary to the previous one
/// and get the code point between them.
/// (Pre-decrementing backward iteration.)
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// The input offset may be the same as the string length.
/// If the offset is behind a trail surrogate unit
/// for a supplementary code point, then the macro will read
/// the preceding lead surrogate as well.
/// If the offset is behind a lead surrogate, then that itself
/// will be returned as the code point.
/// The result is undefined if the offset is behind a single, unpaired trail surrogate.
///
/// @param s const char16_t * string
/// @param i string offset
/// @param c output char32_t variable
/// @see previous
///
inline void previousUnsafe(const char16_t * s, int32_t & i, char32_t & c) noexcept {
	c = s[--i];

	if (isTrail((char16_t) (c))) {
		c = getSupplementary(s[--i], c);
	}
}

///
/// Move the string offset from one code point boundary to the previous one
/// and get the code point between them.
/// (Pre-decrementing backward iteration.)
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// The input offset may be the same as the string length.
/// If the offset is behind a trail surrogate unit
/// for a supplementary code point, then the macro will read
/// the preceding lead surrogate as well.
/// If the offset is behind a lead surrogate or behind a single, unpaired
/// trail surrogate, then c is set to that unpaired surrogate.
///
/// @param s const char16_t * string
/// @param start starting string offset (usually 0)
/// @param i string offset, must be start<i
/// @param c output char32_t variable
/// @see previousUnsafe
///
inline void previous(const char16_t * s, const int32_t start, int32_t & i, char32_t & c) noexcept {
	c = s[--i];

	if (isTrail((char16_t) (c))) {
		uint16_t c2;

		if (i > start && isLead((char16_t) (c2 = s[i - 1]))) {
			--i;
			c = getSupplementary(c2, c);
		}
	}
}

///
/// Move the string offset from one code point boundary to the previous one.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// @param s const char16_t * string
/// @param i string offset
/// @see backOne
///
inline void backOneUnsafe(const char16_t * s, int32_t & i) noexcept {
	if (isTrail(s[--i])) {
		--i;
	}
}

///
/// Move the string offset from one code point boundary to the previous one.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// @param s const char16_t * string
/// @param start starting string offset (usually 0)
/// @param i string offset, must be start<i
/// @see backOneUnsafe
///
inline void backOne(const char16_t * s, const int32_t start, int32_t & i) noexcept {
	if (isTrail(s[--i]) && i > start && isLead(s[i - 1])) {
		--i;
	}
}

///
/// Move the string offset from one code point boundary to the n-th one before it,
/// i.e., move backward by n code points.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// @param s const char16_t * string
/// @param i string offset
/// @param n number of code points to skip
/// @see backN
///
inline void backNUnsafe(const char16_t * s, int32_t & i, const int32_t n) noexcept {
	int32_t N = n;

	while (N > 0) {
		backOneUnsafe(s, i);
		--N;
	}
}

///
/// Move the string offset from one code point boundary to the n-th one before it,
/// i.e., move backward by n code points.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// @param s const char16_t * string
/// @param start start of string
/// @param i string offset, must be start<i
/// @param n number of code points to skip
/// @see backNUnsafe
///
inline void backN(const char16_t * s, const int32_t start, int32_t & i, const int32_t n) noexcept {
	int32_t N = n;

	while (N > 0 && i > start) {
		backOne(s, start, i);
		--N;
	}
}

///
/// Adjust a random-access offset to a code point boundary after a code point.
/// If the offset is behind the lead surrogate of a surrogate pair,
/// then the offset is incremented.
/// Otherwise, it is not modified.
/// The input offset may be the same as the string length.
/// "Unsafe" macro, assumes well-formed UTF-16.
///
/// @param s const char16_t * string
/// @param i string offset
/// @see setCpLimit
///
inline void setCpLimitUnsafe(const char16_t * s, int32_t & i) noexcept {
	if (isLead(s[i - 1])) {
		++i;
	}
}

///
/// Adjust a random-access offset to a code point boundary after a code point.
/// If the offset is behind the lead surrogate of a surrogate pair,
/// then the offset is incremented.
/// Otherwise, it is not modified.
/// The input offset may be the same as the string length.
/// "Safe" macro, handles unpaired surrogates and checks for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// @param s const char16_t * string
/// @param start int32_t starting string offset (usually 0)
/// @param i int32_t string offset, start<=i<=length
/// @param length int32_t string length
/// @see setCpLimitUnsafe
///
inline void setCpLimit(const char16_t * s, const int32_t start, int32_t & i, const int32_t length) noexcept {
	if (start < i && (i < length || length < 0) && isLead(s[i - 1]) && isTrail(s[i])) {
		++i;
	}
}

} // namespace Balau::Unicode

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF16
