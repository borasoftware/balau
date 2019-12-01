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
/// @file EnvironmentConfiguration.hpp
///
/// Environment configurations specify environment injector bindings via properties files.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_CONFIGURATION
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_CONFIGURATION

#include <Balau/Application/BindingBuilder.hpp>
#include <Balau/Application/InjectorConfiguration.hpp>
#include <Balau/Application/Impl/EnvironmentConfigurationBuilder.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>
#include <Balau/Resource/StringUri.hpp>
#include <Balau/Util/Streams.hpp>
#include <Balau/Util/Vectors.hpp>

// Avoid false positives.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau {

///
/// %Environment configurations specify typed and untyped environment injector bindings.
///
/// Implementations of this class can be used to load simple or hierarchical properties
/// files into an application's injector, in order to parameterise the configuration of
/// an application according to the specific environment.
///
/// There are two ways to use the environment configuration class:
///
///   - derive from the %EnvironmentConfiguration class and hard wire property type
///     specification declarations in the configure() method of the derived class;
///
///   - instantiate an %EnvironmentConfiguration instance by specifying one or more
///     type specification properties files.
///
/// The two approaches can be mixed if required, by deriving from
/// EnvironmentConfiguration and passing one or more type specification properties
/// files to the base class constructor.
///
/// The instantiated %EnvironmentConfiguration or derived class(es) should then be
/// passed to the injector create function along with the application configuration
/// instance(s).
///
/// Each instance of %EnvironmentConfiguration or derived classes should be provided
/// with a properties file input. During injector instantiation, a set of named
/// value and shared bindings are created. These bindings represent the simple and
/// composite properties in the properties file.
///
/// Typically, environment properties are sourced from one or more properties files
/// on the local disk. Providing paths to the environment's property file(s) can be
/// achieved by various methods, a couple of simple ones being:
///
///  - passing the environment's home directory via a command line argument and hard
///    wiring the environment properties file name(s) in code;
///
///  - passing an environment identifier via a command line argument and resolving
///    the home directory and environment configuration file path(s) accordingly.
///
/// Once the environment property file path(s) have been obtained, one or more
/// corresponding %EnvironmentConfiguration instance(s) can be created and passed to
/// the injector.
///
/// The %EnvironmentConfiguration class is designed to fail immediately if the property
/// text is not well formed, preventing the application from starting up with an
/// invalid configuration.
///
/// If a property type has been registered for a property name, the subsequent creation
/// of a property binding for that property will result in a named value binding of that
/// type being created if the property is simple. The value will be created via a call
/// to the fromString function and placed within the named value binding. For all simple
/// properties in the properties file that do not have types registered for them, a
/// std::string named value binding is created.
///
/// All root composite properties in the properties file will result in injector named
/// bindings being created of type EnvironmentProperties. Any property types registered
/// for these property names will be ignored (a warning is also logged).
///
/// All simple and composite properties defined as descendants of root composite
/// properties result in named value and shared bindings being created directly or
/// indirectly within the root composite properties bindings in the injector. Refer to
/// the EnvironmentProperties class for more information on the bindings created for
/// descendant properties.
///
class EnvironmentConfiguration : public InjectorConfiguration {
	//////////////////////////// Type registration ////////////////////////////

	///
	/// Add a non-polymorphic type custom property binding builder factory to the global property binding builder factory map.
	///
	/// Instances of the non-polymorphic (value) type will be created via prototype
	/// copying.
	///
	/// The type must have a corresponding fromString function, otherwise there will
	/// be a compilation error.
	///
	/// This function must be called before the injector is instantiated, in order
	/// to define the custom types.
	///
	/// @tparam ValueT the type of the property's value
	/// @param typeString the string identification of the type
	/// @param factory a pointer to the factory to return when typeString is requested
	///
	public: template <typename ValueT>
	static void registerValueType(const std::string & typeString) {
		Impl::registerEnvironmentPropertyValueType<ValueT>(typeString);
	}

	///
	/// Add a polymorphic type custom property binding builder factory to the global property binding builder factory map.
	///
	/// Instances of the polymorphic (unique) type will be created by calling the
	/// supplied cloner function.
	///
	/// The type must have a corresponding fromString function, otherwise there will
	/// be a compilation error.
	///
	/// This function must be called before the injector is instantiated, in order
	/// to define the custom types.
	///
	/// @tparam BaseT the type of the property's value
	/// @param typeString the string identification of the type
	/// @param factory a pointer to the factory to return when typeString is requested
	/// @param cloner a function that is used to clone the value
	///
	public: template <typename BaseT>
	static void registerUniqueType(const std::string & typeString, const UniquePropertyCloner<BaseT> & cloner) {
		Impl::registerEnvironmentPropertyUniqueType<BaseT>(typeString, cloner);
	}

	///
	/// Register C++ specific unsigned integer value type property binding builder factories in the global property binding builder factory map.
	///
	/// These types are not pre-registered as they are not listed in the environment
	/// configuration pre-registered type list.
	///
	/// The additional types registered are:
	///
	///  - unsigned byte  (type = unsigned char);
	///  - unsigned short (type = unsigned short);
	///  - unsigned int   (type = unsigned int);
	///  - unsigned long  (type = unsigned long long).
	///
	/// This function must be called before the injector is instantiated, in order
	/// to define the custom types.
	///
	public: static void registerUnsignedTypes() {
		Impl::registerEnvironmentPropertyUnsignedTypes();
	}

	////////////////////////////// Construction ///////////////////////////////

	///
	/// Create an environment configuration by specifying a vector of URIs pointing to the source properties texts and a vector of URIs pointing to type specification texts.
	///
	/// @param inputs a vector of URIs pointing to the source properties texts
	/// @param specs a vector of URIs pointing to the type specification texts
	///
	public: EnvironmentConfiguration(const std::vector<std::shared_ptr<Resource::Uri>> & inputs,
	                                 const std::vector<std::shared_ptr<Resource::Uri>> & specs = {})
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings(inputs))
		, typeSpecificationsUris(specs) {}

	///
	/// Create an environment configuration by specifying a vector of files pointing to the source properties texts and a vector of files pointing to type specification texts.
	///
	/// This convenience constructor allows file URIS to be supplied by value instead of via pointer containers.
	///
	/// @param inputs a vector of files pointing to the source properties texts
	/// @param specs a vector of files pointing to the type specification texts
	///
	public: EnvironmentConfiguration(const std::vector<Resource::File> & inputs,
	                                 const std::vector<Resource::File> & specs = {})
		: EnvironmentConfiguration(
			Util::Vectors::map<std::shared_ptr<Resource::Uri>, Resource::File>(
				inputs, [] (const auto & file) { return std::shared_ptr<Resource::Uri>(new Resource::File(file)); }
			)
			, Util::Vectors::map<std::shared_ptr<Resource::Uri>, Resource::File>(
				specs, [] (const auto & file) { return std::shared_ptr<Resource::Uri>(new Resource::File(file)); }
			)
		) {}

	///
	/// Create an environment configuration by specifying the source properties file and the property types via a properties file.
	///
	/// @tparam FileT the Resource::File type for moreTypeSpecifications
	/// @param input the path to the properties file to parse
	/// @param firstTypeSpecifications the first properties type specifications file
	/// @param moreTypeSpecifications more properties type specifications files
	///
	public: template <typename ... UriT>
	EnvironmentConfiguration(const Resource::Uri & input,
	                         const Resource::Uri & firstTypeSpecifications,
	                         const UriT & ... moreTypeSpecifications)
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings({ input.clone() }))
		, typeSpecificationsUris(
			Util::Vectors::pushBack(
				  std::shared_ptr<Resource::Uri>(firstTypeSpecifications.clone())
				, std::shared_ptr<Resource::Uri>(moreTypeSpecifications.clone()) ...
			)
		) {}

	///
	/// Create an environment configuration by specifying a URI pointing to the source properties text and the property types via a properties file.
	///
	/// @tparam UriT the std::shared_ptr<Resource::Uri> type for moreTypeSpecifications
	/// @param input the URI pointing to the source properties text
	/// @param firstTypeSpecifications the first properties type specifications source
	/// @param moreTypeSpecifications more properties type specifications sources
	///
	public: template <typename ... UriT>
	EnvironmentConfiguration(const std::shared_ptr<Resource::Uri> & input,
	                         const std::shared_ptr<Resource::Uri> & firstTypeSpecifications,
	                         const UriT & ... moreTypeSpecifications)
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings({ input }))
		, typeSpecificationsUris(Util::Vectors::pushBack(firstTypeSpecifications, moreTypeSpecifications ...)) {}

	///
	/// Create an environment configuration by specifying a string for the source properties text and the property types via a properties file.
	///
	/// @param input the properties text
	/// @param typeSpecification the properties type specifications
	///
	public: EnvironmentConfiguration(const std::string & input, const std::string & typeSpecifications_)
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings({ std::shared_ptr<Resource::Uri>(new Resource::StringUri(input)) }))
		, typeSpecificationsUris(Util::Vectors::pushBack(std::shared_ptr<Resource::Uri>(new Resource::StringUri(input)))) {}

	///
	/// Create an environment configuration by specifying the source properties file.
	///
	/// @param input the path to the properties file to parse
	///
	public: EnvironmentConfiguration(const Resource::Uri & input)
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings({ input.clone() })) {}

	///
	/// Create an environment configuration by specifying a URI pointing to the source properties text.
	///
	/// @param input the URI pointing to the source properties text
	///
	public: EnvironmentConfiguration(const std::shared_ptr<Resource::Uri> & input)
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings({ input })) {}

	///
	/// Create an environment configuration by specifying a string for the source properties text.
	///
	/// @param input the properties text
	///
	public: EnvironmentConfiguration(const std::string & input)
		: propertyStringsHierarchy(Impl::EnvironmentConfigurationBuilderUtils::createPropertyStrings({ std::shared_ptr<Resource::Uri>(new Resource::StringUri(input)) })) {}

	/////////////// Hard wired type specification declarations ////////////////

	///
	/// Declare a composite property specification in the environment configuration.
	///
	/// This call is used when the declaration needs to state that the composite property is required.
	///
	/// @tparam FactoryPtrT the intermediate binding builder factory pointer type
	/// @param name the name of the composite property
	/// @param tokens the child property specifications
	/// @return a transiently defined object used within the fluent declaration call
	///
	protected: template <typename ... FactoryPtrT>
	Impl::PropertyBindingBuilderFactoryPtr group(std::string name, FactoryPtrT ... tokens) const {
		auto factory = Impl::PropertyBindingBuilderFactoryPtr(
			new Impl::CompositePropertyBindingBuilderFactory(std::move(name))
		);

		allBindingBuilderFactories.emplace_back(factory);

		auto composite = std::static_pointer_cast<Impl::CompositePropertyBindingBuilderFactory>(factory);
		Impl::EnvironmentConfigurationBuilderUtils::addToComposite(*composite, tokens ...);
		Impl::EnvironmentConfigurationBuilderUtils::incrementLevel(tokens ...);

		return factory;
	}

	///
	/// Declare a non-polymorphic value property specification in the environment configuration.
	///
	/// @tparam ValueT the value type that the property value will be converted to
	/// @param name the name of the value property
	/// @return a transiently defined object used within the fluent declaration call
	///
	protected: template <typename ValueT = std::string>
	Impl::PropertyBindingBuilderFactoryPtr value(std::string name) const {
		auto factory = Impl::PropertyBindingBuilderFactoryPtr(
			new Impl::ValuePropertyBindingBuilderFactory<ValueT>(std::move(name))
		);

		allBindingBuilderFactories.emplace_back(factory);
		return factory;
	}

	///
	/// Declare a non-polymorphic value property specification in the environment configuration, specifying a default value.
	///
	/// This overload should be used when a default value for the property should be
	/// used when no such property is provided in the environment properties file.
	///
	/// @tparam ValueT the value type that the property value will be converted to
	/// @param name the name of the value property
	/// @param defaultValue the value used in the resulting binding if the property is not set
	/// @return a transiently defined object used within the fluent declaration call
	///
	protected: template <typename ValueT = std::string>
	Impl::PropertyBindingBuilderFactoryPtr value(std::string name, ValueT defaultValue) const {
		auto factory = Impl::PropertyBindingBuilderFactoryPtr(
			new Impl::ValuePropertyBindingBuilderFactory<ValueT>(std::move(name), defaultValue)
		);

		allBindingBuilderFactories.emplace_back(factory);
		return factory;
	}

	///
	/// Declare a polymorphic "unique" value property specification in the environment configuration.
	///
	/// A binding builder factory (that supplies the unique pointer cloner for this type) will
	/// be required to have been registered for custom types.
	///
	/// If no entry is found in the binding builder factory map for the specified type,
	/// an exception will be thrown.
	///
	/// @tparam BaseT the base type of the class that the property value will be converted to
	/// @param name the name of the value property
	/// @return a transiently defined object used within the fluent declaration call
	/// @throw EnvironmentConfigurationException if no binding builder factory map for the specified type has been registered
	///
	protected: template <typename BaseT>
	Impl::PropertyBindingBuilderFactoryPtr unique(std::string name) const {
		Impl::PropertyBindingBuilderFactoryPtr factory = Impl::getBindingBuilderFactory(typeid(BaseT), name, "");
		allBindingBuilderFactories.emplace_back(factory);
		return factory;
	}

	///
	/// Declare a polymorphic "unique" value property specification in the environment configuration, specifying a default value.
	///
	/// The unique pointer cloner function will be looked up in the registered factory map.
	/// If no entry is found in the factory map, an exception will be thrown.
	///
	/// This overload should be used when a default value for the property should be
	/// used when no such property is provided in the environment properties file.
	///
	/// @tparam BaseT the base type of the class that the property value will be converted to
	/// @param name the name of the value property
	/// @param defaultValue the value used in the resulting binding if the property is not set
	/// @return a transiently defined object used within the fluent declaration call
	///
	protected: template <typename BaseT>
	Impl::PropertyBindingBuilderFactoryPtr unique(std::string name, std::unique_ptr<BaseT> && defaultValue) const {
		Impl::PropertyBindingBuilderFactoryPtr factory = Impl::getBindingBuilderFactory(typeid(BaseT), name, "");
		auto f = factory->clone(name, defaultValue);
		allBindingBuilderFactories.emplace_back(f);
		return f;
	}

	///
	/// Declare a polymorphic "unique" value property specification in the environment configuration.
	///
	/// The supplied unique pointer cloner function will be used for cloning the unique pointer prototype value.
	///
	/// @tparam BaseT the base type of the class that the property value will be converted to
	/// @param name the name of the value property
	/// @return a transiently defined object used within the fluent declaration call
	///
	protected: template <typename BaseT>
	Impl::PropertyBindingBuilderFactoryPtr unique(std::string name, const UniquePropertyCloner<BaseT> & cloner) const {
		auto factory = Impl::PropertyBindingBuilderFactoryPtr(
			new Impl::UniquePropertyBindingBuilderFactory<BaseT>(std::move(name), cloner)
		);

		allBindingBuilderFactories.emplace_back(factory);
		return factory;
	}

	///
	/// Declare a polymorphic "unique" value property specification in the environment configuration, specifying a default value.
	///
	/// The supplied unique pointer cloner function will be used for cloning the unique pointer prototype value.
	///
	/// This overload should be used when a default value for the property should be
	/// used when no such property is provided in the environment properties file.
	///
	/// @tparam BaseT the base type of the class that the property value will be converted to
	/// @param name the name of the value property
	/// @param defaultValue the value used in the resulting binding if the property is not set
	/// @return a transiently defined object used within the fluent declaration call
	///
	protected: template <typename BaseT>
	Impl::PropertyBindingBuilderFactoryPtr unique(std::string name,
	                                              const UniquePropertyCloner<BaseT> & cloner,
	                                              std::unique_ptr<BaseT> && defaultValue) const {
		auto factory = Impl::PropertyBindingBuilderFactoryPtr(
			new Impl::UniquePropertyBindingBuilderFactory<BaseT>(std::move(name), cloner, std::move(defaultValue))
		);

		allBindingBuilderFactories.emplace_back(factory);
		return factory;
	}

	////////////////////////// Private implementation /////////////////////////

	//
	// Empty configure() implementation. Called when a non-derived EnvironmentConfiguration
	// instance is created by specifying one or more type specification URIs.
	//
	public: void configure() const override {}

	//
	// The main build method. Run after the configure() method by the base class.
	// This method delegates the building to a separate builder class.
	//
	public: std::vector<std::shared_ptr<Impl::BindingBuilderBase>> build() const override {
		if (!testing) {
			std::vector<Impl::PropertyBindingBuilderFactoryPtr> bindingBuilderFactoriesVector;

			for (auto & factory : allBindingBuilderFactories) {
				if (factory->level == 0) {
					bindingBuilderFactoriesVector.push_back(std::move(factory));
				}
			}

			builders = Impl::EnvironmentConfigurationBuilder::build(
				typeSpecificationsUris, bindingBuilderFactoriesVector, propertyStringsHierarchy
			);
		}

		return builders;
	}

	public: std::vector<const InjectorConfiguration *> getExtraConfiguration() const override {
		return std::vector<const InjectorConfiguration *>(); // Not used.
	}

	public: std::list<std::function<void (const Injector& )>> getPostConstructionCalls() const override {
		return std::list<std::function<void (const Injector& )>>(); // Not used.
	}

	public: std::list<std::function<void ()>> getPreDestructionCalls() const override {
		return std::list<std::function<void ()>>(); // Not used.
	}

	public: std::list<std::unique_ptr<StaticSingletonRegistrationBase>> getStaticSingletonPostConstructionCalls() const override {
		return std::list<std::unique_ptr<StaticSingletonRegistrationBase>>(); // Not used.
	}

	private: static std::shared_ptr<Resource::Uri> verifyPropertyFileExists(const Resource::File & propertyFile) {
		using ::toString;

		if (!propertyFile.exists()) {
			ThrowBalauException(
				  Exception::EnvironmentConfigurationException
				, toString("The property file does not exist: ", propertyFile)
			);
		}

		if (!propertyFile.isRegularFile()) {
			ThrowBalauException(
				  Exception::EnvironmentConfigurationException
				, toString("The property file is not a regular file: ", propertyFile)
			);
		}

		return propertyFile.clone();
	}

	friend class Impl::PropertyTypeSpecificationVisitor;

	//
	// The property strings trie is built during construction, by passing the input properties file
	// to the property parser service, then running the visitor to construct the hierarchy.
	//
	private: Container::ObjectTrie<Impl::PropertyString> propertyStringsHierarchy;

	//
	// The URIs of the type specification sources that will be used
	// (used during direct usage of the EnvironmentConfiguration class).
	//
	private: std::vector<std::shared_ptr<Resource::Uri>> typeSpecificationsUris;

	//
	// Temporary data structure used to hold binding builder factories during the
	// configure() method call. The binding builder factories are kept in an
	// initially flatted structure (this vector).
	//
	// Before executing the main builder logic, the level 0 factories are extracted
	// into a new vector.
	//
	private: mutable std::vector<Impl::PropertyBindingBuilderFactoryPtr> allBindingBuilderFactories;

	//
	// The binding builders are constructed in the second part of the build() method call,
	// by iterating over the property strings hierarchy.
	//
	//  - If the property is composite, an EnvironmentProperties named binding is created.
	//
	//  - If the property is simple and a corresponding binding builder factory is found
	//    in the binding builder factory hierarchy, a typed named binding is created.
	//
	//  - If the property is simple and no corresponding binding builder factory is found
	//    in the binding builder factory hierarchy, a std::string named binding is created.
	//
	// During iteration over the property strings hierarchy, the constructed bindings are
	// placed within the current binding level. The level is either the root (injector)
	// level or is a descendant EnvironmentProperties instance.
	//
	private: mutable std::vector<std::shared_ptr<Impl::BindingBuilderBase>> builders;

	friend struct Impl::EnvironmentConfigurationBuilderTest;

	// For testing only.
	private: static EnvironmentConfiguration testInstance(std::vector<std::shared_ptr<Impl::BindingBuilderBase>> && builders_) {
		EnvironmentConfiguration conf;
		conf.builders = std::move(builders_);
		return conf;
	}

	// For testing only.
	private: EnvironmentConfiguration() : testing(true) {}

	// For testing only. Bypasses the normal binding builder creation.
	private: const bool testing = false;
};

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_CONFIGURATION
