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
/// @file PropertyScanner.hpp
///
/// A scanner for the property parser.
///

#ifndef COM_BORA_SOFTWARE_BALAU_LANG_PROPERTY_PARSER_PROPERTY_SCANNER
#define COM_BORA_SOFTWARE_BALAU_LANG_PROPERTY_PARSER_PROPERTY_SCANNER

#include <Balau/Lang/Common/AbstractScanner.hpp>
#include <Balau/Lang/Property/Lang/PropertyToken.hpp>

#include <stack>

namespace Balau::Lang::Property {

///
/// A scanner for the property parser.
///
class PropertyScanner : public AbstractScanner<PropertyToken> {
	///
	/// Instantiate a scanner with text obtained from the URI.
	///
	public: explicit PropertyScanner(std::shared_ptr<Resource::Uri> uri)
		: AbstractScanner<PropertyToken>(std::move(uri)) {}

	///
	/// Instantiate a scanner with text obtained from the URI.
	///
	public: explicit PropertyScanner(std::unique_ptr<Resource::Uri> && uri)
		: AbstractScanner<PropertyToken>(std::move(uri)) {}

	////////////////////////// Private implementation /////////////////////////

	private: PropertyToken getNextToken() override {
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
};

} // namespace Balau::Lang::Property

#endif // COM_BORA_SOFTWARE_BALAU_LANG_PROPERTY_PARSER_PROPERTY_SCANNER
