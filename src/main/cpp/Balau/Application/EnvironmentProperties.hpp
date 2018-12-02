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
/// @file EnvironmentProperties.hpp
///
/// A hierarchical environment properties holder created from a composite property.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_PROPERTIES
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_PROPERTIES

#include <Balau/Dev/Assert.hpp>
#include <Balau/Application/BindingBuilder.hpp>
#include <Balau/Application/Impl/BindingMap.hpp>
#include <Balau/Exception/InjectorExceptions.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Type/FromString.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

namespace Impl {

class EnvironmentConfigurationBuilder;

} // namespace Impl

///
/// A hierarchical environment properties holder created from a composite property.
///
/// An instance of this class contains a set of value and/or shared injector bindings,
/// which were created from the parsing of a composite property within a hierarchical
/// property file.
///
/// The direct or indirect value binding descendants of an %EnvironmentProperties
/// instance represent the simple and composite properties that were contained within
/// the composite property in the hierarchical property file. Descendant value bindings
/// may be a mixture of untyped simple properties (supplied as std::string) and typed
/// simple properties (supplied as the type specified within the injector configuration).
/// All descendant shared bindings are more instances of the %EnvironmentProperties.
///
class EnvironmentProperties {
	///
	/// Returns true if the composite property has a value binding matching the specified type and name.
	///
	/// @tparam ValueT the value binding type
	/// @param name the name of the binding
	/// @return true if the composite property has a value binding matching the specified type and name
	///
	public: template <typename ValueT>
	bool hasValue(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), name);
		return bindings->hasBinding(keyView);
	}

	///
	/// Returns true if the composite property has a child composite binding with the specified name.
	///
	/// @param name the name of the binding
	/// @return true if the composite property has a child composite binding with the specified name
	///
	public: bool hasCompositeValue(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), name);
		return bindings->hasBinding(keyView);
	}

	///
	/// Get a non-polymorphic value of the specified type and name.
	///
	/// The value is copy constructed from the prototype in the value binding.
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// @tparam ValueT the type of the value binding
	/// @param name the name of the binding
	/// @return a new value
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename ValueT> ValueT getValue(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractValueBinding<ValueT> *>(binding->get())->get(nullptr);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	///
	/// Get a non-polymorphic value of the specified type and name.
	///
	/// The value is copy constructed from the prototype in the value binding.
	///
	/// If no suitable binding is found, the supplied default value is copied into the return value.
	///
	/// @tparam ValueT the type of the value binding
	/// @param name the name of the binding
	/// @param defaultValue a default value to copy if no suitable binding was found
	/// @return a new value or a copy of the supplied default value
	///
	public: template <typename ValueT> ValueT getValue(std::string_view name, const ValueT & defaultValue) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractValueBinding<ValueT> *>(binding->get())->get(nullptr);
		} else {
			return defaultValue;
		}
	}

	///
	/// Get a polymorphic value of the specified type and name.
	///
	/// The value is cloned from the prototype via the base type's provider function in the unique binding.
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param name the name of the binding
	/// @return a new polymorphic value
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename BaseT> std::unique_ptr<BaseT> getUnique(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractUniquePtrBinding<BaseT> *>(binding->get())->get(nullptr);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	///
	/// Get a polymorphic value of the specified type and name.
	///
	/// The value is cloned from the prototype via the type's provider function in the unique binding.
	///
	/// If no suitable binding is found, the supplied default value is moved into the return value.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param name the name of the binding
	/// @param defaultValue a default value to copy if no suitable binding was found
	/// @return a new polymorphic value or the supplied default value
	///
	public: template <typename BaseT> std::unique_ptr<BaseT> getUnique(std::string_view name, std::unique_ptr<BaseT> && defaultValue) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractUniquePtrBinding<BaseT> *>(binding->get())->get(nullptr);
		} else {
			return std::move(defaultValue);
		}
	}

	///
	/// Get a child environment properties instance with the specified name.
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// @param name the name of the binding
	/// @return the child environment properties instance with the specified name
	/// @throw NoBindingException if no suitable binding was found
	///
	public: std::shared_ptr<EnvironmentProperties> getShared(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			// Environment properties shared bindings are always of type ProvidedSingletonBinding.
			// These do not use the injector instance, thus nullptr can be passed without any issues.
			return static_cast<const Impl::AbstractSharedPtrBinding<EnvironmentProperties> *>(binding->get())->get(nullptr);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	///
	/// Get a child environment properties instance with the specified name.
	///
	/// If no suitable binding is found, null is returned.
	///
	/// @param name the name of the binding
	/// @return child environment properties instance with the specified name or null
	///
	public: std::shared_ptr<EnvironmentProperties> getSharedOrNull(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			// Environment properties shared bindings are always of type ProvidedSingletonBinding.
			// These do not use the injector instance, thus nullptr can be passed without any issues.
			return static_cast<const Impl::AbstractSharedPtrBinding<EnvironmentProperties> *>(binding->get())->get(nullptr);
		} else {
			return std::shared_ptr<EnvironmentProperties>();
		}
	}

	///////////////////////// Private implementation //////////////////////////

	friend class EnvironmentConfiguration;
	friend class Impl::EnvironmentConfigurationBuilder;

	private: EnvironmentProperties(std::unique_ptr<const Impl::BindingMap> && bindings_)
		: bindings(std::move(bindings_)) {}

	private: const std::unique_ptr<const Impl::BindingMap> bindings;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_PROPERTIES
