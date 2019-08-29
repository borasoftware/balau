// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file Injector.hpp
///
/// The main Injector class.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR

#include <Balau/Application/ApplicationConfiguration.hpp>
#include <Balau/Application/EnvironmentConfiguration.hpp>
#include <Balau/Util/Memory.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau {

///
/// The main dependency injector class.
///
/// Injectors are instantiated by passing one or more injector configuration
/// instances.
///
/// An instantiated injector can be used by multiple threads without requiring
/// any synchronisation.
///
/// See the injector <a href="../manual/Application/Injector.html"> manual page</a>
/// for detailed usage information and the injector unit tests for examples.
///
class Injector final : public std::enable_shared_from_this<Injector> {
	////////////////////////////// Construction ///////////////////////////////

	///
	/// Construct an injector with one or more injector configurations.
	///
	/// @tparam Conf the first configuration type(s) (must derive from InjectorConfiguration)
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a shared pointer to the newly created injector
	///
	public: template <typename ... Conf> static std::shared_ptr<Injector> create(const Conf & ... conf) {
		return createInjector(std::shared_ptr<Injector>(), conf ...);
	}

	///
	/// Construct an injector with one or more injector configurations supplied in a vector.
	///
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a shared pointer to the newly created injector
	///
	public: static std::shared_ptr<Injector> create(const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) {
		return createInjector(std::shared_ptr<Injector>(), conf);
	}

	///
	/// Register with the injector a callback that will be called by the injector at the end of construction.
	///
	/// In order to use this method, inject the injector into the injectable via a
	/// weak pointer and call the method.
	///
	/// @param call the callback
	///
	public: void registerPostConstructionCall(const std::function<void (const Injector &)> & call) const {
		registerPostConstructionCallImpl(call);
	}

	///
	/// Register with the injector a callback that will be called in the injector's destructor, before the bindings are deleted.
	///
	/// Although pre-destruction callbacks must be noexcept(true), the pre-destruction
	/// function signature does not contain noexcept(true), as this is not yet handled
	/// by std::function in C++17. Despite this, functions registered as pre-destruction
	/// callbacks must nevertheless be noexcept(true).
	///
	/// In order to use this method, inject the injector into the injectable via a
	/// weak pointer and call the method.
	///
	/// @param call the callback
	///
	public: void registerPreDestructionCall(const std::function<void ()> & call) const {
		registerPreDestructionCallImpl(call);
	}

	///
	/// Register a static singleton pointer that the injector will set up post-construction and invalidate pre-destruction.
	///
	/// The static pointer will be valid immediately after injection construction
	/// up to the start of injector destruction.
	///
	/// This call is a convenience method for calling the registerPostConstructionCall
	/// and registerPreDestructionCall methods in order to set up and tear down the static
	/// singleton pointer.
	///
	/// In order to use this method, inject the injector into the injectable via a
	/// weak pointer and call the method.
	///
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a shared pointer to the newly created injector
	///
	public: template <typename T> void registerStaticSingleton(std::shared_ptr<T> * ptrPtr, std::string_view name = std::string_view()) const {
		registerPostConstructionCallImpl([ptrPtr, name] (const Injector & injector) { *ptrPtr  = injector.getShared<T>(name); });
		registerPreDestructionCallImpl([ptrPtr] () { ptrPtr->reset(); });
	}

	///////////////////////// Child injector creation /////////////////////////

	///
	/// Construct a child injector with one or more injector configurations.
	///
	/// @tparam Conf the first configuration type(s) (must derive from InjectorConfiguration)
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a shared pointer to the newly created child injector
	///
	public: template <typename ... Conf> std::shared_ptr<Injector> createChild(const Conf & ... conf) const {
		return createInjector(shared_from_this(), conf ...);
	}

	///
	/// Construct a child injector with one or more injector configurations supplied in a vector.
	///
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a shared pointer to the newly created child injector
	///
	public: std::shared_ptr<Injector> createChild(const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) const {
		return createInjector(shared_from_this(), conf);
	}

	///
	/// Construct a child injector from the previously constructed prototype.
	///
	/// Constructing a prototype child injector and subsequently calling this
	/// function multiple times with the same prototype avoids running the
	/// construction and validation phases each time a child injector is required.
	///
	/// Note that the bindings of the prototype will be shared between all child
	/// injectors created from the prototype. This implies that singleton and
	/// thread-local singleton instances will be shared across the injectors. If
	/// this is not desired behaviour, then a new child injector must be created
	/// via the other createChildInjector functions that instantiate their own
	/// bindings.
	///
	/// @param prototype the prototype child injector to copy
	/// @return a shared pointer to the newly created child injector
	///
	public: std::shared_ptr<Injector> createChild(const std::shared_ptr<Injector> & prototype) const {
		return std::shared_ptr<Injector>(new Injector(PrototypeConstruction(), shared_from_this(), prototype));
	}

	/////////////////////////////// Validation ////////////////////////////////

	///
	/// Encapsulates a validated parent injector for subsequent child validation calls.
	///
	/// Instances of ValidationParent are created by the Injector::validate and
	/// Injector::validateChild static methods.
	///
	/// @see Injector::validate
	/// @see Injector::validateChild
	///
	public: class ValidationParent {
		private: std::shared_ptr<const Injector> parent;

		private: explicit ValidationParent(std::shared_ptr<const Injector> parent_)
			: parent(std::move(parent_)) {}

		friend class Injector;
	};

	///
	/// Validate the runtime specified injector configuration.
	///
	/// @tparam Conf the first configuration type(s) (must derive from InjectorConfiguration)
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a ValidationParent instance representing the validated injector
	/// @throw MissingDependencyException if a required dependency is missing
	/// @throw DuplicateBindingException if duplicate bindings exist
	/// @throw CyclicDependencyException if there is a cyclic dependency
	/// @throw SharedInjectorException if one of the injectable classes specifies a shared pointer injector dependency
	///
	public: template <typename ... Conf>
	static ValidationParent validate(const Conf & ... conf) {
		return performValidation(std::shared_ptr<Injector>(), conf ...);
	}

	///
	/// Validate the runtime specified injector configuration.
	///
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a ValidationParent instance representing the validated injector
	/// @throw MissingDependencyException if a required dependency is missing
	/// @throw DuplicateBindingException if duplicate bindings exist
	/// @throw CyclicDependencyException if there is a cyclic dependency
	/// @throw SharedInjectorException if one of the injectable classes specifies a shared pointer injector dependency
	///
	public: static ValidationParent validate(const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) {
		return performValidation(std::shared_ptr<Injector>(), conf);
	}

	///
	/// Validate the runtime specified child injector configuration.
	///
	/// @tparam Conf the first configuration type(s) (must derive from InjectorConfiguration)
	/// @param parent the ValidationParent object representing the parent of the child injector
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a ValidationParent instance representing the validated child injector
	/// @throw MissingDependencyException if a required dependency is missing
	/// @throw DuplicateBindingException if duplicate bindings exist
	/// @throw CyclicDependencyException if there is a cyclic dependency
	/// @throw SharedInjectorException if one of the injectable classes specifies a shared pointer injector dependency
	///
	public: template <typename ... Conf>
	static ValidationParent validateChild(ValidationParent parent, const Conf & ... conf) {
		return performValidation(parent.parent, conf ...);
	}

	///
	/// Validate the runtime specified child injector configuration.
	///
	/// @param parent the ValidationParent object representing the parent of the child injector
	/// @param conf the runtime supplied InjectorConfiguration objects
	/// @return a ValidationParent instance representing the validated child injector
	/// @throw MissingDependencyException if a required dependency is missing
	/// @throw DuplicateBindingException if duplicate bindings exist
	/// @throw CyclicDependencyException if there is a cyclic dependency
	/// @throw SharedInjectorException if one of the injectable classes specifies a shared pointer injector dependency
	///
	public: static ValidationParent validateChild(const ValidationParent & parent,
	                                              const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) {
		return performValidation(parent.parent, conf);
	}

	///
	/// Returns true if the injector or its parent has a binding matching the specified type and name.
	///
	/// The supplied type InstT is ValueT, std::unique_ptr<BaseT>, BaseT &, or std::shared_ptr<BaseT>,
	/// and also determines the binding meta-type (Value, Unique, Reference or Shared).
	///
	/// @tparam InstT the full binding type (this also determines the meta-type)
	/// @param name the name of the binding (default = empty)
	/// @return true if a binding with the specified key exists
	///
	public: template <typename InstT>
	bool hasBinding(std::string_view name = std::string_view()) const {
		return GetInstance<InstT>(this).hasBinding(name);
	}

	///
	/// Used for binding error reporting.
	///
	/// The supplied type InstT is ValueT, std::unique_ptr<BaseT>, BaseT &, or std::shared_ptr<BaseT>,
	/// and also determines the binding meta-type (Value, Unique, Reference or Shared).
	///
	/// @tparam InstT the full binding type (this also determines the meta-type)
	/// @param name the name of the binding (default = empty)
	/// @return a binding key
	///
	public: template <typename InstT>
	static Impl::BindingKey createBindingKey(const std::string & name = "") {
		return GetInstance<InstT>::createBindingKey(name);
	}

	///
	/// Print the bindings of this injector and optionally the injector's ancestors.
	///
	/// The bindings will be printed to the "balau.injector" logging namespace at info level.
	///
	/// @param printAncestor print ancestor bindings as well (default = false)
	/// @return a string containing a the binding printout
	///
	public: std::string printBindings(bool printAncestor = false) const {
		return performPrintBindings(printAncestor);
	}

	///
	/// Print detailed bindings of this injector and the injector's ancestors.
	///
	/// This version of the print functions prints the bindings and their dependencies.
	/// This can be useful for examining cyclic dependency issues.
	///
	/// The bindings will be printed to the "balau.injector" logging namespace at info level.
	///
	/// @param printAncestor print ancestor bindings as well (default = false)
	/// @return a string containing a the binding printout
	///
	public: std::string printBindingsDetailed() const {
		return performPrintBindingsDetailed();
	}

	/////////////////// Instantiation using full return type //////////////////

	///
	/// Get an instance, the meta-type of which is determined by the specified type.
	///
	/// If the type InstT is a std::shared_ptr<BaseT>, the getShared() method is implicitly called.
	/// If the type InstT is a BaseT &, the getReference() method is implicitly called.
	/// If the type InstT is a std::unique_ptr<BaseT>, the getUnique() method is implicitly called.
	/// Otherwise, the getValue() method is implicitly called.
	///
	/// @tparam InstT the complete type of the value, unique, reference, or shared binding
	/// @param name the name of the binding
	/// @return a value, unique_ptr, reference, or shared_ptr depending on the specified meta-type
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename InstT>
	InstT getInstance(std::string_view name = std::string_view()) const {
		return GetInstance<InstT>(this).get(name);
	}

	/////////////////////////// Value instantiation ///////////////////////////

	///
	/// Get-instance method for values (non-polymorphic new instances).
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// A value may be instance sourced (stack constructed or copied from a prototype).
	///
	/// @tparam ValueT the type of the value binding
	/// @param name the name of the binding
	/// @return a new value
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename ValueT>
	ValueT getValue(std::string_view name = std::string_view()) const {
		return GetInstance<ValueT>(this).get(name);
	}

	///
	/// Get-instance method for values (non-polymorphic new instances).
	///
	/// If no suitable binding is found, the supplied default value is copied into the return value.
	///
	/// A value may be instance sourced (stack constructed or copied from a prototype).
	///
	/// @tparam ValueT the type of the value binding
	/// @param defaultValue a default value to copy if no suitable binding was found
	/// @return a new value or a copy of the supplied default value
	///
	public: template <typename ValueT>
	ValueT getValue(const ValueT & defaultValue) const {
		return GetInstance<ValueT>(this).get(std::string_view(), defaultValue);
	}

	///
	/// Get-instance method for values (non-polymorphic new instances).
	///
	/// If no suitable binding is found, the supplied default value is copied into the return value.
	///
	/// A value may be instance sourced (stack constructed or copied from a prototype).
	///
	/// @tparam ValueT the type of the value binding
	/// @param name the name of the binding
	/// @param defaultValue a default value to copy if no suitable binding was found
	/// @return a new value or a copy of the supplied default value
	///
	public: template <typename ValueT>
	ValueT getValue(std::string_view name, const ValueT & defaultValue) const {
		return GetInstance<ValueT>(this).get(name, defaultValue);
	}

	///
	/// Get-instance method for values (non-polymorphic new instances).
	///
	/// If no suitable binding is found, a default value is obtained by the supplied function.
	///
	/// A value may be instance sourced (stack constructed or copied from a prototype).
	///
	/// @tparam ValueT the type of the value binding
	/// @param defaultValueSupplier a function that supplies a default value if no suitable binding is found
	/// @return a new value or a default value from the function
	///
	public: template <typename ValueT>
	ValueT getValue(const std::function<ValueT ()> & defaultValueSupplier) const {
		return GetInstance<ValueT>(this).get(std::string_view(), defaultValueSupplier);
	}

	///
	/// Get-instance method for values (non-polymorphic new instances).
	///
	/// If no suitable binding is found, a default value is obtained by the supplied function.
	///
	/// A value may be instance sourced (stack constructed or copied from a prototype).
	///
	/// @tparam ValueT the type of the value binding
	/// @param name the name of the binding
	/// @param defaultValueSupplier a function that supplies a default value if no suitable binding is found
	/// @return a new value or a copy of the supplied default value
	///
	public: template <typename ValueT>
	ValueT getValue(std::string_view name, const std::function<ValueT ()> & defaultValueSupplier) const {
		return GetInstance<ValueT>(this).get(name, defaultValueSupplier);
	}

	////////////////////////// Unique instantiation ///////////////////////////

	///
	/// Get-instance method for unique pointers (polymorphic new instances).
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// A std::unique_ptr may be instance sourced.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param name the name of the binding
	/// @return a unique pointer containing the new instance
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename BaseT>
	std::unique_ptr<BaseT> getUnique(std::string_view name = std::string_view()) const {
		return GetInstance<std::unique_ptr<BaseT>>(this).get(name);
	}

	///
	/// Get-instance method for unique pointers (polymorphic new instances).
	///
	/// If no suitable binding is found, the supplied default value is moved into the return value.
	///
	/// A std::unique_ptr may be instance sourced.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param defaultValue a default value to move if no suitable binding was found
	/// @return a unique pointer containing the new instance or the supplied default
	///
	public: template <typename BaseT>
	std::unique_ptr<BaseT> getUnique(std::unique_ptr<BaseT> && defaultValue) const {
		return GetInstance<std::unique_ptr<BaseT>>(this).get("", std::move(defaultValue));
	}

	///
	/// Get-instance method for unique pointers (polymorphic new instances).
	///
	/// If no suitable binding is found, the supplied default value is moved into the return value.
	///
	/// A std::unique_ptr may be instance sourced.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param name the name of the binding
	/// @param defaultValue a default value to move if no suitable binding was found
	/// @return a unique pointer containing the new instance or the supplied default
	///
	public: template <typename BaseT>
	std::unique_ptr<BaseT> getUnique(std::string_view name, std::unique_ptr<BaseT> && defaultValue) const {
		return GetInstance<std::unique_ptr<BaseT>>(this).get(name, std::move(defaultValue));
	}

	///
	/// Get-instance method for unique pointers (polymorphic new instances).
	///
	/// If no suitable binding is found, a default value is obtained by the supplied function.
	///
	/// A std::unique_ptr may be instance sourced.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param defaultValueSupplier a function that supplies a default value if no suitable binding is found
	/// @return a unique pointer containing the new instance or the supplied default
	///
	public: template <typename BaseT>
	std::unique_ptr<BaseT> getUnique(std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
		return GetInstance<std::unique_ptr<BaseT>>(this).get("", defaultValueSupplier);
	}

	///
	/// Get-instance method for unique pointers (polymorphic new instances).
	///
	/// If no suitable binding is found, a default value is obtained by the supplied function.
	///
	/// A std::unique_ptr may be instance sourced.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param name the name of the binding
	/// @param defaultValueSupplier a function that supplies a default value if no suitable binding is found
	/// @return a unique pointer containing the new instance or the supplied default
	///
	public: template <typename BaseT>
	std::unique_ptr<BaseT> getUnique(std::string_view name, std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
		return GetInstance<std::unique_ptr<BaseT>>(this).get(name, defaultValueSupplier);
	}

	///
	/// Get-instance method for unique pointers (polymorphic new instances).
	///
	/// Return an empty unique_ptr if there is no suitable binding.
	///
	/// A std::unique_ptr may be instance sourced.
	///
	/// @tparam BaseT the base type of the unique binding
	/// @param name the name of the binding
	/// @return a unique pointer containing the new instance or nullptr
	///
	public: template <typename BaseT>
	std::unique_ptr<BaseT> getUniqueOrNull(std::string_view name = std::string_view()) const {
		return GetInstance<std::unique_ptr<BaseT>>(this).get(name, std::unique_ptr<BaseT>());
	}

	/////////////////////////// Reference instances ///////////////////////////

	///
	/// Get-instance method for reference providers.
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// @tparam BaseT the base type of the reference binding
	/// @param name the name of the binding
	/// @return the reference supplied by the binding
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename BaseT> BaseT & getReference(std::string_view name = std::string_view()) const {
		return GetInstance<BaseT &>(this).get(name);
	}

	///
	/// Get-instance method for reference providers.
	///
	/// If no suitable binding is found, the supplied default value is copied into the return value.
	///
	/// @tparam BaseT the base type of the reference binding
	/// @param defaultValue a default reference to return if no suitable binding was found
	/// @return the reference supplied by the binding or the supplied default reference
	///
	public: template <typename BaseT> BaseT & getReference(BaseT & defaultValue) const {
		return GetInstance<BaseT &>(this).get(std::string_view(), defaultValue);
	}

	///
	/// Get-instance method for reference providers.
	///
	/// If no suitable binding is found, the supplied default value is referenced instead.
	///
	/// @tparam BaseT the base type of the reference binding
	/// @param name the name of the binding
	/// @param defaultValue a default reference to return if no suitable binding was found
	/// @return the reference supplied by the binding or the supplied default reference
	///
	public: template <typename BaseT> BaseT & getReference(std::string_view name, BaseT & defaultValue) const {
		return GetInstance<BaseT &>(this).get(name, defaultValue);
	}

	//////////////////////////// Shared instances /////////////////////////////

	///
	/// Get-instance method for shared pointers (polymorphic thread-local or non-thread-local singletons).
	///
	/// If no suitable binding is found, NoBindingException is thrown.
	///
	/// A std::shared_ptr may be singleton or thread-local singleton sourced.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param name the name of the binding
	/// @return a shared pointer containing the instance
	/// @throw NoBindingException if no suitable binding was found
	///
	public: template <typename BaseT> std::shared_ptr<BaseT> getShared(std::string_view name = std::string_view()) const {
		return GetInstance<std::shared_ptr<BaseT>>(this).get(name);
	}

	///
	/// Get-instance method for shared pointers (polymorphic thread-local or non-thread-local singletons).
	///
	/// If no suitable binding is found, the supplied default value is shared into the return value.
	///
	/// A std::shared_ptr may be singleton or thread-local singleton sourced.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param defaultValue a default pointer to copy if no suitable binding was found
	/// @return a shared pointer containing the instance or the default pointer
	///
	public: template <typename BaseT>
	std::shared_ptr<BaseT> getShared(const std::shared_ptr<BaseT> & defaultValue) const {
		return GetInstance<std::shared_ptr<BaseT>>(this).get(std::string_view(), defaultValue);
	}

	///
	/// Get-instance method for shared pointers (polymorphic thread-local or non-thread-local singletons).
	///
	/// If no suitable binding is found, the supplied default value is shared into the return value.
	///
	/// A std::shared_ptr may be singleton or thread-local singleton sourced.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param name the name of the binding
	/// @param defaultValue a default pointer to copy if no suitable binding was found
	/// @return a shared pointer containing the instance or the default pointer
	///
	public: template <typename BaseT>
	std::shared_ptr<BaseT> getShared(std::string_view name, const std::shared_ptr<BaseT> & defaultValue) const {
		return GetInstance<std::shared_ptr<BaseT>>(this).get(name, defaultValue);
	}

	///
	/// Get-instance method for shared pointers (polymorphic thread-local or non-thread-local singletons).
	///
	/// If no suitable binding is found, a default value is obtained by the supplied function.
	///
	/// A std::shared_ptr may be singleton or thread-local singleton sourced.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param defaultValueSupplier a function that supplies a default value if no suitable binding is found
	/// @return a shared pointer containing the instance or the default pointer
	///
	public: template <typename BaseT>
	std::shared_ptr<BaseT> getShared(const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
		return GetInstance<std::shared_ptr<BaseT>>(this).get(std::string_view(), defaultValueSupplier);
	}

	///
	/// Get-instance method for shared pointers (polymorphic thread-local or non-thread-local singletons).
	///
	/// If no suitable binding is found, a default value is obtained by the supplied function.
	///
	/// A std::shared_ptr may be singleton or thread-local singleton sourced.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param name the name of the binding
	/// @param defaultValueSupplier a function that supplies a default value if no suitable binding is found
	/// @return a shared pointer containing the instance or the default pointer
	///
	public: template <typename BaseT>
	std::shared_ptr<BaseT> getShared(std::string_view name, const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
		return GetInstance<std::shared_ptr<BaseT>>(this).get(name, defaultValueSupplier);
	}

	///
	/// Get-instance method for shared pointers (polymorphic thread-local or non-thread-local singletons).
	///
	/// Return an empty shared_ptr if no suitable binding is found.
	///
	/// A std::shared_ptr may be singleton or thread-local singleton sourced.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param name the name of the binding
	/// @return a shared pointer containing the instance or nullptr
	///
	public: template <typename BaseT>
	std::shared_ptr<BaseT> getSharedOrNull(std::string_view name = std::string_view()) const {
		return GetInstance<std::shared_ptr<BaseT>>(this).get(name, std::shared_ptr<BaseT>());
	}

	/////////////////////////// Singleton iteration ///////////////////////////

	///
	/// For all singleton bindings of the specified type, call the supplied function with the binding's singleton object.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param func the function to call
	/// @param includeThreadLocal if true then thread local bindings will be include (default is false)
	///
	public: template <typename BaseT>
	void iterate(const std::function<void (std::shared_ptr<BaseT>)> & func, bool includeThreadLocal = false) const {
		iterateOverShared<BaseT>(func, includeThreadLocal);
	}

	///
	/// For all singleton bindings of the specified type, call the supplied function with the binding's singleton object.
	///
	/// @tparam BaseT the base type of the shared binding
	/// @param func the function to call
	/// @param includeThreadLocal if true then thread local bindings will be include (default is false)
	///
	public: template <typename BaseT>
	void iterate(const std::function<void (std::shared_ptr<const BaseT>)> & func, bool includeThreadLocal = false) const {
		iterateOverShared<BaseT>(func, includeThreadLocal);
	}

	///////////////////////// Private implementation //////////////////////////

	//
	// Non-specialised GetInstance template class.
	// This GetInstance template class deals with the non-const value meta-type.
	//
	private: template <typename T> struct GetInstance {
		T get(std::string_view name) const {
			return injector->getValueImpl<T>(name);
		}

		T get(std::string_view name, const T & defaultValue) const {
			return injector->getValueImpl<T>(name, defaultValue);
		}

		T get(std::string_view name, const std::function<T ()> & defaultValueSupplier) const {
			return injector->getValueImpl<T>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const T.
	// This immediately promotes to T.
	//
	private: template <typename T> struct GetInstance<const T> {
		using U = typename std::remove_const<T>::type;

		U get(std::string_view name) const {
			return injector->getValueImpl<U>(name);
		}

		U get(std::string_view name, const T & defaultValue) const {
			return injector->getValueImpl<U>(name, defaultValue);
		}

		U get(std::string_view name, const std::function<T ()> & defaultValueSupplier) const {
			return injector->getValueImpl<U>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::unique_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<std::unique_ptr<BaseT>> {
		std::unique_ptr<BaseT> get(std::string_view name) const {
			return injector->getUniqueImpl<BaseT>(name);
		}

		std::unique_ptr<BaseT> get(std::string_view name, std::unique_ptr<BaseT> & defaultValue) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValue);
		}

		std::unique_ptr<BaseT> get(std::string_view name, const std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const std::unique_ptr<T>.
	// This immediately promotes to std::unique_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<const std::unique_ptr<BaseT>> {
		std::unique_ptr<BaseT> get(std::string_view name) const {
			return injector->getUniqueImpl<BaseT>(name);
		}

		std::unique_ptr<BaseT> get(std::string_view name, std::unique_ptr<BaseT> & defaultValue) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValue);
		}

		std::unique_ptr<BaseT> get(std::string_view name, const std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::unique_ptr<const T>.
	// This immediately promotes to std::unique_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<std::unique_ptr<const BaseT>> {
		std::unique_ptr<BaseT> get(std::string_view name) const {
			return injector->getUniqueImpl<BaseT>(name);
		}

		std::unique_ptr<BaseT> get(std::string_view name, std::unique_ptr<BaseT> & defaultValue) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValue);
		}

		std::unique_ptr<BaseT> get(std::string_view name, const std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const std::unique_ptr<const T>.
	// This immediately promotes to std::unique_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<const std::unique_ptr<const BaseT>> {
		std::unique_ptr<BaseT> get(std::string_view name) const {
			return injector->getUniqueImpl<BaseT>(name);
		}

		std::unique_ptr<BaseT> get(std::string_view name, std::unique_ptr<BaseT> & defaultValue) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValue);
		}

		std::unique_ptr<BaseT> get(std::string_view name, const std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getUniqueImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for T &.
	//
	private: template <typename T> struct GetInstance<T &> {
		T & get(std::string_view name) const {
			return injector->getReferenceImpl<T>(name);
		}

		T & get(std::string_view name, T & defaultValue) const {
			return injector->getReferenceImpl<T>(name, defaultValue);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Reference, T>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Reference, T>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const T &.
	// This will use a suitable non-const T & binding if no const T & binding is not available.
	//
	private: template <typename T> struct GetInstance<const T &> {
		const T & get(std::string_view name) const {
			return injector->getReferenceImpl<const T>(name);
		}

		const T & get(std::string_view name, const T & defaultValue) const {
			return injector->getReferenceImpl<const T>(name, defaultValue);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Reference, const T>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Reference, const T>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::shared_ptr<BaseT>.
	//
	private: template <typename BaseT> struct GetInstance<std::shared_ptr<BaseT>> {
		std::shared_ptr<BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<BaseT>(name);
		}

		std::shared_ptr<BaseT> get(std::string_view name, const std::shared_ptr<BaseT> & defaultValue) const {
			return injector->getSharedImpl<BaseT>(name, defaultValue);
		}

		std::shared_ptr<BaseT> get(std::string_view name, const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::shared_ptr<BaseT>.
	// This immediately promotes to std::shared_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<const std::shared_ptr<BaseT>> {
		std::shared_ptr<BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<BaseT>(name);
		}

		std::shared_ptr<BaseT> get(std::string_view name, const std::shared_ptr<BaseT> & defaultValue) const {
			return injector->getSharedImpl<BaseT>(name, defaultValue);
		}

		std::shared_ptr<BaseT> get(std::string_view name, const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::shared_ptr<const T>.
	// This will use a suitable non-const std::shared_ptr<const T> binding
	// if no const std::shared_ptr<const T> binding is not available.
	//
	private: template <typename BaseT> struct GetInstance<std::shared_ptr<const BaseT>> {
		std::shared_ptr<const BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<const BaseT>(name);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, std::shared_ptr<const BaseT> & defaultValue) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValue);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, const std::function<std::shared_ptr<const BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const std::shared_ptr<const T>.
	// This immediately promotes to std::shared_ptr<const T>.
	//
	private: template <typename BaseT> struct GetInstance<const std::shared_ptr<const BaseT>> {
		std::shared_ptr<const BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<const BaseT>(name);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, std::shared_ptr<const BaseT> & defaultValue) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValue);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, const std::function<std::shared_ptr<const BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::weak_ptr<T>.
	// This immediately promotes to std::shared_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<std::weak_ptr<BaseT>> {
		std::shared_ptr<BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<BaseT>(name);
		}

		std::shared_ptr<BaseT> get(std::string_view name, std::shared_ptr<BaseT> & defaultValue) const {
			return injector->getSharedImpl<BaseT>(name, defaultValue);
		}

		std::shared_ptr<BaseT> get(std::string_view name, const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const std::weak_ptr<T>.
	// This immediately promotes to std::shared_ptr<T>.
	//
	private: template <typename BaseT> struct GetInstance<const std::weak_ptr<BaseT>> {
		std::shared_ptr<BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<BaseT>(name);
		}

		std::shared_ptr<BaseT> get(std::string_view name, std::shared_ptr<BaseT> & defaultValue) const {
			return injector->getSharedImpl<BaseT>(name, defaultValue);
		}

		std::shared_ptr<BaseT> get(std::string_view name, const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for std::weak_ptr<const T>.
	// This immediately promotes to std::shared_ptr<const T>.
	//
	private: template <typename BaseT> struct GetInstance<std::weak_ptr<const BaseT>> {
		std::shared_ptr<const BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<const BaseT>(name);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, std::shared_ptr<const BaseT> & defaultValue) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValue);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, const std::function<std::shared_ptr<const BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	//
	// GetInstance template class specialised for const std::weak_ptr<const T>.
	// This immediately promotes to std::shared_ptr<const T>.
	//
	private: template <typename BaseT> struct GetInstance<const std::weak_ptr<const BaseT>> {
		std::shared_ptr<const BaseT> get(std::string_view name) const {
			return injector->getSharedImpl<const BaseT>(name);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, std::shared_ptr<const BaseT> & defaultValue) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValue);
		}

		std::shared_ptr<const BaseT> get(std::string_view name, const std::function<std::shared_ptr<const BaseT> ()> & defaultValueSupplier) const {
			return injector->getSharedImpl<const BaseT>(name, defaultValueSupplier);
		}

		bool hasBinding(std::string_view name) const {
			return injector->hasBinding(createBindingKeyView(name));
		}

		static Impl::BindingKey createBindingKey(std::string name) {
			return Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), std::move(name));
		}

		static Impl::BindingKeyView createBindingKeyView(std::string_view name) {
			return Impl::BindingKeyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, const BaseT>), name);
		}

		explicit GetInstance(const Injector * injector_) : injector(injector_) {}
		const Injector * injector;
	};

	///////////////////////////////////////////////////////////////////////////

	private: template <typename T> T getValueImpl(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return createValueInstance<T>(*binding);
		} else if (parent) {
			return parent->getValueImpl<T>(name);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	private: template <typename T> T getValueImpl(std::string_view name, const T & defaultValue) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return createValueInstance<T>(*binding);
		} else if (parent) {
			return parent->getValueImpl<T>(name, defaultValue);
		} else {
			return defaultValue;
		}
	}

	private: template <typename T> T getValueImpl(std::string_view name, const std::function<T ()> & defaultValueSupplier) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Value, T>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return createValueInstance<T>(*binding);
		} else if (parent) {
			return parent->getValueImpl<T>(name, defaultValueSupplier);
		} else {
			return defaultValueSupplier();
		}
	}

	private: template <typename T>
	T & getReferenceImpl(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Reference, T>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return getReferenceInstance<T>(*binding);
		} else if (parent) {
			return parent->getReferenceImpl<T>(name);
		} else {
			// Try a non-const binding?
			if (Impl::BindingKeyType<Impl::BindingMetaType::Reference, T>::isConst) {
				return getReferenceImpl<typename std::remove_const<T>::type>(name);
			} else {
				ThrowBalauException(Exception::NoBindingException, keyView.toKey());
			}
		}
	}

	private: template <typename T>
	T & getReferenceImpl(std::string_view name, T & defaultValue) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Reference, T>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return getReferenceInstance<T>(*binding);
		} else if (parent) {
			return parent->getReferenceImpl<T>(name, defaultValue);
		} else {
			// Try a non-const binding?
			if (Impl::BindingKeyType<Impl::BindingMetaType::Reference, T>::isConst) {
				return getReferenceImpl<typename std::remove_const<T>::type>(name, defaultValue);
			} else {
				return defaultValue;
			}
		}
	}

	private: template <typename BaseT>
	std::unique_ptr<BaseT> getUniqueImpl(std::string_view name) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return createUniqueInstance<BaseT>(*binding);
		} else if (parent) {
			return parent->getUniqueImpl<BaseT>(name);
		} else {
			ThrowBalauException(Exception::NoBindingException, keyView.toKey());
		}
	}

	private: template <typename BaseT>
	std::unique_ptr<BaseT> getUniqueImpl(std::string_view name, std::unique_ptr<BaseT> && defaultValue) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return createUniqueInstance<BaseT>(*binding);
		} else if (parent) {
			return parent->getUniqueImpl<BaseT>(name, std::move(defaultValue));
		} else {
			return std::move(defaultValue);
		}
	}

	private: template <typename BaseT>
	std::unique_ptr<BaseT> getUniqueImpl(std::string_view name, std::function<std::unique_ptr<BaseT> ()> & defaultValueSupplier) const {
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Unique, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return createUniqueInstance<BaseT>(*binding);
		} else if (parent) {
			return parent->getUniqueImpl<BaseT>(name, defaultValueSupplier);
		} else {
			return defaultValueSupplier();
		}
	}

	private: template <typename BaseT>
	std::shared_ptr<BaseT> getSharedImpl(std::string_view name) const {
		const auto typeIndex = std::type_index(typeid(BaseT));
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return getSharedInstance<BaseT>(*binding);
		} else if (typeIndex == std::type_index(typeid(Injector))) {
			// If we are here, then BaseT == Injector and the cast disappears.
			auto ptr = std::const_pointer_cast<Injector>(shared_from_this());
			void * p1 = static_cast<void *>(&ptr);
			auto * p2 = static_cast<std::shared_ptr<BaseT> *>(p1);
			return *p2;
		} else if (parent) {
			return parent->getSharedImpl<BaseT>(name);
		} else {
			// Try a non-const binding?
			if (Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>::isConst) {
				return getSharedImpl<typename std::remove_const<BaseT>::type>(name);
			} else {
				ThrowBalauException(Exception::NoBindingException, keyView.toKey());
			}
		}
	}

	private: template <typename BaseT>
	std::shared_ptr<BaseT> getSharedImpl(std::string_view name, const std::shared_ptr<BaseT> & defaultValue) const {
		const auto typeIndex = std::type_index(typeid(BaseT));
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return getSharedInstance<BaseT>(*binding);
		} else if (typeIndex == std::type_index(typeid(Injector))) {
			// If we are here, then BaseT == Injector and the cast disappears.
			auto ptr = std::const_pointer_cast<Injector>(shared_from_this());
			void * p1 = static_cast<void *>(&ptr);
			auto * p2 = static_cast<std::shared_ptr<BaseT> *>(p1);
			return *p2;
		} else if (parent) {
			return parent->getSharedImpl<BaseT>(name, defaultValue);
		} else {
			// Try a non-const binding?
			if (Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>::isConst) {
				return getSharedImpl<typename std::remove_const<BaseT>::type>(name, defaultValue);
			} else {
				return defaultValue;
			}
		}
	}

	private: template <typename BaseT>
	std::shared_ptr<BaseT> getSharedImpl(std::string_view name, const std::function<std::shared_ptr<BaseT> ()> & defaultValueSupplier) const {
		const auto typeIndex = std::type_index(typeid(BaseT));
		const Impl::BindingKeyView keyView(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), name);

		const auto * binding = bindings->find(keyView);

		if (binding != nullptr) {
			return getSharedInstance<BaseT>(*binding);
		} else if (typeIndex == std::type_index(typeid(Injector))) {
			// If we are here, then BaseT == Injector and the cast disappears.
			auto ptr = std::const_pointer_cast<Injector>(shared_from_this());
			void * p1 = static_cast<void *>(&ptr);
			auto * p2 = static_cast<std::shared_ptr<BaseT> *>(p1);
			return *p2;
		} else if (parent) {
			return parent->getSharedImpl<BaseT>(name, defaultValueSupplier);
		} else {
			// Try a non-const binding?
			if (Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>::isConst) {
				return getSharedImpl<typename std::remove_const<BaseT>::type>(name, defaultValueSupplier);
			} else {
				return defaultValueSupplier();
			}
		}
	}

	private: template <typename BaseT>
	void iterateOverShared(const std::function<void (std::shared_ptr<BaseT>)> & func, bool includeThreadLocal = false) {
		const auto typeIndex = std::type_index(typeid(BaseT));

		for (const auto & binding : (*bindings)) {
			if (binding.key.getType() == typeIndex) {
				if (includeThreadLocal || !binding.value->isThreadLocalBinding()) {
					func(getSharedInstance<BaseT>(binding.value));
				}
			}
		}
	}

	private: template <typename BaseT>
	void iterateOverShared(const std::function<void (std::shared_ptr<const BaseT>)> & func, bool includeThreadLocal = false) {
		const auto typeIndex = std::type_index(typeid(BaseT));

		for (const auto & binding : (*bindings)) {
			if (binding.key.getType() == typeIndex) {
				if (includeThreadLocal || !binding.value->isThreadLocalBinding()) {
					func(getSharedInstance<BaseT>(binding.value));
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////

	// Create a non-polymorphic value from the supplied value, prototype, or value provider
	// binding. The type T has been tunnelled through and matched to the binding.
	private: template <typename T>
	T createValueInstance(const std::unique_ptr<Impl::AbstractBinding> & binding) const {
		return static_cast<const Impl::AbstractValueBinding<T> *>(binding.get())->get(this);
	}

	// Get a reference from the supplied reference binding. The type T has been
	// tunnelled through and matched to the binding.
	private: template <typename T>
	T & getReferenceInstance(const std::unique_ptr<Impl::AbstractBinding> & binding) const {
		return static_cast<const Impl::AbstractReferenceBinding<T> *>(binding.get())->get(this);
	}

	// Create a polymorphic unique ptr from the supplied unique ptr or unique ptr provider
	// binding. The type T has been tunnelled through and matched to the binding.
	private: template <typename T>
	std::unique_ptr<T> createUniqueInstance(const std::unique_ptr<Impl::AbstractBinding> & binding) const {
		return static_cast<const Impl::AbstractUniquePtrBinding<T> *>(binding.get())->get(this);
	}

	// Get a polymorphic shared ptr from the supplied thread-local singleton or singleton binding.
	// The type T has been tunnelled through and matched to the binding.
	private: template <typename T>
	std::shared_ptr<T> getSharedInstance(const std::unique_ptr<Impl::AbstractBinding> & binding) const {
		return static_cast<const Impl::AbstractSharedPtrBinding<T> *>(binding.get())->get(this);
	}

	///////////////////////////////////////////////////////////////////////////

	// Used to determine presence of a binding the injector or its parent.
	private: bool hasBinding(const Impl::BindingKeyView & keyView) const {
		return bindings->hasBinding(keyView) || (parent && parent->hasBinding(keyView));
	}

	public: Injector(const Injector &) = delete;
	public: Injector & operator = (const Injector &) = delete;

	//
	// Environment property text is owned by the injector.
	//
	// This allows the const char pointers of the strings to be used in the
	// binding keys without dangling pointer issues.
	//
	using PropertyTextVectorPtr = std::shared_ptr<std::vector<std::string>>;

	private: template <typename ... Conf>
	static std::shared_ptr<Injector> createInjector(const std::shared_ptr<const Injector> & parent, const Conf & ... conf) {
		auto injector = std::shared_ptr<Injector>(new Injector(parent, conf ...)); // NOLINT
		Impl::BindingGraph graph;
		injector->performValidation(graph);

		// Instantiate eager singletons in the correct order.
		for (auto & key : graph.dependencyOrder()) {
			injector->bindings->get(key)->instantiateIfEager(*injector);
		}

		Impl::InjectorLogger::log().info(injector->printBindings(false).c_str());

		for (const auto & f : injector->postConstructionCalls) {
			f(*injector);
		}

		return injector;
	}

	private: static std::shared_ptr<Injector> createInjector(const std::shared_ptr<const Injector> & parent,
	                                                         const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) {
		auto injector = std::shared_ptr<Injector>(new Injector(parent, conf)); // NOLINT
		Impl::BindingGraph graph;
		injector->performValidation(graph);

		// Instantiate eager singletons in the correct order.
		for (auto & key : graph.dependencyOrder()) {
			injector->bindings->get(key)->instantiateIfEager(*injector);
		}

		Impl::InjectorLogger::log().info(injector->printBindings(false).c_str());

		for (const auto & f : injector->postConstructionCalls) {
			f(*injector);
		}

		return injector;
	}

	// Main constructor.
	private: template <typename ... Conf>
	explicit Injector(std::shared_ptr<const Injector> parent_, const Conf & ... conf)
		: parent(std::move(parent_))
		, bindings(createBindings(conf ...)) {}

	// Main constructor.
	private: Injector(std::shared_ptr<const Injector> parent_, const std::vector<std::shared_ptr<InjectorConfiguration>> & conf)
		: parent(std::move(parent_))
		, bindings(createBindings(conf)) {}

	private: class PrototypeConstruction {};

	// Prototype constructor.
	// Copy the bindings shared ptr instead of moving it.
	private: Injector(PrototypeConstruction,
	                  std::shared_ptr<const Injector> parent_,
	                  const std::shared_ptr<const Injector> & prototype)
		: parent(std::move(parent_))
		, bindings(prototype->bindings) {}

	public: ~Injector() {
		for (const auto & f : preDestructionCalls) {
			f();
		}
	}

	private: template <typename ... Conf>
	static std::shared_ptr<Impl::BindingMap> createBindings(const Conf & ... conf) {
		std::vector<const InjectorConfiguration *> extraConfiguration;

		auto builders = Util::Memory::makeSharedV<InjectorConfiguration, Impl::BindingBuilderBase>(
			[&extraConfiguration] (const InjectorConfiguration & conf) {
				auto b = conf.execute();
				auto extra = conf.getExtraConfiguration();
				extraConfiguration.insert(extraConfiguration.end(), extra.begin(), extra.end());
				return b;
			}
			, conf ...
		);

		for (const auto & c : extraConfiguration) {
			Util::Vectors::append(builders, c->execute());
		}

		auto bindings = std::make_shared<Impl::BindingMap>();

		for (auto & builder : builders) {
			Impl::BindingKey key(builder->key);

			// Check for duplicates.
			if (bindings->hasBinding(key)) {
				ThrowBalauException(Exception::DuplicateBindingException, key);
			}

			bindings->put(key, builder->build());
		}

		return bindings;
	}

	private: static std::shared_ptr<Impl::BindingMap> createBindings(const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) {
		std::vector<std::shared_ptr<Impl::BindingBuilderBase>> builders;

		for (const auto & c : conf) {
			Util::Vectors::append(builders, c->execute());
		}

		for (const auto & c : conf) {
			for (const auto * c2 : c->getExtraConfiguration()) {
				Util::Vectors::append(builders, c2->execute());
			}
		}

		auto bindings = std::make_shared<Impl::BindingMap>();

		for (auto & builder : builders) {
			Impl::BindingKey key(builder->key);

			// Check for duplicates.
			if (bindings->hasBinding(key)) {
				ThrowBalauException(Exception::DuplicateBindingException, key);
			}

			bindings->put(key, builder->build());
		}

		return bindings;
	}

	private: void registerPostConstructionCallImpl(const std::function<void (const Injector &)> & call) const {
		std::lock_guard<std::recursive_mutex> lock(callSync);
		postConstructionCalls.push_back(call);
	}

	private: void registerPreDestructionCallImpl(const std::function<void ()> & call) const {
		std::lock_guard<std::recursive_mutex> lock(callSync);
		preDestructionCalls.push_back(call);
	}

	private: template <typename ... Conf>
	static ValidationParent performValidation(const std::shared_ptr<const Injector> & parent, const Conf & ... conf) {
		auto injector = std::shared_ptr<Injector>(new Injector(parent, conf ...)); // NOLINT
		Impl::BindingGraph graph;
		injector->performValidation(graph);
		return ValidationParent(injector);
	}

	private: static ValidationParent performValidation(const std::shared_ptr<const Injector> & parent,
	                                                   const std::vector<std::shared_ptr<InjectorConfiguration>> & conf) {
		auto injector = std::shared_ptr<Injector>(new Injector(std::move(parent), conf)); // NOLINT
		Impl::BindingGraph graph;
		injector->performValidation(graph);
		return ValidationParent(injector);
	}

	private: void performValidation(Impl::BindingGraph & graph) const {
		// Create the vertices of the dependency graph.
		for (const auto & binding : *bindings) {
			const auto & key = binding.key;

			// Check for duplicates.
			if (graph.lookupDependency(key) != nullptr) {
				ThrowBalauException(Exception::DuplicateBindingException, key);
			}

			graph.addDependency(key);
		}

		const auto weak = Impl::BindingMetaType::WeakPromotion;
		const auto shared = Impl::BindingMetaType::Shared;
		const Impl::BindingKey weakKey(typeid(Impl::BindingKeyType<weak, Injector>), "");
		const Impl::BindingKey constWeakKey(typeid(Impl::BindingKeyType<weak, const Injector>), "");
		const Impl::BindingKey sharedKey(typeid(Impl::BindingKeyType<shared, Injector>), "");
		const Impl::BindingKey constSharedKey(typeid(Impl::BindingKeyType<shared, const Injector>), "");

		// Validate and create the edges of the dependency graph.
		for (const auto & binding : (*bindings)) {
			binding.value->registerDependencies(graph, weakKey, constWeakKey, sharedKey, constSharedKey);
		}

		graph.logGraph(LoggingLevel::TRACE, "Injector dependency graph");

		std::vector<std::pair<Impl::BindingKey, Impl::BindingKey>> cycleEdges;

		if (graph.hasCycles(cycleEdges)) {
			ThrowBalauException(Exception::CyclicDependencyException, "Cycle edges found:\n" + printCyclicEdges(cycleEdges));
		}
	}

	private: std::string performPrintBindings(bool printAncestor) const {
		std::ostringstream builder;
		std::string_view prefix;
		performPrintBindings(builder, prefix, printAncestor);
		return builder.str();
	}

	private: void performPrintBindings(std::ostream & builder, std::string_view & prefix, bool pa) const {
		for (const auto & binding : (*bindings)) {
			builder << prefix << Impl::toString(binding.key);
			prefix = "\n";
		}

		if (pa && parent) {
			parent->performPrintBindings(builder, prefix, pa);
		}
	}

	private: std::string performPrintBindingsDetailed() const {
		std::ostringstream builder;
		std::string_view prefix;
		performPrintBindingsDetailed(builder, prefix);
		return builder.str();
	}

	private: void performPrintBindingsDetailed(std::ostream & builder, std::string_view & prefix) const {
		for (const auto & binding : (*bindings)) {
			builder << prefix << Impl::toString(binding.key);
			prefix = "\n";

			for (const auto & dependency : binding.value->getDependencyKeys()) {
				builder << prefix << "    " << Impl::toString(dependency);
			}
		}

		if (parent) {
			parent->performPrintBindingsDetailed(builder, prefix);
		}
	}

	private: static std::string printCyclicEdges(const std::vector<std::pair<Impl::BindingKey, Impl::BindingKey>> & cycleEdges) {
		std::ostringstream builder;
		std::string_view prefix;

		for (const auto & cycleEdge : cycleEdges) {
			builder << prefix << Impl::toString(cycleEdge.first) << " --> " << Impl::toString(cycleEdge.second);
			prefix = "\n";
		}

		return builder.str();
	}

	// Main injector state.
	private: const std::shared_ptr<const Injector> parent;
	private: const std::shared_ptr<const Impl::BindingMap> bindings;

	// Data structures for callbacks.
	private: mutable std::list<std::function<void (const Injector& )>> postConstructionCalls;
	private: mutable std::list<std::function<void ()>> preDestructionCalls;
	private: mutable std::recursive_mutex callSync;
};

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR
