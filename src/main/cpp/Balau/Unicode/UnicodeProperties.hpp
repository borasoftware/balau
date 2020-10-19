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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_PROPERTIES
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_PROPERTIES

#include <Balau/Unicode/UnicodeErrorCode.hpp>
#include <Balau/Unicode/UnicodeChar.hpp>

namespace Balau::Unicode {

///
/// indexes[] entries
///
enum {
	UPROPS_PROPS32_INDEX,
	UPROPS_EXCEPTIONS_INDEX,
	UPROPS_EXCEPTIONS_TOP_INDEX,

	UPROPS_ADDITIONAL_TRIE_INDEX,
	UPROPS_ADDITIONAL_VECTORS_INDEX,
	UPROPS_ADDITIONAL_VECTORS_COLUMNS_INDEX,

	UPROPS_SCRIPT_EXTENSIONS_INDEX,

	UPROPS_RESERVED_INDEX_7,
	UPROPS_RESERVED_INDEX_8,

	///
	/// size of the data file (number of 32-bit units after the header)
	///
	UPROPS_DATA_TOP_INDEX,

	///
	/// maximum values for code values in vector word 0
	///
	UPROPS_MAX_VALUES_INDEX = 10,

	///
	/// maximum values for code values in vector word 2
	///
	UPROPS_MAX_VALUES_2_INDEX,

	UPROPS_INDEX_COUNT = 16
};

///
/// Definitions for the main properties words.
///
enum {
	// general category shift==0           0 (5 bits)
	// reserved                            5 (1 bit)
	UPROPS_NUMERIC_TYPE_VALUE_SHIFT = 6 // 6 (10 bits)
};

inline uint32_t GET_CATEGORY(uint32_t props) {
	return props & 0x1f;
}

inline uint32_t CAT_MASK(uint32_t props) {
	return uMask(GET_CATEGORY(props));
}

inline uint32_t GET_NUMERIC_TYPE_VALUE(uint32_t props) {
	return props >> UPROPS_NUMERIC_TYPE_VALUE_SHIFT;
}

///
/// Constants for the storage form of numeric types and values.
///
enum {
	///
	/// No numeric value.
	///
	UPROPS_NTV_NONE = 0,

	///
	/// Decimal digits: nv=0..9
	///
	UPROPS_NTV_DECIMAL_START = 1,

	///
	/// Other digits: nv=0..9
	///
	UPROPS_NTV_DIGIT_START = 11,

	///
	/// Small integers: nv=0..154
	///
	UPROPS_NTV_NUMERIC_START = 21,

	///
	/// Fractions: ((ntv>>4)-12) / ((ntv&0xf)+1) = -1..17 / 1..16
	///
	UPROPS_NTV_FRACTION_START = 0xb0,

	///
	/// Large integers:
	/// ((ntv>>5)-14) * 10^((ntv&0x1f)+2) = (1..9)*(10^2..10^33)
	/// (only one significant decimal digit)
	///
	UPROPS_NTV_LARGE_START = 0x1e0,

	///
	/// Sexagesimal numbers:
	/// ((ntv>>2)-0xbf) * 60^((ntv&3)+1) = (1..9)*(60^1..60^4)
	///
	UPROPS_NTV_BASE60_START = 0x300,

	///
	/// Fraction-20 values:
	/// frac20 = ntv-0x324 = 0..0x17 -> 1|3|5|7 / 20|40|80|160|320|640
	/// numerator: num = 2*(frac20&3)+1
	/// denominator: den = 20<<(frac20>>2)
	///
	UPROPS_NTV_FRACTION20_START = UPROPS_NTV_BASE60_START + 36,  // 0x300+9*4=0x324

	///
	/// No numeric value (yet).
	///
	UPROPS_NTV_RESERVED_START = UPROPS_NTV_FRACTION20_START + 24,  // 0x324+6*4=0x34c

	UPROPS_NTV_MAX_SMALL_INT = UPROPS_NTV_FRACTION_START - UPROPS_NTV_NUMERIC_START - 1
};

///
/// Number of properties vector words.
///
constexpr int UPROPS_VECTOR_WORDS = 3;

//
// Properties in vector word 0
// Bits
// 31..24   DerivedAge version major/minor one nibble each
// 23..22   3..1: Bits 7..0 = Script_Extensions index
//             3: Script value from Script_Extensions
//             2: Script=Inherited
//             1: Script=Common
//             0: Script=bits 7..0
// 21..20   reserved
// 19..17   East Asian Width
// 16.. 8   UBlockCode
//  7.. 0   UScriptCode, or index to Script_Extensions
//

///
/// Derived age: one nibble each for major and minor version numbers.
///
constexpr unsigned int UPROPS_AGE_MASK = 0xff000000;

constexpr unsigned int UPROPS_AGE_SHIFT = 24;

///
/// Script_Extensions: mask includes Script.
///
constexpr unsigned int UPROPS_SCRIPT_X_MASK = 0x00c000ff;

constexpr unsigned int UPROPS_SCRIPT_X_SHIFT = 22;

constexpr unsigned int UPROPS_EA_MASK = 0x000e0000;

constexpr unsigned int UPROPS_EA_SHIFT = 17;

constexpr unsigned int UPROPS_BLOCK_MASK = 0x0001ff00;

constexpr unsigned int UPROPS_BLOCK_SHIFT = 8;

constexpr unsigned int UPROPS_SCRIPT_MASK = 0x000000ff;

///
/// UPROPS_SCRIPT_X_WITH_COMMON must be the lowest value that involves Script_Extensions.
///
constexpr unsigned int UPROPS_SCRIPT_X_WITH_COMMON = 0x400000;

constexpr unsigned int UPROPS_SCRIPT_X_WITH_INHERITED = 0x800000;

constexpr unsigned int UPROPS_SCRIPT_X_WITH_OTHER = 0xc00000;

///
/// Properties in vector word 1.
///
/// Each bit encodes one binary property.
/// The following constants represent the bit number, use 1<<UPROPS_XYZ.
/// UPROPS_BINARY_1_TOP<=32!
///
/// Keep this list of property enums in sync with
/// propListNames[] in icu/source/tools/genprops/props2.c!
///
/// ICU 2.6/uprops format version 3.2 stores full properties instead of "Other_".
///
enum {
	UPROPS_WHITE_SPACE,
	UPROPS_DASH,
	UPROPS_HYPHEN,
	UPROPS_QUOTATION_MARK,
	UPROPS_TERMINAL_PUNCTUATION,
	UPROPS_MATH,
	UPROPS_HEX_DIGIT,
	UPROPS_ASCII_HEX_DIGIT,
	UPROPS_ALPHABETIC,
	UPROPS_IDEOGRAPHIC,
	UPROPS_DIACRITIC,
	UPROPS_EXTENDER,
	UPROPS_NONCHARACTER_CODE_POINT,
	UPROPS_GRAPHEME_EXTEND,
	UPROPS_GRAPHEME_LINK,
	UPROPS_IDS_BINARY_OPERATOR,
	UPROPS_IDS_TRINARY_OPERATOR,
	UPROPS_RADICAL,
	UPROPS_UNIFIED_IDEOGRAPH,
	UPROPS_DEFAULT_IGNORABLE_CODE_POINT,
	UPROPS_DEPRECATED,
	UPROPS_LOGICAL_ORDER_EXCEPTION,
	UPROPS_XID_START,
	UPROPS_XID_CONTINUE,
	UPROPS_ID_START,                            /* ICU 2.6, uprops format version 3.2 */
	UPROPS_ID_CONTINUE,
	UPROPS_GRAPHEME_BASE,
	UPROPS_S_TERM,                              /* new in ICU 3.0 and Unicode 4.0.1 */
	UPROPS_VARIATION_SELECTOR,
	UPROPS_PATTERN_SYNTAX,                      /* new in ICU 3.4 and Unicode 4.1 */
	UPROPS_PATTERN_WHITE_SPACE,
	UPROPS_PREPENDED_CONCATENATION_MARK,        // new in ICU 60 and Unicode 10
	UPROPS_BINARY_1_TOP                         /* ==32 - full! */
};

///
/// Properties in vector word 2.
///
/// Bits
/// 31..27   http://www.unicode.org/reports/tr51/#Emoji_Properties
///     26   reserved
/// 25..20   Line Break
/// 19..15   Sentence Break
/// 14..10   Word Break
///  9.. 5   Grapheme Cluster Break
///  4.. 0   Decomposition Type
///
enum {
	UPROPS_2_EMOJI_COMPONENT = 27,
	UPROPS_2_EMOJI,
	UPROPS_2_EMOJI_PRESENTATION,
	UPROPS_2_EMOJI_MODIFIER,
	UPROPS_2_EMOJI_MODIFIER_BASE
};

///
/// Gets the main properties value for a code point.
///
uint32_t getMainProperties(char32_t c);

///
/// Implemented in uchar.c for uprops.cpp.
///
/// @return 0 if no data or illegal argument
///
uint32_t getUnicodeProperties(char32_t c, int32_t column);

///
/// Get the the maximum values for some enum/int properties.
/// Use the same column numbers as for getUnicodeProperties().
/// The returned value will contain maximum values stored in the same bit fields
/// as where the enum values are stored in the getUnicodeProperties()
/// return values for the same columns.
///
/// Valid columns are those for properties words that contain enumerated values.
/// (ICU 2.6: columns 0 and 2)
///
/// For other column numbers, this function will return 0.
///
/// @internal
///
int32_t uprv_getMaxValues(int32_t column);

///
/// Checks if c is alphabetic, or a decimal digit; implements UCHAR_POSIX_ALNUM.
///
/// @internal
///
bool isAlphaNumPOSIX(char32_t c);

///
/// Checks if c is in  [^\p{space}\p{gc=Control}\p{gc=Surrogate}\p{gc=Unassigned}]  with space=\p{Whitespace} and Control=Cc.
///
/// Implements UCHAR_POSIX_GRAPH.
///
/// @internal
///
bool isGraphPOSIX(char32_t c);

///
/// Checks if c is in \p{graph}\p{blank} - \p{cntrl}.
/// Implements UCHAR_POSIX_PRINT.
///
/// @internal
///
bool isPrintPOSIX(char32_t c);

///
/// Turn a bit index into a bit flag.
///
/// @internal
///
inline constexpr uint32_t FLAG(UCharCategory n) noexcept {
	return 1 << Util::Enums::toUnderlying(n);
}

//// Flags for general categories in the order of UCharCategory. @internal ////

constexpr uint32_t _Cn = FLAG(UCharCategory::U_GENERAL_OTHER_TYPES);
constexpr uint32_t _Lu = FLAG(UCharCategory::U_UPPERCASE_LETTER);
constexpr uint32_t _Ll = FLAG(UCharCategory::U_LOWERCASE_LETTER);
constexpr uint32_t _Lt = FLAG(UCharCategory::U_TITLECASE_LETTER);
constexpr uint32_t _Lm = FLAG(UCharCategory::U_MODIFIER_LETTER);
constexpr uint32_t _Lo = FLAG(UCharCategory::U_OTHER_LETTER);
constexpr uint32_t _Mn = FLAG(UCharCategory::U_NON_SPACING_MARK);
constexpr uint32_t _Me = FLAG(UCharCategory::U_ENCLOSING_MARK);
constexpr uint32_t _Mc = FLAG(UCharCategory::U_COMBINING_SPACING_MARK);
constexpr uint32_t _Nd = FLAG(UCharCategory::U_DECIMAL_DIGIT_NUMBER);
constexpr uint32_t _Nl = FLAG(UCharCategory::U_LETTER_NUMBER);
constexpr uint32_t _No = FLAG(UCharCategory::U_OTHER_NUMBER);
constexpr uint32_t _Zs = FLAG(UCharCategory::U_SPACE_SEPARATOR);
constexpr uint32_t _Zl = FLAG(UCharCategory::U_LINE_SEPARATOR);
constexpr uint32_t _Zp = FLAG(UCharCategory::U_PARAGRAPH_SEPARATOR);
constexpr uint32_t _Cc = FLAG(UCharCategory::U_CONTROL_CHAR);
constexpr uint32_t _Cf = FLAG(UCharCategory::U_FORMAT_CHAR);
constexpr uint32_t _Co = FLAG(UCharCategory::U_PRIVATE_USE_CHAR);
constexpr uint32_t _Cs = FLAG(UCharCategory::U_SURROGATE);
constexpr uint32_t _Pd = FLAG(UCharCategory::U_DASH_PUNCTUATION);
constexpr uint32_t _Ps = FLAG(UCharCategory::U_START_PUNCTUATION);
constexpr uint32_t _Pe = FLAG(UCharCategory::U_END_PUNCTUATION);
constexpr uint32_t _Pc = FLAG(UCharCategory::U_CONNECTOR_PUNCTUATION);
constexpr uint32_t _Po = FLAG(UCharCategory::U_OTHER_PUNCTUATION);
constexpr uint32_t _Sm = FLAG(UCharCategory::U_MATH_SYMBOL);
constexpr uint32_t _Sc = FLAG(UCharCategory::U_CURRENCY_SYMBOL);
constexpr uint32_t _Sk = FLAG(UCharCategory::U_MODIFIER_SYMBOL);
constexpr uint32_t _So = FLAG(UCharCategory::U_OTHER_SYMBOL);
constexpr uint32_t _Pi = FLAG(UCharCategory::U_INITIAL_PUNCTUATION);
constexpr uint32_t _Pf = FLAG(UCharCategory::U_FINAL_PUNCTUATION);

///
/// Some code points.
///
/// @internal
///
enum {
	TAB = 0x0009,
	LF = 0x000a,
	FF = 0x000c,
	CR = 0x000d,
	U_A = 0x0041,
	U_F = 0x0046,
	U_Z = 0x005a,
	U_a = 0x0061,
	U_f = 0x0066,
	U_z = 0x007a,
	DEL = 0x007f,
	NL = 0x0085,
	NBSP = 0x00a0,
	CGJ = 0x034f,
	FIGURESP = 0x2007,
	HAIRSP = 0x200a,
	ZWNJ = 0x200c,
	ZWJ = 0x200d,
	RLM = 0x200f,
	NNBSP = 0x202f,
	WJ = 0x2060,
	INHSWAP = 0x206a,
	NOMDIG = 0x206f,
	U_FW_A = 0xff21,
	U_FW_F = 0xff26,
	U_FW_Z = 0xff3a,
	U_FW_a = 0xff41,
	U_FW_f = 0xff46,
	U_FW_z = 0xff5a,
	ZWNBSP = 0xfeff
};

} // namespace Balau::Unicode

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_PROPERTIES
