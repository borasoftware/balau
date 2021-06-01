// @formatter:off
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

///
/// @file Character.hpp
///
/// Utilities for unicode characters and code points.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__CHARACTER
#define COM_BORA_SOFTWARE__BALAU_TYPE__CHARACTER

#include <Balau/Type/StdTypes.hpp>

#include <unicode/uchar.h>
#include <unicode/utf8.h>

namespace Balau {

///
/// Utilities for unicode characters and code points.
///
struct Character {
	//////////////////////////// Classification /////////////////////////////

	///
	/// Does the specified code point have the general category "Ll" (lowercase letter).
	///
	static bool isLower(char32_t c) {
		return u_islower((UChar32) c);
	}

	///
	/// Does the specified code point have the general category "Lu" (uppercase letter).
	///
	static bool isUpper(char32_t c) {
		return u_isupper((UChar32) c);
	}

	///
	/// Does the specified code point have the general category "Nd" (decimal digit numbers).
	///
	static bool isDigit(char32_t c) {
		return u_isdigit((UChar32) c);
	}

	///
	/// Does the specified code point have the general category "Nd" (decimal digit numbers) or is one of the ASCII latin letters a-f or A-F.
	///
	static bool isHexDigit(char32_t c) {
		return c <= 0x7f && u_isxdigit((UChar32) c);
	}

	///
	/// Is the specified code point one of the ASCII characters 0-7.
	///
	static bool isOctalDigit(char32_t c) {
		return c >= U'0' && c <= U'9';
	}

	///
	/// Is the specified code point one of the ASCII characters 0-1.
	///
	static bool isBinaryDigit(char32_t c) {
		return c == U'0' || c == U'1';
	}

	///
	/// Does the specified code point have the general category "L" (letters).
	///
	static bool isAlpha(char32_t c) {
		return u_isalpha((UChar32) c);
	}

	///
	/// Does the specified code point have the general category "L" (letters) or "Nd" (decimal digit numbers).
	///
	static bool isAlphaOrDecimal(char32_t c) {
		return u_isalnum((UChar32) c);
	}

	///
	/// Is the specified code point a control character.
	///
	static bool isControlCharacter(char32_t c) {
		return u_iscntrl((UChar32) c);
	}

	///
	/// Is the specified code point a space character (excluding CR / LF).
	///
	static bool isSpace(char32_t c) {
		return u_isJavaSpaceChar((UChar32) c);
	}

	///
	/// Is the specified code point a whitespace character.
	///
	static bool isWhitespace(char32_t c) {
		return u_isWhitespace((UChar32) c);
	}

	///
	/// Is the specified code point a character that visibly separates words on a line.
	///
	static bool isBlank(char32_t c) {
		return u_isblank((UChar32) c);
	}

	///
	/// Is the specified code point a printable character.
	///
	static bool isPrintable(char32_t c) {
		return u_isprint((UChar32) c);
	}

	///
	/// Does the specified code point have the general category "P" (punctuation).
	///
	static bool isPunctuation(char32_t c) {
		return u_ispunct((UChar32) c);
	}

	///
	/// Does the specified code point have the general category "L" (letters) or "Nl" (letter numbers).
	///
	static bool isIdStart(char32_t c) {
		return u_isIDStart((UChar32) c);
	}

	///
	/// Is the specified code point valid as part of an Id.
	///
	static bool isIdPart(char32_t c) {
		return u_isIDPart((UChar32) c);
	}

	///
	/// Is the specified code point a breakable character for line endings.
	///
	static bool isBreakableCharacter(char32_t c) {
		return (u_isWhitespace((UChar32) c) || c == U'-');
	}

	///
	/// Is the specified code point a breakable character for line endings that should be printed.
	///
	static bool isInclusiveBreakableCharacter(char32_t c) {
		return c == U'-';
	}

	///
	/// Returns the number of bytes that the character occupies when UTF-8 encoded.
	///
	static size_t utf8ByteCount(char32_t c) {
		return U8_LENGTH((UChar32) c);
	}

	//////////////////////////////// Iteration ////////////////////////////////

	///
	/// Get the next code point from the UTF-8 string view.
	///
	/// The code point is obtained at the specified code point boundary offset and
	/// the offset is advanced to the next code point boundary.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be at least zero and less than the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static char32_t getNextUtf8(const std::string_view & text, int & offset) {
		UChar32 newUChar;
		U8_NEXT_UNSAFE(text.data(), offset, newUChar); // NOLINT
		return (char32_t) newUChar;
	}

	///
	/// Get the next code point from the UTF-8 string view (validating version).
	///
	/// If the string has invalid UTF-8 text, then the resulting char is <0.
	///
	/// The code point is obtained at the specified code point boundary offset and
	/// the offset is advanced to the next code point boundary.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be at least zero and less than the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static char32_t getNextUtf8Safe(const std::string_view & text, int & offset) {
		UChar32 newUChar;
		U8_NEXT(text.data(), offset, (int32_t) text.length(), newUChar); // NOLINT
		return (char32_t) newUChar;
	}

	///
	/// Get the previous code point from the UTF-8 string view.
	///
	/// The offset is retreated to the previous code point boundary and the code
	/// point is obtained at the resulting code point boundary offset.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be at least zero and be less or equal to the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static char32_t getPreviousUtf8(const std::string_view & text, int & offset) {
		UChar32 newUChar;
		U8_PREV_UNSAFE(text.data(), offset, newUChar); // NOLINT
		return (char32_t) newUChar;
	}

	///
	/// Get the previous code point from the UTF-8 string view (validating version).
	///
	/// If the string has invalid UTF-8 text, then the resulting char is <0.
	///
	/// The offset is retreated to the previous code point boundary and the code
	/// point is obtained at the resulting code point boundary offset.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be at least zero and be less or equal to the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static char32_t getPreviousUtf8Safe(const std::string_view & text, int & offset) {
		UChar32 newUChar;
		U8_PREV(text.data(), 0, offset, newUChar); // NOLINT
		return (char32_t) newUChar;
	}

	///
	/// Advance the supplied offset from one code point boundary to the next one.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be equal or less than the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static void advanceUtf8(const std::string_view & text, int & offset) {
		U8_FWD_1_UNSAFE(text.data(), offset); // NOLINT
	}

	///
	/// Advance the supplied offset from one code point boundary to the next one (validating version).
	///
	/// If the string has invalid UTF-8 text, then the resulting char is <0.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be equal or less than the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static void advanceUtf8Safe(const std::string_view & text, int & offset) {
		U8_FWD_1(text.data(), offset, ((int32_t) text.length())); // NOLINT
	}

	///
	/// Retreat the supplied offset from one code point boundary to the previous one.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be equal or less than the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static void retreatUtf8(const std::string_view & text, int & offset) {
		U8_BACK_1_UNSAFE(text.data(), offset);
	}

	///
	/// Retreat the supplied offset from one code point boundary to the previous one (validating version).
	///
	/// If the string has invalid UTF-8 text, then the resulting char is <0.
	///
	/// @param text   the immutable input string view
	/// @param offset the offset into the string (must be equal or less than the string's length)
	///
	/// TODO decide on most appropriate error handling strategy.
	///
	static void retreatUtf8Safe(const std::string_view & text, int & offset) {
		U8_BACK_1(((uint8_t *) text.data()), ((int32_t) 0), offset);
	}

	/////////////////////////////// Validation ////////////////////////////////

	static bool isValid(char32_t c) {
		return c < 0x10FFFF; // Maximum Unicode code point value.
	}

	//////////////////////////////// Mutation /////////////////////////////////

	///
	/// Convert the supplied code point to uppercase.
	///
	static char32_t toUpper(char32_t c) {
		return (char32_t) u_toupper((UChar32) c);
	}

	///
	/// Convert the supplied code point to lowercase.
	///
	static char32_t toLower(char32_t c) {
		return (char32_t) u_tolower((UChar32) c);
	}

	///
	/// Write a code point into the supplied UTF-8 string.
	///
	/// Set bytes in the destination string at the specified offset to the UTF-8 bytes
	/// resulting from the supplied code point. Advance the offset to immediate after
	/// the written bytes.
	///
	/// It is the responsibility of the caller to ensure that the destination string
	/// has enough bytes available for the code point at the specified offset.
	///
	/// @param destination the UTF-8 string into which the bytes will be written
	/// @param offset the offset into the destination string (this will be advanced)
	/// @param c the code point
	///
	///
	static void setUtf8AndAdvanceOffset(std::string & destination, int & offset, char32_t c) {
		auto newUChar = (UChar32) c;
		U8_APPEND_UNSAFE(&destination[0], offset, newUChar); // NOLINT
	}
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__CHARACTER
