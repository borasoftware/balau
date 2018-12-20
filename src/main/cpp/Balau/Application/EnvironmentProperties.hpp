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
/// %EnvironmentProperties instances may be iterated over. As the contained items being
/// iterated over are of different types, (T, std::unique_ptr<T>,
/// and std::shared_ptr<EnvironmentProperties>), de-referenced iterators provide opaque
/// EnvironmentProperties::Item proxy objects. The EnvironmentProperties::Item class
/// provides a similar API to the EnvironmentProperties class, allowing type and name
/// examination of the contained binding, and extraction via getValue, getUnique, and
/// getComposite methods.
///
class EnvironmentProperties {
	///
	/// Instances of the %Item class are returned from iterators.
	///
	public: class Item {
		///
		/// Get the name of the item.
		///
		/// @return the name of the item
		///
		public: std::string_view getName() const {
			return key.name;
		}

		///
		/// Returns true if the item is a value binding matching the specified type and name.
		///
		/// @tparam ValueT the value binding type
		/// @param name the name of the binding
		/// @return true if the item is a value binding matching the specified type and name
		///
		public: template <typename ValueT> bool isValue() const {
			return key == Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), key.name);
		}

		///
		/// Returns true if the item is a composite binding with the specified name.
		///
		/// @param name the name of the binding
		/// @return true if the item is a composite binding with the specified name
		///
		public: bool isComposite() const {
			return key == Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), key.name);
		}

		///
		/// Get the non-polymorphic value of the specified type and name.
		///
		/// The value is copy constructed from the prototype in the value binding.
		///
		/// If the item does not proxy to such a binding, NoBindingException is thrown.
		///
		/// @tparam ValueT the type of the value binding
		/// @param name the name of the binding
		/// @return a new value
		/// @throw NoBindingException if the item does not proxy to such a binding
		///
		public: template <typename ValueT> ValueT getValue() const {
			return owner.getValue<ValueT>(
				Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), key.name)
			);
		}

		///
		/// Get the non-polymorphic value of the specified type and name.
		///
		/// The value is copy constructed from the prototype in the value binding.
		///
		/// If the item does not proxy to such a binding, the supplied default value is copied into the return value.
		///
		/// @tparam ValueT the type of the value binding
		/// @param name the name of the binding
		/// @param defaultValue a default value to copy if the item does not proxy to such a binding
		/// @return a new value or a copy of the supplied default value
		///
		public: template <typename ValueT> ValueT getValue(const ValueT & defaultValue) const {
			return owner.getValue<ValueT>(
				Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), key.name), defaultValue
			);
		}

		///
		/// Get the polymorphic value of the specified type and name.
		///
		/// The value is cloned from the prototype via the base type's provider function in the unique binding.
		///
		/// If the item does not proxy to such a binding, NoBindingException is thrown.
		///
		/// @tparam BaseT the base type of the unique binding
		/// @param name the name of the binding
		/// @return a new polymorphic value
		/// @throw NoBindingException if the item does not proxy to such a binding
		///
		public: template <typename BaseT> std::unique_ptr<BaseT> getUnique() const {
			return owner.getUnique<BaseT>(
				Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), key.name)
			);
		}

		///
		/// Get the polymorphic value of the specified type and name.
		///
		/// The value is cloned from the prototype via the type's provider function in the unique binding.
		///
		/// If the item does not proxy to such a binding, the supplied default value is moved into the return value.
		///
		/// @tparam BaseT the base type of the unique binding
		/// @param name the name of the binding
		/// @param defaultValue a default value to copy if the item does not proxy to such a binding
		/// @return a new polymorphic value or the supplied default value
		///
		public: template <typename BaseT> std::unique_ptr<BaseT> getUnique(std::unique_ptr<BaseT> && defaultValue) const {
			return owner.getUnique<BaseT>(
				  Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), key.name)
				, std::move(defaultValue)
			);
		}

		///
		/// Get the environment properties instance with the specified name.
		///
		/// If the item does not proxy to such a binding, NoBindingException is thrown.
		///
		/// @param name the name of the binding
		/// @return the child environment properties instance with the specified name
		/// @throw NoBindingException if the item does not proxy to such a binding
		///
		public: std::shared_ptr<EnvironmentProperties> getComposite() const {
			return owner.getComposite(
				Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), key.name)
			);
		}

		///
		/// Get the environment properties instance with the specified name.
		///
		/// If the item does not proxy to such a binding, null is returned.
		///
		/// @param name the name of the binding
		/// @return child environment properties instance with the specified name or null
		///
		public: std::shared_ptr<EnvironmentProperties> getCompositeOrNull() const {
			return owner.getCompositeOrNull(
				Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), key.name)
			);
		}

		friend class EnvironmentProperties;

		private: Item(const EnvironmentProperties & owner_, const Impl::BindingKeyView & key_)
			: owner(owner_)
			, key(key_) {}

		private: const EnvironmentProperties & owner;
		private: const Impl::BindingKeyView key;
	};

	///
	/// The %EnvironmentProperties iterator.
	///
	public: class iterator {
		///
		/// Create an iterator by copying the supplied iterator.
		///
		public: iterator(const iterator & copy)
			: owner(copy.owner)
			, mapIterator(copy.mapIterator) {}

		///
		/// Set the current iterator to a copy of the supplied iterator.
		///
		public: iterator & operator = (const iterator & copy) {
			mapIterator = copy.mapIterator;
			return *this;
		}

		///
		/// Increment the iterator (post increment).
		///
		public: iterator operator ++ (int) {
			iterator ret = *this;
			++mapIterator;
			return ret;
		}

		///
		/// Increment the iterator.
		///
		public: iterator & operator ++ () {
			++mapIterator;
			return *this;
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: Item operator * () const {
			return Item(owner, Impl::BindingKeyView(mapIterator->key));
		}

		///
		/// returns true if the current iterator is equal to the supplied iterator.
		///
		public: bool operator == (const iterator & rhs) const {
			return mapIterator == rhs.mapIterator;
		}

		///
		/// returns true if the current iterator is not equal to the supplied iterator.
		///
		public: bool operator != (const iterator & rhs) const {
			return mapIterator != rhs.mapIterator;
		}

		friend class EnvironmentProperties;

		private: explicit iterator(const EnvironmentProperties & owner_, Impl::BindingMap::ConstIterator mapIterator_)
			: owner(owner_)
			, mapIterator(mapIterator_) {}

		private: const EnvironmentProperties & owner;
		private: Impl::BindingMap::ConstIterator mapIterator;
	};

	///
	/// The const iterator of EnvironmentProperties is the same as a non-const one.
	///
	public: typedef iterator const_iterator;

	///////////////////////////// Public methods //////////////////////////////

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
	/// Returns true if the composite property has a unique pointer binding matching the specified type and name.
	///
	/// @tparam BaseT the base type of the binding
	/// @param name the name of the binding
	/// @return true if the composite property has a unique pointer binding matching the specified type and name
	///
	public: template <typename BaseT>
	bool hasUnique(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);
		return bindings->hasBinding(keyView);
	}

	///
	/// Returns true if the composite property has a child composite binding with the specified name.
	///
	/// @param name the name of the binding
	/// @return true if the composite property has a child composite binding with the specified name
	///
	public: bool hasComposite(std::string_view name) const {
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
		return getValue<ValueT>(
			Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), name)
		);
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
		return getValue<ValueT>(
			Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, ValueT>), name), defaultValue
		);
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
		return getUnique<BaseT>(
			Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name)
		);
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
		return getUnique<BaseT>(
			Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name), defaultValue
		);
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
	public: std::shared_ptr<EnvironmentProperties> getComposite(std::string_view name) const {
		return getComposite(
			Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), name)
		);
	}

	///
	/// Get a child environment properties instance with the specified name.
	///
	/// If no suitable binding is found, null is returned.
	///
	/// @param name the name of the binding
	/// @return child environment properties instance with the specified name or null
	///
	public: std::shared_ptr<EnvironmentProperties> getCompositeOrNull(std::string_view name) const {
		return getCompositeOrNull(
			Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>), name)
		);
	}

	///
	/// Get an iterator positioned at the beginning of the environment properties list.
	///
	public: iterator begin() {
		return iterator(*this, bindings->begin());
	}

	///
	/// Get an iterator positioned at the end of the environment properties list.
	///
	public: iterator end() {
		return iterator(*this, bindings->end());
	}

	///
	/// Get a const iterator positioned at the beginning of the environment properties list.
	///
	public: const_iterator begin() const {
		return const_iterator(*this, bindings->begin());
	}

	///
	/// Get a const iterator positioned at the end of the environment properties list.
	///
	public: const_iterator end() const {
		return const_iterator(*this, bindings->end());
	}

	///
	/// Create an environment properties instance.
	///
	/// This is usually performed by the injector environment configuration.
	///
	public: EnvironmentProperties(std::unique_ptr<const Impl::BindingMap> && bindings_)
		: bindings(std::move(bindings_)) {}

	///
	/// Create an empty environment properties instance.
	///
	/// This is required by certain Balau components.
	///
	public: EnvironmentProperties()
		: bindings(new Impl::BindingMap()) {}

	///////////////////////// Private implementation //////////////////////////

	friend class EnvironmentConfiguration;
	friend class Impl::EnvironmentConfigurationBuilder;

	private: template <typename ValueT> ValueT getValue(const Impl::BindingKeyView & keyView) const {
		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractValueBinding<ValueT> *>(binding->get())->get(nullptr);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	private: template <typename ValueT> ValueT getValue(const Impl::BindingKeyView & keyView, const ValueT & defaultValue) const {
		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractValueBinding<ValueT> *>(binding->get())->get(nullptr);
		} else {
			return defaultValue;
		}
	}

	private: template <typename BaseT> std::unique_ptr<BaseT> getUnique(const Impl::BindingKeyView & keyView) const {
		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractUniquePtrBinding<BaseT> *>(binding->get())->get(nullptr);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	private: template <typename BaseT> std::unique_ptr<BaseT> getUnique(const Impl::BindingKeyView & keyView, std::unique_ptr<BaseT> && defaultValue) const {
		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return static_cast<const Impl::AbstractUniquePtrBinding<BaseT> *>(binding->get())->get(nullptr);
		} else {
			return std::move(defaultValue);
		}
	}

	private: std::shared_ptr<EnvironmentProperties> getComposite(const Impl::BindingKeyView & keyView) const {
		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			// Environment properties shared bindings are always of type ProvidedSingletonBinding.
			// These do not use the injector instance, thus nullptr can be passed without any issues.
			return static_cast<const Impl::AbstractSharedPtrBinding<EnvironmentProperties> *>(binding->get())->get(nullptr);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	private: std::shared_ptr<EnvironmentProperties> getCompositeOrNull(const Impl::BindingKeyView & keyView) const {
		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			// Environment properties shared bindings are always of type ProvidedSingletonBinding.
			// These do not use the injector instance, thus nullptr can be passed without any issues.
			return static_cast<const Impl::AbstractSharedPtrBinding<EnvironmentProperties> *>(binding->get())->get(nullptr);
		} else {
			return std::shared_ptr<EnvironmentProperties>();
		}
	}

	private: const std::unique_ptr<const Impl::BindingMap> bindings;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__ENVIRONMENT_PROPERTIES
