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
