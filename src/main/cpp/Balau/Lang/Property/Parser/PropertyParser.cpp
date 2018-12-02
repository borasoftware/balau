// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PropertyParser.hpp"
#include "../../../Exception/ParsingExceptions.hpp"
#include "../../../Dev/Assert.hpp"

namespace Balau {

using namespace Util;

namespace Lang::Property {

using namespace AST;

Properties PropertyParser::parse() {
	scannedTokens.pushWhitespaceMode(WhitespaceMode::DoNotConsume);
	ScannedToken token = scannedTokens.get();
	CodeSpan startCodeSpan = token.codeSpan;
	auto nodes = producePropertyNodes(PropertyToken::EndOfFile);
	scannedTokens.popWhitespaceMode();
	CodeSpan finishCodeSpan = token.codeSpan;

	return Properties(
		  CodeSpan::totalCodeSpan(startCodeSpan, finishCodeSpan)
		, std::move(scannedTokens.moveTextOut())
		, std::move(nodes)
	);
}

std::vector<std::unique_ptr<PropertyNode>> PropertyParser::producePropertyNodes(PropertyToken endToken) {
	std::vector<std::unique_ptr<PropertyNode>> nodes;
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

			default: {
				ThrowBalauException(
					Exception::SyntaxErrorException
				, "Invalid input: " + std::string(token.text)
				, token.codeSpan
				);
			}
		}

		token = scannedTokens.get();
	}

	return nodes;
}

inline std::string_view extend(const std::string_view & start, const std::string_view & finish) {
	return std::string_view(start.data(), finish.data() - start.data() + finish.length());
}

std::unique_ptr<PropertyNode> PropertyParser::produceProperty() {
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

		return std::unique_ptr<PropertyNode>(
			new ValueProperty(CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan), key, std::string_view())
		);
	}

	if (token.token == PropertyToken::OpenCurly) {
		return produceComplex(key);
	}

	return produceSimple(key);
}

std::unique_ptr<PropertyNode> PropertyParser::produceSimple(std::string_view key) {
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

			case PropertyToken::LineBreak: {
				scannedTokens.consume();
				valueFinished = true;
				break;
			}

			default: {
				break;
			}
		}
	}

	return std::unique_ptr<PropertyNode>(
		new ValueProperty(
			  CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan)
			, key
			, value
		)
	);
}

std::unique_ptr<PropertyNode> PropertyParser::produceComplex(std::string_view key) {
	CodeSpan startCodeSpan = scannedTokens.get().codeSpan;
	scannedTokens.consume(); // '{'

	scannedTokens.expect(
		PropertyToken::LineBreak, "Expected line break after opening curly bracket in composite property."
	);

	std::vector<std::unique_ptr<PropertyNode>> nodes = producePropertyNodes(PropertyToken::CloseCurly);

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

	return std::unique_ptr<PropertyNode>(
		new CompositeProperty(CodeSpan::totalCodeSpan(startCodeSpan, finishCodeSpan), key, std::move(nodes))
	);
}

std::unique_ptr<PropertyNode> PropertyParser::produceInclude() {
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

	return std::unique_ptr<PropertyNode>(
		new IncludePropertyNode(CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan), text)
	);
}

std::unique_ptr<PropertyNode> PropertyParser::produceComment() {
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

	return std::unique_ptr<PropertyNode>(
		new CommentPropertyNode(CodeSpan::totalCodeSpan(startCodeSpan, endCodeSpan), text)
	);
}

std::unique_ptr<PropertyNode> PropertyParser::produceEmptyLine() {
	ScannedToken token = scannedTokens.get();
	CodeSpan codeSpan = token.codeSpan;
	scannedTokens.expect(PropertyToken::LineBreak, "Bug? Expected line break token to begin empty line production.");
	return std::unique_ptr<PropertyNode>(new EmptyLinePropertyNode(codeSpan));
}

} // namespace Lang::Property

} // namespace Balau
