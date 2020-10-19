// @formatter:off
//
// Balau core C++ library
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
