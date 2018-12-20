// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PropertyBindingBuilderFactory.hpp"
#include "../../Exception/InjectorExceptions.hpp"

namespace Balau::Impl {

// The map of creators by type string.
std::map<std::string, PropertyBindingBuilderFactoryPtr> propertyBindingBuilderFactoryFactoriesByTypeString = {
	  std::pair("byte",    PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed char>("")))
	, std::pair("short",   PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed short>("")))
	, std::pair("int",     PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed int>("")))
	, std::pair("long",    PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed long long>("")))

	, std::pair("float",   PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<float>("")))
	, std::pair("double",  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<double>("")))

	, std::pair("string",  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<std::string>("")))
	, std::pair("char",    PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<char>("")))
	, std::pair("boolean", PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<bool>("")))

	, std::pair(
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
	  std::pair(std::type_index(typeid(signed char)),  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed char>("")))
	, std::pair(std::type_index(typeid(signed short)), PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed short>("")))
	, std::pair(std::type_index(typeid(signed int)),   PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed int>("")))
	, std::pair(std::type_index(typeid(signed long)),  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<signed long long>("")))

	, std::pair(std::type_index(typeid(float)),        PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<float>("")))
	, std::pair(std::type_index(typeid(double)),       PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<double>("")))

	, std::pair(std::type_index(typeid(std::string)),  PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<std::string>("")))
	, std::pair(std::type_index(typeid(char)),         PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<char>("")))
	, std::pair(std::type_index(typeid(bool)),         PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<bool>("")))

	, std::pair(
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
	propertyBindingBuilderFactoryFactoriesByTypeString.insert(std::pair(typeString, factory));
	propertyBindingBuilderFactoryFactoriesByTypeId.insert(std::pair(typeIndex, factory));
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
