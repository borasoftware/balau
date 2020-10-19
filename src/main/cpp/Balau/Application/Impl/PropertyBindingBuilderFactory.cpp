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

#include "PropertyBindingBuilderFactory.hpp"
#include "../../Exception/InjectorExceptions.hpp"

namespace Balau::Impl {

// The map of creators by type string.
std::map<std::string, PropertyBindingBuilderFactoryPtr> propertyBindingBuilderFactoryFactoriesByTypeString = {
	  std::pair<std::string, PropertyBindingBuilderFactoryPtr>("byte",    PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed char>("")))
	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("short",   PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed short>("")))
	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("int",     PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed int>("")))
	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("long",    PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed long long>("")))

	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("float",   PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<float>("")))
	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("double",  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<double>("")))

	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("string",  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<std::string>("")))
	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("char",    PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<char>("")))
	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>("boolean", PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<bool>("")))

	, std::pair<std::string, PropertyBindingBuilderFactoryPtr>(
		  "uri"
		, PropertyBindingBuilderFactoryPtr(
			new UniquePropertyBindingBuilderFactory<Resource::Uri>(
				  ""
				, [] (const std::unique_ptr<const Resource::Uri> & uri) -> std::unique_ptr<Resource::Uri> {
					return uri->clone();
				}
			)
		)
	)
};

// The map of creators by type id.
std::map<std::type_index, PropertyBindingBuilderFactoryPtr> propertyBindingBuilderFactoryFactoriesByTypeId = {
	  std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(signed char)),  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed char>("")))
	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(signed short)), PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed short>("")))
	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(signed int)),   PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed int>("")))
	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(signed long)),  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed long long>("")))

	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(float)),        PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<float>("")))
	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(double)),       PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<double>("")))

	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(std::string)),  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<std::string>("")))
	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(char)),         PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<char>("")))
	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(std::type_index(typeid(bool)),         PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<bool>("")))

	, std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(
		  std::type_index(typeid(Resource::Uri))
		, PropertyBindingBuilderFactoryPtr(
			new UniquePropertyBindingBuilderFactory<Resource::Uri>(
				  ""
				, [] (const std::unique_ptr<const Resource::Uri> & uri) -> std::unique_ptr<Resource::Uri> {
					return uri->clone();
				}
			)
		)
	)
};

void registerEnvironmentPropertyType(const std::string & typeString,
                                     std::type_index typeIndex,
                                     PropertyBindingBuilderFactoryPtr factory) {
	static std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	propertyBindingBuilderFactoryFactoriesByTypeString.insert(std::pair<std::string, PropertyBindingBuilderFactoryPtr>(typeString, factory));
	propertyBindingBuilderFactoryFactoriesByTypeId.insert(std::pair<std::type_index, PropertyBindingBuilderFactoryPtr>(typeIndex, factory));
}

void registerEnvironmentPropertyUnsignedTypes() {
	registerEnvironmentPropertyType("unsigned byte",  typeid(unsigned char),      PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<unsigned char>("")));
	registerEnvironmentPropertyType("unsigned short", typeid(unsigned short),     PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<unsigned short>("")));
	registerEnvironmentPropertyType("unsigned int",   typeid(unsigned int),       PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<unsigned int>("")));
	registerEnvironmentPropertyType("unsigned long",  typeid(unsigned long long), PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<unsigned long long>("")));
}

PropertyBindingBuilderFactoryPtr getBindingBuilderFactory(const std::string & typeString,
                                                          const std::string & name,
                                                          const std::string & defaultValueString) {
	auto iter = propertyBindingBuilderFactoryFactoriesByTypeString.find(typeString);

	if (iter == propertyBindingBuilderFactoryFactoriesByTypeString.end()) {
		ThrowBalauException(
			  Exception::EnvironmentConfigurationException
			, ::toString("No registered property type for type string '", typeString, "'.")
		);
	}

	auto factory = iter->second->clone(name, defaultValueString);

	return factory;
}

PropertyBindingBuilderFactoryPtr getBindingBuilderFactory(const std::type_index typeIndex,
                                                          const std::string & name,
                                                          const std::string & defaultValueString) {
	auto iter = propertyBindingBuilderFactoryFactoriesByTypeId.find(typeIndex);

	if (iter == propertyBindingBuilderFactoryFactoriesByTypeId.end()) {
		ThrowBalauException(
			  Exception::EnvironmentConfigurationException
			, ::toString("No registered property type for type string '", boost::core::demangle(typeIndex.name()), "'.")
		);
	}

	auto factory = iter->second->clone(name, defaultValueString);

	return factory;
}

} // namespace Balau::Impl
