// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "LoggingConfigurationParsing.hpp"

namespace Balau::LoggingSystem::LoggingConfigurationParsing {

ConfigurationScanner::ConfigurationScanner(std::istream & configurationText)
	: Lang::AbstractScanner<Token>(std::shared_ptr<Resource::Uri>(), configurationText) {}

Token ConfigurationScanner::getNextToken() {
	readNextChar();

	if (currentChar == U'#') {
		do {
			readNextChar();
		} while (currentChar != U'\r' && currentChar != U'\n' && currentChar != std::char_traits<char32_t>::eof());

		if (currentChar == std::char_traits<char32_t>::eof()) {
			putBackCurrentChar();
		} else if (currentChar == U'\n') {
			// Optional newline with carriage return.
			readNextChar();

			if (currentChar != U'\r') {
				putBackCurrentChar();
			}
		} else { // '\r'
			// Optional carriage return with newline.
			readNextChar();

			if (currentChar != U'\n') {
				putBackCurrentChar();
			}
		}

		return Token::CommentLine;
	}

	if (Balau::Character::isBlank(currentChar)) {
		return createBlankToken();
	}

	switch (currentChar) {
		case std::char_traits<char32_t>::eof(): {
			return Token::EndOfFile;
		}

		case U'.': {
			return Token::Dot;
		}

		case U'=': {
			return Token::Equals;
		}

		case U',': {
			return Token::Comma;
		}

		case U':': {
			return Token::Colon;
		}

		case U'"': {
			extractStringConstantDoubleQuotes();
			return Token::String;
		}

		case U'\n':
		case U'\r': {
			return createLineBreakToken();
		}

		default: {
			// Non-quoted string (no whitespace, dots, equals, commas, or colons).
			do {
				readNextChar();
			} while (
				!Balau::Character::isWhitespace(currentChar)
				&& currentChar != '.'
				&& currentChar != '='
				&& currentChar != ','
				&& currentChar != ':'
				&& currentChar != std::char_traits<char32_t>::eof()
				);

			putBackCurrentChar();
			return Token::String;
		}
	}
}

ConfigurationParser::ConfigurationParser(Lang::ScannedTokens<Token> && scannedTokens_)
	: scannedTokens(std::move(scannedTokens_)) {}

Entries ConfigurationParser::parse() {
	scannedTokens.pushWhitespaceMode(Lang::WhitespaceMode::ConsumeWhitespaceAndComments);
	Entries entries = produceEntrySet();
	scannedTokens.popWhitespaceMode();
	return entries;
}

Namespace ConfigurationParser::parseNamespace() {
	scannedTokens.pushWhitespaceMode(Lang::WhitespaceMode::ConsumeWhitespaceAndComments);
	Namespace ns = produceNamespace();
	scannedTokens.popWhitespaceMode();
	return ns;
}

Entries ConfigurationParser::produceEntrySet() {
	Lang::ScannedToken<Token> token = scannedTokens.get();
	std::vector<Entry> entries;

	while (token.token != Token::EndOfFile) {
		entries.emplace_back(produceEntry());
		token = scannedTokens.get();
	}

	return Entries(std::move(entries));
}

Entry ConfigurationParser::produceEntry() {
	Namespace nameSpace = produceNamespace();
	scannedTokens.expect(Token::Equals, "Expected '=' between logging namespace and property list.");
	Properties properties = produceProperties();
	return Entry(std::move(nameSpace), std::move(properties));
}

Namespace ConfigurationParser::produceNamespace() {
	std::vector<std::string> identifiers;
	Lang::ScannedToken<Token> token = scannedTokens.get();

	if (token.token == Token::Dot) {
		// Special global empty namespace.
		scannedTokens.consume();
	} else if (token.token == Token::EndOfFile) {
		// Special global empty namespace.
	} else {
		identifiers.emplace_back(token.text);
		scannedTokens.expect(Token::String, "Expected identifier in namespace.");

		if (!std::regex_match(std::string(token.text), namespaceRegex)) {
			ThrowBalauException(
				  Exception::SyntaxErrorException
				, "Illegal namespace component found in configuration: " + std::string(token.text)
				, Lang::CodeSpan()
			);
		}

		token = scannedTokens.get();

		while (token.token == Token::Dot) {
			scannedTokens.consume();
			token = scannedTokens.get();
			identifiers.emplace_back(token.text);
			scannedTokens.expect(Token::String, "Expected identifier in namespace.");

			if (!std::regex_match(identifiers.back(), namespaceRegex)) {
				ThrowBalauException(
					  Exception::SyntaxErrorException
					, "Illegal namespace component found in configuration: " + std::string(token.text)
					, Lang::CodeSpan()
				);
			}

			token = scannedTokens.get();
		}
	}

	return Namespace(std::move(identifiers));
}

Properties ConfigurationParser::produceProperties() {
	std::vector<Property> properties;
	properties.emplace_back(produceProperty());
	Lang::ScannedToken<Token> token = scannedTokens.get();

	while (token.token == Token::Comma) {
		scannedTokens.consume();
		properties.emplace_back(produceProperty());
		token = scannedTokens.get();
	}

	return Properties(std::move(properties));
}

Property ConfigurationParser::produceProperty() {
	Lang::ScannedToken<Token> token = scannedTokens.get();
	std::string_view name = token.text;
	scannedTokens.expect(Token::String, "Expected property name.");

	// Not a static constant due to the logger parser being run from a static context.
	const std::set<std::string> validProperties {
		  "level"
		, "format"
		, "stream"
		, "trace-stream"
		, "debug-stream"
		, "info-stream"
		, "warn-stream"
		, "error-stream"
	};

	if (validProperties.find(std::string(name)) == validProperties.end()) {
		ThrowBalauException(
			  Exception::SyntaxErrorException
			, "Unknown property encountered in configuration: " + std::string(name)
			, Lang::CodeSpan()
		);
	}

	scannedTokens.expect(Token::Colon, "Expected colon between property name and value.");
	token = scannedTokens.get();
	std::string_view value = token.text;
	scannedTokens.expect(Token::String, "Expected property value.");

	// Remove quotes if present.
	//if (value.length() >= 2 && Util::Strings::startsWith(value, "\"") && Util::Strings::endsWith(value, "\"")) {
	//	value = value.substr(1, value.length() - 2);
	//}

	return Property(std::string(name), std::string(value));
}

Namespace parseNamespace(const std::string & namespaceText) {
	std::istringstream input(namespaceText);
	ConfigurationScanner scanner(input);
	return ConfigurationParser(scanner.scan()).parseNamespace();
}

} // namespace Balau::LoggingSystem::LoggingConfigurationParsing
