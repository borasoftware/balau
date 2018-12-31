// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file PropertyToken.hpp
///
/// The Property parser Token enum.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_LANG__PROPERTY_TOKEN
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_LANG__PROPERTY_TOKEN

#include <Balau/Type/ToString.hpp>

namespace Balau::Lang::Property {

/////////////////////////////// Language tokens ///////////////////////////////

///
/// Property language tokens.
///
enum class PropertyToken : unsigned char {
	  OpenCurly
	, CloseCurly
	, Arobase
	, Colon
	, Equals
	, Blank
	, LineBreak
	, Hash
	, Exclamation
	, EndOfFile

	, Text
	, BackSlash

	, EscapedOpenCurly
	, EscapedCloseCurly
	, EscapedArobase
	, EscapedColon
	, EscapedEquals
	, EscapedHash
	, EscapedExclamation
	, EscapedBackSlash
	, EscapedBlank
	, EscapedChar
	, EscapedLineBreak

	// Required for scanner
	, CommentBlock
	, CommentLine
};

///
/// Print the property token as a UTF-8 string.
///
/// @return a UTF-8 string representing the token
///
std::string toString(const PropertyToken & token);

///
/// Print the property token as a UTF-8 string.
///
/// @return a UTF-8 string representing the token
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const PropertyToken & token) {
	return toString<AllocatorT>(toString(token));
}

} // namespace Balau::Lang::Css

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_LANG__PROPERTY_TOKEN
