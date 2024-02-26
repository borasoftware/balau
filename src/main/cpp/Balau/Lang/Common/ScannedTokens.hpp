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
/// @file ScannedTokens.hpp
///
/// Encapsulation of a set of tokens, text, and offsets plus various API wrappers.
///

#ifndef COM_BORA_SOFTWARE_BALAU_LANG_SCANNED_TOKENS
#define COM_BORA_SOFTWARE_BALAU_LANG_SCANNED_TOKENS

#include <Balau/Exception/ContainerExceptions.hpp>
#include <Balau/Exception/ParsingExceptions.hpp>
#include <Balau/Lang/Common/CodeSpan.hpp>
#include <Balau/Dev/Assert.hpp>
#include <Balau/Util/Strings.hpp>

#include <stack>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Lang {

///
/// Determines the whitespace scanning mode used in the scanning API.
///
enum class WhitespaceMode {
	  ConsumeWhitespaceAndComments
	, ConsumeWhitespace
	, ConsumeBlanksAndComments
	, ConsumeBlanks
	, ConsumeLineBreaksAndComments
	, ConsumeLineBreaks
	, ConsumeComments
	, DoNotConsume
};

template <typename TokenT> class ScannedTokens;
template <typename TokenT> class RandomAccessScannedTokens;
template <typename TokenT> class ScannerApiScannedTokens;
template <typename TokenT> class IterativeScannedTokens;

///
/// Encapsulation of a set of language tokens, source text, and start offsets.
///
/// This is the data structure returned by the scanner. It is structured to use
/// the minimum amount of memory necessary in order to hold information on the
/// whole scanned source file.
///
/// The supplied scanned tokens MUST end in an EndOfFile token.
///
/// There are three APIs defined by external adaptor classes:
///  - random access API;
///  - scanner API;
///  - iterative API.
///
/// When used by a parser, the scanner API provides a traditional scanner API with
/// infinite lookahead and putback. The whitespace policy may be set dynamically by
/// pushing and popping whitespace modes during consummation of the tokens.
///
/// For other uses, the random access and iterative APIs may be used.
///
/// The data structure is not random access. The token code span and string information
/// is constructed serially in the scanner API. This permits the data structure to be as
/// small as possible.
///
/// If random access is required, the RandomAccessScannedTokens wrapper may be built.
/// Note that the memory overhead of the RandomAccessScannedTokens wrapper is far
/// greater than that of the ScannedTokens data structure.
///
/// The scanned token type TokenT must be an enum containing at least the following
/// entries:
///  - Blank;
///  - LineBreak;
///  - CommentLine.
///
/// In addition to the LineBreak token, the CommentLine enum entry is also considered
/// to end with a line break.
///
/// @tparam TokenT the language token enum type
///
template <typename TokenT> class ScannedTokens final {
	public: ScannedTokens(std::shared_ptr<const Resource::Uri> uri_,
	                      std::string && text_,
	                      std::vector<TokenT> && tokens_,
	                      std::vector<unsigned int> && startOffsets_)
		: uri(std::move(uri_))
		, text(std::move(text_))
		, tokens(tokens_)
		, startOffsets(startOffsets_) {
		Assert::assertion(tokens.back() == TokenT::EndOfFile, "ScannedTokens must end with an EndOfFile token.");
	}

	///
	/// Get the text of the token with the specified index.
	///
	public: std::string_view getText(unsigned int index) {
		if (index >= tokens.size()) {
			return std::string_view(text.data() + text.length(), 0);
		} else if (index == tokens.size() - 1) {
			return std::string_view(
				text.data() + startOffsets[index], text.length() - startOffsets[index]
			);
		} else {
			return std::string_view(
				  text.data() + startOffsets[index]
				, startOffsets[index + 1] - startOffsets[index]
			);
		}
	}

	///
	/// Move the input text string to its final destination.
	///
	/// The scanned tokens instance must not be used after the move is performed.
	///
	/// @return the input text string as an rvalue reference
	///
	public: std::string && moveTextOut() {
		return std::move(text);
	}

	///
	/// Get the input uri.
	///
	/// @return the input uri
	///
	public: std::shared_ptr<const Resource::Uri> getUri() {
		return uri;
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::shared_ptr<const Resource::Uri> uri;
	private: std::string text;
	private: std::vector<TokenT> tokens;
	private: std::vector<unsigned int> startOffsets;

	friend class ScannerApiScannedTokens<TokenT>;
	friend class IterativeScannedTokens<TokenT>;
	friend class RandomAccessScannedTokens<TokenT>;
};

////////////////////////// RandomAccessScannedTokens //////////////////////////

///
/// Immutable, random access wrapper over a ScannedTokens data structure.
///
/// This class has a memory overhead which is greater than that of the ScannedTokens
/// data structure due to the code span information.
///
/// @tparam TokenT the language token enum type
/// @tparam unsigned int the offset unsigned integer type
///
template <typename TokenT> class RandomAccessScannedTokens final {
	///
	/// Instantiate a random access scanned tokens data structure.
	///
	/// Code spans for all the scanned tokens will be created during construction.
	///
	public: explicit RandomAccessScannedTokens(ScannedTokens<TokenT> && scannedTokens_)
		: scannedTokens(std::move(scannedTokens_))
		, codeSpans(buildCodeSpans(scannedTokens)) {}

	///
	/// Get the scanned tokens structure that this random access wrapper contains.
	///
	/// @return the scanned tokens structure that this random access wrapper contains
	///
	public: const ScannedTokens<TokenT> & getScannedTokens() const {
		return scannedTokens;
	}

	///
	/// Get the pre-calculated code spans of each scanned token.
	///
	/// @return the pre-calculated code spans of each scanned token
	///
	public: const std::vector<CodeSpan> & getCodeSpans() const {
		return codeSpans;
	}

	///
	/// Utility to determine the overall code span for the specified index.
	///
	/// This method will iterate through all the tokens in order to build the code span.
	///
	/// @return the overall code span for the supplied scanned tokens
	///
	public: static CodeSpan determineCodeSpan(const ScannedTokens<TokenT> & scannedTokens,
	                                          unsigned int index) {
		return determineCodeSpan(scannedTokens.text, scannedTokens.tokens, scannedTokens.startOffsets, index);
	}

	///
	/// Utility to determine the overall code span for the specified index.
	///
	/// This method will iterate through all the tokens in order to build the code span.
	///
	/// @throw IndexOutOfRangeException if the specified index is greater or equal to the vector size
	///
	public: static CodeSpan determineCodeSpan(const std::string & text,
	                                          const std::vector<TokenT> & tokens,
	                                          const std::vector<unsigned int> & startOffsets,
	                                          unsigned int index) {
		if (tokens.size() <= index) {
			ThrowBalauException(
				  Exception::IndexOutOfRangeException
				, std::string("scanned token range is 0 to ") + ::toString(tokens.size()), index
			);
		}

		std::vector<unsigned int> lineLengthCache;
		CodePosition startPosition(1, 1);
		CodePosition endPosition(1, 1);

		// tokens.size() > 0

		for (size_t thisIndex = 0; thisIndex < tokens.size() - 1; thisIndex++) {
			determineNewEnd(
				  tokens[thisIndex]
				, text
				, startOffsets[thisIndex]
				, startOffsets[thisIndex + 1]
				, endPosition
				, lineLengthCache
			);

			startPosition = endPosition;

			if (index == thisIndex) {
				return CodeSpan(startPosition, endPosition);
			}
		}

		determineNewEnd(
			  tokens[tokens.size() - 1]
			, text
			, startOffsets[tokens.size() - 1]
			, (unsigned int) text.length()
			, endPosition
			, lineLengthCache
		);

		return CodeSpan(startPosition, endPosition);
	}

	///
	/// Move the input text string to its final destination.
	///
	/// The scanned tokens instance must not be used after the move is performed.
	///
	/// @return the input text string as an rvalue
	///
	public: std::string && moveTextOut() {
		return std::move(scannedTokens.moveTextOut());
	}

	///////////////////////// Private implementation //////////////////////////

	// Builds all the code spans from the supplied scanned tokens.
	private: static std::vector<CodeSpan> buildCodeSpans(const ScannedTokens<TokenT> & scannedTokens) {
		const std::vector<TokenT> & tokens = scannedTokens.tokens;
		const std::vector<unsigned int> & startOffsets = scannedTokens.startOffsets;

		std::vector<CodeSpan> builtCodeSpans;

		if (tokens.size() == 0) {
			return builtCodeSpans;
		}

		std::vector<unsigned int> lineLengthCache;
		CodePosition startPosition(1, 1);
		CodePosition endPosition(1, 1);

		// tokens.size() > 0

		for (size_t index = 0; index < tokens.size() - 1; index++) {
			determineNewEnd(
				  tokens[index]
				, scannedTokens.text
				, startOffsets[index]
				, startOffsets[index + 1]
				, endPosition
				, lineLengthCache
			);

			builtCodeSpans.emplace_back(CodeSpan(startPosition, endPosition));

			startPosition = endPosition;
		}

		determineNewEnd(
			  tokens[tokens.size() - 1]
			, scannedTokens.text
			, startOffsets[tokens.size() - 1]
			, (unsigned int) scannedTokens.text.length()
			, endPosition
			, lineLengthCache
		);

		builtCodeSpans.emplace_back(CodeSpan(startPosition, endPosition));

		return builtCodeSpans;
	}

	// Updates in place the supplied end position for advancing.
	// Also updates the line length cache when a line break is encountered.
	private: static void determineNewEnd(TokenT token,
	                                     const std::string & fullText,
	                                     unsigned int thisOffset,
	                                     unsigned int nextOffset,
	                                     CodePosition & endPosition,
	                                     std::vector<unsigned int> & lineLengthCache) {
		static const std::regex endLineBreakRegex = std::regex("(\n\r|\r\n|\n|\r)$");

		if (token == TokenT::LineBreak || token == TokenT::CommentLine || token == TokenT::CommentBlock) {
			const std::string thisText = fullText.substr(thisOffset, nextOffset - thisOffset);
			const std::vector<size_t> lineLengths = Util::Strings::lineLengths(thisText);
			const bool hasCurrentLine = !std::regex_search(thisText, endLineBreakRegex);

			using ::toString;

			Assert::assertion(
				  !lineLengths.empty()
				, [&] () { return std::string(
					"A line break token was supplied in the determineNewEnd function"
					"call but no line end was found in the supplied text. "
					"Start offset = "
				) + toString(thisOffset) + ", end offset = " +
					toString(nextOffset) + ", token text = " + thisText; }
			);

			// The first line length is added to the current line length, then
			// the intermediate line lengths are added, then the current line
			// length becomes the current line length.

			// First.
			endPosition.column += lineLengths[0];
			++endPosition.line;
			lineLengthCache.push_back(endPosition.column - 1); // one indexed column position

			if (hasCurrentLine) {
				// Intermediate.
				for (size_t m = 1; m < lineLengths.size() - 1; m++) {
					lineLengthCache.push_back((unsigned int) lineLengths[m]);
					++endPosition.line;
				}

				// Current.
				endPosition.column = 1 + (unsigned int) lineLengths.back();
			} else {
				// Intermediate.
				for (size_t m = 1; m < lineLengths.size(); m++) {
					lineLengthCache.push_back((unsigned int) lineLengths[m]);
					++endPosition.line;
				}

				// Current.
				endPosition.column = 1;
			}
		} else {
			endPosition.column += nextOffset - thisOffset;
		}
	}

	// Updates in place the supplied start position when retreating.
	// Also updates the line length cache when a line break is encountered.
	private: static void determineNewStart(TokenT token,
	                                       const std::string & fullText,
	                                       unsigned int thisOffset,
	                                       unsigned int nextOffset,
	                                       CodePosition & startPosition,
	                                       std::vector<unsigned int> & lineLengthCache) {
		static const std::regex lineBreakRegex = std::regex("\n\r|\r\n|\n|\r");

		if (token == TokenT::LineBreak || token == TokenT::CommentLine || token == TokenT::CommentBlock) {
			const unsigned int lineBreaks = (unsigned int) Util::Strings::occurrences(fullText.substr(thisOffset, nextOffset - thisOffset), lineBreakRegex);

			using ::toString;

			Assert::assertion(
				  lineBreaks != 0
				, [&] () {
				  	return toString(
						  "A line break token was supplied in the determineNewStart function"
						, "call but no line end was found in the supplied text. "
						, "Start offset = ", thisOffset
						, ", end offset = ", nextOffset
						, ", token text = ", fullText.substr(thisOffset, nextOffset - thisOffset)
					);
				}
			);

			for (size_t m = 0; m < lineBreaks; m++) {
				startPosition.column = lineLengthCache.back() + 1; // one indexed column position
				lineLengthCache.pop_back();
			}

			startPosition.line -= lineBreaks;
		} else {
			startPosition.column -= nextOffset - thisOffset;
		}
	}

	friend class ScannerApiScannedTokens<TokenT>;

	private: ScannedTokens<TokenT> scannedTokens;
	private: std::vector<CodeSpan> codeSpans;
};

/////////////////////////// ScannerApiScannedTokens ///////////////////////////

///
/// Represents a single scanned token in the data structure.
///
/// When accessed via the scanner API, the scanned tokens returned from the get()
/// method contain full code span information.
///
/// The supplied scanned tokens MUST end in an EndOfFile token.
///
/// If full code span information is required with random access, the higher overhead
/// RandomAccessScannedTokens class may be used to wrap the ScannedTokens class.
///
template <typename TokenT> class ScannedToken {
	///
	/// The token type for this scanned token.
	///
	public: TokenT token;

	///
	/// The start and end positions of the token's text within the multi-line source file.
	///
	public: CodeSpan codeSpan;

	///
	/// UTF-8 formatted string of the token.
	///
	public: std::string_view text;

	////////////////////////// Private implementation /////////////////////////

	private: ScannedToken(TokenT token_, CodeSpan codeSpan_, std::string_view text_)
		: token(token_)
		, codeSpan(std::move(codeSpan_))
		, text(text_) {}

	friend class ScannerApiScannedTokens<TokenT>;
};

///
/// A ScannedTokens adaptor that provides a traditional scanner API.
///
template <typename TokenT> class ScannerApiScannedTokens final {
	///
	/// Create a scanner API scanned tokens instance.
	///
	public: explicit ScannerApiScannedTokens(ScannedTokens<TokenT> && scannedTokens_) noexcept
		: scannedTokens(std::move(scannedTokens_))
		, currentIndex(0)
		, currentCodeSpan(1, 1, 1, 1) {
		whitespaceModeStack.push(WhitespaceMode::DoNotConsume);
		if (scannedTokens.tokens.size() > 1) {
			RandomAccessScannedTokens<TokenT>::determineNewEnd(
				  scannedTokens.tokens[0]
				, scannedTokens.text
				, scannedTokens.startOffsets[0]
				, scannedTokens.startOffsets[1]
				, currentCodeSpan.end
				, lineLengthCache
			);
		}
	}

	///
	/// Rest the scanner API state to the beginning of the scanned tokens.
	///
	public: void reset() {
		while (!whitespaceModeStack.empty()) {
			whitespaceModeStack.pop();
		}

		whitespaceModeStack.push(WhitespaceMode::DoNotConsume);
		currentIndex = 0;
		currentCodeSpan = CodeSpan(1, 1, 1, 1);
		lineLengthCache.clear();
	}

	///
	/// Get the current token.
	///
	/// Depending on the whitespace mode, this may skip over whitespace and/or line break tokens.
	public: ScannedToken<TokenT> get() {
		const std::vector<TokenT> & tokens = scannedTokens.tokens;

		switch (whitespaceModeStack.top()) {
			case WhitespaceMode::ConsumeWhitespaceAndComments: {
				while (tokens[currentIndex] == TokenT::Blank
					|| tokens[currentIndex] == TokenT::LineBreak
					|| tokens[currentIndex] == TokenT::CommentLine) {
					advanceCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeWhitespace: {
				while (tokens[currentIndex] == TokenT::Blank || tokens[currentIndex] == TokenT::LineBreak) {
					advanceCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeBlanksAndComments: {
				while (tokens[currentIndex] == TokenT::Blank || tokens[currentIndex] == TokenT::CommentLine) {
					advanceCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeBlanks: {
				while (tokens[currentIndex] == TokenT::Blank) {
					advanceCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeLineBreaksAndComments: {
				while (tokens[currentIndex] == TokenT::LineBreak || tokens[currentIndex] == TokenT::CommentLine) {
					advanceCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeLineBreaks: {
				while (tokens[currentIndex] == TokenT::LineBreak) {
					advanceCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeComments: {
				while (tokens[currentIndex] == TokenT::CommentLine) {
					advanceCurrentIndex();
				}

				break;
			}

			default: { // WhitespaceMode::DoNotConsume
				break;
			}
		}

		return ScannedToken<TokenT>(
			tokens[currentIndex], currentCodeSpan, scannedTokens.getText(currentIndex)
		);
	}

	///
	/// Consume the current token.
	///
	/// If the current token is EndOfFile, then no action is taken.
	///
	public: void consume() {
		advanceCurrentIndex();
	}

	///
	/// Expect the supplied token.
	///
	/// Throws a syntax error exception with the supplied error message if the
	/// supplied token does not match the current token.
	///
	/// @param token the expected token
	/// @param errorMessage an error message to pass to the exception
	/// @throw SyntaxErrorException if the token was not found
	///
	public: void expect(const TokenT token, std::string_view errorMessage) {
		get();

		if (scannedTokens.tokens[currentIndex] == token) {
			if (currentIndex < scannedTokens.tokens.size() - 1) {
				advanceCurrentIndex();
			}
		} else {
			ThrowBalauException(Exception::SyntaxErrorException, std::string(errorMessage), getCurrentCodeSpan());
		}
	}

	///
	/// Expect the supplied token.
	///
	/// Adds an error report to the supplied error report container and returns false if the
	/// supplied token does not match the current token.
	///
	/// The current error count is also passed to the error report generation function in order
	/// to allow a generic report to be generated when the container contains the maximum number
	/// of errors.
	///
	/// @param token the expected token
	/// @param container the error report container to add the error report to
	/// @param maxErrorCount the maximum number of errors that the container is allowed to have
	/// @param errorReport a function that generates the error report
	/// @return true if the expected token was matched, false otherwise
	///
	public: template <template <typename ...> class ContainerT, typename ... ArgT, typename ReportT>
	bool expect(const TokenT token,
	            ContainerT<ReportT, ArgT ...> & container,
	            const size_t maxErrorCount,
	            const std::function<ReportT (const TokenT &, const CodeSpan &, size_t)> & errorReport) {
		get();

		if (scannedTokens.tokens[currentIndex] == token) {
			if (currentIndex < scannedTokens.tokens.size() - 1) {
				advanceCurrentIndex();
			}

			return true;
		} else {
			auto error = errorReport(token, getCurrentCodeSpan(), container.size());

			if (container.size() < maxErrorCount) {
				container.push_back(errorReport(token, getCurrentCodeSpan(), container.size()));
			}

			return false;
		}
	}

	///
	/// Expect one of the supplied tokens.
	///
	/// Throws a syntax error exception with the supplied error message if none
	/// of the supplied tokens match the current token.
	///
	/// @throw SyntaxErrorException if one of the tokens was not found
	///
	public: void expect(const std::vector<TokenT> & tokens, std::string_view errorMessage) {
		get();

		if (std::find(tokens.begin(), tokens.end(), scannedTokens.tokens[currentIndex]) != tokens.end()) {
			advanceCurrentIndex();
		} else {
			ThrowBalauException(Exception::SyntaxErrorException, std::string(errorMessage), getCurrentCodeSpan());
		}
	}

	///
	/// Expect one of the supplied tokens.
	///
	/// Adds an error report to the supplied error report container and returns false if none
	/// of the supplied tokens match the current token.
	///
	/// The current error count is also passed to the error report generation function in order
	/// to allow a generic report to be generated when the container contains the maximum number
	/// of errors.
	///
	/// @param token the expected token
	/// @param container the error report container to add the error report to
	/// @param maxErrorCount the maximum number of errors that the container is allowed to have
	/// @param errorReport a function that generates the error report
	/// @return true if one of the expected tokens was matched, false otherwise
	///
	public: template <template <typename ...> class TokenContainerT, template <typename ...> class ContainerT, typename ... TokenArgT, typename ... ArgT, typename ReportT>
	bool expect(const TokenContainerT<TokenT, TokenArgT ...> & tokens,
	            ContainerT<ReportT, ArgT ...> & container,
	            const size_t maxErrorCount,
	            const std::function<ReportT (const TokenContainerT<TokenT, TokenArgT ...> &, const CodeSpan &, size_t)> & errorReport) {
		get();

		if (std::find(tokens.begin(), tokens.end(), scannedTokens.tokens[currentIndex]) != tokens.end()) {
			advanceCurrentIndex();
			return true;
		} else {
			if (container.size() < maxErrorCount) {
				container.push_back(errorReport(tokens, getCurrentCodeSpan(), container.size()));
			}

			return false;
		}
	}

	///
	/// A position marker that can be obtained at any point during parsing, in order to put back multiple tokens.
	///
	public: class Marker {
		public: Marker(const Marker & rhs) noexcept : index(rhs.index) {}

		public: Marker(Marker && rhs) noexcept : index(rhs.index) {}

		public: Marker & operator = (const Marker & rhs) noexcept {
			index = rhs.index;
			return *this;
		}

		public: Marker & operator = (Marker && rhs) noexcept {
			index = rhs.index;
			return *this;
		}

		private: explicit Marker(size_t index_) : index(index_) {}

		private: size_t index;

		friend class ScannerApiScannedTokens;
	};

	public: Marker mark() const {
		return Marker(currentIndex);
	}

	public: void putBack(const Marker & marker) {
		if (marker.index > currentIndex) {
			ThrowBalauException(
				  Exception::IndexOutOfRangeException
				, ::toString("Marker index (", marker.index, ") is greater than current index (", currentIndex, ")")
				, marker.index
			);
		}

		const auto steps = currentIndex - marker.index;

		for (size_t m = 0; m < steps; m++) {
			putBack();
		}
	}

	///
	/// Put back the current token.
	///
	/// Depending on the whitespace mode, this may also reverse skip whitespace
	/// and/or line break tokens.
	///
	public: void putBack() {
		const std::vector<TokenT> & tokens = scannedTokens.tokens;

		retreatCurrentIndex();

		switch (whitespaceModeStack.top()) {
			case WhitespaceMode::ConsumeWhitespaceAndComments: {
				while (currentIndex > 0 && (tokens[currentIndex - 1] == TokenT::Blank || tokens[currentIndex - 1] == TokenT::LineBreak || tokens[currentIndex - 1] == TokenT::CommentLine)) {
					retreatCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeWhitespace: {
				while (currentIndex > 0 && (tokens[currentIndex - 1] == TokenT::Blank || tokens[currentIndex - 1] == TokenT::LineBreak)) {
					retreatCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeBlanksAndComments: {
				while (currentIndex > 0 && (tokens[currentIndex - 1] == TokenT::Blank || tokens[currentIndex - 1] == TokenT::CommentLine)) {
					retreatCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeBlanks: {
				while (currentIndex > 0 && tokens[currentIndex - 1] == TokenT::Blank) {
					retreatCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeLineBreaksAndComments: {
				while (currentIndex > 0 && (tokens[currentIndex - 1] == TokenT::LineBreak || tokens[currentIndex - 1] == TokenT::CommentLine)) {
					retreatCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeLineBreaks: {
				while (currentIndex > 0 && (tokens[currentIndex - 1] == TokenT::LineBreak)) {
					retreatCurrentIndex();
				}

				break;
			}

			case WhitespaceMode::ConsumeComments: {
				while (currentIndex > 0 && (tokens[currentIndex - 1] == TokenT::CommentLine)) {
					retreatCurrentIndex();
				}

				break;
			}

			default: { // WhitespaceMode::DoNotConsume
				break;
			}
		}
	}

	///
	/// Get the code span of the current token.
	///
	public: CodeSpan getCurrentCodeSpan() const {
		return currentCodeSpan;
	}

	///
	/// Is the next token a blank?
	///
	public: bool currentIsBlank() const {
		return currentIndex < scannedTokens.tokens.size() && scannedTokens.tokens[currentIndex] == TokenT::Blank;
	}

	///
	/// Is the next token line break?
	///
	public: bool currentIsLineBreak() const {
		return currentIndex < scannedTokens.tokens.size() && scannedTokens.tokens[currentIndex] == TokenT::LineBreak;
	}

	///
	/// Is the next token whitespace?
	///
	public: bool currentIsWhitespace() const {
		if (currentIndex >= scannedTokens.tokens.size()) {
			return false;
		}

		const TokenT token = scannedTokens.tokens[currentIndex];
		return token == TokenT::Blank || token == TokenT::LineBreak;
	}

	///
	/// Is the marked token a blank?
	///
	public: bool isBlank(const Marker & marker) const {
		return scannedTokens.tokens[marker.index] == TokenT::Blank;
	}

	///
	/// Is the marked token line break?
	///
	public: bool isLineBreak(const Marker & marker) const {
		return scannedTokens.tokens[marker.index] == TokenT::LineBreak;
	}

	///
	/// Is the marked token whitespace?
	///
	public: bool isWhitespace(const Marker & marker) const {
		const TokenT token = scannedTokens.tokens[marker.index];
		return token == TokenT::Blank || token == TokenT::LineBreak;
	}

	///
	/// Push the supplied whitespace mode onto the whitespace mode stack.
	///
	public: void pushWhitespaceMode(WhitespaceMode newMode) {
		whitespaceModeStack.push(newMode);
	}

	///
	/// Pop the top of the whitespace mode stack.
	///
	public: void popWhitespaceMode() {
		whitespaceModeStack.pop();
	}

	///
	/// Get the number of tokens.
	///
	public: size_t size() const {
		return scannedTokens.tokens.size();
	}

	///
	/// Move the input text string to its final destination.
	///
	/// The scanned tokens instance must not be used after the move is performed.
	///
	/// @return the input text string as an rvalue reference
	///
	public: std::string && moveTextOut() {
		return std::move(scannedTokens.moveTextOut());
	}

	///
	/// Move the input uri to its final destination.
	///
	/// @return the input uri rvalue reference
	///
	public: std::shared_ptr<const Resource::Uri> getUri() {
		return scannedTokens.getUri();
	}

	////////////////////////// Private implementation /////////////////////////

	private: void advanceCurrentIndex() {
		if (currentIndex == scannedTokens.tokens.size() - 1) {
			return;
		}

		currentCodeSpan.start = currentCodeSpan.end;
		++currentIndex;

		if (currentIndex < scannedTokens.tokens.size() - 1) {
			// Calculate the new end point.
			RandomAccessScannedTokens<TokenT>::determineNewEnd(
				  scannedTokens.tokens[currentIndex]
				, scannedTokens.text
				, scannedTokens.startOffsets[currentIndex]
				, scannedTokens.startOffsets[currentIndex + 1]
				, currentCodeSpan.end
				, lineLengthCache
			);
		}
	}

	private: void retreatCurrentIndex() {
		if (currentIndex == 0) {
			return;
		}

		currentCodeSpan.end = currentCodeSpan.start;

		RandomAccessScannedTokens<TokenT>::determineNewStart(
			  scannedTokens.tokens[currentIndex - 1]
			, scannedTokens.text
			, scannedTokens.startOffsets[currentIndex - 1]
			, scannedTokens.startOffsets[currentIndex]
			, currentCodeSpan.start
			, lineLengthCache
		);

		--currentIndex;
	}

	private: ScannedTokens<TokenT> scannedTokens;
	private: unsigned int currentIndex;
	private: CodeSpan currentCodeSpan;
	private: std::stack<WhitespaceMode> whitespaceModeStack;
	private: std::vector<unsigned int> lineLengthCache;
};

/////////////////////////// IterativeScannedTokens ////////////////////////////

///
/// A ScannedTokens adaptor that provides an iteration API.
///
template <typename TokenT> class IterativeScannedTokens final {
	///
	/// Returned by const iterators when accessing elements.
	///
	public: class ConstElement {
		private: const IterativeScannedTokens<TokenT> * parent;
		private: const size_t index;

		public: ConstElement(const ScannedTokens<TokenT> * parent_, size_t index_)
			: parent(parent_)
			, index(index_) {}

		public: TokenT token() const {
			return parent->getToken(index);
		}

		public: unsigned int startOffset() const {
			return parent->getStartOffset(index);
		}
	};

	///
	/// Scanned tokens const iterator.
	///
	public: class const_iterator {
		friend class IterativeScannedTokens<TokenT>;

		private: const IterativeScannedTokens<TokenT> * parent;
		private: size_t index;

		private: explicit const_iterator(const ScannedTokens<TokenT> * parent_, size_t index_)
			: parent(parent_)
			, index(index_) {}

		public: const_iterator(const const_iterator & copy)
			: parent(copy.parent)
			, index(copy.index) {}

		public: const_iterator & operator = (const const_iterator & copy) {
			parent = copy.parent;
			index = copy.index;
			return *this;
		}

		public: const_iterator & operator ++ () {
			++index;
			return *this;
		}

		public: ConstElement operator * () const {
			return ConstElement(parent->scannedTokens.tokens[index], parent->scannedTokens.startOffsets[index]);
		}

		public: ConstElement operator -> () const {
			return ConstElement(parent->scannedTokens.tokens[index], parent->scannedTokens.startOffsets[index]);
		}

		public: bool operator == (const const_iterator & rhs) const {
			return parent == rhs.parent && index == rhs.index;
		}

		public: bool operator != (const const_iterator & rhs) const {
			return ! operator == (rhs);
		}
	};

	///
	/// Returned by non-const iterators when accessing elements.
	///
	public: class Element {
		private: IterativeScannedTokens<TokenT> * parent;
		private: const size_t index;

		public: Element(ScannedTokens<TokenT> * parent_, size_t index_)
			: parent(parent_)
			, index(index_) {}

		public: TokenT token() const {
			return parent->getToken(index);
		}

		public: TokenT & token() {
			return parent->getToken(index);
		}

		public: unsigned int startOffset() const {
			return parent->getStartOffset(index);
		}

		public: unsigned int & startOffset() {
			return parent->getStartOffset(index);
		}
	};

	///
	/// Scanned tokens non-const iterator.
	///
	public: class iterator {
		friend class IterativeScannedTokens<TokenT>;

		private: IterativeScannedTokens<TokenT> * parent;
		private: size_t index;

		private: explicit iterator(ScannedTokens<TokenT> * parent_, size_t index_)
			: parent(parent_)
			, index(index_) {}

		public: iterator(const iterator & copy)
			: parent(copy.parent)
			, index(copy.index) {}

		public: iterator & operator = (const iterator & copy) {
			parent = copy.parent;
			index = copy.index;
			return *this;
		}

		public: iterator & operator ++ () {
			++index;
			return *this;
		}

		public: ConstElement operator * () const {
			return ConstElement(parent->scannedTokens.tokens[index], parent->scannedTokens.startOffsets[index]);
		}

		public: Element operator * () {
			return Element(parent->scannedTokens.tokens[index], parent->scannedTokens.startOffsets[index]);
		}

		public: ConstElement operator -> () const {
			return ConstElement(parent->scannedTokens.tokens[index], parent->scannedTokens.startOffsets[index]);
		}

		public: Element operator -> () {
			return Element(parent->scannedTokens.tokens[index], parent->scannedTokens.startOffsets[index]);
		}

		public: bool operator == (const iterator & rhs) const {
			return parent == rhs.parent && index == rhs.index;
		}

		public: bool operator != (const iterator & rhs) const {
			return ! operator == (rhs);
		}
	};

	friend class const_iterator;
	friend class iterator;

	///
	/// Create a scanned tokens data structure, specifying the source URI and the source text.
	///
	/// The token data must be supplied via calls to addToken().
	///
	public: explicit IterativeScannedTokens(ScannedTokens<TokenT> & scannedTokens_)
		: scannedTokens(scannedTokens_)  {}

	///
	/// Add a token to the data structure, specifying the token and the start offset into the text.
	///
	public: void addToken(TokenT token, unsigned int startOffset) {
		scannedTokens.tokens.emplace_back(token);
		scannedTokens.startOffsets.emplace_back(startOffset);
	}

	///
	/// Get the number of tokens.
	///
	public: size_t size() const {
		return scannedTokens.tokens.size();
	}

	///
	/// Get the token with the specified index.
	///
	public: TokenT getToken(unsigned int index) const {
		return scannedTokens.tokens[index];
	}

	///
	/// Get a reference to the token with the specified index.
	///
	public: TokenT & getToken(unsigned int index) {
		return scannedTokens.tokens[index];
	}


	///
	/// Get the start offset of the token with the specified index.
	///
	public: unsigned int getStartOffset(unsigned int index) const {
		return scannedTokens.startOffsets[index];
	}

	///
	/// Get a reference to the start offset of the token with the specified index.
	///
	public: unsigned int & getStartOffset(unsigned int index) {
		return scannedTokens.startOffsets[index];
	}

	///
	/// Get the token vector.
	///
	public: const std::vector<TokenT> & getTokens() const {
		return scannedTokens.tokens;
	}

	///
	/// Get the start offsets vector.
	///
	public: const std::vector<unsigned int> & getStartOffsets() const {
		return scannedTokens.startOffsets;
	}

	///
	/// Get a const iterator positioned at the beginning of the data.
	///
	public: const_iterator begin() const {
		return const_iterator(this, 0);
	}

	///
	/// Get a non-const iterator positioned at the beginning of the data.
	///
	public: iterator begin() {
		return iterator(this, 0);
	}

	///
	/// Get a const iterator positioned at the end of the data.
	///
	public: const_iterator end() const {
		return const_iterator(this, scannedTokens.tokens.size());
	}

	///
	/// Get a non-const iterator positioned at the end of the data.
	///
	public: iterator end() {
		return iterator(this, scannedTokens.tokens.size());
	}

	///
	/// Move the input text string to its final destination.
	///
	/// The scanned tokens instance must not be used after the move is performed.
	///
	/// @return the input text string as an rvalue
	///
	public: std::string && moveTextOut() {
		return std::move(scannedTokens.moveTextOut());
	}

	////////////////////////// Private implementation /////////////////////////

	private: ScannedTokens<TokenT> scannedTokens;
};

} // namespace Balau::Lang

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE_BALAU_LANG_SCANNED_TOKENS
