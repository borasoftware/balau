// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

template <typename Unused = int>
std::string toString2(const PropertyToken & token) {
	switch (token) {
		case PropertyToken::OpenCurly:          return "OpenCurly";
		case PropertyToken::CloseCurly:         return "CloseCurly";
		case PropertyToken::Arobase:            return "Arobase";
		case PropertyToken::Colon:              return "Colon";
		case PropertyToken::Equals:             return "Equals";
		case PropertyToken::Blank:              return "Blank";
		case PropertyToken::LineBreak:          return "LineBreak";
		case PropertyToken::Hash:               return "Hash";
		case PropertyToken::Exclamation:        return "Exclamation";
		case PropertyToken::EndOfFile:          return "EndOfFile";

		case PropertyToken::Text:               return "Text";
		case PropertyToken::BackSlash:          return "BackSlash";

		case PropertyToken::EscapedOpenCurly:   return "EscapedOpenCurly";
		case PropertyToken::EscapedCloseCurly:  return "EscapedCloseCurly";
		case PropertyToken::EscapedArobase:     return "EscapedArobase";
		case PropertyToken::EscapedColon:       return "EscapedColon";
		case PropertyToken::EscapedEquals:      return "EscapedEquals";
		case PropertyToken::EscapedHash:        return "EscapedHash";
		case PropertyToken::EscapedExclamation: return "EscapedExclamation";
		case PropertyToken::EscapedBackSlash:   return "EscapedBackSlash";
		case PropertyToken::EscapedBlank:       return "EscapedBlank";
		case PropertyToken::EscapedChar:        return "EscapedChar";
		case PropertyToken::EscapedLineBreak:   return "EscapedLineBreak";

		case PropertyToken::CommentBlock:       return "CommentBlock";
		case PropertyToken::CommentLine:        return "CommentLine";

		default: ThrowBalauException(Exception::BugException, "Unhandled property token in switch statement.");
	}
}

///
/// Print the property token as a UTF-8 string.
///
/// @return a UTF-8 string representing the token
///
inline std::string toString(const PropertyToken & token) {
	return toString2<>(token);
}

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
