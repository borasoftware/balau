// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_CONFIGURATION_PARSING
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_CONFIGURATION_PARSING

#include <Balau/Lang/Common/AbstractScanner.hpp>

namespace Balau::LoggingSystem::LoggingConfigurationParsing {

//
// ////// CONFIGURATION GRAMMAR //////
//
// S          = Entries
// Entries    = Entry*
// Entry      = Namespace '=' Properties
// Namespace  = '.' | IDENTIFIER ('.' IDENTIFIER)*
// Properties = Property (',' Property)*
// Property   = IDENTIFIER ':' VALUE
//
// IDENTIFIER = [a-zA-Z][a-zA-Z0-9_]*(\.[a-zA-Z][a-zA-Z0-9_]*)*
// VALUE      = [a-zA-Z0-9_\.+-\\/!"£$^&*\(\);'@<>?{}~%\[\]]+
//            | '"' [a-zA-Z0-9_\.+-\\/!"£$^&*\(\);'@<>?{}~%\[\] :,]+ '"'
//
// ////// CONFIGURATION EXAMPLE //////
//
//    .          = level: warn
//               , stream: stdout
//               , format: "%Y-%m-%d %H:%M:%S [%thread] %level - %ns - %message"
//
//    com.borasoftware   = level: info
//
//    com.borasoftware.a = level: debug
//

////////////////////////////// Language tokens  ///////////////////////////////

// The language tokens.
enum class Token {
	  String
	, Dot
	, Equals
	, Comma
	, Colon
	, CommentLine
	, EndOfFile
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
	, CommentBlock // Required for scanner
	, Blank        // Required for scanner
	, LineBreak    // Required for scanner
	#pragma clang diagnostic pop
};

///////////////////////////////// AST classes /////////////////////////////////

class Namespace final {
	public: std::vector<std::string> identifiers;

	public: explicit Namespace(std::vector<std::string> && identifiers_)
		: identifiers(std::move(identifiers_)) {}

	public: Namespace(Namespace && rhs) noexcept
		: identifiers(std::move(rhs.identifiers)) {}

	public: Namespace(const Namespace & copy) = default;

	public: std::string join() const {
		std::string ret;
		std::string prefix;

		for (const std::string & identifier : identifiers) {
			ret += prefix + identifier;
			prefix = ".";
		}

		return ret;
	}

	public: bool operator < (const Namespace & rhs) const {
		return identifiers < rhs.identifiers;
	}
};

class Property final {
	public: std::string name;
	public: std::string value;

	public: explicit Property(std::string && name_, std::string && value_)
		: name(std::move(name_))
		, value(std::move(value_)) {}

	public: Property(Property && rhs) noexcept
		: name(std::move(rhs.name))
		,value(std::move(rhs.value)) {}
};

class Properties final {
	public: std::vector<Property> properties;

	public: explicit Properties(std::vector<Property> && properties_)
		: properties(std::move(properties_)) {}

	public: Properties(Properties && rhs) noexcept
		: properties(std::move(rhs.properties)) {}
};

class Entry final {
	public: Namespace nameSpace;
	public: Properties properties;

	public: Entry(Namespace && nameSpace_, Properties && properties_)
		: nameSpace(std::move(nameSpace_))
		,properties(std::move(properties_)) {}

	public: Entry(Entry && rhs) noexcept
		: nameSpace(std::move(rhs.nameSpace))
		,properties(std::move(rhs.properties)) {}
};

class Entries final {
	public: std::vector<Entry> entries;

	public: explicit Entries(std::vector<Entry> && entries_)
		: entries(std::move(entries_)) {}

	public: Entries(Entries && rhs) noexcept
		: entries(std::move(rhs.entries)) {}
};

/////////////////////////////////// Scanner ///////////////////////////////////

class ConfigurationScanner final : public Lang::AbstractScanner<Token> {
	public: explicit ConfigurationScanner(std::istream & configurationText);

	private: Token getNextToken() override;
};

//////////////////////////////////// Parser ///////////////////////////////////

class ConfigurationParser final {
	private: Lang::ScannerApiScannedTokens<Token> scannedTokens;
	private: std::regex namespaceRegex = std::regex("[a-zA-Z][a-zA-Z0-9_]*");

	public: explicit ConfigurationParser(Lang::ScannedTokens<Token> && scannedTokens_);

	// Parse complete configuration text from scanner.
	public: Entries parse();

	// Parse single namespace from scanner.
	public: Namespace parseNamespace();

	private: Entries produceEntrySet();
	private: Entry produceEntry();
	private: Namespace produceNamespace();
	private: Properties produceProperties();
	private: Property produceProperty();
};

// Use the parser to create a namespace object from the supplied namespace text.
Namespace parseNamespace(const std::string & namespaceText);

} // namespace Balau::LoggingSystem::LoggingConfigurationParsing

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_CONFIGURATION_PARSING
