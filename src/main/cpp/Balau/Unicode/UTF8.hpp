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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF8
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF8

#include <Balau/Unicode/UnicodeErrorCode.hpp>

namespace Balau::Unicode {

///
/// The maximum number of UTF-8 code units (bytes) per Unicode code point (U+0000..U+10ffff).
///
constexpr uint8_t utf8MaxLength = 4;

///
/// Does this code unit (byte) encode a code point by itself (US-ASCII 0..0x7f)?
///
/// @param c 8-bit code unit (byte)
/// @return true or false
///
inline bool isSingle(const char c) noexcept {
	return (c & 0x80) == 0;
}

///
/// Is this code unit (byte) a UTF-8 lead byte? (0xC2..0xF4)?
///
/// @param c 8-bit code unit (byte)
/// @return true or false
///
inline bool isLead(const char c) noexcept {
	return (uint8_t) (c - 0xc2) <= 0x32;
}

///
/// Is this code unit (byte) a UTF-8 trail byte? (0x80..0xBF)?
///
/// @param c 8-bit code unit (byte)
/// @return true or false
///
inline bool isTrail(const char c) noexcept {
	return (int8_t) c < -0x40;
}

///
/// How many code units (bytes) are used for the UTF-8 encoding of this Unicode code point?
///
/// @param c 32-bit code point
/// @return 1..4, or 0 if c is a surrogate or not a Unicode code point
///
inline unsigned int utf8Length(const char32_t c) noexcept {
	const auto uc = (uint32_t) c;

	return uc <= 0x7f
		? 1 : uc <= 0x7ff
		? 2 : uc <= 0xd7ff
		? 3 : uc <= 0xdfff || uc > 0x10ffff
		? 0 : uc <= 0xffff
		? 3 : 4;
}

///
/// Counts the trail bytes for a UTF-8 lead byte.
///
/// Returns 0 for 0..0xc1 as well as for 0xf5..0xff.
/// leadByte might be evaluated multiple times.
///
/// This is internal since it is not meant to be called directly by external clients;
/// however it is called by public macros in this file and thus must remain stable.
///
/// @param leadByte The first byte of a UTF-8 sequence. Must be 0..0xff.
///
inline unsigned int countTrailBytes(const char leadByte) noexcept {
	return isLead(leadByte)
		? ((uint8_t) leadByte >= 0xe0) + ((uint8_t) leadByte >= 0xf0) + 1
		: 0;
}

///
/// Counts the trail bytes for a UTF-8 lead byte of a valid UTF-8 sequence.
///
/// Returns 0 for 0..0xc1. Undefined for 0xf5..0xff.
/// leadByte might be evaluated multiple times.
///
/// This is internal since it is not meant to be called directly by external clients;
/// however it is called by public macros in this file and thus must remain stable.
///
/// @param leadByte The first byte of a UTF-8 sequence. Must be 0..0xff.
///
inline unsigned int countTrailBytesUnsafe(const char leadByte) noexcept {
	const auto & leadByteU = * (const uint8_t *) &leadByte;
	return (leadByteU >= 0xc2) + (leadByteU >= 0xe0) + (leadByteU >= 0xf0);
}

///
/// Mask a UTF-8 lead byte, leave only the lower bits that form part of the code point value.
///
/// This is internal since it is not meant to be called directly by external clients;
/// however it is called by public macros in this file and thus must remain stable.
///
inline void maskLeadByte(char & leadByte, const unsigned int countTrailBytes) noexcept {
	leadByte &= (1 << (6 - countTrailBytes)) - 1;
}

///
/// Internal bit vector for 3-byte UTF-8 validity check, for use in isValidLead3AndT1.
/// Each bit indicates whether one lead byte + first trail byte pair starts a valid sequence.
/// Lead byte E0..EF bits 3..0 are used as byte index,
/// first trail byte bits 7..5 are used as bit index into that byte.
/// @see isValidLead3AndT1
///
extern const char * utf8Lead3T1Bits;

///
/// Internal 3-byte UTF-8 validity check.
/// Non-zero if lead byte E0..EF and first trail byte 00..FF start a valid sequence.
///
inline bool isValidLead3AndT1(const char lead, const char t1) noexcept {
	return utf8Lead3T1Bits[(uint8_t) lead & 0xf] & (1 << ((uint8_t) t1 >> 5));
}

///
/// Internal bit vector for 4-byte UTF-8 validity check, for use in isValidLead4AndT1.
/// Each bit indicates whether one lead byte + first trail byte pair starts a valid sequence.
/// First trail byte bits 7..4 are used as byte index,
/// lead byte F0..F4 bits 2..0 are used as bit index into that byte.
///
/// @see isValidLead4AndT1
///
extern const char * utf8Lead4T1Bits;

///
/// Internal 4-byte UTF-8 validity check.
/// Non-zero if lead byte F0..F4 and first trail byte 00..FF start a valid sequence.
///
inline bool isValidLead4AndT1(const char lead, const char t1) noexcept {
	return utf8Lead4T1Bits[(uint8_t) t1 >> 4] & (1 << (lead & 7));
}

///
/// Function for handling "append code point" with error-checking.
///
/// This is internal since it is not meant to be called directly by external clients;
/// however it is U_STABLE (not U_INTERNAL) since it is called by public macros in this
/// file and thus must remain stable, and should not be hidden when other internal
/// functions are hidden (otherwise public macros would fail to compile).
///
int32_t appendCharSafeBody(char * s, int32_t i, int32_t length, char32_t c, bool * pIsError);

///
/// Function for handling "skip backward one code point" with error-checking.
///
/// This is internal since it is not meant to be called directly by external clients;
/// however it is U_STABLE (not U_INTERNAL) since it is called by public macros in this
/// file and thus must remain stable, and should not be hidden when other internal
/// functions are hidden (otherwise public macros would fail to compile).
///
int32_t back1SafeBody(const char * s, int32_t start, int32_t i);

///
/// Adjust a random-access offset to a code point boundary at the start of a code point.
///
/// If the offset points to a UTF-8 trail byte, then the offset is moved backward to the corresponding lead byte.
///
/// Otherwise, it is not modified.
///
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @see setCpStart
///
inline void setCpStartUnsafe(const char * s, int32_t & i) noexcept {
	while (isTrail(s[i])) {
		--i;
	}
}

///
/// Get a code point from a string at a code point boundary offset,
/// and advance the offset to the next code point boundary.
/// (Post-incrementing forward iteration.)
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// The offset may point to the lead byte of a multi-byte sequence,
/// in which case the macro will read the whole sequence.
/// The result is undefined if the offset points to a trail byte
/// or an illegal UTF-8 sequence.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @param c output char32_t variable
/// @see next
///
inline void nextUnsafe(const char * s, int32_t & i, char32_t & c) noexcept {
	const auto * str = (const uint8_t * ) s;
	c = str[i++];

	if (!isSingle(c)) {
		if (c < 0xe0) {
			c = ((c & 0x1f) << 6) | (str[i++] & 0x3f);
		} else if (c < 0xf0) {
			// No need for (c&0xf) because the upper bits are truncated after <<12 in the cast to (char16_t).
			c = (char16_t) ((c << 12) | ((str[i] & 0x3f) << 6) | (str[i + 1] & 0x3f));
			i += 2;
		} else {
			c = ((c & 7) << 18) | ((str[i] & 0x3f) << 12) | ((str[i + 1] & 0x3f) << 6) | (str[i + 2] & 0x3f);
			i += 3;
		}
	}
}

///
/// Get a code point from a string at a random-access offset, without changing the offset.
///
/// The offset may point to either the lead byte or one of the trail bytes for a code point,
/// in which case the macro will read all of the bytes for the code point.
///
/// The result is undefined if the offset points to an illegal UTF-8 byte sequence.
///
/// Iteration through a string is more efficient with nextUnsafe or next.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @param c output char32_t variable
/// @see get
///
inline void getUnsafe(const char * s, const int32_t i, char32_t & c) noexcept {
	int32_t _u8_get_unsafe_index = (int32_t) i;
	setCpStartUnsafe(s, _u8_get_unsafe_index);
	nextUnsafe(s, _u8_get_unsafe_index, c);
}

///
/// Adjust a random-access offset to a code point boundary
/// at the start of a code point.
/// If the offset points to a UTF-8 trail byte,
/// then the offset is moved backward to the corresponding lead byte.
/// Otherwise, it is not modified.
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset (usually 0)
/// @param i int32_t string offset, must be start<=i
/// @see setCpStartUnsafe
///
inline void setCpStart(const char * s, const int32_t start, int32_t & i) noexcept {
	if (isTrail(s[i])) {
		i = back1SafeBody(s, start, i);
	}
}

//
// Function for handling "next code point" with error-checking.
//
// This is internal since it is not meant to be called directly by external clients;
// however it is U_STABLE (not U_INTERNAL) since it is called by public macros in this
// file and thus must remain stable, and should not be hidden when other internal
// functions are hidden (otherwise public macros would fail to compile).
//
char32_t nextCharSafeBody_(const char * s, int32_t * pi, int32_t length, char32_t c, int8_t strict);

///
/// Get a code point from a string at a code point boundary offset,
/// and advance the offset to the next code point boundary.
/// (Post-incrementing forward iteration.)
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// The offset may point to the lead byte of a multi-byte sequence,
/// in which case the macro will read the whole sequence.
/// If the offset points to a trail byte or an illegal UTF-8 sequence, then
/// c is set to a negative value.
///
/// @param s const uint8_t * string
/// @param i int32_t string offset, must be i<length
/// @param length int32_t string length
/// @param c output char32_t variable, set to <0 in case of an error
/// @see nextUnsafe
///
inline void next(const char * s, int32_t & i, const int32_t length, char32_t & c) {
	const auto * str = (const uint8_t * ) s;
	c = (uint8_t) str[i++];

	if (!isSingle(c)) {
		uint8_t t1, t2;

		if ((0xe0 <= c && c < 0xf0) && ((i + 1) < length || length < 0) && isValidLead3AndT1(c, t1 = str[i]) && (t2 = str[i + 1] - 0x80) <= 0x3f) {
			// handle U+0800..U+FFFF inline
			c = ((c & 0xf) << 12) | ((t1 & 0x3f) << 6) | t2;
			i += 2;
		} else if ((c < 0xe0 && c >= 0xc2) && (i != length) && (t1 = str[i] - 0x80) <= 0x3f) {
			// handle U+0080..U+07FF inline
			c = ((c & 0x1f) << 6) | t1;
			++i;
		} else {
			// function call for "complicated" and error cases
			c = nextCharSafeBody_(s, &i, length, c, -1);
		}
	}
}

///
/// Get a code point from a string at a random-access offset, without changing the offset.
///
/// The offset may point to either the lead byte or one of the trail bytes
/// for a code point, in which case the macro will read all of the bytes
/// for the code point.
///
/// The length can be negative for a NUL-terminated string.
///
/// If the offset points to an illegal UTF-8 byte sequence, then
/// c is set to a negative value.
/// Iteration through a string is more efficient with nextUnsafe or next.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset
/// @param i int32_t string offset, must be start<=i<length
/// @param length int32_t string length
/// @param c output char32_t variable, set to <0 in case of an error
/// @see getUnsafe
///
inline void get(const char * s, const int32_t start, const int32_t i, const int32_t length, char32_t & c) {
	int32_t _u8_get_index = i;
	setCpStart(s, start, _u8_get_index);
	next(s, _u8_get_index, length, c);
}

///
/// Get a code point from a string at a code point boundary offset, and advance the offset to the next code point boundary.
///
/// (Post-incrementing forward iteration.)
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// The offset may point to the lead byte of a multi-byte sequence,
/// in which case the macro will read the whole sequence.
/// If the offset points to a trail byte or an illegal UTF-8 sequence, then
/// c is set to U+FFFD.
///
/// This macro does not distinguish between a real U+FFFD in the text
/// and U+FFFD returned for an ill-formed sequence.
/// Use next() if that distinction is important.
///
/// @param s const uint8_t * string
/// @param i int32_t string offset, must be i<length
/// @param length int32_t string length
/// @param c output char32_t variable, set to U+FFFD in case of an error
/// @see next
///
inline void nextOrFffd(const char * s, int32_t & i, const int32_t length, char32_t & c) {
	const auto * str = (const uint8_t * ) s;
	c = (uint8_t) str[i++];

	if (!isSingle(c)) {
		uint8_t t1, t2;

		if ((0xe0 <= c && c < 0xf0) && ((i + 1) < length || length < 0) && isValidLead3AndT1(c, t1 = s[i]) && (t2 = str[i + 1] - 0x80) <= 0x3f) {
			// handle U+0800..U+FFFF inline
			c = ((c & 0xf) << 12) | ((t1 & 0x3f) << 6) | t2;
			i += 2;
		} else if ((c < 0xe0 && c >= 0xc2) && (i != length) && (t1 = str[i] - 0x80) <= 0x3f) {
			// handle U+0080..U+07FF inline
			c = ((c & 0x1f) << 6) | t1;
			++i;
		} else {
			// function call for "complicated" and error cases
			c = nextCharSafeBody_(s, &i, length, c, -3);
		}
	}
}

///
/// Get a code point from a string at a random-access offset, without changing the offset.
///
/// The offset may point to either the lead byte or one of the trail bytes
/// for a code point, in which case the macro will read all of the bytes
/// for the code point.
///
/// The length can be negative for a NUL-terminated string.
///
/// If the offset points to an illegal UTF-8 byte sequence, then
/// c is set to U+FFFD.
/// Iteration through a string is more efficient with nextUnsafe or nextOrFffd.
///
/// This macro does not distinguish between a real U+FFFD in the text
/// and U+FFFD returned for an ill-formed sequence.
/// Use get() if that distinction is important.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset
/// @param i int32_t string offset, must be start<=i<length
/// @param length int32_t string length
/// @param c output char32_t variable, set to U+FFFD in case of an error
/// @see get
///
inline void getOrFffd(const char * s, const int32_t start, const int32_t i, const int32_t length, char32_t & c) {
	int32_t _u8_get_index = i;
	setCpStart(s, start, _u8_get_index);
	nextOrFffd(s, _u8_get_index, length, c);
}

///
/// Append a code point to a string, overwriting 1 to 4 bytes.
/// The offset points to the current end of the string contents
/// and is advanced (post-increment).
/// "Unsafe" macro, assumes a valid code point and sufficient space in the string.
/// Otherwise, the result is undefined.
///
/// @param s const uint8_t * string buffer
/// @param i string offset
/// @param c code point to append
/// @see append
///
inline void appendUnsafe(char * s, int32_t & i, char32_t c) noexcept {
	auto * str = (uint8_t * ) s;

	if ((uint32_t) c <= 0x7f) {
		str[i++] = (uint8_t) c;
	} else {
		if ((uint32_t) c <= 0x7ff) {
			str[i++] = (uint8_t) ((c >> 6) | 0xc0);
		} else {
			if ((uint32_t) c <= 0xffff) {
				str[i++] = (uint8_t) ((c >> 12) | 0xe0);
			} else {
				str[i++] = (uint8_t) ((c >> 18) | 0xf0);
				str[i++] = (uint8_t) (((c >> 12) & 0x3f) | 0x80);
			}

			str[i++] = (uint8_t) (((c >> 6) & 0x3f) | 0x80);
		}

		str[i++] = (uint8_t) ((c & 0x3f) | 0x80);
	}
}

///
/// Append a code point to a string, overwriting 1 to 4 bytes.
/// The offset points to the current end of the string contents
/// and is advanced (post-increment).
/// "Safe" macro, checks for a valid code point.
/// If a non-ASCII code point is written, checks for sufficient space in the string.
/// If the code point is not valid or trail bytes do not fit,
/// then isError is set to true.
///
/// @param s const uint8_t * string buffer
/// @param i int32_t string offset, must be i<capacity
/// @param capacity int32_t size of the string buffer
/// @param c char32_t code point to append
/// @see appendUnsafe
/// @return false if an error occurs, otherwise true
///
inline bool append(char * s, int32_t & i, const int32_t capacity, char32_t & c) {
	auto * str = (uint8_t * ) s;

	if ((uint32_t) c <= 0x7f) {
		str[i++] = (uint8_t) c;
	} else if ((uint32_t) c <= 0x7ff && i + 1 < capacity) {
		str[i++] = (uint8_t) ((c >> 6) | 0xc0);
		str[i++] = (uint8_t) ((c & 0x3f) | 0x80);
	} else if ((uint32_t) c <= 0xd7ff && i + 2 < capacity) {
		str[i++] = (uint8_t) ((c >> 12) | 0xe0);
		str[i++] = (uint8_t) (((c >> 6) & 0x3f) | 0x80);
		str[i++] = (uint8_t) ((c & 0x3f) | 0x80);
	} else {
		bool isError;
		i = appendCharSafeBody(s, i, capacity, c, &isError);
		return !isError;
	}

	return true;
}

///
/// Advance the string offset from one code point boundary to the next.
/// (Post-incrementing iteration.)
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @see forwardOne
///
inline void forwardOneUnsafe(const char * s, int32_t & i) noexcept {
	i += 1 + countTrailBytesUnsafe(s[i]);
}

///
/// Advance the string offset from one code point boundary to the next.
/// (Post-incrementing iteration.)
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// @param s const uint8_t * string
/// @param i int32_t string offset, must be i<length
/// @param length int32_t string length
/// @see forwardOneUnsafe
///
inline void forwardOne(const char * s, int32_t & i, const int32_t length) noexcept {
	const auto * str = (const uint8_t * ) s;
	uint8_t b = str[i++];

	if (isLead(b) && i != length) {
		uint8_t t1 = str[i];

		if ((0xe0 <= b && b < 0xf0)) {
			if (isValidLead3AndT1(b, t1) && ++i != length && isTrail(s[i])) {
				++i;
			}
		} else if (b < 0xe0) {
			if (isTrail(t1)) {
				++i;
			}
		} else /* c>=0xf0 */ {
			if (isValidLead4AndT1(b, t1) && ++i != length && isTrail(s[i]) && ++i != length && isTrail(s[i])) {
				++i;
			}
		}
	}
}

///
/// Advance the string offset from one code point boundary to the n-th next one,
/// i.e., move forward by n code points.
/// (Post-incrementing iteration.)
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @param n number of code points to skip
/// @see forwardN
///
inline void forwardNUnsafe(const char * s, int32_t & i, const int32_t n) noexcept {
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
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// @param s const uint8_t * string
/// @param i int32_t string offset, must be i<length
/// @param length int32_t string length
/// @param n number of code points to skip
/// @see forwardNUnsafe
///
inline void forwardN(const char * s, int32_t & i, const int32_t length, const int32_t n) {
	int32_t N = n;

	while (N > 0 && (i < length || (length < 0 && s[i] != 0))) {
		forwardOne(s, i, length);
		--N;
	}
}


///
/// Move the string offset from one code point boundary to the previous one
/// and get the code point between them.
/// (Pre-decrementing backward iteration.)
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// The input offset may be the same as the string length.
/// If the offset is behind a multi-byte sequence, then the macro will read
/// the whole sequence.
/// If the offset is behind a lead byte, then that itself
/// will be returned as the code point.
/// The result is undefined if the offset is behind an illegal UTF-8 sequence.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @param c output char32_t variable
/// @see previous
///
inline void previousUnsafe(const char * s, int32_t & i, char32_t & c) {
	c = (uint8_t) s[--i];

	if (isTrail(c)) {
		uint8_t count = 1, shift = 6;

		// c is a trail byte
		c &= 0x3f;

		for (;;) {
			auto b = s[--i];

			if ((uint8_t) b >= 0xc0) {
				maskLeadByte(b, count);
				c |= (char32_t) b << shift;
				break;
			} else {
				c |= (char32_t) (b & 0x3f) << shift;
				++count;
				shift += 6;
			}
		}
	}
}

//
// Function for handling "previous code point" with error-checking.
//
// This is internal since it is not meant to be called directly by external clients;
// however it is U_STABLE (not U_INTERNAL) since it is called by public macros in this
// file and thus must remain stable, and should not be hidden when other internal
// functions are hidden (otherwise public macros would fail to compile).
//
char32_t prevCharSafeBody_(const char * s, int32_t start, int32_t * pi, char32_t c, int8_t strict);

///
/// Move the string offset from one code point boundary to the previous one
/// and get the code point between them.
/// (Pre-decrementing backward iteration.)
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The input offset may be the same as the string length.
/// If the offset is behind a multi-byte sequence, then the macro will read
/// the whole sequence.
/// If the offset is behind a lead byte, then that itself
/// will be returned as the code point.
/// If the offset is behind an illegal UTF-8 sequence, then c is set to a negative value.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset (usually 0)
/// @param i int32_t string offset, must be start<i
/// @param c output char32_t variable, set to <0 in case of an error
/// @see previousUnsafe
///
inline void previous(const char * s, const int32_t start, int32_t & i, char32_t & c) {
	c = (uint8_t) s[--i];

	if (!isSingle(c)) {
		c = ::Balau::Unicode::prevCharSafeBody_(s, start, &i, c, -1);
	}
}

///
/// Move the string offset from one code point boundary to the previous one
/// and get the code point between them.
/// (Pre-decrementing backward iteration.)
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The input offset may be the same as the string length.
/// If the offset is behind a multi-byte sequence, then the macro will read
/// the whole sequence.
/// If the offset is behind a lead byte, then that itself
/// will be returned as the code point.
/// If the offset is behind an illegal UTF-8 sequence, then c is set to U+FFFD.
///
/// This macro does not distinguish between a real U+FFFD in the text
/// and U+FFFD returned for an ill-formed sequence.
/// Use previous() if that distinction is important.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset (usually 0)
/// @param i int32_t string offset, must be start<i
/// @param c output char32_t variable, set to U+FFFD in case of an error
/// @see previous
///
inline void previousOrFffd(const char * s, const int32_t start, int32_t & i, char32_t & c) {
	c = (uint8_t) s[--i];

	if (!isSingle(c)) {
		c = ::Balau::Unicode::prevCharSafeBody_(s, start, &i, c, -3);
	}
}

///
/// Move the string offset from one code point boundary to the previous one.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @see backOne
///
inline void backOneUnsafe(const char * s, int32_t & i) {
	while (isTrail(s[--i])) {}
}

///
/// Move the string offset from one code point boundary to the previous one.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset (usually 0)
/// @param i int32_t string offset, must be start<i
/// @see backOneUnsafe
///
inline void backOne(const char * s, const int32_t start, int32_t & i) {
	if (isTrail(s[--i])) {
		i = ::Balau::Unicode::back1SafeBody(s, start, i);
	}
}

///
/// Move the string offset from one code point boundary to the n-th one before it,
/// i.e., move backward by n code points.
/// (Pre-decrementing backward iteration.)
/// The input offset may be the same as the string length.
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @param n number of code points to skip
/// @see backN
///
inline void backNUnsafe(const char * s, int32_t & i, const int32_t n) {
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
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// @param s const uint8_t * string
/// @param start int32_t index of the start of the string
/// @param i int32_t string offset, must be start<i
/// @param n number of code points to skip
/// @see backNUnsafe
///
inline void backN(const char * s, const int32_t start, int32_t & i, const int32_t n) {
	int32_t N = n;

	while (N > 0 && i > (start)) {
		backOne(s, start, i);
		--N;
	}
}

///
/// Adjust a random-access offset to a code point boundary after a code point.
/// If the offset is behind a partial multi-byte sequence,
/// then the offset is incremented to behind the whole sequence.
/// Otherwise, it is not modified.
/// The input offset may be the same as the string length.
/// "Unsafe" macro, assumes well-formed UTF-8.
///
/// @param s const uint8_t * string
/// @param i string offset
/// @see setCpLimit
///
inline void setCpLimitUnsafe(const char * s, int32_t & i) {
	backOneUnsafe(s, i);
	forwardOneUnsafe(s, i);
}

///
/// Adjust a random-access offset to a code point boundary after a code point.
/// If the offset is behind a partial multi-byte sequence,
/// then the offset is incremented to behind the whole sequence.
/// Otherwise, it is not modified.
/// The input offset may be the same as the string length.
/// "Safe" macro, checks for illegal sequences and for string boundaries.
///
/// The length can be negative for a NUL-terminated string.
///
/// @param s const uint8_t * string
/// @param start int32_t starting string offset (usually 0)
/// @param i int32_t string offset, must be start<=i<=length
/// @param length int32_t string length
/// @see setCpLimitUnsafe
///
inline void setCpLimit(const char * s, const int32_t start, int32_t & i, const int32_t length) {
	if ((start) < i && (i < length || length < 0)) {
		backOne(s, start, i);
		forwardOne(s, i, length);
	}
}

} // namespace Balau::Unicode

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF8
