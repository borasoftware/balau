// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_STRINGS
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_STRINGS

#include <Balau/Container/ObjectTrie.hpp>
#include <Balau/Lang/Property/Util/PropertyVisitor.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>

namespace Balau {

namespace Impl {

//
// Holds property string data:
//  - name and value for simple properties;
//  - name only for composite properties.
//
struct PropertyString {
	enum class Type { Value, Composite, NotSet };

	Type type;
	std::string name;
	std::string value;

	PropertyString(std::string name_, std::string value_)
		: type(Type::Value)
		, name(std::move(name_))
		, value(std::move(value_)) {}

	PropertyString(std::string name_)
		: type(Type::Composite)
		, name(std::move(name_)) {}

	PropertyString()
		: type(Type::NotSet) {}

	PropertyString(const PropertyString & ) = default;
	PropertyString(PropertyString && ) = default;

	PropertyString & operator = (const PropertyString & ) = default;
	PropertyString & operator = (PropertyString && ) = default;
};

inline bool operator == (const PropertyString & lhs, const PropertyString & rhs) {
	return lhs.type == rhs.type && lhs.name == rhs.name;
}

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_STRINGS
