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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_CHAR
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_CHAR

#include <Balau/Unicode/UnicodeErrorCode.hpp>
#include <Balau/Util/Enums.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Unicode {

///
/// Unicode version number, default for the current ICU version.
/// The actual Unicode Character Database (UCD) data is stored in uprops.dat
/// and may be generated from UCD files from a different Unicode version.
/// Call u_getUnicodeVersion to get the actual Unicode version of the data.
///
inline const char * unicodeVersion() noexcept {
	return "10.0";
}

///
/// \file
/// \brief C API: Unicode Properties
///
/// This C API provides low-level access to the Unicode Character Database.
/// In addition to raw property values, some convenience functions calculate
/// derived properties, for example for Java-style programming.
///
/// Unicode assigns each code point (not just assigned character) values for
/// many properties.
/// Most of them are simple boolean flags, or constants from a small enumerated list.
/// For some properties, values are strings or other relatively more complex types.
///
/// For more information see
/// "About the Unicode Character Database" (http://www.unicode.org/ucd/)
/// and the ICU User Guide chapter on Properties (http://icu-project.org/userguide/properties.html).
///
/// Many functions are designed to match java.lang.Character functions.
/// See the individual function documentation,
/// and see the JDK 1.4 java.lang.Character documentation
/// at http://java.sun.com/j2se/1.4/docs/api/java/lang/Character.html
///
/// There are also functions that provide easy migration from C/POSIX functions
/// like isblank(). Their use is generally discouraged because the C/POSIX
/// standards do not define their semantics beyond the ASCII range, which means
/// that different implementations exhibit very different behavior.
/// Instead, Unicode properties should be used directly.
///
/// There are also only a few, broad C/POSIX character classes, and they tend
/// to be used for conflicting purposes. For example, the "isalpha()" class
/// is sometimes used to determine word boundaries, while a more sophisticated
/// approach would at least distinguish initial letters from continuation
/// characters (the latter including combining marks).
/// (In ICU, BreakIterator is the most sophisticated API for word boundaries.)
/// Another example: There is no "istitle()" class for titlecase characters.
///
/// ICU 3.4 and later provides API access for all twelve C/POSIX character classes.
/// ICU implements them according to the Standard Recommendations in
/// Annex C: Compatibility Properties of UTS #18 Unicode Regular Expressions
/// (http://www.unicode.org/reports/tr18/#Compatibility_Properties).
///
/// API access for C/POSIX character classes is as follows:
/// - alpha:     isUAlphabetic(c) or u_hasBinaryProperty(c, UProperty::UCHAR_ALPHABETIC)
/// - lower:     u_isULowercase(c) or u_hasBinaryProperty(c, UCHAR_LOWERCASE)
/// - upper:     u_isUUppercase(c) or u_hasBinaryProperty(c, UCHAR_UPPERCASE)
/// - punct:     u_ispunct(c)
/// - digit:     u_isdigit(c) or u_charType(c)==U_DECIMAL_DIGIT_NUMBER
/// - xdigit:    u_isxdigit(c) or u_hasBinaryProperty(c, UCHAR_POSIX_XDIGIT)
/// - alnum:     u_hasBinaryProperty(c, UCHAR_POSIX_ALNUM)
/// - space:     u_isUWhiteSpace(c) or u_hasBinaryProperty(c, UCHAR_WHITE_SPACE)
/// - blank:     u_isblank(c) or u_hasBinaryProperty(c, UCHAR_POSIX_BLANK)
/// - cntrl:     u_charType(c)==U_CONTROL_CHAR
/// - graph:     u_hasBinaryProperty(c, UCHAR_POSIX_GRAPH)
/// - print:     u_hasBinaryProperty(c, UCHAR_POSIX_PRINT)
///
/// Note: Some of the u_isxyz() functions in uchar.h predate, and do not match,
/// the Standard Recommendations in UTS #18. Instead, they match Java
/// functions according to their API documentation.
///
/// \htmlonly
/// The C/POSIX character classes are also available in UnicodeSet patterns,
/// using patterns like [:graph:] or \p{graph}.
/// \endhtmlonly
///
/// Note: There are several ICU whitespace functions.
/// Comparison:
/// - isUWhiteSpace=UCHAR_WHITE_SPACE: Unicode White_Space property;
///       most of general categories "Z" (separators) + most whitespace ISO controls
///       (including no-break spaces, but excluding IS1..IS4 and ZWSP)
/// - isWhitespace: Java isWhitespace; Z + whitespace ISO controls but excluding no-break spaces
/// - isJavaSpaceChar: Java isSpaceChar; just Z (including no-break spaces)
/// - isspace: Z + whitespace ISO controls (including no-break spaces)
/// - isblank: "horizontal spaces" = TAB + Zs - ZWSP
///

///
/// The lowest Unicode code point value. Code points are non-negative.
///
constexpr uint32_t uCharMinValue = 0;

///
/// The highest Unicode code point value (scalar value) according to
/// The Unicode Standard. This is a 21-bit value (20.1 bits, rounded up).
/// For a single character, char32_t is a simple type that can hold any code point value.
///
/// @see char32_t
///
constexpr uint32_t uCharMaxValue = 0;

///
/// Get a single-bit bit set (a flag) from a bit number 0..31.
///
inline uint32_t constexpr uMask(uint32_t x) noexcept {
	return 1 << x;
}

///
/// Selection constants for Unicode properties.
/// These constants are used in functions like u_hasBinaryProperty to select
/// one of the Unicode properties.
///
/// The properties APIs are intended to reflect Unicode properties as defined
/// in the Unicode Character Database (UCD) and Unicode Technical Reports (UTR).
///
/// For details about the properties see
/// UAX #44: Unicode Character Database (http://www.unicode.org/reports/tr44/).
///
/// Important: If ICU is built with UCD files from Unicode versions below, e.g., 3.2,
/// then properties marked with "new in Unicode 3.2" are not or not fully available.
/// Check u_getUnicodeVersion to be sure.
///
/// @see hasBinaryProperty
/// @see getIntPropertyValue
/// @see getUnicodeVersion
///
enum class UProperty {
	//
	// Note: UProperty constants are parsed by preparseucd.py.
	// It matches lines like
	//     UCHAR_<Unicode property name>=<integer>,
	//

	// Note: Place UProperty::UCHAR_ALPHABETIC before UProperty::UCHAR_BINARY_START so that
	// debuggers display UProperty::UCHAR_ALPHABETIC as the symbolic name for 0,
	// rather than UProperty::UCHAR_BINARY_START.  Likewise for other *_START
	// identifiers.

	///
	/// Binary property Alphabetic. Same as isUAlphabetic, different from u_isalpha.
	/// Lu+Ll+Lt+Lm+Lo+Nl+Other_Alphabetic
	///
	UCHAR_ALPHABETIC = 0,

	///
	/// First constant for binary Unicode properties.
	///
	UCHAR_BINARY_START = UCHAR_ALPHABETIC,

	///
	/// Binary property ASCII_Hex_Digit. 0-9 A-F a-f
	///
	UCHAR_ASCII_HEX_DIGIT = 1,

	///
	/// Binary property Bidi_Control.
	/// Format controls which have specific functions
	/// in the Bidi Algorithm.
	///
	UCHAR_BIDI_CONTROL = 2,

	///
	/// Binary property Bidi_Mirrored.
	/// Characters that may change display in RTL text.
	/// Same as u_isMirrored.
	/// See Bidi Algorithm, UTR 9.
	///
	UCHAR_BIDI_MIRRORED = 3,

	///
	/// Binary property Dash. Variations of dashes.
	///
	UCHAR_DASH = 4,

	///
	/// Binary property Default_Ignorable_Code_Point (new in Unicode 3.2).
	/// Ignorable in most processing.
	/// <2060..206F, FFF0..FFFB, E0000..E0FFF>+Other_Default_Ignorable_Code_Point+(Cf+Cc+Cs-White_Space)
	///
	UCHAR_DEFAULT_IGNORABLE_CODE_POINT = 5,

	///
	/// Binary property Deprecated (new in Unicode 3.2).
	/// The usage of deprecated characters is strongly discouraged.
	///
	UCHAR_DEPRECATED = 6,

	///
	/// Binary property Diacritic. Characters that linguistically modify
	/// the meaning of another character to which they apply.
	///
	UCHAR_DIACRITIC = 7,

	///
	/// Binary property Extender.
	/// Extend the value or shape of a preceding alphabetic character,
	/// e.g., length and iteration marks.
	///
	UCHAR_EXTENDER = 8,

	///
	/// Binary property Full_Composition_Exclusion.
	/// CompositionExclusions.txt+Singleton Decompositions+
	/// Non-Starter Decompositions.
	///
	UCHAR_FULL_COMPOSITION_EXCLUSION = 9,

	///
	/// Binary property Grapheme_Base (new in Unicode 3.2).
	/// For programmatic determination of grapheme cluster boundaries.
	/// [0..10FFFF]-Cc-Cf-Cs-Co-Cn-Zl-Zp-Grapheme_Link-Grapheme_Extend-CGJ
	///
	UCHAR_GRAPHEME_BASE = 10,

	///
	/// Binary property Grapheme_Extend (new in Unicode 3.2).
	/// For programmatic determination of grapheme cluster boundaries.
	/// Me+Mn+Mc+Other_Grapheme_Extend-Grapheme_Link-CGJ
	///
	UCHAR_GRAPHEME_EXTEND = 11,

	///
	/// Binary property Grapheme_Link (new in Unicode 3.2).
	/// For programmatic determination of grapheme cluster boundaries.
	///
	UCHAR_GRAPHEME_LINK = 12,

	///
	/// Binary property Hex_Digit.
	/// Characters commonly used for hexadecimal numbers.
	///
	UCHAR_HEX_DIGIT = 13,

	///
	/// Binary property Hyphen. Dashes used to mark connections
	/// between pieces of words, plus the Katakana middle dot.
	///
	UCHAR_HYPHEN = 14,

	///
	/// Binary property ID_Continue.
	/// Characters that can continue an identifier.
	/// DerivedCoreProperties.txt also says "NOTE: Cf characters should be filtered out."
	/// ID_Start+Mn+Mc+Nd+Pc
	///
	UCHAR_ID_CONTINUE = 15,

	///
	/// Binary property ID_Start.
	/// Characters that can start an identifier.
	/// Lu+Ll+Lt+Lm+Lo+Nl
	///
	UCHAR_ID_START = 16,

	///
	/// Binary property Ideographic.
	/// CJKV ideographs.
	///
	UCHAR_IDEOGRAPHIC = 17,

	///
	/// Binary property IDS_Binary_Operator (new in Unicode 3.2).
	/// For programmatic determination of
	/// Ideographic Description Sequences.
	///
	UCHAR_IDS_BINARY_OPERATOR = 18,

	///
	/// Binary property IDS_Trinary_Operator (new in Unicode 3.2).
	/// For programmatic determination of
	/// Ideographic Description Sequences.
	///
	UCHAR_IDS_TRINARY_OPERATOR = 19,

	///
	/// Binary property Join_Control.
	/// Format controls for cursive joining and ligation.
	///
	UCHAR_JOIN_CONTROL = 20,

	///
	/// Binary property Logical_Order_Exception (new in Unicode 3.2).
	/// Characters that do not use logical order and
	/// require special handling in most processing.
	///
	UCHAR_LOGICAL_ORDER_EXCEPTION = 21,

	///
	/// Binary property Lowercase. Same as u_isULowercase, different from u_islower.
	/// Ll+Other_Lowercase
	///
	UCHAR_LOWERCASE = 22,

	///
	/// Binary property Math. Sm+Other_Math
	///
	UCHAR_MATH = 23,

	///
	/// Binary property Noncharacter_Code_Point.
	/// Code points that are explicitly defined as illegal
	/// for the encoding of characters.
	///
	UCHAR_NONCHARACTER_CODE_POINT = 24,

	///
	/// Binary property Quotation_Mark.
	///
	UCHAR_QUOTATION_MARK = 25,

	///
	/// Binary property Radical (new in Unicode 3.2).
	/// For programmatic determination of
	/// Ideographic Description Sequences.
	///
	UCHAR_RADICAL = 26,

	///
	/// Binary property Soft_Dotted (new in Unicode 3.2).
	/// Characters with a "soft dot", like i or j.
	/// An accent placed on these characters causes
	/// the dot to disappear.
	///
	UCHAR_SOFT_DOTTED = 27,

	///
	/// Binary property Terminal_Punctuation.
	/// Punctuation characters that generally mark
	/// the end of textual units.
	///
	UCHAR_TERMINAL_PUNCTUATION = 28,

	///
	/// Binary property Unified_Ideograph (new in Unicode 3.2).
	/// For programmatic determination of
	/// Ideographic Description Sequences.
	///
	UCHAR_UNIFIED_IDEOGRAPH = 29,

	///
	/// Binary property Uppercase. Same as u_isUUppercase, different from u_isupper.
	/// Lu+Other_Uppercase
	///
	UCHAR_UPPERCASE = 30,

	///
	/// Binary property White_Space.
	/// Same as u_isUWhiteSpace, different from u_isspace and u_isWhitespace.
	/// Space characters+TAB+CR+LF-ZWSP-ZWNBSP
	///
	UCHAR_WHITE_SPACE = 31,

	///
	/// Binary property XID_Continue.
	/// ID_Continue modified to allow closure under
	/// normalization forms NFKC and NFKD.
	///
	UCHAR_XID_CONTINUE = 32,

	///
	/// Binary property XID_Start. ID_Start modified to allow
	/// closure under normalization forms NFKC and NFKD.
	///
	UCHAR_XID_START = 33,

	///
	/// Binary property Case_Sensitive. Either the source of a case
	/// mapping or _in_ the target of a case mapping. Not the same as
	/// the general category Cased_Letter.
	///
	UCHAR_CASE_SENSITIVE = 34,

	///
	/// Binary property STerm (new in Unicode 4.0.1).
	/// Sentence Terminal. Used in UAX #29: Text Boundaries
	///(http://www.unicode.org/reports/tr29/)
	///
	UCHAR_S_TERM = 35,

	///
	/// Binary property Variation_Selector (new in Unicode 4.0.1).
	/// Indicates all those characters that qualify as Variation Selectors.
	/// For details on the behavior of these characters,
	/// see StandardizedVariants.html and 15.6 Variation Selectors.
	///
	UCHAR_VARIATION_SELECTOR = 36,

	///
	/// Binary property NFD_Inert.
	/// ICU-specific property for characters that are inert under NFD,
	/// i.e., they do not interact with adjacent characters.
	/// See the documentation for the Normalizer2 class and the
	/// Normalizer2::isInert() method.
	///
	UCHAR_NFD_INERT = 37,

	///
	/// Binary property NFKD_Inert.
	/// ICU-specific property for characters that are inert under NFKD,
	/// i.e., they do not interact with adjacent characters.
	/// See the documentation for the Normalizer2 class and the
	/// Normalizer2::isInert() method.
	///
	UCHAR_NFKD_INERT = 38,

	///
	/// Binary property NFC_Inert.
	/// ICU-specific property for characters that are inert under NFC,
	/// i.e., they do not interact with adjacent characters.
	/// See the documentation for the Normalizer2 class and the
	/// Normalizer2::isInert() method.
	///
	UCHAR_NFC_INERT = 39,

	///
	/// Binary property NFKC_Inert.
	/// ICU-specific property for characters that are inert under NFKC,
	/// i.e., they do not interact with adjacent characters.
	/// See the documentation for the Normalizer2 class and the
	/// Normalizer2::isInert() method.
	///
	UCHAR_NFKC_INERT = 40,

	///
	/// Binary Property Segment_Starter.
	/// ICU-specific property for characters that are starters in terms of
	/// Unicode normalization and combining character sequences.
	/// They have ccc=0 and do not occur in non-initial position of the
	/// canonical decomposition of any character
	/// (like a-umlaut in NFD and a Jamo T in an NFD(Hangul LVT)).
	/// ICU uses this property for segmenting a string for generating a set of
	/// canonically equivalent strings, e.g. for canonical closure while
	/// processing collation tailoring rules.
	///
	UCHAR_SEGMENT_STARTER = 41,

	///
	/// Binary property Pattern_Syntax (new in Unicode 4.1).
	/// See UAX #31 Identifier and Pattern Syntax
	/// (http://www.unicode.org/reports/tr31/)
	///
	UCHAR_PATTERN_SYNTAX = 42,

	///
	/// Binary property Pattern_White_Space (new in Unicode 4.1).
	/// See UAX #31 Identifier and Pattern Syntax
	/// (http://www.unicode.org/reports/tr31/)
	///
	UCHAR_PATTERN_WHITE_SPACE = 43,

	///
	/// Binary property alnum (a C/POSIX character class).
	/// Implemented according to the UTS #18 Annex C Standard Recommendation.
	///
	UCHAR_POSIX_ALNUM = 44,

	///
	/// Binary property blank (a C/POSIX character class).
	/// Implemented according to the UTS #18 Annex C Standard Recommendation.
	/// See the uchar.h file documentation.
	///
	UCHAR_POSIX_BLANK = 45,

	///
	/// Binary property graph (a C/POSIX character class).
	/// Implemented according to the UTS #18 Annex C Standard Recommendation.
	///
	UCHAR_POSIX_GRAPH = 46,

	///
	/// Binary property print (a C/POSIX character class).
	/// Implemented according to the UTS #18 Annex C Standard Recommendation.
	///
	UCHAR_POSIX_PRINT = 47,

	///
	/// Binary property xdigit (a C/POSIX character class).
	/// Implemented according to the UTS #18 Annex C Standard Recommendation.
	///
	UCHAR_POSIX_XDIGIT = 48,

	///
	/// Binary property Cased. For Lowercase, Uppercase and Titlecase characters.
	///
	UCHAR_CASED = 49,

	///
	/// Binary property Case_Ignorable. Used in context-sensitive case mappings.
	///
	UCHAR_CASE_IGNORABLE = 50,

	///
	/// Binary property Changes_When_Lowercased.
	///
	UCHAR_CHANGES_WHEN_LOWERCASED = 51,

	///
	/// Binary property Changes_When_Uppercased.
	///
	UCHAR_CHANGES_WHEN_UPPERCASED = 52,

	///
	/// Binary property Changes_When_Titlecased.
	///
	UCHAR_CHANGES_WHEN_TITLECASED = 53,

	///
	/// Binary property Changes_When_Casefolded.
	///
	UCHAR_CHANGES_WHEN_CASEFOLDED = 54,

	///
	/// Binary property Changes_When_Casemapped.
	///
	UCHAR_CHANGES_WHEN_CASEMAPPED = 55,

	///
	/// Binary property Changes_When_NFKC_Casefolded.
	///
	UCHAR_CHANGES_WHEN_NFKC_CASEFOLDED = 56,

	///
	/// Binary property Emoji.
	/// See http://www.unicode.org/reports/tr51/#Emoji_Properties
	///
	UCHAR_EMOJI = 57,

	///
	/// Binary property Emoji_Presentation.
	/// See http://www.unicode.org/reports/tr51/#Emoji_Properties
	///
	UCHAR_EMOJI_PRESENTATION = 58,

	///
	/// Binary property Emoji_Modifier.
	/// See http://www.unicode.org/reports/tr51/#Emoji_Properties
	///
	UCHAR_EMOJI_MODIFIER = 59,

	///
	/// Binary property Emoji_Modifier_Base.
	/// See http://www.unicode.org/reports/tr51/#Emoji_Properties
	///
	UCHAR_EMOJI_MODIFIER_BASE = 60,

	///
	/// Binary property Emoji_Component.
	/// See http://www.unicode.org/reports/tr51/#Emoji_Properties
	///
	UCHAR_EMOJI_COMPONENT = 61,

	///
	/// Binary property Regional_Indicator.
	///
	UCHAR_REGIONAL_INDICATOR = 62,

	///
	/// Binary property Prepended_Concatenation_Mark.
	///
	UCHAR_PREPENDED_CONCATENATION_MARK = 63,

	///
	/// One more than the last constant for binary Unicode properties.
	///
	UCHAR_BINARY_LIMIT,

	///
	/// Enumerated property Bidi_Class.
	/// Same as u_charDirection, returns UCharDirection values.
	///
	UCHAR_BIDI_CLASS = 0x1000,

	///
	/// First constant for enumerated/integer Unicode properties.
	///
	UCHAR_INT_START = UCHAR_BIDI_CLASS,

	///
	/// Enumerated property Block.
	/// Same as ublock_getCode, returns UBlockCode values.
	///
	UCHAR_BLOCK = 0x1001,

	///
	/// Enumerated property Canonical_Combining_Class.
	/// Same as u_getCombiningClass, returns 8-bit numeric values.
	///
	UCHAR_CANONICAL_COMBINING_CLASS = 0x1002,

	///
	/// Enumerated property Decomposition_Type.
	/// Returns UDecompositionType values.
	///
	UCHAR_DECOMPOSITION_TYPE = 0x1003,

	///
	/// Enumerated property East_Asian_Width.
	/// See http://www.unicode.org/reports/tr11/
	/// Returns UEastAsianWidth values.
	///
	UCHAR_EAST_ASIAN_WIDTH = 0x1004,

	///
	/// Enumerated property General_Category.
	/// Same as u_charType, returns UCharCategory values.
	///
	UCHAR_GENERAL_CATEGORY = 0x1005,

	///
	/// Enumerated property Joining_Group.
	/// Returns UJoiningGroup values.
	///
	UCHAR_JOINING_GROUP = 0x1006,

	///
	/// Enumerated property Joining_Type.
	/// Returns UJoiningType values.
	///
	UCHAR_JOINING_TYPE = 0x1007,

	///
	/// Enumerated property Line_Break.
	/// Returns ULineBreak values.
	///
	UCHAR_LINE_BREAK = 0x1008,

	///
	/// Enumerated property Numeric_Type.
	/// Returns UNumericType values.
	///
	UCHAR_NUMERIC_TYPE = 0x1009,

	///
	/// Enumerated property Script.
	/// Same as uscript_getScript, returns UScriptCode values.
	///
	UCHAR_SCRIPT = 0x100A,

	///
	/// Enumerated property Hangul_Syllable_Type, new in Unicode 4.
	/// Returns UHangulSyllableType values.
	///
	UCHAR_HANGUL_SYLLABLE_TYPE = 0x100B,

	///
	/// Enumerated property NFD_Quick_Check.
	/// Returns UNormalizationCheckResult values.
	///
	UCHAR_NFD_QUICK_CHECK = 0x100C,

	///
	/// Enumerated property NFKD_Quick_Check.
	/// Returns UNormalizationCheckResult values.
	///
	UCHAR_NFKD_QUICK_CHECK = 0x100D,

	///
	/// Enumerated property NFC_Quick_Check.
	/// Returns UNormalizationCheckResult values.
	///
	UCHAR_NFC_QUICK_CHECK = 0x100E,

	///
	/// Enumerated property NFKC_Quick_Check.
	/// Returns UNormalizationCheckResult values.
	///
	UCHAR_NFKC_QUICK_CHECK = 0x100F,

	///
	/// Enumerated property Lead_Canonical_Combining_Class.
	/// ICU-specific property for the ccc of the first code point
	/// of the decomposition, or lccc(c)=ccc(NFD(c)[0]).
	/// Useful for checking for canonically ordered text;
	/// see UNORM_FCD and http://www.unicode.org/notes/tn5/#FCD .
	/// Returns 8-bit numeric values like UCHAR_CANONICAL_COMBINING_CLASS.
	///
	UCHAR_LEAD_CANONICAL_COMBINING_CLASS = 0x1010,

	///
	/// Enumerated property Trail_Canonical_Combining_Class.
	/// ICU-specific property for the ccc of the last code point
	/// of the decomposition, or tccc(c)=ccc(NFD(c)[last]).
	/// Useful for checking for canonically ordered text;
	/// see UNORM_FCD and http://www.unicode.org/notes/tn5/#FCD .
	/// Returns 8-bit numeric values like UCHAR_CANONICAL_COMBINING_CLASS.
	///
	UCHAR_TRAIL_CANONICAL_COMBINING_CLASS = 0x1011,

	///
	/// Enumerated property Grapheme_Cluster_Break (new in Unicode 4.1).
	/// Used in UAX #29: Text Boundaries
	/// (http://www.unicode.org/reports/tr29/)
	/// Returns UGraphemeClusterBreak values.
	///
	UCHAR_GRAPHEME_CLUSTER_BREAK = 0x1012,

	///
	/// Enumerated property Sentence_Break (new in Unicode 4.1).
	/// Used in UAX #29: Text Boundaries
	/// (http://www.unicode.org/reports/tr29/)
	/// Returns USentenceBreak values.
	///
	UCHAR_SENTENCE_BREAK = 0x1013,

	///
	/// Enumerated property Word_Break (new in Unicode 4.1).
	/// Used in UAX #29: Text Boundaries
	/// (http://www.unicode.org/reports/tr29/)
	/// Returns UWordBreakValues values.
	///
	UCHAR_WORD_BREAK = 0x1014,

	///
	/// Enumerated property Bidi_Paired_Bracket_Type (new in Unicode 6.3).
	/// Used in UAX #9: Unicode Bidirectional Algorithm
	/// (http://www.unicode.org/reports/tr9/)
	/// Returns UBidiPairedBracketType values.
	///
	UCHAR_BIDI_PAIRED_BRACKET_TYPE = 0x1015,

	///
	/// One more than the last constant for enumerated/integer Unicode properties.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	UCHAR_INT_LIMIT = 0x1016,

	///
	/// Bitmask property General_Category_Mask.
	/// This is the General_Category property returned as a bit mask.
	/// When used in u_getIntPropertyValue(c), same as uMask(u_charType(c)),
	/// returns bit masks for UCharCategory values where exactly one bit is set.
	/// When used with u_getPropertyValueName() and u_getPropertyValueEnum(),
	/// a multi-bit mask is used for sets of categories like "Letters".
	/// Mask values should be cast to uint32_t.
	///
	UCHAR_GENERAL_CATEGORY_MASK = 0x2000,

	///
	/// First constant for bit-mask Unicode properties.
	///
	UCHAR_MASK_START = UCHAR_GENERAL_CATEGORY_MASK,

	///
	/// One more than the last constant for bit-mask Unicode properties.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	UCHAR_MASK_LIMIT = 0x2001,

	///
	/// Double property Numeric_Value.
	/// Corresponds to u_getNumericValue.
	///
	UCHAR_NUMERIC_VALUE = 0x3000,

	///
	/// First constant for double Unicode properties.
	///
	UCHAR_DOUBLE_START = UCHAR_NUMERIC_VALUE,

	///
	/// One more than the last constant for double Unicode properties.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	UCHAR_DOUBLE_LIMIT = 0x3001,

	///
	/// String property Age.
	/// Corresponds to u_charAge.
	///
	UCHAR_AGE = 0x4000,

	///
	/// First constant for string Unicode properties.
	///
	UCHAR_STRING_START = UCHAR_AGE,

	///
	/// String property Bidi_Mirroring_Glyph.
	/// Corresponds to u_charMirror.
	///
	UCHAR_BIDI_MIRRORING_GLYPH = 0x4001,

	///
	/// String property Case_Folding.
	/// Corresponds to u_strFoldCase in ustring.h.
	///
	UCHAR_CASE_FOLDING = 0x4002,

	///
	/// Deprecated string property ISO_Comment.
	/// Corresponds to u_getISOComment. @deprecated ICU 49
	///
	UCHAR_ISO_COMMENT = 0x4003,

	///
	/// String property Lowercase_Mapping.
	/// Corresponds to u_strToLower in ustring.h.
	///
	UCHAR_LOWERCASE_MAPPING = 0x4004,

	///
	/// String property Name.
	/// Corresponds to u_charName.
	///
	UCHAR_NAME = 0x4005,

	///
	/// String property Simple_Case_Folding.
	/// Corresponds to u_foldCase.
	///
	UCHAR_SIMPLE_CASE_FOLDING = 0x4006,

	///
	/// String property Simple_Lowercase_Mapping.
	/// Corresponds to toLower.
	///
	UCHAR_SIMPLE_LOWERCASE_MAPPING = 0x4007,

	///
	/// String property Simple_Titlecase_Mapping.
	/// Corresponds to u_totitle.
	///
	UCHAR_SIMPLE_TITLECASE_MAPPING = 0x4008,

	///
	/// String property Simple_Uppercase_Mapping.
	/// Corresponds to toUpper.
	///
	UCHAR_SIMPLE_UPPERCASE_MAPPING = 0x4009,

	///
	/// String property Titlecase_Mapping.
	/// Corresponds to u_strToTitle in ustring.h.
	///
	UCHAR_TITLECASE_MAPPING = 0x400A,

	///
	/// String property Unicode_1_Name.
	/// This property is of little practical value.
	/// Beginning with ICU 49, ICU APIs return an empty string for this property.
	/// Corresponds to u_charName(U_UNICODE_10_CHAR_NAME). @deprecated ICU 49
	///
	UCHAR_UNICODE_1_NAME = 0x400B,

	///
	/// String property Uppercase_Mapping.
	/// Corresponds to u_strToUpper in ustring.h.
	///
	UCHAR_UPPERCASE_MAPPING = 0x400C,

	///
	/// String property Bidi_Paired_Bracket (new in Unicode 6.3).
	/// Corresponds to u_getBidiPairedBracket.
	///
	UCHAR_BIDI_PAIRED_BRACKET = 0x400D,

	///
	/// One more than the last constant for string Unicode properties.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	UCHAR_STRING_LIMIT = 0x400E,

	///
	/// Miscellaneous property Script_Extensions (new in Unicode 6.0).
	/// Some characters are commonly used in multiple scripts.
	/// For more information, see UAX #24: http://www.unicode.org/reports/tr24/.
	/// Corresponds to uscript_hasScript and uscript_getScriptExtensions in uscript.h.
	///
	UCHAR_SCRIPT_EXTENSIONS = 0x7000,

	///
	/// First constant for Unicode properties with unusual value types.
	///
	UCHAR_OTHER_PROPERTY_START = UCHAR_SCRIPT_EXTENSIONS,

	///
	///
	/// One more than the last constant for Unicode properties with unusual value types.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	///
	UCHAR_OTHER_PROPERTY_LIMIT = 0x7001,

	///
	/// Represents a nonexistent or invalid property or property value.
	///
	UCHAR_INVALID_CODE = -1
};

///
/// Data for enumerated Unicode general category types.
/// See http://www.unicode.org/Public/UNIDATA/UnicodeData.html .
///
enum class UCharCategory {
	//
	// Note: UCharCategory constants and their API comments are parsed by preparseucd.py.
	// It matches pairs of lines like
	//     / ** <Unicode 2-letter General_Category value> comment... * /
	//     U_<[A-Z_]+> = <integer>,
	//

	///
	/// Non-category for unassigned and non-character code points.
	///
	U_UNASSIGNED = 0,

	///
	/// Cn "Other, Not Assigned (no characters in [UnicodeData.txt] have this property)" (same as U_UNASSIGNED!)
	///
	U_GENERAL_OTHER_TYPES = 0,

	///
	/// Lu
	///
	U_UPPERCASE_LETTER = 1,

	///
	/// Ll
	///
	U_LOWERCASE_LETTER = 2,

	///
	/// Lt
	///
	U_TITLECASE_LETTER = 3,

	///
	/// Lm
	///
	U_MODIFIER_LETTER = 4,

	///
	/// Lo
	///
	U_OTHER_LETTER = 5,

	///
	/// Mn
	///
	U_NON_SPACING_MARK = 6,

	///
	/// Me
	///
	U_ENCLOSING_MARK = 7,

	///
	/// Mc
	///
	U_COMBINING_SPACING_MARK = 8,

	///
	/// Nd
	///
	U_DECIMAL_DIGIT_NUMBER = 9,

	///
	/// Nl
	///
	U_LETTER_NUMBER = 10,

	///
	/// No
	///
	U_OTHER_NUMBER = 11,

	///
	/// Zs
	///
	U_SPACE_SEPARATOR = 12,

	///
	/// Zl
	///
	U_LINE_SEPARATOR = 13,

	///
	/// Zp
	///
	U_PARAGRAPH_SEPARATOR = 14,

	///
	/// Cc
	///
	U_CONTROL_CHAR = 15,

	///
	/// Cf
	///
	U_FORMAT_CHAR = 16,

	///
	/// Co
	///
	U_PRIVATE_USE_CHAR = 17,

	///
	/// Cs
	///
	U_SURROGATE = 18,

	///
	/// Pd
	///
	U_DASH_PUNCTUATION = 19,

	///
	/// Ps
	///
	U_START_PUNCTUATION = 20,

	///
	/// Pe
	///
	U_END_PUNCTUATION = 21,

	///
	/// Pc
	///
	U_CONNECTOR_PUNCTUATION = 22,

	///
	/// Po
	///
	U_OTHER_PUNCTUATION = 23,

	///
	/// Sm
	///
	U_MATH_SYMBOL = 24,

	///
	/// Sc
	///
	U_CURRENCY_SYMBOL = 25,

	///
	/// Sk
	///
	U_MODIFIER_SYMBOL = 26,

	///
	/// So
	///
	U_OTHER_SYMBOL = 27,

	///
	/// Pi
	///
	U_INITIAL_PUNCTUATION = 28,

	///
	/// Pf
	///
	U_FINAL_PUNCTUATION = 29,

	///
	/// One higher than the last enum UCharCategory constant.
	/// This numeric value is stable (will not change), see
	/// http://www.unicode.org/policies/stability_policy.html#Property_Value
	///
	///
	U_CHAR_CATEGORY_COUNT
};

///
/// This specifies the language directional property of a character set.
///
enum class UCharDirection {
	//
	// Note: UCharDirection constants and their API comments are parsed by preparseucd.py.
	// It matches pairs of lines like
	//     / ** <Unicode 1..3-letter Bidi_Class value> comment... * /
	//     U_<[A-Z_]+> = <integer>,
	//

	///
	/// L
	///
	U_LEFT_TO_RIGHT = 0,

	///
	/// R
	///
	U_RIGHT_TO_LEFT = 1,

	///
	/// EN
	///
	U_EUROPEAN_NUMBER = 2,

	///
	/// ES
	///
	U_EUROPEAN_NUMBER_SEPARATOR = 3,

	///
	/// ET
	///
	U_EUROPEAN_NUMBER_TERMINATOR = 4,

	///
	/// AN
	///
	U_ARABIC_NUMBER = 5,

	///
	/// CS
	///
	U_COMMON_NUMBER_SEPARATOR = 6,

	///
	/// B
	///
	U_BLOCK_SEPARATOR = 7,

	///
	/// S
	///
	U_SEGMENT_SEPARATOR = 8,

	///
	/// WS
	///
	U_WHITE_SPACE_NEUTRAL = 9,

	///
	/// ON
	///
	U_OTHER_NEUTRAL = 10,

	///
	/// LRE
	///
	U_LEFT_TO_RIGHT_EMBEDDING = 11,

	///
	/// LRO
	///
	U_LEFT_TO_RIGHT_OVERRIDE = 12,

	///
	/// AL
	///
	U_RIGHT_TO_LEFT_ARABIC = 13,

	///
	/// RLE
	///
	U_RIGHT_TO_LEFT_EMBEDDING = 14,

	///
	/// RLO
	///
	U_RIGHT_TO_LEFT_OVERRIDE = 15,

	///
	/// PDF
	///
	U_POP_DIRECTIONAL_FORMAT = 16,

	///
	/// NSM
	///
	U_DIR_NON_SPACING_MARK = 17,

	///
	/// BN
	///
	U_BOUNDARY_NEUTRAL = 18,

	///
	/// FSI
	///
	U_FIRST_STRONG_ISOLATE = 19,

	///
	/// LRI
	///
	U_LEFT_TO_RIGHT_ISOLATE = 20,

	///
	/// RLI
	///
	U_RIGHT_TO_LEFT_ISOLATE = 21,

	///
	/// PDI
	///
	U_POP_DIRECTIONAL_ISOLATE = 22,

	///
	/// One more than the highest UCharDirection value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_BIDI_CLASS).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_CHAR_DIRECTION_COUNT
};

///
/// Bidi Paired Bracket Type constants.
///
/// @see UCHAR_BIDI_PAIRED_BRACKET_TYPE
///
enum class UBidiPairedBracketType {
	//
	// Note: UBidiPairedBracketType constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_BPT_<Unicode Bidi_Paired_Bracket_Type value name>
	//

	///
	/// Not a paired bracket.
	///
	U_BPT_NONE,

	///
	/// Open paired bracket.
	///
	U_BPT_OPEN,

	///
	/// Close paired bracket.
	///
	U_BPT_CLOSE,

	///
	/// One more than the highest normal UBidiPairedBracketType value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_BIDI_PAIRED_BRACKET_TYPE).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_BPT_COUNT /// 3
};

///
/// Constants for Unicode blocks, see the Unicode Data file Blocks.txt
///
enum class UBlockCode {
	//
	// Note: UBlockCode constants are parsed by preparseucd.py.
	// It matches lines like
	//     UBLOCK_<Unicode Block value name> = <integer>,
	//

	///
	/// New No_Block value in Unicode 4.
	///
	UBLOCK_NO_BLOCK = 0,                                         // [none] Special range indicating No_Block
	UBLOCK_BASIC_LATIN = 1,                                      // [0000]
	UBLOCK_LATIN_1_SUPPLEMENT = 2,                               // [0080]
	UBLOCK_LATIN_EXTENDED_A = 3,                                 // [0100]
	UBLOCK_LATIN_EXTENDED_B = 4,                                 // [0180]
	UBLOCK_IPA_EXTENSIONS = 5,                                   // [0250]
	UBLOCK_SPACING_MODIFIER_LETTERS = 6,                         // [02B0]
	UBLOCK_COMBINING_DIACRITICAL_MARKS = 7,                      // [0300]

	///
	/// Unicode 3.2 renames this block to "Greek and Coptic".
	///
	UBLOCK_GREEK = 8,                                            // [0370]

	UBLOCK_CYRILLIC = 9,                                         // [0400]
	UBLOCK_ARMENIAN = 10,                                        // [0530]
	UBLOCK_HEBREW = 11,                                          // [0590]
	UBLOCK_ARABIC = 12,                                          // [0600]
	UBLOCK_SYRIAC = 13,                                          // [0700]
	UBLOCK_THAANA = 14,                                          // [0780]
	UBLOCK_DEVANAGARI = 15,                                      // [0900]
	UBLOCK_BENGALI = 16,                                         // [0980]
	UBLOCK_GURMUKHI = 17,                                        // [0A00]
	UBLOCK_GUJARATI = 18,                                        // [0A80]
	UBLOCK_ORIYA = 19,                                           // [0B00]
	UBLOCK_TAMIL = 20,                                           // [0B80]
	UBLOCK_TELUGU = 21,                                          // [0C00]
	UBLOCK_KANNADA = 22,                                         // [0C80]
	UBLOCK_MALAYALAM = 23,                                       // [0D00]
	UBLOCK_SINHALA = 24,                                         // [0D80]
	UBLOCK_THAI = 25,                                            // [0E00]
	UBLOCK_LAO = 26,                                             // [0E80]
	UBLOCK_TIBETAN = 27,                                         // [0F00]
	UBLOCK_MYANMAR = 28,                                         // [1000]
	UBLOCK_GEORGIAN = 29,                                        // [10A0]
	UBLOCK_HANGUL_JAMO = 30,                                     // [1100]
	UBLOCK_ETHIOPIC = 31,                                        // [1200]
	UBLOCK_CHEROKEE = 32,                                        // [13A0]
	UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS = 33,           // [1400]
	UBLOCK_OGHAM = 34,                                           // [1680]
	UBLOCK_RUNIC = 35,                                           // [16A0]
	UBLOCK_KHMER = 36,                                           // [1780]
	UBLOCK_MONGOLIAN = 37,                                       // [1800]
	UBLOCK_LATIN_EXTENDED_ADDITIONAL = 38,                       // [1E00]
	UBLOCK_GREEK_EXTENDED = 39,                                  // [1F00]
	UBLOCK_GENERAL_PUNCTUATION = 40,                             // [2000]
	UBLOCK_SUPERSCRIPTS_AND_SUBSCRIPTS = 41,                     // [2070]
	UBLOCK_CURRENCY_SYMBOLS = 42,                                // [20A0]

	///
	/// Unicode 3.2 renames this block to "Combining Diacritical Marks for Symbols".
	///
	UBLOCK_COMBINING_MARKS_FOR_SYMBOLS = 43,                     // [20D0]

	UBLOCK_LETTERLIKE_SYMBOLS = 44,                              // [2100]
	UBLOCK_NUMBER_FORMS = 45,                                    // [2150]
	UBLOCK_ARROWS = 46,                                          // [2190]
	UBLOCK_MATHEMATICAL_OPERATORS = 47,                          // [2200]
	UBLOCK_MISCELLANEOUS_TECHNICAL = 48,                         // [2300]
	UBLOCK_CONTROL_PICTURES = 49,                                // [2400]
	UBLOCK_OPTICAL_CHARACTER_RECOGNITION = 50,                   // [2440]
	UBLOCK_ENCLOSED_ALPHANUMERICS = 51,                          // [2460]
	UBLOCK_BOX_DRAWING = 52,                                     // [2500]
	UBLOCK_BLOCK_ELEMENTS = 53,                                  // [2580]
	UBLOCK_GEOMETRIC_SHAPES = 54,                                // [25A0]
	UBLOCK_MISCELLANEOUS_SYMBOLS = 55,                           // [2600]
	UBLOCK_DINGBATS = 56,                                        // [2700]
	UBLOCK_BRAILLE_PATTERNS = 57,                                // [2800]
	UBLOCK_CJK_RADICALS_SUPPLEMENT = 58,                         // [2E80]
	UBLOCK_KANGXI_RADICALS = 59,                                 // [2F00]
	UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS = 60,              // [2FF0]
	UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION = 61,                     // [3000]
	UBLOCK_HIRAGANA = 62,                                        // [3040]
	UBLOCK_KATAKANA = 63,                                        // [30A0]
	UBLOCK_BOPOMOFO = 64,                                        // [3100]
	UBLOCK_HANGUL_COMPATIBILITY_JAMO = 65,                       // [3130]
	UBLOCK_KANBUN = 66,                                          // [3190]
	UBLOCK_BOPOMOFO_EXTENDED = 67,                               // [31A0]
	UBLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS = 68,                 // [3200]
	UBLOCK_CJK_COMPATIBILITY = 69,                               // [3300]
	UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A = 70,              // [3400]
	UBLOCK_CJK_UNIFIED_IDEOGRAPHS = 71,                          // [4E00]
	UBLOCK_YI_SYLLABLES = 72,                                    // [A000]
	UBLOCK_YI_RADICALS = 73,                                     // [A490]
	UBLOCK_HANGUL_SYLLABLES = 74,                                // [AC00]
	UBLOCK_HIGH_SURROGATES = 75,                                 // [D800]
	UBLOCK_HIGH_PRIVATE_USE_SURROGATES = 76,                     // [DB80]
	UBLOCK_LOW_SURROGATES = 77,                                  // [DC00]

	///
	/// Same as UBLOCK_PRIVATE_USE.
	/// Until Unicode 3.1.1, the corresponding block name was "Private Use",
	/// and multiple code point ranges had this block.
	/// Unicode 3.2 renames the block for the BMP PUA to "Private Use Area" and
	/// adds separate blocks for the supplementary PUAs.
	///
	///
	UBLOCK_PRIVATE_USE_AREA = 78, /// [E000]

	///
	/// Same as UBLOCK_PRIVATE_USE_AREA.
	/// Until Unicode 3.1.1, the corresponding block name was "Private Use",
	/// and multiple code point ranges had this block.
	/// Unicode 3.2 renames the block for the BMP PUA to "Private Use Area" and
	/// adds separate blocks for the supplementary PUAs.
	///
	///
	UBLOCK_PRIVATE_USE = UBLOCK_PRIVATE_USE_AREA,

	UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS = 79,                    // [F900]
	UBLOCK_ALPHABETIC_PRESENTATION_FORMS = 80,                   // [FB00]
	UBLOCK_ARABIC_PRESENTATION_FORMS_A = 81,                     // [FB50]
	UBLOCK_COMBINING_HALF_MARKS = 82,                            // [FE20]
	UBLOCK_CJK_COMPATIBILITY_FORMS = 83,                         // [FE30]
	UBLOCK_SMALL_FORM_VARIANTS = 84,                             // [FE50]
	UBLOCK_ARABIC_PRESENTATION_FORMS_B = 85,                     // [FE70]
	UBLOCK_SPECIALS = 86,                                        // [FFF0]
	UBLOCK_HALFWIDTH_AND_FULLWIDTH_FORMS = 87,                   // [FF00]

	// New blocks in Unicode 3.1

	UBLOCK_OLD_ITALIC = 88,                                      // [10300]
	UBLOCK_GOTHIC = 89,                                          // [10330]
	UBLOCK_DESERET = 90,                                         // [10400]
	UBLOCK_BYZANTINE_MUSICAL_SYMBOLS = 91,                       // [1D000]
	UBLOCK_MUSICAL_SYMBOLS = 92,                                 // [1D100]
	UBLOCK_MATHEMATICAL_ALPHANUMERIC_SYMBOLS = 93,               // [1D400]
	UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B = 94,              // [20000]
	UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT = 95,         // [2F800]
	UBLOCK_TAGS = 96,                                            // [E0000]

	// New blocks in Unicode 3.2

	UBLOCK_CYRILLIC_SUPPLEMENT = 97,                             // [0500]

	///
	/// Unicode 4.0.1 renames the "Cyrillic Supplementary" block to "Cyrillic Supplement".
	///
	UBLOCK_CYRILLIC_SUPPLEMENTARY = UBLOCK_CYRILLIC_SUPPLEMENT,

	UBLOCK_TAGALOG = 98,                                         // [1700]
	UBLOCK_HANUNOO = 99,                                         // [1720]
	UBLOCK_BUHID = 100,                                          // [1740]
	UBLOCK_TAGBANWA = 101,                                       // [1760]
	UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A = 102,           // [27C0]
	UBLOCK_SUPPLEMENTAL_ARROWS_A = 103,                          // [27F0]
	UBLOCK_SUPPLEMENTAL_ARROWS_B = 104,                          // [2900]
	UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B = 105,           // [2980]
	UBLOCK_SUPPLEMENTAL_MATHEMATICAL_OPERATORS = 106,            // [2A00]
	UBLOCK_KATAKANA_PHONETIC_EXTENSIONS = 107,                   // [31F0]
	UBLOCK_VARIATION_SELECTORS = 108,                            // [FE00]
	UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_A = 109,               // [F0000]
	UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_B = 110,               // [100000]

	// New blocks in Unicode 4

	UBLOCK_LIMBU = 111,                                          // [1900]
	UBLOCK_TAI_LE = 112,                                         // [1950]
	UBLOCK_KHMER_SYMBOLS = 113,                                  // [19E0]
	UBLOCK_PHONETIC_EXTENSIONS = 114,                            // [1D00]
	UBLOCK_MISCELLANEOUS_SYMBOLS_AND_ARROWS = 115,               // [2B00]
	UBLOCK_YIJING_HEXAGRAM_SYMBOLS = 116,                        // [4DC0]
	UBLOCK_LINEAR_B_SYLLABARY = 117,                             // [10000]
	UBLOCK_LINEAR_B_IDEOGRAMS = 118,                             // [10080]
	UBLOCK_AEGEAN_NUMBERS = 119,                                 // [10100]
	UBLOCK_UGARITIC = 120,                                       // [10380]
	UBLOCK_SHAVIAN = 121,                                        // [10450]
	UBLOCK_OSMANYA = 122,                                        // [10480]
	UBLOCK_CYPRIOT_SYLLABARY = 123,                              // [10800]
	UBLOCK_TAI_XUAN_JING_SYMBOLS = 124,                          // [1D300]
	UBLOCK_VARIATION_SELECTORS_SUPPLEMENT = 125,                 // [E0100]

	// New blocks in Unicode 4.1

	UBLOCK_ANCIENT_GREEK_MUSICAL_NOTATION = 126,                 // [1D200]
	UBLOCK_ANCIENT_GREEK_NUMBERS = 127,                          // [10140]
	UBLOCK_ARABIC_SUPPLEMENT = 128,                              // [0750]
	UBLOCK_BUGINESE = 129,                                       // [1A00]
	UBLOCK_CJK_STROKES = 130,                                    // [31C0]
	UBLOCK_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT = 131,         // [1DC0]
	UBLOCK_COPTIC = 132,                                         // [2C80]
	UBLOCK_ETHIOPIC_EXTENDED = 133,                              // [2D80]
	UBLOCK_ETHIOPIC_SUPPLEMENT = 134,                            // [1380]
	UBLOCK_GEORGIAN_SUPPLEMENT = 135,                            // [2D00]
	UBLOCK_GLAGOLITIC = 136,                                     // [2C00]
	UBLOCK_KHAROSHTHI = 137,                                     // [10A00]
	UBLOCK_MODIFIER_TONE_LETTERS = 138,                          // [A700]
	UBLOCK_NEW_TAI_LUE = 139,                                    // [1980]
	UBLOCK_OLD_PERSIAN = 140,                                    // [103A0]
	UBLOCK_PHONETIC_EXTENSIONS_SUPPLEMENT = 141,                 // [1D80]
	UBLOCK_SUPPLEMENTAL_PUNCTUATION = 142,                       // [2E00]
	UBLOCK_SYLOTI_NAGRI = 143,                                   // [A800]
	UBLOCK_TIFINAGH = 144,                                       // [2D30]
	UBLOCK_VERTICAL_FORMS = 145,                                 // [FE10]

	// New blocks in Unicode 5.0

	UBLOCK_NKO = 146,                                            // [07C0]
	UBLOCK_BALINESE = 147,                                       // [1B00]
	UBLOCK_LATIN_EXTENDED_C = 148,                               // [2C60]
	UBLOCK_LATIN_EXTENDED_D = 149,                               // [A720]
	UBLOCK_PHAGS_PA = 150,                                       // [A840]
	UBLOCK_PHOENICIAN = 151,                                     // [10900]
	UBLOCK_CUNEIFORM = 152,                                      // [12000]
	UBLOCK_CUNEIFORM_NUMBERS_AND_PUNCTUATION = 153,              // [12400]
	UBLOCK_COUNTING_ROD_NUMERALS = 154,                          // [1D360]

	// New blocks in Unicode 5.1

	UBLOCK_SUNDANESE = 155,                                      // [1B80]
	UBLOCK_LEPCHA = 156,                                         // [1C00]
	UBLOCK_OL_CHIKI = 157,                                       // [1C50]
	UBLOCK_CYRILLIC_EXTENDED_A = 158,                            // [2DE0]
	UBLOCK_VAI = 159,                                            // [A500]
	UBLOCK_CYRILLIC_EXTENDED_B = 160,                            // [A640]
	UBLOCK_SAURASHTRA = 161,                                     // [A880]
	UBLOCK_KAYAH_LI = 162,                                       // [A900]
	UBLOCK_REJANG = 163,                                         // [A930]
	UBLOCK_CHAM = 164,                                           // [AA00]
	UBLOCK_ANCIENT_SYMBOLS = 165,                                // [10190]
	UBLOCK_PHAISTOS_DISC = 166,                                  // [101D0]
	UBLOCK_LYCIAN = 167,                                         // [10280]
	UBLOCK_CARIAN = 168,                                         // [102A0]
	UBLOCK_LYDIAN = 169,                                         // [10920]
	UBLOCK_MAHJONG_TILES = 170,                                  // [1F000]
	UBLOCK_DOMINO_TILES = 171,                                   // [1F030]

	// New blocks in Unicode 5.2

	UBLOCK_SAMARITAN = 172,                                      // [0800]
	UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED = 173, // [18B0]
	UBLOCK_TAI_THAM = 174,                                       // [1A20]
	UBLOCK_VEDIC_EXTENSIONS = 175,                               // [1CD0]
	UBLOCK_LISU = 176,                                           // [A4D0]
	UBLOCK_BAMUM = 177,                                          // [A6A0]
	UBLOCK_COMMON_INDIC_NUMBER_FORMS = 178,                      // [A830]
	UBLOCK_DEVANAGARI_EXTENDED = 179,                            // [A8E0]
	UBLOCK_HANGUL_JAMO_EXTENDED_A = 180,                         // [A960]
	UBLOCK_JAVANESE = 181,                                       // [A980]
	UBLOCK_MYANMAR_EXTENDED_A = 182,                             // [AA60]
	UBLOCK_TAI_VIET = 183,                                       // [AA80]
	UBLOCK_MEETEI_MAYEK = 184,                                   // [ABC0]
	UBLOCK_HANGUL_JAMO_EXTENDED_B = 185,                         // [D7B0]
	UBLOCK_IMPERIAL_ARAMAIC = 186,                               // [10840]
	UBLOCK_OLD_SOUTH_ARABIAN = 187,                              // [10A60]
	UBLOCK_AVESTAN = 188,                                        // [10B00]
	UBLOCK_INSCRIPTIONAL_PARTHIAN = 189,                         // [10B40]
	UBLOCK_INSCRIPTIONAL_PAHLAVI = 190,                          // [10B60]
	UBLOCK_OLD_TURKIC = 191,                                     // [10C00]
	UBLOCK_RUMI_NUMERAL_SYMBOLS = 192,                           // [10E60]
	UBLOCK_KAITHI = 193,                                         // [11080]
	UBLOCK_EGYPTIAN_HIEROGLYPHS = 194,                           // [13000]
	UBLOCK_ENCLOSED_ALPHANUMERIC_SUPPLEMENT = 195,               // [1F100]
	UBLOCK_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT = 196,                // [1F200]
	UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C = 197,             // [2A700]

	// New blocks in Unicode 6.0

	UBLOCK_MANDAIC = 198,                                        // [0840]
	UBLOCK_BATAK = 199,                                          // [1BC0]
	UBLOCK_ETHIOPIC_EXTENDED_A = 200,                            // [AB00]
	UBLOCK_BRAHMI = 201,                                         // [11000]
	UBLOCK_BAMUM_SUPPLEMENT = 202,                               // [16800]
	UBLOCK_KANA_SUPPLEMENT = 203,                                // [1B000]
	UBLOCK_PLAYING_CARDS = 204,                                  // [1F0A0]
	UBLOCK_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS = 205,          // [1F300]
	UBLOCK_EMOTICONS = 206,                                      // [1F600]
	UBLOCK_TRANSPORT_AND_MAP_SYMBOLS = 207,                      // [1F680]
	UBLOCK_ALCHEMICAL_SYMBOLS = 208,                             // [1F700]
	UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D = 209,             // [2B740]

	/// New blocks in Unicode 6.1

	UBLOCK_ARABIC_EXTENDED_A = 210,                              // [08A0]
	UBLOCK_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS = 211,         // [1EE00]
	UBLOCK_CHAKMA = 212,                                         // [11100]
	UBLOCK_MEETEI_MAYEK_EXTENSIONS = 213,                        // [AAE0]
	UBLOCK_MEROITIC_CURSIVE = 214,                               // [109A0]
	UBLOCK_MEROITIC_HIEROGLYPHS = 215,                           // [10980]
	UBLOCK_MIAO = 216,                                           // [16F00]
	UBLOCK_SHARADA = 217,                                        // [11180]
	UBLOCK_SORA_SOMPENG = 218,                                   // [110D0]
	UBLOCK_SUNDANESE_SUPPLEMENT = 219,                           // [1CC0]
	UBLOCK_TAKRI = 220,                                          // [11680]

	/// New blocks in Unicode 7.0

	UBLOCK_BASSA_VAH = 221,                                      // [16AD0]
	UBLOCK_CAUCASIAN_ALBANIAN = 222,                             // [10530]
	UBLOCK_COPTIC_EPACT_NUMBERS = 223,                           // [102E0]
	UBLOCK_COMBINING_DIACRITICAL_MARKS_EXTENDED = 224,           // [1AB0]
	UBLOCK_DUPLOYAN = 225,                                       // [1BC00]
	UBLOCK_ELBASAN = 226,                                        // [10500]
	UBLOCK_GEOMETRIC_SHAPES_EXTENDED = 227,                      // [1F780]
	UBLOCK_GRANTHA = 228,                                        // [11300]
	UBLOCK_KHOJKI = 229,                                         // [11200]
	UBLOCK_KHUDAWADI = 230,                                      // [112B0]
	UBLOCK_LATIN_EXTENDED_E = 231,                               // [AB30]
	UBLOCK_LINEAR_A = 232,                                       // [10600]
	UBLOCK_MAHAJANI = 233,                                       // [11150]
	UBLOCK_MANICHAEAN = 234,                                     // [10AC0]
	UBLOCK_MENDE_KIKAKUI = 235,                                  // [1E800]
	UBLOCK_MODI = 236,                                           // [11600]
	UBLOCK_MRO = 237,                                            // [16A40]
	UBLOCK_MYANMAR_EXTENDED_B = 238,                             // [A9E0]
	UBLOCK_NABATAEAN = 239,                                      // [10880]
	UBLOCK_OLD_NORTH_ARABIAN = 240,                              // [10A80]
	UBLOCK_OLD_PERMIC = 241,                                     // [10350]
	UBLOCK_ORNAMENTAL_DINGBATS = 242,                            // [1F650]
	UBLOCK_PAHAWH_HMONG = 243,                                   // [16B00]
	UBLOCK_PALMYRENE = 244,                                      // [10860]
	UBLOCK_PAU_CIN_HAU = 245,                                    // [11AC0]
	UBLOCK_PSALTER_PAHLAVI = 246,                                // [10B80]
	UBLOCK_SHORTHAND_FORMAT_CONTROLS = 247,                      // [1BCA0]
	UBLOCK_SIDDHAM = 248,                                        // [11580]
	UBLOCK_SINHALA_ARCHAIC_NUMBERS = 249,                        // [111E0]
	UBLOCK_SUPPLEMENTAL_ARROWS_C = 250,                          // [1F800]
	UBLOCK_TIRHUTA = 251,                                        // [11480]
	UBLOCK_WARANG_CITI = 252,                                    // [118A0]

	/// New blocks in Unicode 8.0

	UBLOCK_AHOM = 253,                                           // [11700]
	UBLOCK_ANATOLIAN_HIEROGLYPHS = 254,                          // [14400]
	UBLOCK_CHEROKEE_SUPPLEMENT = 255,                            // [AB70]
	UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E = 256,             // [2B820]
	UBLOCK_EARLY_DYNASTIC_CUNEIFORM = 257,                       // [12480]
	UBLOCK_HATRAN = 258,                                         // [108E0]
	UBLOCK_MULTANI = 259,                                        // [11280]
	UBLOCK_OLD_HUNGARIAN = 260,                                  // [10C80]
	UBLOCK_SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS = 261,           // [1F900]
	UBLOCK_SUTTON_SIGNWRITING = 262,                             // [1D800]

	/// New blocks in Unicode 9.0

	UBLOCK_ADLAM = 263,                                          // [1E900]
	UBLOCK_BHAIKSUKI = 264,                                      // [11C00]
	UBLOCK_CYRILLIC_EXTENDED_C = 265,                            // [1C80]
	UBLOCK_GLAGOLITIC_SUPPLEMENT = 266,                          // [1E000]
	UBLOCK_IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION = 267,            // [16FE0]
	UBLOCK_MARCHEN = 268,                                        // [11C70]
	UBLOCK_MONGOLIAN_SUPPLEMENT = 269,                           // [11660]
	UBLOCK_NEWA = 270,                                           // [11400]
	UBLOCK_OSAGE = 271,                                          // [104B0]
	UBLOCK_TANGUT = 272,                                         // [17000]
	UBLOCK_TANGUT_COMPONENTS = 273,                              // [18800]

	// New blocks in Unicode 10.0

	UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F = 274,             // [2CEB0]
	UBLOCK_KANA_EXTENDED_A = 275,                                // [1B100]
	UBLOCK_MASARAM_GONDI = 276,                                  // [11D00]
	UBLOCK_NUSHU = 277,                                          // [1B170]
	UBLOCK_SOYOMBO = 278,                                        // [11A50]
	UBLOCK_SYRIAC_SUPPLEMENT = 279,                              // [0860]
	UBLOCK_ZANABAZAR_SQUARE = 280,                               // [11A00]

	///
	/// One more than the highest normal UBlockCode value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_BLOCK).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	UBLOCK_COUNT = 281,

	UBLOCK_INVALID_CODE = -1
};

///
/// East Asian Width constants.
///
/// @see UCHAR_EAST_ASIAN_WIDTH
/// @see getIntPropertyValue
///
enum class UEastAsianWidth {
	//
	// Note: UEastAsianWidth constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_EA_<Unicode East_Asian_Width value name>
	//

	U_EA_NEUTRAL,   // [N]
	U_EA_AMBIGUOUS, // [A]
	U_EA_HALFWIDTH, // [H]
	U_EA_FULLWIDTH, // [F]
	U_EA_NARROW,    // [Na]
	U_EA_WIDE,      // [W]

	///
	/// One more than the highest normal UEastAsianWidth value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_EAST_ASIAN_WIDTH).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_EA_COUNT
};

///
/// Selector constants for u_charName().
/// u_charName() returns the "modern" name of a
/// Unicode character; or the name that was defined in
/// Unicode version 1.0, before the Unicode standard merged
/// with ISO-10646; or an "extended" name that gives each
/// Unicode code point a unique name.
///
/// @see charName
///
enum class UCharNameChoice {
	///
	/// Unicode character name (Name property).
	///
	U_UNICODE_CHAR_NAME,

	///
	/// The Unicode_1_Name property value which is of little practical value.
	/// Beginning with ICU 49, ICU APIs return an empty string for this name choice.
	/// @deprecated ICU 49
	///
	U_UNICODE_10_CHAR_NAME,

	///
	/// Standard or synthetic character name.
	///
	U_EXTENDED_CHAR_NAME = U_UNICODE_CHAR_NAME + 2,

	///
	/// Corrected name from NameAliases.txt.
	///
	U_CHAR_NAME_ALIAS,

	///
	/// One more than the highest normal UCharNameChoice value.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_CHAR_NAME_CHOICE_COUNT

};

///
/// Selector constants for u_getPropertyName() and
/// u_getPropertyValueName().  These selectors are used to choose which
/// name is returned for a given property or value.  All properties and
/// values have a long name.  Most have a short name, but some do not.
/// Unicode allows for additional names, beyond the long and short
/// name, which would be indicated by U_LONG_PROPERTY_NAME + i, where
/// i=1, 2,...
///
/// @see getPropertyName()
/// @see getPropertyValueName()
///
enum class UPropertyNameChoice {
	U_SHORT_PROPERTY_NAME,
	U_LONG_PROPERTY_NAME,

	///
	/// One more than the highest normal UPropertyNameChoice value.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_PROPERTY_NAME_CHOICE_COUNT
};

///
/// Decomposition Type constants.
///
/// @see UCHAR_DECOMPOSITION_TYPE
///
enum class UDecompositionType {
	//
	// Note: UDecompositionType constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_DT_<Unicode Decomposition_Type value name>
	//

	U_DT_NONE,              // [none]
	U_DT_CANONICAL,         // [can]
	U_DT_COMPAT,            // [com]
	U_DT_CIRCLE,            // [enc]
	U_DT_FINAL,             // [fin]
	U_DT_FONT,              // [font]
	U_DT_FRACTION,          // [fra]
	U_DT_INITIAL,           // [init]
	U_DT_ISOLATED,          // [iso]
	U_DT_MEDIAL,            // [med]
	U_DT_NARROW,            // [nar]
	U_DT_NOBREAK,           // [nb]
	U_DT_SMALL,             // [sml]
	U_DT_SQUARE,            // [sqr]
	U_DT_SUB,               // [sub]
	U_DT_SUPER,             // [sup]
	U_DT_VERTICAL,          // [vert]
	U_DT_WIDE,              // [wide]

	///
	/// One more than the highest normal UDecompositionType value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_DECOMPOSITION_TYPE).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_DT_COUNT // 18
};

///
/// Joining Type constants.
///
/// @see UCHAR_JOINING_TYPE
///
enum class UJoiningType {
	//
	// Note: UJoiningType constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_JT_<Unicode Joining_Type value name>
	//

	U_JT_NON_JOINING,       // [U]
	U_JT_JOIN_CAUSING,      // [C]
	U_JT_DUAL_JOINING,      // [D]
	U_JT_LEFT_JOINING,      // [L]
	U_JT_RIGHT_JOINING,     // [R]
	U_JT_TRANSPARENT,       // [T]

	///
	/// One more than the highest normal UJoiningType value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_JOINING_TYPE).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_JT_COUNT // 6
};

///
/// Joining Group constants.
///
/// @see UCHAR_JOINING_GROUP
///
enum class UJoiningGroup {
	//
	// Note: UJoiningGroup constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_JG_<Unicode Joining_Group value name>
	//

	U_JG_NO_JOINING_GROUP,
	U_JG_AIN,
	U_JG_ALAPH,
	U_JG_ALEF,
	U_JG_BEH,
	U_JG_BETH,
	U_JG_DAL,
	U_JG_DALATH_RISH,
	U_JG_E,
	U_JG_FEH,
	U_JG_FINAL_SEMKATH,
	U_JG_GAF,
	U_JG_GAMAL,
	U_JG_HAH,
	U_JG_TEH_MARBUTA_GOAL,
	U_JG_HAMZA_ON_HEH_GOAL = U_JG_TEH_MARBUTA_GOAL,
	U_JG_HE,
	U_JG_HEH,
	U_JG_HEH_GOAL,
	U_JG_HETH,
	U_JG_KAF,
	U_JG_KAPH,
	U_JG_KNOTTED_HEH,
	U_JG_LAM,
	U_JG_LAMADH,
	U_JG_MEEM,
	U_JG_MIM,
	U_JG_NOON,
	U_JG_NUN,
	U_JG_PE,
	U_JG_QAF,
	U_JG_QAPH,
	U_JG_REH,
	U_JG_REVERSED_PE,
	U_JG_SAD,
	U_JG_SADHE,
	U_JG_SEEN,
	U_JG_SEMKATH,
	U_JG_SHIN,
	U_JG_SWASH_KAF,
	U_JG_SYRIAC_WAW,
	U_JG_TAH,
	U_JG_TAW,
	U_JG_TEH_MARBUTA,
	U_JG_TETH,
	U_JG_WAW,
	U_JG_YEH,
	U_JG_YEH_BARREE,
	U_JG_YEH_WITH_TAIL,
	U_JG_YUDH,
	U_JG_YUDH_HE,
	U_JG_ZAIN,
	U_JG_FE,
	U_JG_KHAPH,
	U_JG_ZHAIN,
	U_JG_BURUSHASKI_YEH_BARREE,
	U_JG_FARSI_YEH,
	U_JG_NYA,
	U_JG_ROHINGYA_YEH,
	U_JG_MANICHAEAN_ALEPH,
	U_JG_MANICHAEAN_AYIN,
	U_JG_MANICHAEAN_BETH,
	U_JG_MANICHAEAN_DALETH,
	U_JG_MANICHAEAN_DHAMEDH,
	U_JG_MANICHAEAN_FIVE,
	U_JG_MANICHAEAN_GIMEL,
	U_JG_MANICHAEAN_HETH,
	U_JG_MANICHAEAN_HUNDRED,
	U_JG_MANICHAEAN_KAPH,
	U_JG_MANICHAEAN_LAMEDH,
	U_JG_MANICHAEAN_MEM,
	U_JG_MANICHAEAN_NUN,
	U_JG_MANICHAEAN_ONE,
	U_JG_MANICHAEAN_PE,
	U_JG_MANICHAEAN_QOPH,
	U_JG_MANICHAEAN_RESH,
	U_JG_MANICHAEAN_SADHE,
	U_JG_MANICHAEAN_SAMEKH,
	U_JG_MANICHAEAN_TAW,
	U_JG_MANICHAEAN_TEN,
	U_JG_MANICHAEAN_TETH,
	U_JG_MANICHAEAN_THAMEDH,
	U_JG_MANICHAEAN_TWENTY,
	U_JG_MANICHAEAN_WAW,
	U_JG_MANICHAEAN_YODH,
	U_JG_MANICHAEAN_ZAYIN,
	U_JG_STRAIGHT_WAW,
	U_JG_AFRICAN_FEH,
	U_JG_AFRICAN_NOON,
	U_JG_AFRICAN_QAF,

	U_JG_MALAYALAM_BHA,
	U_JG_MALAYALAM_JA,
	U_JG_MALAYALAM_LLA,
	U_JG_MALAYALAM_LLLA,
	U_JG_MALAYALAM_NGA,
	U_JG_MALAYALAM_NNA,
	U_JG_MALAYALAM_NNNA,
	U_JG_MALAYALAM_NYA,
	U_JG_MALAYALAM_RA,
	U_JG_MALAYALAM_SSA,
	U_JG_MALAYALAM_TTA,

	///
	/// One more than the highest normal UJoiningGroup value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_JOINING_GROUP).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_JG_COUNT
};

///
/// Grapheme Cluster Break constants.
///
/// @see UCHAR_GRAPHEME_CLUSTER_BREAK
///
enum class UGraphemeClusterBreak {
	//
	// Note: UGraphemeClusterBreak constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_GCB_<Unicode Grapheme_Cluster_Break value name>
	//

	U_GCB_OTHER = 0,                // [XX]
	U_GCB_CONTROL = 1,              // [CN]
	U_GCB_CR = 2,                   // [CR]
	U_GCB_EXTEND = 3,               // [EX]
	U_GCB_L = 4,                    // [L]
	U_GCB_LF = 5,                   // [LF]
	U_GCB_LV = 6,                   // [LV]
	U_GCB_LVT = 7,                  // [LVT]
	U_GCB_T = 8,                    // [T]
	U_GCB_V = 9,                    // [V]
	U_GCB_SPACING_MARK = 10,        // [SM] from here on: new in Unicode 5.1/ICU 4.0
	U_GCB_PREPEND = 11,             // [PP]
	U_GCB_REGIONAL_INDICATOR = 12,  // [RI] new in Unicode 6.2/ICU 50
	U_GCB_E_BASE = 13,              // [EB] from here on: new in Unicode 9.0/ICU 58
	U_GCB_E_BASE_GAZ = 14,          // [EBG]
	U_GCB_E_MODIFIER = 15,          // [EM]
	U_GCB_GLUE_AFTER_ZWJ = 16,      // [GAZ]
	U_GCB_ZWJ = 17,                 // [ZWJ]

	///
	/// One more than the highest normal UGraphemeClusterBreak value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_GRAPHEME_CLUSTER_BREAK).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_GCB_COUNT = 18
};

///
/// Word Break constants.
/// (UWordBreak is a pre-existing enum type in ubrk.h for word break status tags.)
///
/// @see UCHAR_WORD_BREAK
///
enum class UWordBreakValues {
	//
	// Note: UWordBreakValues constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_WB_<Unicode Word_Break value name>
	//

	U_WB_OTHER = 0,                 // [XX]
	U_WB_ALETTER = 1,               // [LE]
	U_WB_FORMAT = 2,                // [FO]
	U_WB_KATAKANA = 3,              // [KA]
	U_WB_MIDLETTER = 4,             // [ML]
	U_WB_MIDNUM = 5,                // [MN]
	U_WB_NUMERIC = 6,               // [NU]
	U_WB_EXTENDNUMLET = 7,          // [EX]
	U_WB_CR = 8,                    // [CR] from here on: new in Unicode 5.1/ICU 4.0
	U_WB_EXTEND = 9,                // [Extend]
	U_WB_LF = 10,                   // [LF]
	U_WB_MIDNUMLET = 11,            // [MB]
	U_WB_NEWLINE = 12,              // [NL]
	U_WB_REGIONAL_INDICATOR = 13,   // [RI] new in Unicode 6.2/ICU 50
	U_WB_HEBREW_LETTER = 14,        // [HL] from here on: new in Unicode 6.3/ICU 52
	U_WB_SINGLE_QUOTE = 15,         // [SQ]
	U_WB_DOUBLE_QUOTE = 16,         // [DQ]
	U_WB_E_BASE = 17,               // [EB] from here on: new in Unicode 9.0/ICU 58
	U_WB_E_BASE_GAZ = 18,           // [EBG]
	U_WB_E_MODIFIER = 19,           // [EM]
	U_WB_GLUE_AFTER_ZWJ = 20,       // [GAZ]
	U_WB_ZWJ = 21,                  // [ZWJ]

	///
	/// One more than the highest normal UWordBreakValues value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_WORD_BREAK).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_WB_COUNT = 22
};

///
/// Sentence Break constants.
///
/// @see UCHAR_SENTENCE_BREAK
///
enum class USentenceBreak {
	//
	// Note: USentenceBreak constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_SB_<Unicode Sentence_Break value name>
	//

	U_SB_OTHER = 0,             // [XX]
	U_SB_ATERM = 1,             // [AT]
	U_SB_CLOSE = 2,             // [CL]
	U_SB_FORMAT = 3,            // [FO]
	U_SB_LOWER = 4,             // [LO]
	U_SB_NUMERIC = 5,           // [NU]
	U_SB_OLETTER = 6,           // [LE]
	U_SB_SEP = 7,               // [SE]
	U_SB_SP = 8,                // [SP]
	U_SB_STERM = 9,             // [ST]
	U_SB_UPPER = 10,            // [UP]
	U_SB_CR = 11,               // [CR] from here on: new in Unicode 5.1/ICU 4.0
	U_SB_EXTEND = 12,           // [EX]
	U_SB_LF = 13,               // [LF]
	U_SB_SCONTINUE = 14,        // [SC]

	///
	/// One more than the highest normal USentenceBreak value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_SENTENCE_BREAK).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_SB_COUNT = 15
};

///
/// Line Break constants.
///
/// @see UCHAR_LINE_BREAK
///
enum class ULineBreak {
	//
	// Note: ULineBreak constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_LB_<Unicode Line_Break value name>
	//

	U_LB_UNKNOWN = 0,                        // [XX]
	U_LB_AMBIGUOUS = 1,                      // [AI]
	U_LB_ALPHABETIC = 2,                     // [AL]
	U_LB_BREAK_BOTH = 3,                     // [B2]
	U_LB_BREAK_AFTER = 4,                    // [BA]
	U_LB_BREAK_BEFORE = 5,                   // [BB]
	U_LB_MANDATORY_BREAK = 6,                // [BK]
	U_LB_CONTINGENT_BREAK = 7,               // [CB]
	U_LB_CLOSE_PUNCTUATION = 8,              // [CL]
	U_LB_COMBINING_MARK = 9,                 // [CM]
	U_LB_CARRIAGE_RETURN = 10,               // [CR]
	U_LB_EXCLAMATION = 11,                   // [EX]
	U_LB_GLUE = 12,                          // [GL]
	U_LB_HYPHEN = 13,                        // [HY]
	U_LB_IDEOGRAPHIC = 14,                   // [ID]

	///
	/// Renamed from the misspelled "inseperable" in Unicode 4.0.1/ICU 3.0
	///
	U_LB_INSEPARABLE = 15,                   // [IN]
	U_LB_INSEPERABLE = U_LB_INSEPARABLE,
	U_LB_INFIX_NUMERIC = 16,                 // [IS]
	U_LB_LINE_FEED = 17,                     // [LF]
	U_LB_NONSTARTER = 18,                    // [NS]
	U_LB_NUMERIC = 19,                       // [NU]
	U_LB_OPEN_PUNCTUATION = 20,              // [OP]
	U_LB_POSTFIX_NUMERIC = 21,               // [PO]
	U_LB_PREFIX_NUMERIC = 22,                // [PR]
	U_LB_QUOTATION = 23,                     // [QU]
	U_LB_COMPLEX_CONTEXT = 24,               // [SA]
	U_LB_SURROGATE = 25,                     // [SG]
	U_LB_SPACE = 26,                         // [SP]
	U_LB_BREAK_SYMBOLS = 27,                 // [SY]
	U_LB_ZWSPACE = 28,                       // [ZW]
	U_LB_NEXT_LINE = 29,                     // [NL] from here on: new in Unicode 4/ICU 2.6
	U_LB_WORD_JOINER = 30,                   // [WJ]
	U_LB_H2 = 31,                            // [H2] from here on: new in Unicode 4.1/ICU 3.4
	U_LB_H3 = 32,                            // [H3]
	U_LB_JL = 33,                            // [JL]
	U_LB_JT = 34,                            // [JT]
	U_LB_JV = 35,                            // [JV]
	U_LB_CLOSE_PARENTHESIS = 36,             // [CP] new in Unicode 5.2/ICU 4.4
	U_LB_CONDITIONAL_JAPANESE_STARTER = 37,  // [CJ] new in Unicode 6.1/ICU 49
	U_LB_HEBREW_LETTER = 38,                 // [HL] new in Unicode 6.1/ICU 49
	U_LB_REGIONAL_INDICATOR = 39,            // [RI] new in Unicode 6.2/ICU 50
	U_LB_E_BASE = 40,                        // [EB] from here on: new in Unicode 9.0/ICU 58
	U_LB_E_MODIFIER = 41,                    // [EM]
	U_LB_ZWJ = 42,                           // [ZWJ]

	///
	/// One more than the highest normal ULineBreak value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_LINE_BREAK).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_LB_COUNT = 43
};

///
/// Numeric Type constants.
///
/// @see UCHAR_NUMERIC_TYPE
///
enum class UNumericType {
	//
	// Note: UNumericType constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_NT_<Unicode Numeric_Type value name>
	//

	U_NT_NONE,              // [None]
	U_NT_DECIMAL,           // [de]
	U_NT_DIGIT,             // [di]
	U_NT_NUMERIC,           // [nu]

	///
	/// One more than the highest normal UNumericType value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_NUMERIC_TYPE).
	///
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_NT_COUNT
};

///
/// Hangul Syllable Type constants.
///
/// @see UCHAR_HANGUL_SYLLABLE_TYPE
///
enum class UHangulSyllableType {
	//
	// Note: UHangulSyllableType constants are parsed by preparseucd.py.
	// It matches lines like
	//     U_HST_<Unicode Hangul_Syllable_Type value name>
	//

	U_HST_NOT_APPLICABLE,   // [NA]
	U_HST_LEADING_JAMO,     // [L]
	U_HST_VOWEL_JAMO,       // [V]
	U_HST_TRAILING_JAMO,    // [T]
	U_HST_LV_SYLLABLE,      // [LV]
	U_HST_LVT_SYLLABLE,     // [LVT]

	///
	/// One more than the highest normal UHangulSyllableType value.
	/// The highest value is available via u_getIntPropertyMaxValue(UCHAR_HANGUL_SYLLABLE_TYPE).
	///
	///
	U_HST_COUNT
};

///
/// Check if a code point has the Alphabetic Unicode property.
/// Same as u_hasBinaryProperty(c, UProperty::UCHAR_ALPHABETIC).
/// This is different from u_isalpha!
/// @param c Code point to test
/// @return true if the code point has the Alphabetic Unicode property, false otherwise
///
/// @see UProperty::UCHAR_ALPHABETIC
/// @see u_isalpha
/// @see u_hasBinaryProperty
///
bool isUAlphabetic(char32_t c);

///
/// Check if a code point has the White_Space Unicode property.
/// Same as u_hasBinaryProperty(c, UCHAR_WHITE_SPACE).
/// This is different from both u_isspace and u_isWhitespace!
///
/// Note: There are several ICU whitespace functions; please see the uchar.h
/// file documentation for a detailed comparison.
///
/// @param c Code point to test
/// @return true if the code point has the White_Space Unicode property, false otherwise.
///
/// @see UCHAR_WHITE_SPACE
/// @see u_isWhitespace
/// @see u_isspace
/// @see u_isJavaSpaceChar
/// @see u_hasBinaryProperty
///
bool u_isUWhiteSpace(char32_t c);

///
/// Get the numeric value for a Unicode code point as defined in the
/// Unicode Character Database.
///
/// A "double" return type is necessary because
/// some numeric values are fractions, negative, or too large for int32_t.
///
/// For characters without any numeric values in the Unicode Character Database,
/// this function will return U_NO_NUMERIC_VALUE.
/// Note: This is different from the Unicode Standard which specifies NaN as the default value.
/// (NaN is not available on all platforms.)
///
/// Similar to java.lang.Character.getNumericValue(), but u_getNumericValue()
/// also supports negative values, large values, and fractions,
/// while Java's getNumericValue() returns values 10..35 for ASCII letters.
///
/// @param c Code point to get the numeric value for.
/// @return Numeric value of c, or U_NO_NUMERIC_VALUE if none is defined.
///
/// @see U_NO_NUMERIC_VALUE
///
double u_getNumericValue(char32_t c);

///
/// Special value that is returned by u_getNumericValue when
/// no numeric value is defined for a code point.
///
/// @see u_getNumericValue
///
constexpr double U_NO_NUMERIC_VALUE = -123456789.0;

///
/// Determines whether the specified code point has the general category "Ll"
/// (lowercase letter).
///
/// Same as java.lang.Character.isLowerCase().
///
/// This misses some characters that are also lowercase but
/// have a different general category value.
/// In order to include those, use UCHAR_LOWERCASE.
///
/// In addition to being equivalent to a Java function, this also serves
/// as a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is an Ll lowercase letter
///
/// @see UCHAR_LOWERCASE
/// @see u_isupper
/// @see u_istitle
///
bool isLower(char32_t c);

///
/// Determines whether the specified code point has the general category "Lu"
/// (uppercase letter).
///
/// Same as java.lang.Character.isUpperCase().
///
/// This misses some characters that are also uppercase but
/// have a different general category value.
/// In order to include those, use UCHAR_UPPERCASE.
///
/// In addition to being equivalent to a Java function, this also serves
/// as a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is an Lu uppercase letter
///
/// @see UCHAR_UPPERCASE
/// @see u_islower
/// @see u_istitle
/// @see toLower
///
bool isUpper(char32_t c);

///
/// Determines whether the specified code point is a titlecase letter.
/// True for general category "Lt" (titlecase letter).
///
/// Same as java.lang.Character.isTitleCase().
///
/// @param c the code point to be tested
/// @return TRUE if the code point is an Lt titlecase letter
///
/// @see u_isupper
/// @see u_islower
/// @see u_totitle
///
bool isTitle(char32_t c);

///
/// Determines whether the specified code point is a digit character according to Java.
/// True for characters with general category "Nd" (decimal digit numbers).
/// Beginning with Unicode 4, this is the same as
/// testing for the Numeric_Type of Decimal.
///
/// Same as java.lang.Character.isDigit().
///
/// In addition to being equivalent to a Java function, this also serves
/// as a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a digit character according to Character.isDigit()
///
///
bool isDigit(char32_t c);

///
/// Determines whether the specified code point is a letter character.
/// True for general categories "L" (letters).
///
/// Same as java.lang.Character.isLetter().
///
/// In addition to being equivalent to a Java function, this also serves
/// as a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a letter character
///
/// @see u_isdigit
/// @see u_isalnum
///
bool isAlpha(char32_t c);

///
/// Determines whether the specified code point is an alphanumeric character
/// (letter or digit) according to Java.
/// True for characters with general categories
/// "L" (letters) and "Nd" (decimal digit numbers).
///
/// Same as java.lang.Character.isLetterOrDigit().
///
/// In addition to being equivalent to a Java function, this also serves
/// as a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is an alphanumeric character according to Character.isLetterOrDigit()
///
///
bool isAlphaNum(char32_t c);

///
/// Determines whether the specified code point is a hexadecimal digit.
/// This is equivalent to u_digit(c, 16)>=0.
/// True for characters with general category "Nd" (decimal digit numbers)
/// as well as Latin letters a-f and A-F in both ASCII and Fullwidth ASCII.
/// (That is, for letters with code points
/// 0041..0046, 0061..0066, FF21..FF26, FF41..FF46.)
///
/// In order to narrow the definition of hexadecimal digits to only ASCII
/// characters, use (c<=0x7f && u_isxdigit(c)).
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a hexadecimal digit
///
///
bool isHexDigit(char32_t c);

///
/// Determines whether the specified code point is a punctuation character.
/// True for characters with general categories "P" (punctuation).
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a punctuation character
///
///
bool isPunctuation(char32_t c);

///
/// Determines whether the specified code point is a "graphic" character
/// (printable, excluding spaces).
/// TRUE for all characters except those with general categories
/// "Cc" (control codes), "Cf" (format controls), "Cs" (surrogates),
/// "Cn" (unassigned), and "Z" (separators).
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a "graphic" character
///
///
bool isGraph(char32_t c);

///
/// Determines whether the specified code point is a "blank" or "horizontal space",
/// a character that visibly separates words on a line.
/// The following are equivalent definitions:
///
/// TRUE for Unicode White_Space characters except for "vertical space controls"
/// where "vertical space controls" are the following characters:
/// U+000A (LF) U+000B (VT) U+000C (FF) U+000D (CR) U+0085 (NEL) U+2028 (LS) U+2029 (PS)
///
/// same as
///
/// TRUE for U+0009 (TAB) and characters with general category "Zs" (space separators)
/// except Zero Width Space (ZWSP, U+200B).
///
/// Note: There are several ICU whitespace functions; please see the uchar.h
/// file documentation for a detailed comparison.
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a "blank"
///
///
bool isBlank(char32_t c);

///
/// Determines whether the specified code point is "defined",
/// which usually means that it is assigned a character.
/// True for general categories other than "Cn" (other, not assigned),
/// i.e., true for all code points mentioned in UnicodeData.txt.
///
/// Note that non-character code points (e.g., U+FDD0) are not "defined"
/// (they are Cn), but surrogate code points are "defined" (Cs).
///
/// Same as java.lang.Character.isDefined().
///
/// @param c the code point to be tested
/// @return TRUE if the code point is assigned a character
///
/// @see u_isdigit
/// @see u_isalpha
/// @see u_isalnum
/// @see u_isupper
/// @see u_islower
/// @see u_istitle
///
bool isDefined(char32_t c);

///
/// Determines if the specified character is a space character or not.
///
/// Note: There are several ICU whitespace functions; please see the uchar.h
/// file documentation for a detailed comparison.
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c    the character to be tested
/// @return  true if the character is a space character; false otherwise.
///
/// @see u_isJavaSpaceChar
/// @see u_isWhitespace
/// @see u_isUWhiteSpace
///
bool isSpace(char32_t c);

///
/// Determine if the specified code point is a space character according to Java.
/// True for characters with general categories "Z" (separators),
/// which does not include control codes (e.g., TAB or Line Feed).
///
/// Same as java.lang.Character.isSpaceChar().
///
/// Note: There are several ICU whitespace functions; please see the uchar.h
/// file documentation for a detailed comparison.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a space character according to Character.isSpaceChar()
///
/// @see u_isspace
/// @see u_isWhitespace
/// @see u_isUWhiteSpace
///
bool isJavaSpaceChar(char32_t c);

///
/// Determines if the specified code point is a whitespace character according to Java/ICU.
/// A character is considered to be a Java whitespace character if and only
/// if it satisfies one of the following criteria:
///
/// - It is a Unicode Separator character (categories "Z" = "Zs" or "Zl" or "Zp"), but is not
///      also a non-breaking space (U+00A0 NBSP or U+2007 Figure Space or U+202F Narrow NBSP).
/// - It is U+0009 HORIZONTAL TABULATION.
/// - It is U+000A LINE FEED.
/// - It is U+000B VERTICAL TABULATION.
/// - It is U+000C FORM FEED.
/// - It is U+000D CARRIAGE RETURN.
/// - It is U+001C FILE SEPARATOR.
/// - It is U+001D GROUP SEPARATOR.
/// - It is U+001E RECORD SEPARATOR.
/// - It is U+001F UNIT SEPARATOR.
///
/// This API tries to sync with the semantics of Java's
/// java.lang.Character.isWhitespace(), but it may not return
/// the exact same results because of the Unicode version
/// difference.
///
/// Note: Unicode 4.0.1 changed U+200B ZERO WIDTH SPACE from a Space Separator (Zs)
/// to a Format Control (Cf). Since then, isWhitespace(0x200b) returns false.
/// See http://www.unicode.org/versions/Unicode4.0.1/
///
/// Note: There are several ICU whitespace functions; please see the uchar.h
/// file documentation for a detailed comparison.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a whitespace character according to Java/ICU
///
/// @see u_isspace
/// @see u_isJavaSpaceChar
/// @see u_isUWhiteSpace
///
bool isWhitespace(char32_t c);

///
/// Determines whether the specified code point is a control character
/// (as defined by this function).
/// A control character is one of the following:
/// - ISO 8-bit control character (U+0000..U+001f and U+007f..U+009f)
/// - U_CONTROL_CHAR (Cc)
/// - U_FORMAT_CHAR (Cf)
/// - U_LINE_SEPARATOR (Zl)
/// - U_PARAGRAPH_SEPARATOR (Zp)
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a control character
///
/// @see UCHAR_DEFAULT_IGNORABLE_CODE_POINT
/// @see u_isprint
///
bool isControlCharacter(char32_t c);

///
/// Determines whether the specified code point is an ISO control code.
/// True for U+0000..U+001f and U+007f..U+009f (general category "Cc").
///
/// Same as java.lang.Character.isISOControl().
///
/// @param c the code point to be tested
/// @return TRUE if the code point is an ISO control code
///
/// @see u_iscntrl
///
bool isISOControlCode(char32_t c);

///
/// Determines whether the specified code point is a printable character.
/// True for general categories <em>other</em> than "C" (controls).
///
/// This is a C/POSIX migration function.
/// See the comments about C/POSIX character classification functions in the
/// documentation at the top of this header file.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a printable character
///
/// @see UCHAR_DEFAULT_IGNORABLE_CODE_POINT
/// @see u_iscntrl
///
bool isPrintable(char32_t c);

///
/// Determines whether the specified code point is a base character.
/// True for general categories "L" (letters), "N" (numbers),
/// "Mc" (spacing combining marks), and "Me" (enclosing marks).
///
/// Note that this is different from the Unicode definition in
/// chapter 3.5, conformance clause D13,
/// which defines base characters to be all characters (not Cn)
/// that do not graphically combine with preceding characters (M)
/// and that are neither control (Cc) or format (Cf) characters.
///
/// @param c the code point to be tested
/// @return TRUE if the code point is a base character according to this function
///
/// @see u_isalpha
/// @see u_isdigit
///
bool isBase(char32_t c);

///
/// Returns the general category value for the code point.
///
/// Same as java.lang.Character.getType().
///
/// @param c the code point to be tested
/// @return the general category (UCharCategory) value
///
/// @see UCharCategory
///
int8_t getCategory(char32_t c);

///
/// Callback from u_enumCharTypes(), is called for each contiguous range
/// of code points c (where start<=c<limit)
/// with the same Unicode general category ("character type").
///
/// The callback function can stop the enumeration by returning false.
///
/// @param context an opaque pointer, as passed into utrie_enum()
/// @param start the first code point in a contiguous range with value
/// @param limit one past the last code point in a contiguous range with value
/// @param type the general category for all code points in [start..limit[
/// @return false to stop the enumeration
///
/// @see UCharCategory
/// @see u_enumCharTypes
///
typedef bool (* UCharEnumTypeRange)(const void * context, char32_t start, char32_t limit, UCharCategory type);

///
/// Enumerate efficiently all code points with their Unicode general categories.
///
/// This is useful for building data structures (e.g., UnicodeSet's),
/// for enumerating all assigned code points (type!=U_UNASSIGNED), etc.
///
/// For each contiguous range of code points with a given general category ("character type"),
/// the UCharEnumTypeRange function is called.
/// Adjacent ranges have different types.
/// The Unicode Standard guarantees that the numeric value of the type is 0..31.
///
/// @param enumRange a pointer to a function that is called for each contiguous range
///                  of code points with the same general category
/// @param context an opaque pointer that is passed on to the callback function
///
/// @see UCharCategory
/// @see UCharEnumTypeRange
///
void enumCharTypes(UCharEnumTypeRange enumRange, const void * context);

///
/// Returns the decimal digit value of a decimal digit character.
/// Such characters have the general category "Nd" (decimal digit numbers)
/// and a Numeric_Type of Decimal.
///
/// Unlike ICU releases before 2.6, no digit values are returned for any
/// Han characters because Han number characters are often used with a special
/// Chinese-style number format (with characters for powers of 10 in between)
/// instead of in decimal-positional notation.
/// Unicode 4 explicitly assigns Han number characters the Numeric_Type
/// Numeric instead of Decimal.
/// See Jitterbug 1483 for more details.
///
/// Use u_getIntPropertyValue(c, UCHAR_NUMERIC_TYPE) and u_getNumericValue()
/// for complete numeric Unicode properties.
///
/// @param c the code point for which to get the decimal digit value
/// @return the decimal digit value of c,
///         or -1 if c is not a decimal digit character
///
/// @see u_getNumericValue
///
int32_t charDigitValue(char32_t c);

///
/// Returns the Unicode allocation block that contains the character.
///
/// @param c the code point to be tested
/// @return the block value (UBlockCode) for c
///
/// @see UBlockCode
///
UBlockCode getAllocationBlockCode(char32_t c);

///
/// Determines if the specified character is permissible as the
/// first character in an identifier according to Unicode
/// (The Unicode Standard, Version 3.0, chapter 5.16 Identifiers).
/// True for characters with general categories "L" (letters) and "Nl" (letter numbers).
///
/// Same as java.lang.Character.isUnicodeIdentifierStart().
/// Same as UCHAR_ID_START
///
/// @param c the code point to be tested
/// @return TRUE if the code point may start an identifier
///
/// @see UCHAR_ID_START
/// @see u_isalpha
/// @see u_isIDPart
///
bool isIDStart(char32_t c);

///
/// Determines if the specified character is permissible
/// in an identifier according to Java.
/// True for characters with general categories "L" (letters),
/// "Nl" (letter numbers), "Nd" (decimal digits),
/// "Mc" and "Mn" (combining marks), "Pc" (connecting punctuation), and
/// u_isIDIgnorable(c).
///
/// Same as java.lang.Character.isUnicodeIdentifierPart().
/// Almost the same as Unicode's ID_Continue (UCHAR_ID_CONTINUE)
/// except that Unicode recommends to ignore Cf which is less than
/// u_isIDIgnorable(c).
///
/// @param c the code point to be tested
/// @return TRUE if the code point may occur in an identifier according to Java
///
/// @see UCHAR_ID_CONTINUE
/// @see u_isIDStart
/// @see u_isIDIgnorable
///
bool isIDPart(char32_t c);

///
/// Determines if the specified character should be regarded
/// as an ignorable character in an identifier,
/// according to Java.
/// True for characters with general category "Cf" (format controls) as well as
/// non-whitespace ISO controls
/// (U+0000..U+0008, U+000E..U+001B, U+007F..U+009F).
///
/// Same as java.lang.Character.isIdentifierIgnorable().
///
/// Note that Unicode just recommends to ignore Cf (format controls).
///
/// @param c the code point to be tested
/// @return TRUE if the code point is ignorable in identifiers according to Java
///
/// @see UCHAR_DEFAULT_IGNORABLE_CODE_POINT
/// @see u_isIDStart
/// @see u_isIDPart
///
bool isIDIgnorable(char32_t c);

///
/// Determines if the specified character is permissible as the
/// first character in a Java identifier.
/// In addition to u_isIDStart(c), true for characters with
/// general categories "Sc" (currency symbols) and "Pc" (connecting punctuation).
///
/// Same as java.lang.Character.isJavaIdentifierStart().
///
/// @param c the code point to be tested
/// @return TRUE if the code point may start a Java identifier
///
/// @see     u_isJavaIDPart
/// @see     u_isalpha
/// @see     u_isIDStart
///
bool isJavaIDStart(char32_t c);

///
/// Determines if the specified character is permissible
/// in a Java identifier.
/// In addition to u_isIDPart(c), true for characters with
/// general category "Sc" (currency symbols).
///
/// Same as java.lang.Character.isJavaIdentifierPart().
///
/// @param c the code point to be tested
/// @return TRUE if the code point may occur in a Java identifier
///
/// @see     u_isIDIgnorable
/// @see     u_isJavaIDStart
/// @see     u_isalpha
/// @see     u_isdigit
/// @see     u_isIDPart
////
bool isJavaIDPart(char32_t c);

///
/// The given character is mapped to its titlecase equivalent
/// according to UnicodeData.txt;
/// if none is defined, the character itself is returned.
///
/// Same as java.lang.Character.toTitleCase().
///
/// This function only returns the simple, single-code point case mapping.
/// Full case mappings should be used whenever possible because they produce
/// better results by working on whole strings.
/// They take into account the string context and the language and can map
/// to a result string with a different length as appropriate.
/// Full case mappings are applied by the string case mapping functions,
/// see ustring.h and the UnicodeString class.
/// See also the User Guide chapter on C/POSIX migration:
/// http://icu-project.org/userguide/posix.html#case_mappings
///
/// @param c the code point to be mapped
/// @return the Simple_Titlecase_Mapping of the code point, if any;
///         otherwise the code point itself.
///
char32_t toTitle(char32_t c);

///
/// The given character is mapped to its case folding equivalent according to
/// UnicodeData.txt and CaseFolding.txt;
/// if the character has no case folding equivalent, the character
/// itself is returned.
///
/// This function only returns the simple, single-code point case mapping.
/// Full case mappings should be used whenever possible because they produce
/// better results by working on whole strings.
/// They take into account the string context and the language and can map
/// to a result string with a different length as appropriate.
/// Full case mappings are applied by the string case mapping functions,
/// see ustring.h and the UnicodeString class.
/// See also the User Guide chapter on C/POSIX migration:
/// http://icu-project.org/userguide/posix.html#case_mappings
///
/// @param c the code point to be mapped
/// @param options Either FoldCaseDefault or U_FOLD_CASE_EXCLUDE_SPECIAL_I
/// @return the Simple_Case_Folding of the code point, if any;
///         otherwise the code point itself.
///
char32_t foldCase(char32_t c, uint32_t options);

///
/// Returns the decimal digit value of the code point in the
/// specified radix.
///
/// If the radix is not in the range <code>2<=radix<=36</code> or if the
/// value of <code>c</code> is not a valid digit in the specified
/// radix, <code>-1</code> is returned. A character is a valid digit
/// if at least one of the following is true:
/// <ul>
/// <li>The character has a decimal digit value.
///     Such characters have the general category "Nd" (decimal digit numbers)
///     and a Numeric_Type of Decimal.
///     In this case the value is the character's decimal digit value.</li>
/// <li>The character is one of the uppercase Latin letters
///     <code>'A'</code> through <code>'Z'</code>.
///     In this case the value is <code>c-'A'+10</code>.</li>
/// <li>The character is one of the lowercase Latin letters
///     <code>'a'</code> through <code>'z'</code>.
///     In this case the value is <code>ch-'a'+10</code>.</li>
/// <li>Latin letters from both the ASCII range (0061..007A, 0041..005A)
///     as well as from the Fullwidth ASCII range (FF41..FF5A, FF21..FF3A)
///     are recognized.</li>
/// </ul>
///
/// Same as java.lang.Character.digit().
///
/// @param   ch      the code point to be tested.
/// @param   radix   the radix.
/// @return  the numeric value represented by the character in the
///          specified radix,
///          or -1 if there is no value or if the value exceeds the radix.
///
/// @see     UCHAR_NUMERIC_TYPE
/// @see     u_forDigit
/// @see     u_charDigitValue
/// @see     u_isdigit
///
int32_t getDigit(char32_t ch, int8_t radix);

///
/// Determines the character representation for a specific digit in
/// the specified radix. If the value of <code>radix</code> is not a
/// valid radix, or the value of <code>digit</code> is not a valid
/// digit in the specified radix, the null character
/// (<code>U+0000</code>) is returned.
/// <p>
/// The <code>radix</code> argument is valid if it is greater than or
/// equal to 2 and less than or equal to 36.
/// The <code>digit</code> argument is valid if
/// <code>0 <= digit < radix</code>.
/// <p>
/// If the digit is less than 10, then
/// <code>'0' + digit</code> is returned. Otherwise, the value
/// <code>'a' + digit - 10</code> is returned.
///
/// Same as java.lang.Character.forDigit().
///
/// @param   digit   the number to convert to a character.
/// @param   radix   the radix.
/// @return  the <code>char</code> representation of the specified digit
///          in the specified radix.
///
/// @see     u_digit
/// @see     u_charDigitValue
/// @see     u_isdigit
///
char32_t forDigit(int32_t digit, int8_t radix);

typedef uint8_t UVersionInfo[4];

///
/// Get the "age" of the code point.
/// The "age" is the Unicode version when the code point was first
/// designated (as a non-character or for Private Use)
/// or assigned a character.
/// This can be useful to avoid emitting code points to receiving
/// processes that do not accept newer characters.
/// The data is from the UCD file DerivedAge.txt.
///
/// @param c The code point.
/// @param versionArray The Unicode version number array, to be filled in.
///
///
void charAge(char32_t c, UVersionInfo & versionArray);

///
/// Gets the Unicode version information.
/// The version array is filled in with the version information
/// for the Unicode standard that is currently used by ICU.
/// For example, Unicode version 3.1.1 is represented as an array with
/// the values { 3, 1, 1, 0 }.
///
/// @param versionArray an output array that will be filled in with
///                     the Unicode version number
///
void getUnicodeVersion(UVersionInfo versionArray);

} // namespace Balau::Unicode

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_CHAR
