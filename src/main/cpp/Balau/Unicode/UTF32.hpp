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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF32
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF32

#include <Balau/Unicode/UnicodeErrorCode.hpp>

namespace Balau::Unicode {

///
/// Is this code point a Unicode noncharacter?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isUnicodeNonCharacter(const char32_t c) noexcept {
	return c >= 0xfdd0 && (c <= 0xfdef || (c & 0xfffe) == 0xfffe) && c <= 0x10ffff;
}

///
/// Is c a Unicode code point value (0..U+10ffff) that can be assigned a character?
///
/// Code points that are not characters include:
/// - single surrogate code points (U+d800..U+dfff, 2048 code points)
/// - the last two code points on each plane (U+__fffe and U+__ffff, 34 code points)
/// - U+fdd0..U+fdef (new with Unicode 3.1, 32 code points)
/// - the highest Unicode code point value is U+10ffff
///
/// This means that all code points below U+d800 are character code points,
/// and that boundary is tested first for performance.
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isUnicodeCharacter(char32_t c) noexcept {
	return (uint32_t) c < 0xd800 || (0xdfff < c && c <= 0x10ffff && !isUnicodeNonCharacter(c));
}

///
/// Is this code point a BMP code point (U+0000..U+ffff)?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isBmpCodePoint(char32_t c) noexcept {
	return (uint32_t) c <= 0xffff;
}

///
/// Is this code point a supplementary code point (U+10000..U+10ffff)?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isSupplementary(char32_t c) noexcept {
	return (uint32_t) (c - 0x10000) <= 0xfffff;
}

///
/// Is this code point a lead surrogate (U+d800..U+dbff)?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isLeadSurrogate(char32_t c) noexcept {
	return (c & 0xfffffc00) == 0xd800;
}

///
/// Is this code point a trail surrogate (U+dc00..U+dfff)?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isTrailSurrogate(char32_t c) noexcept {
	return (c & 0xfffffc00) == 0xdc00;
}

///
/// Is this code point a surrogate (U+d800..U+dfff)?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isSurrogate(char32_t c) noexcept {
	return (c & 0xfffff800) == 0xd800;
}

///
/// Assuming c is a surrogate code point (U_IS_SURROGATE(c)), is it a lead surrogate?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isSurrogateLead(char32_t c) noexcept {
	return (c & 0x400) == 0;
}

///
/// Assuming c is a surrogate code point (U_IS_SURROGATE(c)), is it a trail surrogate?
///
/// @param c 32-bit code point
/// @return true or false
///
inline bool isSurrogateTrail(char32_t c) noexcept {
	return (c & 0x400) != 0;
}

} // namespace Balau::Unicode

#endif  // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UTF32
