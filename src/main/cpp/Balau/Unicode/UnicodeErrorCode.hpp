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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_ERROR_CODE
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_ERROR_CODE

#include <limits>
#include <cstdint>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Unicode {

///
/// Error code to replace exception handling, so that the code is compatible with all C++ compilers,
/// and to use the same mechanism for C and C++.
///
/// ICU functions that take a reference (C++) or a pointer (C) to a UErrorCode
/// first test if(isFailure(errorCode)) { return immediately; }
/// so that in a chain of such functions the first one that sets an error code
/// causes the following ones to not perform any operations.
///
/// Error codes should be tested using isFailure() and isSuccess().
///
enum class UErrorCode {
	//
	// The ordering of U_ERROR_INFO_START Vs U_USING_FALLBACK_WARNING looks weird
	// and is that way because VC++ debugger displays first encountered constant,
	// which is not the what the code is used for
	//

	///
	/// A resource bundle lookup returned a fallback result (not an error)
	///
	U_USING_FALLBACK_WARNING = -128,

	///
	/// Start of information results (semantically successful)
	///
	U_ERROR_WARNING_START = -128,

	///
	/// A resource bundle lookup returned a result from the root locale (not an error)
	///
	U_USING_DEFAULT_WARNING = -127,

	///
	/// A SafeClone operation required allocating memory (informational only)
	///
	U_SAFECLONE_ALLOCATED_WARNING = -126,

	///
	/// ICU has to use compatibility layer to construct the service.
	/// Expect performance/memory usage degradation. Consider upgrading
	///
	U_STATE_OLD_WARNING = -125,

	///
	/// An output string could not be NUL-terminated because output length==destCapacity.
	///
	U_STRING_NOT_TERMINATED_WARNING = -124,

	///
	/// Number of levels requested in getBound is higher than the number of levels in the sort key
	///
	U_SORT_KEY_TOO_SHORT_WARNING = -123,

	///
	/// This converter alias can go to different converter implementations
	///
	U_AMBIGUOUS_ALIAS_WARNING = -122,

	///
	/// ucol_open encountered a mismatch between UCA version and collator image version,
	/// so the collator was constructed from rules.
	///
	/// No impact to further function.
	///
	U_DIFFERENT_UCA_VERSION = -121,

	///
	/// A plugin caused a level change.
	/// May not be an error, but later plugins may not load.
	///
	U_PLUGIN_CHANGED_LEVEL_WARNING = -120,

	///
	/// One more than the highest normal UErrorCode warning value.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_ERROR_WARNING_LIMIT,

	///
	/// No error, no warning.
	///
	U_ZERO_ERROR = 0,

	///
	/// Start of codes indicating failure.
	///
	U_ILLEGAL_ARGUMENT_ERROR = 1,

	///
	/// The requested resource cannot be found.
	///
	U_MISSING_RESOURCE_ERROR = 2,

	///
	/// Data format is not what is expected.
	///
	U_INVALID_FORMAT_ERROR = 3,

	///
	/// The requested file cannot be found.
	///
	U_FILE_ACCESS_ERROR = 4,

	///
	/// Indicates a bug in the library code.
	///
	U_INTERNAL_PROGRAM_ERROR = 5,

	///
	/// Unable to parse a message (message format).
	///
	U_MESSAGE_PARSE_ERROR = 6,

	///
	/// Memory allocation error
	///
	U_MEMORY_ALLOCATION_ERROR = 7,

	///
	/// Trying to access the index that is out of bounds
	///
	U_INDEX_OUTOFBOUNDS_ERROR = 8,

	///
	/// Equivalent to Java ParseException
	///
	U_PARSE_ERROR = 9,

	///
	/// Character conversion: Unmappable input sequence. In other APIs: Invalid character.
	///
	U_INVALID_CHAR_FOUND = 10,

	///
	/// Character conversion: Incomplete input sequence.
	///
	U_TRUNCATED_CHAR_FOUND = 11,

	///
	/// Character conversion: Illegal input sequence/combination of input units.
	///
	U_ILLEGAL_CHAR_FOUND = 12,

	///
	/// Conversion table file found, but corrupted
	///
	U_INVALID_TABLE_FORMAT = 13,

	///
	/// Conversion table file not found
	///
	U_INVALID_TABLE_FILE = 14,

	///
	/// A result would not fit in the supplied buffer
	///
	U_BUFFER_OVERFLOW_ERROR = 15,

	///
	/// Requested operation not supported in current context
	///
	U_UNSUPPORTED_ERROR = 16,

	///
	/// an operation is requested over a resource that does not support it
	///
	U_RESOURCE_TYPE_MISMATCH = 17,

	///
	/// ISO-2022 illlegal escape sequence
	///
	U_ILLEGAL_ESCAPE_SEQUENCE = 18,

	///
	/// ISO-2022 unsupported escape sequence
	///
	U_UNSUPPORTED_ESCAPE_SEQUENCE = 19,

	///
	/// No space available for in-buffer expansion for Arabic shaping
	///
	U_NO_SPACE_AVAILABLE = 20,

	///
	/// Currently used only while setting variable top, but can be used generally
	///
	U_CE_NOT_FOUND_ERROR = 21,

	///
	/// User tried to set variable top to a primary that is longer than two bytes
	///
	U_PRIMARY_TOO_LONG_ERROR = 22,

	///
	/// ICU cannot construct a service from this state, as it is no longer supported
	///
	U_STATE_TOO_OLD_ERROR = 23,

	///
	/// There are too many aliases in the path to the requested resource.
	/// It is very possible that a circular alias definition has occurred.
	///
	U_TOO_MANY_ALIASES_ERROR = 24,

	///
	/// UEnumeration out of sync with underlying collection
	///
	U_ENUM_OUT_OF_SYNC_ERROR = 25,

	///
	/// Unable to convert a char16_t* string to char* with the invariant converter.
	///
	U_INVARIANT_CONVERSION_ERROR = 26,

	///
	/// Requested operation can not be completed with ICU in its current state
	///
	U_INVALID_STATE_ERROR = 27,

	///
	/// Collator version is not compatible with the base version
	///
	U_COLLATOR_VERSION_MISMATCH = 28,

	///
	/// Collator is options only and no base is specified
	///
	U_USELESS_COLLATOR_ERROR = 29,

	///
	/// Attempt to modify read-only or constant data.
	///
	U_NO_WRITE_PERMISSION = 30,

	///
	/// One more than the highest standard error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_STANDARD_ERROR_LIMIT,

	// Error codes in the range 0x10000 0x10100 are reserved for Transliterator.

	///
	/// Missing '$' or duplicate variable name
	///
	U_BAD_VARIABLE_DEFINITION = 0x10000,

	///
	/// Start of Transliterator errors
	///
	U_PARSE_ERROR_START = 0x10000,

	///
	/// Elements of a rule are misplaced
	///
	U_MALFORMED_RULE,

	///
	/// A UnicodeSet pattern is invali
	////
	U_MALFORMED_SET,

	///
	/// UNUSED as of ICU 2.4
	///
	U_MALFORMED_SYMBOL_REFERENCE,

	///
	/// A Unicode escape pattern is invali
	////
	U_MALFORMED_UNICODE_ESCAPE,

	///
	/// A variable definition is invalid
	///
	U_MALFORMED_VARIABLE_DEFINITION,

	///
	/// A variable reference is invalid
	///
	U_MALFORMED_VARIABLE_REFERENCE,

	///
	/// UNUSED as of ICU 2.4
	///
	U_MISMATCHED_SEGMENT_DELIMITERS,

	///
	/// A start anchor appears at an illegal position
	///
	U_MISPLACED_ANCHOR_START,

	///
	/// A cursor offset occurs at an illegal position
	///
	U_MISPLACED_CURSOR_OFFSET,

	///
	/// A quantifier appears after a segment close delimiter
	///
	U_MISPLACED_QUANTIFIER,

	///
	/// A rule contains no operator
	///
	U_MISSING_OPERATOR,

	///
	/// UNUSED as of ICU 2.4
	///
	U_MISSING_SEGMENT_CLOSE,

	///
	/// More than one ante context
	///
	U_MULTIPLE_ANTE_CONTEXTS,

	///
	/// More than one cursor
	///
	U_MULTIPLE_CURSORS,

	///
	/// More than one post context
	///
	U_MULTIPLE_POST_CONTEXTS,

	///
	/// A dangling backslash
	///
	U_TRAILING_BACKSLASH,

	///
	/// A segment reference does not correspond to a defined segment
	///
	U_UNDEFINED_SEGMENT_REFERENCE,

	///
	/// A variable reference does not correspond to a defined variable
	///
	U_UNDEFINED_VARIABLE,

	///
	/// A special character was not quoted or escaped
	///
	U_UNQUOTED_SPECIAL,

	///
	/// A closing single quote is missing
	///
	U_UNTERMINATED_QUOTE,

	///
	/// A rule is hidden by an earlier more general rule
	///
	U_RULE_MASK_ERROR,

	///
	/// A compound filter is in an invalid location
	///
	U_MISPLACED_COMPOUND_FILTER,

	///
	/// More than one compound filter
	///
	U_MULTIPLE_COMPOUND_FILTERS,

	///
	/// A "::id" rule was passed to the RuleBasedTransliterator parser
	///
	U_INVALID_RBT_SYNTAX,

	///
	/// UNUSED as of ICU 2.4
	///
	U_INVALID_PROPERTY_PATTERN,

	///
	/// A 'use' pragma is invlalid
	///
	U_MALFORMED_PRAGMA,

	///
	/// A closing ')' is missing
	///
	U_UNCLOSED_SEGMENT,

	///
	/// UNUSED as of ICU 2.4
	///
	U_ILLEGAL_CHAR_IN_SEGMENT,

	///
	/// Too many stand-ins generated for the given variable range
	///
	U_VARIABLE_RANGE_EXHAUSTED,

	///
	/// The variable range overlaps characters used in rules
	///
	U_VARIABLE_RANGE_OVERLAP,

	///
	/// A special character is outside its allowed context
	///
	U_ILLEGAL_CHARACTER,

	///
	/// Internal transliterator system error
	///
	U_INTERNAL_TRANSLITERATOR_ERROR,

	///
	/// A "::id" rule specifies an unknown transliterator
	///
	U_INVALID_ID,

	///
	/// A "&fn()" rule specifies an unknown transliterator
	///
	U_INVALID_FUNCTION,

	///
	/// One more than the highest normal Transliterator error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_PARSE_ERROR_LIMIT,

	// Error codes in the range 0x10100 0x10200 are reserved for the formatting API.

	///
	/// Syntax error in format pattern
	///
	U_UNEXPECTED_TOKEN = 0x10100,

	///
	/// Start of format library errors
	///
	U_FMT_PARSE_ERROR_START = 0x10100,

	///
	/// More than one decimal separator in number pattern
	///
	U_MULTIPLE_DECIMAL_SEPARATORS,

	///
	/// Typo: kept for backward compatibility. Use U_MULTIPLE_DECIMAL_SEPARATORS
	///
	U_MULTIPLE_DECIMAL_SEPERATORS = U_MULTIPLE_DECIMAL_SEPARATORS,

	///
	/// More than one exponent symbol in number pattern
	///
	U_MULTIPLE_EXPONENTIAL_SYMBOLS,

	///
	/// Grouping symbol in exponent pattern
	///
	U_MALFORMED_EXPONENTIAL_PATTERN,

	///
	/// More than one percent symbol in number pattern
	///
	U_MULTIPLE_PERCENT_SYMBOLS,

	///
	/// More than one permill symbol in number pattern
	///
	U_MULTIPLE_PERMILL_SYMBOLS,

	///
	/// More than one pad symbol in number pattern
	///
	U_MULTIPLE_PAD_SPECIFIERS,

	///
	/// Syntax error in format pattern
	///
	U_PATTERN_SYNTAX_ERROR,

	///
	/// Pad symbol misplaced in number pattern
	///
	U_ILLEGAL_PAD_POSITION,

	///
	/// Braces do not match in message pattern
	///
	U_UNMATCHED_BRACES,

	///
	/// UNUSED as of ICU 2.4
	///
	U_UNSUPPORTED_PROPERTY,

	///
	/// UNUSED as of ICU 2.4
	///
	U_UNSUPPORTED_ATTRIBUTE,

	///
	/// Argument name and argument index mismatch in MessageFormat functions
	///
	U_ARGUMENT_TYPE_MISMATCH,

	///
	/// Duplicate keyword in PluralFormat
	///
	U_DUPLICATE_KEYWORD,

	///
	/// Undefined Plural keyword
	///
	U_UNDEFINED_KEYWORD,

	///
	/// Missing DEFAULT rule in plural rules
	///
	U_DEFAULT_KEYWORD_MISSING,

	///
	/// Decimal number syntax error
	///
	U_DECIMAL_NUMBER_SYNTAX_ERROR,

	///
	/// Cannot format a number exactly and rounding mode is ROUND_UNNECESSARY @stable ICU 4.8
	///
	U_FORMAT_INEXACT_ERROR,

	///
	/// One more than the highest normal formatting API error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_FMT_PARSE_ERROR_LIMIT,

	// Error codes in the range 0x10200 0x102ff are reserved for BreakIterator.

	///
	/// An internal error (bug) was detected.
	///
	U_BRK_INTERNAL_ERROR = 0x10200,

	///
	/// Start of codes indicating Break Iterator failures
	///
	U_BRK_ERROR_START = 0x10200,

	///
	//// Hex digits expected as part of a escaped char in a rule.
	///
	U_BRK_HEX_DIGITS_EXPECTED,

	///
	/// Missing ';' at the end of a RBBI rule.
	///
	U_BRK_SEMICOLON_EXPECTED,

	///
	/// Syntax error in RBBI rule.
	///
	U_BRK_RULE_SYNTAX,

	///
	/// UnicodeSet witing an RBBI rule missing a closing ']'.
	///
	U_BRK_UNCLOSED_SET,

	///
	/// Syntax error in RBBI rule assignment statement.
	///
	U_BRK_ASSIGN_ERROR,

	///
	/// RBBI rule $Variable redefined.
	///
	U_BRK_VARIABLE_REDFINITION,

	///
	/// Mis-matched parentheses in an RBBI rule.
	///
	U_BRK_MISMATCHED_PAREN,

	///
	/// Missing closing quote in an RBBI rule.
	///
	U_BRK_NEW_LINE_IN_QUOTED_STRING,

	///
	/// Use of an undefined $Variable in an RBBI rule.
	///
	U_BRK_UNDEFINED_VARIABLE,

	///
	/// Initialization failure.  Probable missing ICU Data.
	///
	U_BRK_INIT_ERROR,

	///
	/// Rule contains an empty Unicode Set.
	///
	U_BRK_RULE_EMPTY_SET,

	///
	/// !!option in RBBI rules not recognized.
	///
	U_BRK_UNRECOGNIZED_OPTION,

	///
	/// The {nnn} tag on a rule is mal formed
	///
	U_BRK_MALFORMED_RULE_TAG,

	///
	/// One more than the highest normal BreakIterator error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_BRK_ERROR_LIMIT,

	// Error codes in the range 0x10300-0x103ff are reserved for regular expression related errors.

	///
	/// An internal error (bug) was detected.
	///
	U_REGEX_INTERNAL_ERROR = 0x10300,

	///
	/// Start of codes indicating Regexp failures.
	///
	U_REGEX_ERROR_START = 0x10300,

	///
	/// Syntax error in regexp pattern.
	///
	U_REGEX_RULE_SYNTAX,

	///
	/// RegexMatcher in invalid state for requested operation
	///
	U_REGEX_INVALID_STATE,

	///
	/// Unrecognized backslash escape sequence in pattern
	///
	U_REGEX_BAD_ESCAPE_SEQUENCE,

	///
	/// Incorrect Unicode property
	///
	U_REGEX_PROPERTY_SYNTAX,

	///
	/// Use of regexp feature that is not yet implemented.
	///
	U_REGEX_UNIMPLEMENTED,

	///
	/// Incorrectly nested parentheses in regexp pattern.
	///
	U_REGEX_MISMATCHED_PAREN,

	///
	/// Decimal number is too large.
	///
	U_REGEX_NUMBER_TOO_BIG,

	///
	/// Error in {min,max} interval
	///
	U_REGEX_BAD_INTERVAL,

	///
	/// In {min,max}, max is less than min.
	///
	U_REGEX_MAX_LT_MIN,

	///
	/// Back-reference to a non-existent capture group.
	///
	U_REGEX_INVALID_BACK_REF,

	///
	/// Invalid value for match mode flags.
	///
	U_REGEX_INVALID_FLAG,

	///
	/// Look-Behind pattern matches must have a bounded maximum length.
	///
	U_REGEX_LOOK_BEHIND_LIMIT,

	///
	/// Regexps cannot have UnicodeSets containing strings.
	///
	U_REGEX_SET_CONTAINS_STRING,

	///
	/// Octal character constants must be <= 0377. @deprecated ICU 54. This error cannot occur.
	///
	U_REGEX_OCTAL_TOO_BIG,

	///
	/// Missing closing bracket on a bracket expression.
	///
	U_REGEX_MISSING_CLOSE_BRACKET = U_REGEX_SET_CONTAINS_STRING + 2,

	///
	/// In a character range [x-y], x is greater than y.
	///
	U_REGEX_INVALID_RANGE,

	///
	/// Regular expression backtrack stack overflow.
	///
	U_REGEX_STACK_OVERFLOW,

	///
	/// Maximum allowed match time exceeded
	///
	U_REGEX_TIME_OUT,

	///
	/// Matching operation aborted by user callback fn.
	///
	U_REGEX_STOPPED_BY_CALLER,

	///
	/// Pattern exceeds limits on size or complexity. @stable ICU 55
	///
	U_REGEX_PATTERN_TOO_BIG,

	///
	/// Invalid capture group name. @stable ICU 55
	///
	U_REGEX_INVALID_CAPTURE_GROUP_NAME,

	///
	/// One more than the highest normal regular expression error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_REGEX_ERROR_LIMIT = U_REGEX_STOPPED_BY_CALLER + 3,

	// Error codes in the range 0x10400-0x104ff are reserved for IDNA related error codes.

	U_IDNA_PROHIBITED_ERROR = 0x10400,

	U_IDNA_ERROR_START = 0x10400,

	U_IDNA_UNASSIGNED_ERROR,

	U_IDNA_CHECK_BIDI_ERROR,

	U_IDNA_STD3_ASCII_RULES_ERROR,

	U_IDNA_ACE_PREFIX_ERROR,

	U_IDNA_VERIFICATION_ERROR,

	U_IDNA_LABEL_TOO_LONG_ERROR,

	U_IDNA_ZERO_LENGTH_LABEL_ERROR,

	U_IDNA_DOMAIN_NAME_TOO_LONG_ERROR,

	///
	/// One more than the highest normal IDNA error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_IDNA_ERROR_LIMIT,

	// Aliases for StringPrep

	U_STRINGPREP_PROHIBITED_ERROR = U_IDNA_PROHIBITED_ERROR,

	U_STRINGPREP_UNASSIGNED_ERROR = U_IDNA_UNASSIGNED_ERROR,

	U_STRINGPREP_CHECK_BIDI_ERROR = U_IDNA_CHECK_BIDI_ERROR,

	// Error codes in the range 0x10500-0x105ff are reserved for Plugin related error codes.

	///
	/// Start of codes indicating plugin failures
	///
	U_PLUGIN_ERROR_START = 0x10500,

	///
	/// The plugin's level is too high to be loaded right now.
	///
	U_PLUGIN_TOO_HIGH = 0x10500,

	///
	/// The plugin didn't call uplug_setPlugLevel in response to a QUERY
	///
	U_PLUGIN_DIDNT_SET_LEVEL,

	///
	/// One more than the highest normal plug-in error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_PLUGIN_ERROR_LIMIT,

	///
	/// One more than the highest normal error code.
	/// @deprecated ICU 58 The numeric value may change over time, see ICU ticket #12420.
	///
	U_ERROR_LIMIT = U_PLUGIN_ERROR_LIMIT
};

///
/// Does the error code indicate success?
///
inline bool isSuccess(UErrorCode code) {
	return code <= UErrorCode::U_ZERO_ERROR;
}

///
/// Does the error code indicate a failure?
///
inline bool isFailure(UErrorCode code) {
	return code > UErrorCode::U_ZERO_ERROR;
}

///
/// This value is intended for sentinel values for APIs that
/// (take or) return single code points (char32_t).
/// It is outside of the Unicode code point range 0..0x10ffff.
///
/// For example, a "done" or "error" value in a new API
/// could be indicated with U_SENTINEL.
///
/// ICU APIs designed before ICU 2.4 usually define service-specific "done"
/// values, mostly 0xffff.
/// Those may need to be distinguished from
/// actual U+ffff text contents by calling functions like
/// CharacterIterator::hasNext() or UnicodeString::length().
///
constexpr char32_t UnicodeSentinel = std::numeric_limits<char32_t>::max();

} // namespace Balau::Unicode

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_ERROR_CODE
