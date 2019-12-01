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
/// @file ApplicationConfiguration.hpp
///
/// %Application configurations specify application injector bindings.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__APPLICATION_CONFIGURATION
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__APPLICATION_CONFIGURATION

#include <Balau/Application/BindingBuilder.hpp>
#include <Balau/Application/InjectorConfiguration.hpp>

namespace Balau {

///
/// %Application configurations specify application injector bindings.
///
/// One or more application configuration implementations are passed to the
/// injector create function.
///
/// To implement a concrete application configuration class, implement this
/// class and place the binding calls in the implemented configure() method.
///
class ApplicationConfiguration : public InjectorConfiguration {
	///
	/// Create a binding specification for the specified named or unnamed interface.
	///
	/// The deleter type is only used for unique bindings, otherwise it is ignored.
	///
	protected: template <typename BaseT, typename DeleterT = std::default_delete<BaseT>>
	BindingBuilder<BaseT, DeleterT> & bind(std::string_view name = std::string_view()) const {
		return Bind<BaseT, DeleterT>(*this).bind(name);
	}

	protected: void addConfiguration(const ApplicationConfiguration & conf) const {
		extraConfiguration.push_back(&conf);
	}

	///
	/// Register with the injector a callback that will be called by the injector at the end of construction.
	///
	/// In order to use this method, inject the injector into the injectable via a
	/// weak pointer and call the method.
	///
	/// @param call the callback
	///
	protected: void registerPostConstructionCall(const std::function<void (const Injector &)> & call) const {
		postConstructionCalls.push_back(call);
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
	protected: void registerPreDestructionCall(const std::function<void ()> & call) const {
		preDestructionCalls.push_back(call);
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
	/// @tparam T the binding type
	/// @param ptrPtr a raw pointer to the statically allocated shared pointer
	/// @param name an optional binding name
	///
	protected: template <typename T> void registerStaticSingleton(std::shared_ptr<T> * ptrPtr, std::string_view name = std::string_view()) const {
		staticSingletonPostConstructionCalls.emplace_back(new StaticSingletonRegistration(ptrPtr, name));
		preDestructionCalls.push_back([ptrPtr] () { ptrPtr->reset(); });
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::vector<std::shared_ptr<Impl::BindingBuilderBase>> build() const override {
		// Move the vector because it is not used anymore after this call from the injector.
		return std::move(builders);
	}

	public: std::vector<const InjectorConfiguration *> getExtraConfiguration() const override {
		return extraConfiguration;
	}

	public: std::list<std::function<void (const Injector& )>> getPostConstructionCalls() const override {
		return postConstructionCalls;
	}

	public: std::list<std::function<void ()>> getPreDestructionCalls() const override {
		return preDestructionCalls;
	}

	public: std::list<std::unique_ptr<StaticSingletonRegistrationBase>> getStaticSingletonPostConstructionCalls() const override {
		return std::move(staticSingletonPostConstructionCalls);
	}

	private: template <typename T, typename DeleterT> struct Bind {
		static_assert(
			  std::negation<typename std::is_pointer<T>::type>::value
			, "Raw pointers are not permitted in bindings. Use Unique or Shared bindings instead."
		);

		const ApplicationConfiguration & parent;

		explicit Bind(const ApplicationConfiguration & parent_) : parent(parent_) {}

		BindingBuilder<T, DeleterT> & bind(std::string_view name) const {
			parent.builders.emplace_back(
				std::shared_ptr<Impl::BindingBuilderBase>(new BindingBuilder<T, DeleterT>(std::string(name)))
			);

			return *reinterpret_cast<BindingBuilder<T, DeleterT> *>(parent.builders.back().get());
		}
	};

	private: template <typename T, typename DeleterT> struct Bind<const T, DeleterT> {
		static_assert(
			  std::negation<typename std::is_pointer<T>::type>::value
			, "Raw pointers are not permitted in bindings. Use Unique or Shared bindings instead."
		);

		const ApplicationConfiguration & parent;

		explicit Bind(const ApplicationConfiguration & parent_) : parent(parent_) {}

		BindingBuilder<const T, DeleterT> & bind(std::string_view name) const {
			parent.builders.emplace_back(
				std::shared_ptr<Impl::BindingBuilderBase>(new BindingBuilder<const T, DeleterT>(std::string(name)))
			);

			return *reinterpret_cast<BindingBuilder<const T, DeleterT> *>(parent.builders.back().get());
		}
	};

	private: mutable std::vector<std::shared_ptr<Impl::BindingBuilderBase>> builders;
	private: mutable std::vector<const InjectorConfiguration *> extraConfiguration;
	private: mutable std::list<std::function<void (const Injector& )>> postConstructionCalls;
	private: mutable std::list<std::function<void ()>> preDestructionCalls;
	private: mutable std::list<std::unique_ptr<StaticSingletonRegistrationBase>> staticSingletonPostConstructionCalls;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__APPLICATION_CONFIGURATION
