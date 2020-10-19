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

///
/// @file AbstractScanner.hpp
///
/// Base class for scanners.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG__ABSTRACT_SCANNER
#define COM_BORA_SOFTWARE__BALAU_LANG__ABSTRACT_SCANNER

#include <Balau/Lang/Common/CodeSpan.hpp>
#include <Balau/Lang/Common/ScannedTokens.hpp>
#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Util/Streams.hpp>

#include <stack>

namespace Balau::Lang {

///
/// Base class for scanners.
///
/// All scanning is made on unicode code points from the supplied UTF-8 encoded
/// input stream supplied to the protected constructor.
///
/// ////////// Line break handling //////////

///
/// There are four possible line break markers:
///  - newline;
///  - carriage return;
///  - carriage return then newline;
///  - newline then carriage return.
///
/// The marker characters are considered to be on the same line as the previous
/// characters of that line. When there is a greedy match of a line break marker,
/// the current line number is incremented. Thus the code span of a complete end
/// of line marker has a column end number of 1 (the start of the next line).
///
/// @tparam TokenT The token type enum - must contain at least the Blank, LineBreak, and EndOfFile values
///
template <typename TokenT> class AbstractScanner {
	///
	/// Scan the input and return a ScannedTokens data structure.
	///
	public: ScannedTokens<TokenT> scan() {
		currentStartOffset = 0U;
		currentEndOffset = 0U;
		TokenT token = getNextToken();

		while (token != TokenT::EndOfFile) {
			tokens.emplace_back(token);
			startOffsets.emplace_back(currentStartOffset);
			currentStartOffset = currentEndOffset;
			token = getNextToken();
		}

		// EOF
		tokens.emplace_back(token);
		startOffsets.emplace_back(currentStartOffset);

		return ScannedTokens<TokenT>(uri, std::move(text), std::move(tokens), std::move(startOffsets));
	}

	////////////////////////// Private implementation /////////////////////////

	protected: explicit AbstractScanner(std::shared_ptr<Resource::Uri> uri_)
		: currentChar(0)
		, uri(std::move(uri_))
		, text(::toString(uri->byteReadResource()->readStream()))
		, currentEndOffset(0) {}

	protected: AbstractScanner(std::shared_ptr<Resource::Uri> uri_, std::istream & inputStream_)
		: currentChar(0)
		, uri(std::move(uri_))
		, text(::toString(inputStream_))
		, currentEndOffset(0) {}

	protected: void readNextChar() {
		if (currentEndOffset == (int) text.length()) {
			currentChar = std::char_traits<char32_t>::eof();
		} else {
			currentChar = Character::getNextUtf8(text, currentEndOffset);

			if (currentChar < 0) {
				ThrowBalauException(
					  Exception::SyntaxErrorException
					, "Invalid UTF-8 text found during parsing of string literal", calculateCurrentCodeSpan()
				);
			}
		}
	}

	// Get a string view of the current token's string. If the token is partly parsed, this view is partial.
	protected: std::string_view getCurrentString() {
		return std::string_view(text.data() + currentStartOffset, currentEndOffset - currentStartOffset);
	}

	protected: template <typename ContainerT, typename ReportT>
	bool readNextChar(ContainerT & container, const std::function<ReportT (const CodeSpan &)> & errorReport) {
		if (currentEndOffset == (int) text.length()) {
			currentChar = std::char_traits<char32_t>::eof();
		} else {
			currentChar = Character::getNextUtf8(text, currentEndOffset);

			if (currentChar < 0) {
				container.push_back(errorReport(calculateCurrentCodeSpan()));
				return false;
			}
		}

		return true;
	}

	protected: void putBackCurrentChar() {
		if (currentChar == std::char_traits<char32_t>::eof() || text.length() == 0 || currentEndOffset == 0) {
			return;
		}

		if (currentEndOffset > 1) {
			Character::retreatUtf8(text, currentEndOffset);
			Character::retreatUtf8(text, currentEndOffset);
			currentChar = Character::getNextUtf8(text, currentEndOffset);
		} else if (currentEndOffset == 1) {
			Character::retreatUtf8(text, currentEndOffset);
			currentChar = 0;
		} else { // currentEndOffset == 0
			// NOP
		}
	}

	protected: void extractStringConstantDoubleQuotes() {
		// The current character must already be the double quote character.
		while (true) {
			readNextChar();

			switch (currentChar) {
				case U'"': {
					return;
				}

				case U'\\': { // Escaped character.
					readNextChar();
					continue;
				}

				case std::char_traits<char32_t>::eof(): {
					ThrowBalauException(
						  Exception::SyntaxErrorException
						, "End of file found during parsing of string literal", calculateCurrentCodeSpan()
					);
				}

				default: {
					continue;
				}
			}
		}
	}

	protected: template <typename ContainerT, typename ReportT>
	bool extractStringConstantDoubleQuotes(ContainerT & container, const std::function<ReportT (const CodeSpan &)> & errorReport) {
		// The current character must already be the double quote character.
		while (true) {
			readNextChar();

			switch (currentChar) {
				case U'"': {
					return true;
				}

				case U'\\': { // Escaped character.
					readNextChar();
					continue;
				}

				case std::char_traits<char32_t>::eof(): {
					container.push_back(errorReport(calculateCurrentCodeSpan()));
					return false;
				}

				default: {
					continue;
				}
			}
		}
	}

	protected: TokenT createWhitespaceToken() {
		// The current character must already be a blank or line break character.

		if (Balau::Character::isBlank(currentChar)) {
			return createBlankToken();
		} else {
			return createLineBreakToken();
		}
	}

	protected: TokenT createBlankToken() {
		// The current character must already be a blank character.

		while (Balau::Character::isBlank(currentChar)) {
			readNextChar();
		}

		putBackCurrentChar();
		return TokenT::Blank;
	}

	protected: TokenT createLineBreakToken() {
		// The current character must already be a line break character.

		if (currentChar == '\n') {
			readNextChar();

			if (currentChar != '\r') {
				putBackCurrentChar();
			}
		} else { // currentChar == '\r'
			readNextChar();

			if (currentChar != '\n') {
				putBackCurrentChar();
			}
		}

		return TokenT::LineBreak;
	}

	protected: CodeSpan calculateCurrentCodeSpan() {
		return RandomAccessScannedTokens<TokenT>::determineCodeSpan(
			text, tokens, startOffsets, (unsigned int) tokens.size() - 1
		);
	}

	//
	// This method is implemented by derived classes and contains the actual scanning logic.
	//
	// Derived classes should parse the next characters and return the token type.
	// The start offsets are handled by the base class.
	//
	private: virtual TokenT getNextToken() = 0;

	protected: char32_t currentChar;

	private: std::shared_ptr<Resource::Uri> uri;
	private: std::string text;
	private: std::istringstream inputStream;
	private: std::stack<char32_t> putBackBuffer;
	private: int currentStartOffset;
	private: int currentEndOffset;
	private: std::vector<TokenT> tokens;
	private: std::vector<unsigned int> startOffsets;
};

} // namespace Balau::Lang

#endif // COM_BORA_SOFTWARE__BALAU_LANG__ABSTRACT_SCANNER
