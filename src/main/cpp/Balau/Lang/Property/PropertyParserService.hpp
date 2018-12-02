// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file PropertyParserService.hpp
///
/// Parser service for property file parsing.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY__PROPERTY_PARSER_SERVICE
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY__PROPERTY_PARSER_SERVICE

#include <Balau/Lang/Property/Parser/PropertyParser.hpp>
#include <Balau/Lang/Property/Lang/PropertyAst.hpp>

#include <stack>

namespace Balau::Lang::Property {

///
/// Encapsulates the creation and execution of a pre-processor, scanner, and parser in a single function.
///
class PropertyParserService final {
	///
	/// Parse the input represented by the URI and generate a property AST tree.
	///
	public: static AST::Properties parse(const std::shared_ptr<Resource::Uri> & uri) {
		PropertyScanner scanner(uri);
		PropertyParser parser(ScannerApiScannedTokens<PropertyToken>(scanner.scan()));
		return parser.parse();
	}

	///
	/// Parse the input represented by the URI and generate a property AST tree.
	///
	public: static AST::Properties parse(const Resource::Uri & uri) {
		PropertyScanner scanner(uri.clone());
		PropertyParser parser(ScannerApiScannedTokens<PropertyToken>(scanner.scan()));
		return parser.parse();
	}

	///////////////////////////////////////////////////////////////////////////

	public: PropertyParserService() = delete;
	public: PropertyParserService(const PropertyParserService &) = delete;
	public: PropertyParserService & operator = (const PropertyParserService &) = delete;
};

} // namespace Balau::Lang::Property

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY__PROPERTY_PARSER_SERVICE
