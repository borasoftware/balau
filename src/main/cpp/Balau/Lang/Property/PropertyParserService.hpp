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
