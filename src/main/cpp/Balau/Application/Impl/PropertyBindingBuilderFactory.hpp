// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_FACTORY
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_FACTORY

#include <Balau/Dev/Assert.hpp>
#include <Balau/Application/BindingBuilder.hpp>
#include <Balau/Exception/InjectorExceptions.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Type/FromString.hpp>
#include <Balau/Util/Files.hpp>

namespace Balau {

class EnvironmentConfiguration;

namespace Impl {

class EnvironmentConfigurationBuilder;
class EnvironmentConfigurationBuilderUtils;
class PropertyTypeSpecificationVisitor;

//
// Holds information for the construction of a binding builder.
//
class PropertyBindingBuilderFactoryBase {
	protected: PropertyBindingBuilderFactoryBase(std::string && name_, bool hasDefaultValue_)
		: name(std::move(name_))
		, hasDefaultValue(hasDefaultValue_)
		, bindingCreated(false)
		, level(0) {}

	protected: PropertyBindingBuilderFactoryBase(const PropertyBindingBuilderFactoryBase & copy)
		: name(copy.name)
		, hasDefaultValue(copy.hasDefaultValue)
		, bindingCreated(copy.bindingCreated)
		, level(copy.level) {}

	public: virtual ~PropertyBindingBuilderFactoryBase() = default;

	public: virtual std::unique_ptr<Impl::BindingBuilderBase> create(const std::string & value) const = 0;

	public: virtual std::unique_ptr<Impl::BindingBuilderBase> createDefault() const = 0;

	public: virtual BindingKey createKey() const = 0;

	public: virtual bool isComposite() const = 0;

	public: bool shouldBuildDefaultBinding() const {
		return !bindingCreated && hasDefaultValue;
	}

	public: size_t getLevel() const {
		return level;
	}

	public: virtual std::shared_ptr<PropertyBindingBuilderFactoryBase> clone(const std::string & defaultValueString) const = 0;

	public: virtual std::shared_ptr<PropertyBindingBuilderFactoryBase> clone(const std::string & newName,
	                                                                         const std::string & defaultValueString) const = 0;

	friend class ::Balau::EnvironmentConfiguration;
	friend class EnvironmentConfigurationBuilder;
	friend class EnvironmentConfigurationBuilderUtils;

	protected: std::string name;
	private: bool hasDefaultValue;
	protected: mutable bool bindingCreated;
	private: size_t level;
};

using PropertyBindingBuilderFactoryPtr = std::shared_ptr<PropertyBindingBuilderFactoryBase>;
using PropertyBindingBuilderFactoryTrie = Container::ObjectTrie<PropertyBindingBuilderFactoryPtr>;
using PropertyBindingBuilderFactoryTrieNode = Container::ObjectTrieNode<PropertyBindingBuilderFactoryPtr>;

//
// Given the property type as a string, instantiate a suitable typed property binding builder factory.
//
// If the type string is unknown, an EnvironmentConfigurationException is thrown.
//
PropertyBindingBuilderFactoryPtr getBindingBuilderFactory(const std::string & typeString,
                                                          const std::string & name,
                                                          const std::string & defaultValueString);

//
// Given the property type as a type_index, instantiate a suitable typed property.
//
// If the type string is unknown, an EnvironmentConfigurationException is thrown.
//
PropertyBindingBuilderFactoryPtr getBindingBuilderFactory(const std::type_index typeIndex,
                                                          const std::string & name,
                                                          const std::string & defaultValueString);

//
// Used to hold environment binding build information.
//
// Composite factories exist as intermediate nodes that do not have an implemented create method.
//
class CompositePropertyBindingBuilderFactory : public PropertyBindingBuilderFactoryBase {
	public: CompositePropertyBindingBuilderFactory(std::string name_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), false) {}

	// The copy constructor is used to clone without the children.
	private: CompositePropertyBindingBuilderFactory(const CompositePropertyBindingBuilderFactory & copy)
		: PropertyBindingBuilderFactoryBase(copy) {}

	private: std::unique_ptr<Impl::BindingBuilderBase> create(const std::string & value) const override {
		ThrowBalauException(Exception::BugException, "CompositePropertyBindingBuilderFactory::create called.");
	}

	public: std::unique_ptr<Impl::BindingBuilderBase> createDefault() const override {
		ThrowBalauException(Exception::BugException, "CompositePropertyBindingBuilderFactory::createDefault called.");
	}

	public: virtual BindingKey createKey() const override {
		ThrowBalauException(Exception::BugException, "CompositePropertyBindingBuilderFactory::createKey called.");
	}

	private: bool isComposite() const override {
		return true;
	}

	public: PropertyBindingBuilderFactoryPtr clone(const std::string & defaultValueString) const override {
		ThrowBalauException(Exception::BugException, "Clone called for CompositePropertyBindingBuilderFactory.");
	}

	public: PropertyBindingBuilderFactoryPtr clone(const std::string & newName,
	                                               const std::string & defaultValueString) const override {
		ThrowBalauException(Exception::BugException, "Clone called for CompositePropertyBindingBuilderFactory.");
	}

	friend struct EnvironmentConfigurationBuilderTest;

	// For testing.
	private: template <typename ... PropertyBindingBuilderFactoryPtrT>
	static PropertyBindingBuilderFactoryPtr makePtr(std::string name_,
	                                                PropertyBindingBuilderFactoryPtr firstChild,
	                                                PropertyBindingBuilderFactoryPtrT ... moreChildren) {
		return PropertyBindingBuilderFactoryPtr(
			new CompositePropertyBindingBuilderFactory(std::move(name_), firstChild, moreChildren ...)
		);
	}

	// For testing.
	private: template <typename ... PropertyBindingBuilderFactoryPtrT>
	CompositePropertyBindingBuilderFactory(std::string name_,
	                                       PropertyBindingBuilderFactoryPtr & firstChild,
	                                       PropertyBindingBuilderFactoryPtrT & ... moreChildren)
		: PropertyBindingBuilderFactoryBase(std::move(name_), false)
		, children(Util::Vectors::pushBack(std::move(firstChild), std::move(moreChildren) ...)) {}

	friend class ::Balau::EnvironmentConfiguration;
	friend class Impl::EnvironmentConfigurationBuilder;
	friend class Impl::EnvironmentConfigurationBuilderUtils;
	friend class Impl::PropertyTypeSpecificationVisitor;

	private: std::vector<PropertyBindingBuilderFactoryPtr> children;
};

//
// Used to hold environment binding build information.
//
// Value factories exist as leaf nodes and have an implemented create method.
//
template <typename ValueT> class ValuePropertyBindingBuilderFactory : public PropertyBindingBuilderFactoryBase {
	public: ValuePropertyBindingBuilderFactory(std::string name_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), false)
		, defaultValue() {}

	public: ValuePropertyBindingBuilderFactory(std::string name_, const ValueT & defaultValue_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), true)
		, defaultValue(defaultValue_) {}

	public: ValuePropertyBindingBuilderFactory(const ValuePropertyBindingBuilderFactory<ValueT> & copy)
		: PropertyBindingBuilderFactoryBase(copy)
		, defaultValue(copy.defaultValue) {}

	public: std::unique_ptr<Impl::BindingBuilderBase> create(const std::string & value) const override {
		// This method is also used in the getBindingBuilderFactory function.
		// If the code here is changed, the getBindingBuilderFactory function must also be reviewed.

		auto ret = std::unique_ptr<Impl::BindingBuilderBase>(new BindingBuilder<ValueT>(std::string(name)));
		auto & bindingBuilder = static_cast<BindingBuilder<ValueT> &>(*ret);
		ValueT typedValue;
		fromString(typedValue, value);
		bindingBuilder.toValue(typedValue);
		bindingCreated = true;
		return ret;
	}

	public: std::unique_ptr<Impl::BindingBuilderBase> createDefault() const override {
		auto ret = std::unique_ptr<Impl::BindingBuilderBase>(new BindingBuilder<ValueT>(std::string(name)));
		auto & bindingBuilder = static_cast<BindingBuilder<ValueT> &>(*ret);
		bindingBuilder.toValue(defaultValue);
		bindingCreated = true;
		return ret;
	}

	public: virtual BindingKey createKey() const override {
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), std::string(name));
	}

	public: bool isComposite() const override {
		return false;
	}

	public: PropertyBindingBuilderFactoryPtr clone(const std::string & defaultValueString) const override {
		if (defaultValueString.empty()) {
			return PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<ValueT>(name));
		} else {
			ValueT d;
			fromString(d, defaultValueString);
			return PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<ValueT>(name, d));
		}
	}

	public: PropertyBindingBuilderFactoryPtr clone(const std::string & newName,
	                                               const std::string & defaultValueString) const override {
		if (defaultValueString.empty()) {
			return PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<ValueT>(newName));
		} else {
			ValueT d;
			fromString(d, defaultValueString);
			return PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<ValueT>(newName, d));
		}
	}

	friend struct EnvironmentConfigurationBuilderTest;

	// For testing.
	private: static PropertyBindingBuilderFactoryPtr makePtr(std::string name_) {
		return PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<ValueT>(std::move(name_)));
	}

	// For testing.
	private: static PropertyBindingBuilderFactoryPtr makePtr(std::string name_, const ValueT & defaultValue_) {
		return PropertyBindingBuilderFactoryPtr(new ValuePropertyBindingBuilderFactory<ValueT>(std::move(name_), defaultValue_));
	}

	friend class ::Balau::EnvironmentConfiguration;

	private: const ValueT defaultValue;
};

//
// Used to hold environment binding build information.
//
// Unique factories exist as leaf nodes and have an implemented create method.
//
template <typename BaseT> class UniquePropertyBindingBuilderFactory : public PropertyBindingBuilderFactoryBase {
	public: UniquePropertyBindingBuilderFactory(std::string name_, const UniquePropertyCloner<BaseT> & clonerFunction_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), false)
		, clonerFunction(clonerFunction_)
		, defaultValue() {}

	public: UniquePropertyBindingBuilderFactory(std::string name_,
	                                            const UniquePropertyCloner<BaseT> & clonerFunction_,
	                                            std::unique_ptr<const BaseT> && defaultValue_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), true)
		, clonerFunction(clonerFunction_)
		, defaultValue(std::move(defaultValue_)) {}

	public: UniquePropertyBindingBuilderFactory(const UniquePropertyBindingBuilderFactory<BaseT> & copy)
		: PropertyBindingBuilderFactoryBase(copy)
		, clonerFunction(copy.clonerFunction)
		, defaultValue(clonerFunction(copy.defaultValue)) {}

	private: class ClonerProvider {
		UniquePropertyCloner<BaseT> clonerFunction;
		std::unique_ptr<const BaseT> prototype;

	public: ClonerProvider(const UniquePropertyCloner<BaseT> & clonerFunction_, std::unique_ptr<const BaseT> && prototype_)
			: clonerFunction(clonerFunction_)
			, prototype(std::move(prototype_)) {
		}

		public: std::unique_ptr<BaseT> operator () () {
			return clonerFunction(prototype);
		}
	};

	public: std::unique_ptr<Impl::BindingBuilderBase> create(const std::string & value) const override {
		// This method is also used in the getBindingBuilderFactory function.
		// If the code here is changed, the getBindingBuilderFactory function must also be reviewed.

		auto ret = std::unique_ptr<Impl::BindingBuilderBase>(new BindingBuilder<BaseT>(std::string(name)));
		auto & bindingBuilder = static_cast<BindingBuilder<BaseT> &>(*ret);
		std::unique_ptr<BaseT> typedValue;
		fromString(typedValue, value);
		auto provider = std::shared_ptr<ClonerProvider>(new ClonerProvider(clonerFunction, std::move(typedValue)));
		bindingBuilder.toUniqueProvider(provider);
		bindingCreated = true;
		return ret;
	}

	public: std::unique_ptr<Impl::BindingBuilderBase> createDefault() const override {
		auto ret = std::unique_ptr<Impl::BindingBuilderBase>(new BindingBuilder<BaseT>(std::string(name)));
		auto & bindingBuilder = static_cast<BindingBuilder<BaseT> &>(*ret);
		auto provider = std::shared_ptr<ClonerProvider>(new ClonerProvider(clonerFunction, std::move(defaultValue)));
		bindingBuilder.toUniqueProvider(provider);
		bindingCreated = true;
		return ret;
	}

	public: virtual BindingKey createKey() const override {
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), std::string(name));
	}

	public: bool isComposite() const override {
		return false;
	}

	public: PropertyBindingBuilderFactoryPtr clone(const std::string & defaultValueString) const override {
		if (defaultValueString.empty()) {
			return PropertyBindingBuilderFactoryPtr(
				new UniquePropertyBindingBuilderFactory<BaseT>(name, clonerFunction)
			);
		} else {
			std::unique_ptr<BaseT> d;
			fromString(d, defaultValueString);

			return PropertyBindingBuilderFactoryPtr(
				new UniquePropertyBindingBuilderFactory<BaseT>(name, clonerFunction, std::move(d))
			);
		}
	}

	public: PropertyBindingBuilderFactoryPtr clone(const std::string & newName,
	                                               const std::string & defaultValueString) const override {
		if (defaultValueString.empty()) {
			return PropertyBindingBuilderFactoryPtr(
				new UniquePropertyBindingBuilderFactory<BaseT>(newName, clonerFunction)
			);
		} else {
			std::unique_ptr<BaseT> d;
			fromString(d, defaultValueString);

			return PropertyBindingBuilderFactoryPtr(
				new UniquePropertyBindingBuilderFactory<BaseT>(newName, clonerFunction, std::move(d))
			);
		}
	}

	friend struct EnvironmentConfigurationBuilderTest;

	// For testing.
	private: static PropertyBindingBuilderFactoryPtr makePtr(std::string name_) {
		return PropertyBindingBuilderFactoryPtr(new UniquePropertyBindingBuilderFactory<BaseT>(std::move(name_)));
	}

	// For testing.
	private: static PropertyBindingBuilderFactoryPtr makePtr(std::string name_, std::unique_ptr<const BaseT> && defaultValue_) {
		return PropertyBindingBuilderFactoryPtr(new UniquePropertyBindingBuilderFactory<BaseT>(std::move(name_), std::move(defaultValue_)));
	}

	// For testing only.
	private: UniquePropertyBindingBuilderFactory(std::string name_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), false)
		, clonerFunction(getClonerFunction(name))
		, defaultValue() {}

	// For testing only.
	private: UniquePropertyBindingBuilderFactory(std::string name_, std::unique_ptr<const BaseT> && defaultValue_)
		: PropertyBindingBuilderFactoryBase(std::move(name_), false)
		, clonerFunction(getClonerFunction(name))
		, defaultValue(std::move(defaultValue_)) {}

	// For testing only.
	private: static UniquePropertyCloner<BaseT> getClonerFunction(const std::string & name) {
		auto factory = getBindingBuilderFactory(typeid(BaseT), name, "");
		auto typedFactory = std::dynamic_pointer_cast<UniquePropertyBindingBuilderFactory<BaseT>>(factory);
		return typedFactory->clonerFunction;
	}

	friend class ::Balau::EnvironmentConfiguration;

	private: UniquePropertyCloner<BaseT> clonerFunction;
	private: mutable std::unique_ptr<const BaseT> defaultValue;
};

//
// The implementation of the public addEnvironmentPropertyType call.
//
void registerEnvironmentPropertyType(const std::string & typeString,
                                     std::type_index typeIndex,
                                     PropertyBindingBuilderFactoryPtr factory);

//
// Register C++ specific unsigned integer value type property binding builder factories
// in the global property binding builder factory map.
//
// These types are not pre-registered as they are not listed in the environment
// configuration pre-registered type list.
//
void registerEnvironmentPropertyUnsignedTypes();

template <typename ValueT>
inline void registerEnvironmentPropertyValueType(const std::string & typeString) {
	using T = typename std::remove_const<ValueT>::type;

	registerEnvironmentPropertyType(
		  typeString
		, typeid(T)
		, Impl::PropertyBindingBuilderFactoryPtr(new Impl::ValuePropertyBindingBuilderFactory<ValueT>(""))
	);
}

template <typename BaseT>
inline void registerEnvironmentPropertyUniqueType(const std::string & typeString,
                                                  const UniquePropertyCloner<BaseT> & cloner) {
	using T = typename std::remove_const<BaseT>::type;

	registerEnvironmentPropertyType(
		  typeString
		, typeid(T)
		, Impl::PropertyBindingBuilderFactoryPtr(new Impl::UniquePropertyBindingBuilderFactory<BaseT>("", cloner))
	);
}

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_FACTORY
