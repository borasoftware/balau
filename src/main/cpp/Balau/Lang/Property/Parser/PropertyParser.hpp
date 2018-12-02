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
	public: AST::Properties parse();

	////////////////////////// Private implementation /////////////////////////

	private: std::vector<std::unique_ptr<AST::PropertyNode>> producePropertyNodes(PropertyToken endToken);
	private: std::unique_ptr<AST::PropertyNode> produceProperty();
	private: std::unique_ptr<AST::PropertyNode> produceSimple(std::string_view key);
	private: std::unique_ptr<AST::PropertyNode> produceComplex(std::string_view key);
	private: std::unique_ptr<AST::PropertyNode> produceInclude();
	private: std::unique_ptr<AST::PropertyNode> produceComment();
	private: std::unique_ptr<AST::PropertyNode> produceEmptyLine();

	private: ScannerApiScannedTokens scannedTokens;
};

} // namespace Balau::Lang::Property

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER
