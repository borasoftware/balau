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
/// @file PropertyScanner.hpp
///
/// A scanner for the property parser.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_SCANNER
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_SCANNER

#include <Balau/Lang/Common/AbstractScanner.hpp>
#include <Balau/Lang/Property/Lang/PropertyToken.hpp>

#include <stack>

namespace Balau::Lang::Property {

///
/// A scanner for the property parser.
///
class PropertyScanner : public AbstractScanner<PropertyToken> {
	///
	/// Instantiate a scanner with text obtained from the URI.
	///
	public: explicit PropertyScanner(std::shared_ptr<Resource::Uri> uri)
		: AbstractScanner<PropertyToken>(std::move(uri)) {}

	///
	/// Instantiate a scanner with text obtained from the URI.
	///
	public: explicit PropertyScanner(std::unique_ptr<Resource::Uri> && uri)
		: AbstractScanner<PropertyToken>(std::move(uri)) {}

	////////////////////////// Private implementation /////////////////////////

	private: PropertyToken getNextToken() override;
};

} // namespace Balau::Lang::Property

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_SCANNER
