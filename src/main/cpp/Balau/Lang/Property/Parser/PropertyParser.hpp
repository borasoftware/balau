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
/// @file PropertyParser.hpp
///
/// A recursive descent property language parser.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER

#include <Balau/Lang/Property/Parser/PropertyScanner.hpp>
#include <Balau/Lang/Property/Lang/PropertyAst.hpp>

#include <regex>

namespace Balau::Lang::Property {

///
/// A recursive descent property language parser.
///
/// All methods may throw a SyntaxErrorException if invalid input is read.
///
class PropertyParser {
	private: using ScannerApiScannedTokens = Lang::ScannerApiScannedTokens<PropertyToken>;
	private: using ScannedToken = Lang::ScannedToken<PropertyToken>;

	///
	/// Create a property parser which will parse the input provided by the scanner.
	///
	public: explicit PropertyParser(ScannerApiScannedTokens && scannedTokens_)
		: scannedTokens(std::move(scannedTokens_)) {}

	///
	/// Parse the input provided by the scanner to produce a property data object.
	///
	public: AST::Properties parse() {
		scannedTokens.pushWhitespaceMode(WhitespaceMode::DoNotConsume);
		ScannedToken token = scannedTokens.get();
		CodeSpan startCodeSpan = token.codeSpan;
		auto nodes = producePropertyNodes(PropertyToken::EndOfFile);
		scannedTokens.popWhitespaceMode();
		CodeSpan finishCodeSpan = token.codeSpan;

		return AST::Properties(
			CodeSpan::totalCodeSpan(startCodeSpan, finishCodeSpan)
			, std::move(scannedTokens.moveTextOut())
			, std::move(nodes)
		);
	}

	////////////////////////// Private implementation /////////////////////////

	private: static std::string_view extend(const std::string_view & start, const std::string_view & finish) {
		return std::string_view(start.data(), finish.data() - start.data() + finish.length());
	}

	private: std::vector<std::unique_ptr<AST::PropertyNode>> producePropertyNodes(PropertyToken endToken) {
		std::vector<std::unique_ptr<AST::PropertyNode>> nodes;
		ScannedToken token = scannedTokens.get();

		while (token.token != endToken) {
			switch (token.token) {
				case PropertyToken::Blank: {
					scannedTokens.consume();
					break;
				}

				case PropertyToken::LineBreak: {
					nodes.emplace_back(produceEmptyLine());
					break;
				}

				case PropertyToken::Hash:
				case PropertyToken::Exclamation: {
					nodes.emplace_back(produceComment());
					break;
				}

				case PropertyToken::Arobase: {
					nodes.emplace_back(produceInclude());
					break;
				}

				case PropertyToken::Text:
				case PropertyToken::EscapedOpenCurly:
				case PropertyToken::EscapedCloseCurly:
				case PropertyToken::EscapedArobase:
				case PropertyToken::EscapedColon:
				case PropertyToken::EscapedEquals:
				case PropertyToken::EscapedHash:
				case PropertyToken::EscapedExclamation:
				case PropertyToken::EscapedBackSlash:
				case PropertyToken::EscapedBlank:
				case PropertyToken::EscapedChar:
				case PropertyToken::EscapedLineBreak: {
					nodes.emplace_back(produceProperty());
					break;
				}

				case PropertyToken::EndOfFile: {
					ThrowBalauException(Exception::SyntaxErrorException, "Unexpected end of file", token.codeSpan);
				}

				default: {
					ThrowBalauException(
						Exception::SyntaxErrorException, "Invalid input: " + std::string(token.text), token.codeSpan
					);
				}
			}

			token = scannedTokens.get();
		}

		return nodes;
	}

	private: std::unique_ptr<AST::PropertyNode> produceProperty() {
		// Key
		ScannedToken startToken = scannedTokens.get();
		CodeSpan startCodeSpan = startToken.codeSpan;
		std::string_view key = startToken.text;

		scannedTokens.consume();
		ScannedToken token = scannedTokens.get();

		if (startToken.token == PropertyToken::EscapedLineBreak) {
			// Line continuation.
			if (token.token == PropertyToken::Blank) {
				key = extend(key, token.text);
				scannedTokens.consume();
				token = scannedTokens.get();
			}

			switch (token.token) {
				case PropertyToken::Text:
				case PropertyToken::EscapedOpenCurly:
				case PropertyToken::EscapedCloseCurly:
				case PropertyToken::EscapedArobase:
				case PropertyToken::EscapedColon:
				case PropertyToken::EscapedEquals:
				case PropertyToken::EscapedHash:
				case PropertyToken::EscapedExclamation:
				case PropertyToken::EscapedBackSlash:
				case PropertyToken::EscapedBlank:
				case PropertyToken::EscapedChar:
				case PropertyToken::EscapedLineBreak: {
					key = extend(key, token.text);
					scannedTokens.consume();
					token = scannedTokens.get();
					break;
				}

				default: {
					ThrowBalauException(
						Exception::SyntaxErrorException
					, "Invalid input: " + std::string(token.text)
					, token.codeSpan
					);
				}
			}
		}

		bool keyFinished = false;

		while (!keyFinished) {
			switch (token.token) {
				case PropertyToken::EscapedLineBreak: {
					key = extend(key, token.text);
					scannedTokens.consume();
					token = scannedTokens.get();

					if (token.token == PropertyToken::Blank) {
						key = extend(key, token.text);
						scannedTokens.consume();
						token = scannedTokens.get();
					}

					break;
				}

				case PropertyToken::Text:
				case PropertyToken::EscapedOpenCurly:
				case PropertyToken::EscapedCloseCurly:
				case PropertyToken::EscapedArobase:
				case PropertyToken::EscapedColon:
				case PropertyToken::EscapedEquals:
				case PropertyToken::EscapedHash:
				case PropertyToken::EscapedExclamation:
				case PropertyToken::EscapedBackSlash:
				case PropertyToken::EscapedBlank:
				case PropertyToken::EscapedChar:
				case PropertyToken::Hash:
				case PropertyToken::Exclamation: {
					key = extend(key, token.text);
					scannedTokens.consume();
					token = scannedTokens.get();
					break;
				}

				default: {
					keyFinished = true;
					break;
				}
			}
		}

		if (token.token == PropertyToken::Blank) {
			scannedTokens.consume();
			token = scannedTokens.get();
		}

		if (token.token == PropertyToken::Equals || token.token == PropertyToken::Colon) {
			scannedTokens.consume();
			token = scannedTokens.get();
		}

		if (token.token == PropertyToken::Blank) {
			scannedTokens.consume();
			token = scannedTokens.get();
		}

		if (token.token == PropertyToken::LineBreak) {
			// Empty value property.
			CodeSpan endCodeSpan = startToken.codeSpan;
			scannedTokens.consume();

			return std::unique_ptr<AST::PropertyNode>(
				new AST::ValueProperty(CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan), key, std::string_view())
			);
		}

		if (token.token == PropertyToken::OpenCurly) {
			return produceComplex(key);
		}

		return produceSimple(key);
	}

	private: std::unique_ptr<AST::PropertyNode> produceSimple(std::string_view key) {
		// Consume everything until the next non-escaped line break.

		ScannedToken token = scannedTokens.get();
		CodeSpan startCodeSpan = token.codeSpan;
		CodeSpan endCodeSpan = startCodeSpan;
		std::string_view value = token.text;

		scannedTokens.consume();
		token = scannedTokens.get();

		bool valueFinished = false;

		while (!valueFinished) {
			switch (token.token) {
				case PropertyToken::EscapedLineBreak: {
					value = extend(value, token.text);
					endCodeSpan = token.codeSpan;
					scannedTokens.consume();
					token = scannedTokens.get();

					if (token.token == PropertyToken::Blank) {
						value = extend(value, token.text);
						endCodeSpan = token.codeSpan;
						scannedTokens.consume();
						token = scannedTokens.get();
					}

					break;
				}

				case PropertyToken::Text:
				case PropertyToken::EscapedOpenCurly:
				case PropertyToken::EscapedCloseCurly:
				case PropertyToken::EscapedArobase:
				case PropertyToken::EscapedColon:
				case PropertyToken::EscapedEquals:
				case PropertyToken::EscapedHash:
				case PropertyToken::EscapedExclamation:
				case PropertyToken::EscapedBackSlash:
				case PropertyToken::EscapedBlank:
				case PropertyToken::EscapedChar:
				case PropertyToken::Hash:
				case PropertyToken::Exclamation:
				case PropertyToken::CloseCurly:
				case PropertyToken::Colon:
				case PropertyToken::Equals:
				case PropertyToken::Blank:
				case PropertyToken::OpenCurly: {
					value = extend(value, token.text);
					endCodeSpan = token.codeSpan;
					scannedTokens.consume();
					token = scannedTokens.get();
					break;
				}

				case PropertyToken::LineBreak:
				case PropertyToken::EndOfFile: {
					scannedTokens.consume();
					valueFinished = true;
					break;
				}

				default: {
					ThrowBalauException(
						Exception::SyntaxErrorException
					, "Invalid input: unexpected token in simple property."
					, token.codeSpan
					);
				}
			}
		}

		return std::unique_ptr<AST::PropertyNode>(
			new AST::ValueProperty(
				CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan)
				, key
				, value
			)
		);
	}

	private: std::unique_ptr<AST::PropertyNode> produceComplex(std::string_view key) {
		CodeSpan startCodeSpan = scannedTokens.get().codeSpan;
		scannedTokens.consume(); // '{'

		scannedTokens.expect(
			PropertyToken::LineBreak, "Expected line break after opening curly bracket in composite property."
		);

		std::vector<std::unique_ptr<AST::PropertyNode>> nodes = producePropertyNodes(PropertyToken::CloseCurly);

		CodeSpan finishCodeSpan = scannedTokens.get().codeSpan;
		scannedTokens.expect(PropertyToken::CloseCurly, "Expected closing curly bracket in composite property.");

		// Line break or EOF.
		ScannedToken token = scannedTokens.get();

		if (token.token == PropertyToken::LineBreak) {
			scannedTokens.consume();
		} else if (token.token != PropertyToken::EndOfFile) {
			ThrowBalauException(
				Exception::SyntaxErrorException
			, "Invalid input: expected line break or end of file after composite property close bracket."
			, token.codeSpan
			);
		}

		return std::unique_ptr<AST::PropertyNode>(
			new AST::CompositeProperty(CodeSpan::totalCodeSpan(startCodeSpan, finishCodeSpan), key, std::move(nodes))
		);
	}

	private: std::unique_ptr<AST::PropertyNode> produceInclude() {
		scannedTokens.expect(PropertyToken::Arobase, "Bug? Expected arobase token to begin comment production.");
		ScannedToken token = scannedTokens.get();
		CodeSpan startCodeSpan = token.codeSpan;
		std::string_view text = token.text;

		while (token.token != PropertyToken::LineBreak && token.token != PropertyToken::EndOfFile) {
			text = extend(text, token.text);
			scannedTokens.consume();
			token = scannedTokens.get();
		}

		CodeSpan endCodeSpan = token.codeSpan;
		scannedTokens.consume();

		return std::unique_ptr<AST::PropertyNode>(
			new AST::IncludePropertyNode(CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan), text)
		);
	}

	private: std::unique_ptr<AST::PropertyNode> produceComment() {
		scannedTokens.expect(PropertyToken::Hash, "Bug? Expected hash token to begin comment production.");
		ScannedToken token = scannedTokens.get();
		CodeSpan startCodeSpan = token.codeSpan;
		std::string_view text = token.text;

		while (token.token != PropertyToken::LineBreak && token.token != PropertyToken::EndOfFile) {
			text = extend(text, token.text);
			scannedTokens.consume();
			token = scannedTokens.get();
		}

		CodeSpan endCodeSpan = token.codeSpan;
		scannedTokens.consume();

		return std::unique_ptr<AST::PropertyNode>(
			new AST::CommentPropertyNode(CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan), text)
		);
	}

	private: std::unique_ptr<AST::PropertyNode> produceEmptyLine() {
		ScannedToken token = scannedTokens.get();
		CodeSpan codeSpan = token.codeSpan;
		scannedTokens.expect(PropertyToken::LineBreak, "Bug? Expected line break token to begin empty line production.");
		return std::unique_ptr<AST::PropertyNode>(new AST::EmptyLinePropertyNode(codeSpan));
	}

	private: ScannerApiScannedTokens scannedTokens;
};

} // namespace Balau::Lang::Property

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER
