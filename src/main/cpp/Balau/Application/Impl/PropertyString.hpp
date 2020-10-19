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
