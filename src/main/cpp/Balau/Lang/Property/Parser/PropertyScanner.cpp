// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PropertyScanner.hpp"

namespace Balau::Lang::Property {

PropertyToken PropertyScanner::getNextToken() {
	readNextChar();

	if (Balau::Character::isBlank(currentChar)) {
		return createBlankToken();
	}

	switch (currentChar) {
		case U'{': {
			return PropertyToken::OpenCurly;
		}

		case U'}': {
			return PropertyToken::CloseCurly;
		}

		case U'@': {
			return PropertyToken::Arobase;
		}

		case U':': {
			return PropertyToken::Colon;
		}

		case U'=': {
			return PropertyToken::Equals;
		}

		case U'\r':
		case U'\n': {
			return createLineBreakToken();
		}

		case U'#': {
			return PropertyToken::Hash;
		}

		case U'!': {
			return PropertyToken::Exclamation;
		}

		case std::char_traits<char32_t>::eof(): {
			return PropertyToken::EndOfFile;
		}

		case U'\\': {
			readNextChar();

			if (Balau::Character::isBlank(currentChar)) {
				return PropertyToken::EscapedBlank;
			}

			switch (currentChar) {
				case U'{': {
					return PropertyToken::EscapedOpenCurly;
				}

				case U'}': {
					return PropertyToken::EscapedCloseCurly;
				}

				case U'@': {
					return PropertyToken::EscapedArobase;
				}

				case U':': {
					return PropertyToken::EscapedColon;
				}

				case U'=': {
					return PropertyToken::EscapedEquals;
				}

				case U'#': {
					return PropertyToken::EscapedHash;
				}

				case U'!': {
					return PropertyToken::EscapedExclamation;
				}

				case U'\\': {
					return PropertyToken::EscapedBackSlash;
				}

				case U'\r':
				case U'\n': {
					createLineBreakToken();
					return PropertyToken::EscapedLineBreak;
				}

				default: {
					return PropertyToken::EscapedChar;
				}
			}
		}

		default: {
			do {
				readNextChar();
			} while (!Character::isWhitespace(currentChar)
				&& currentChar != U'{'
				&& currentChar != U'}'
				&& currentChar != U':'
				&& currentChar != U'='
				&& currentChar != U'#'
				&& currentChar != U'!'
				&& currentChar != U'\\'
				&& currentChar != U'\t'
				&& currentChar != U'\r'
				&& currentChar != U'\n'
				&& currentChar != std::char_traits<char32_t>::eof()
			);

			putBackCurrentChar();
			return PropertyToken::Text;
		}
	}
}

} // namespace Balau::Lang::Property
