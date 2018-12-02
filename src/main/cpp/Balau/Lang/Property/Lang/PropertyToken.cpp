// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PropertyToken.hpp"
#include "../../../Exception/BalauException.hpp"

namespace Balau::Lang::Property {

std::string toString(const PropertyToken & token) {
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

} // namespace Balau::Lang::Property
