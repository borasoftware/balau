// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Container/DependencyGraph.hpp>
#include <Balau/Concurrent/LazyValue.hpp>
#include <Balau/Concurrent/ThreadLocalInstance.hpp>
#include <Balau/Exception/InjectorExceptions.hpp>

// Avoid false positives.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau {

class Injector;
class ApplicationConfiguration;
class EnvironmentConfiguration;

namespace LoggingSystem {

class LoggingState;

} // namespace LoggingSystem

namespace Impl {

class EnvironmentConfigurationBuilder;

using BindingGraph = Container::DependencyGraph<BindingKey>;

//////////////// Non-parameterised abstract binding base class. ///////////////

// The non-parameterised abstract binding class.
class AbstractBinding {
	friend class ::Balau::Injector;
	friend class ::Balau::ApplicationConfiguration;
	friend class ::Balau::EnvironmentConfiguration;

	public: virtual ~AbstractBinding() = default;

	public: AbstractBinding(const AbstractBinding & rhs) = delete;
	public: AbstractBinding & operator = (const AbstractBinding & rhs) = delete;

	protected: explicit AbstractBinding(BindingKey && key_)
		: key(std::move(key_)) {}

	//
	// Called after the bindings are created in order to register the relationships
	// between the bindings. These form the dependency graph edges.
	//
	// Also handled are the injector binding key special cases.
	//
	private: void registerDependencies(BindingGraph & bindingGraph,
	                                   const BindingKey & weakInjectorKey,
	                                   const BindingKey & constWeakInjectorKey,
	                                   const BindingKey & sharedInjectorKey,
	                                   const BindingKey & constSharedInjectorKey) const {
		const std::vector<BindingKey> dependencyKeys = getDependencyKeys();

		for (const auto & dependencyKey : dependencyKeys) {
			checkForIllegalInjectorInjection(dependencyKey, sharedInjectorKey, constSharedInjectorKey);
			const BindingKey thisKey = determineFinalBindingKey();

			if (dependencyKey == weakInjectorKey
				|| dependencyKey == constWeakInjectorKey
				|| dependencyKey == sharedInjectorKey
				|| dependencyKey == constSharedInjectorKey) {
				continue; // Special case of injector being injected.
			} else if (bindingGraph.hasDependency(dependencyKey)) {
				bindingGraph.addRelationship(dependencyKey, thisKey);
			} else {
				ThrowBalauException(Exception::MissingDependencyException, thisKey, dependencyKey);
			}
		}
	}

	//
	// Called after the bindings are created but before the injector
	// pointer becomes available for use. Most Supplier implementations
	// do nothing, hence the default here implementation is empty.
	//
	private: virtual void instantiateIfEager(Injector & injector) = 0;

	private: virtual std::vector<BindingKey> getDependencyKeys() const = 0;

	private: virtual void checkForIllegalInjectorInjection(const BindingKey & dependencyKey,
	                                                       const BindingKey & injectorKey,
	                                                       const BindingKey & constInjectorKey) const = 0;

	private: virtual BindingKey determineFinalBindingKey() const = 0;

	private: virtual bool isThreadLocalBinding() const = 0;

	protected: BindingKey key;
};

//////////////// Abstract binding types used by the injector. /////////////////

// Abstract base class of the value bindings.
template <typename T>
class AbstractValueBinding : public AbstractBinding {
	protected: explicit AbstractValueBinding(BindingKey && key_)
		: AbstractBinding(std::move(key_)) {}

	public: virtual T get(const Injector * injector) const = 0;

	private: void checkForIllegalInjectorInjection(const BindingKey & dependencyKey,
	                                               const BindingKey & injectorKey,
	                                               const BindingKey & constInjectorKey) const override {}

	private: bool isThreadLocalBinding() const override {
		return false;
	}
};

// Abstract base class of the unique pointer bindings.
template <typename BaseT, typename DeleterT>
class AbstractUniquePtrBinding : public AbstractBinding {
	protected: explicit AbstractUniquePtrBinding(BindingKey && key_)
		: AbstractBinding(std::move(key_)) {}

	public: virtual std::unique_ptr<BaseT, DeleterT> get(const Injector * injector) const = 0;

	private: void checkForIllegalInjectorInjection(const BindingKey & dependencyKey,
	                                               const BindingKey & injectorKey,
	                                               const BindingKey & constInjectorKey) const override {}

	private: bool isThreadLocalBinding() const override {
		return false;
	}
};

// Abstract base class of the reference bindings.
template <typename BaseT>
class AbstractReferenceBinding : public AbstractBinding {
	protected: explicit AbstractReferenceBinding(BindingKey && key_)
		: AbstractBinding(std::move(key_)) {}

	public: virtual BaseT & get(const Injector * injector) const = 0;

	private: void checkForIllegalInjectorInjection(const BindingKey & dependencyKey,
	                                               const BindingKey & injectorKey,
	                                               const BindingKey & constInjectorKey) const override {}

	private: bool isThreadLocalBinding() const override {
		return false;
	}
};

// Abstract base class of the shared pointer bindings.
template <typename BaseT>
class AbstractSharedPtrBinding : public AbstractBinding {
	protected: explicit AbstractSharedPtrBinding(BindingKey && key_)
		: AbstractBinding(std::move(key_)) {}

	public: virtual std::shared_ptr<BaseT> get(const Injector * injector) const = 0;

	private: void checkForIllegalInjectorInjection(const BindingKey & dependencyKey,
	                                               const BindingKey & sharedInjectorKey,
	                                               const BindingKey & constSharedInjectorKey) const override {
		if (dependencyKey == sharedInjectorKey || dependencyKey == constSharedInjectorKey) {
			ThrowBalauException(Exception::SharedInjectorException, key);
		}
	}
};

/////////////////////////// Concrete binding types. ///////////////////////////

template <typename T>
class InstantiatingValueBinding final : public AbstractValueBinding<T> {
	public: explicit InstantiatingValueBinding(BindingKey && key_)
		: AbstractValueBinding<T>(std::move(key_)) {}

	public: T get(const Injector * injector) const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return T::BALAU_newStackInstance(*injector);
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return T::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override {
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename T>
class PrototypeBinding final : public AbstractValueBinding<T> {
	private: const T prototype;

	public: PrototypeBinding(BindingKey && key_, const T & prototype_)
		: AbstractValueBinding<T>(std::move(key_))
		, prototype(prototype_) {}

	public: T get(const Injector * ) const override {
		return prototype; // Calls copy constructor.
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override {
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename T, typename ProviderFunctionT>
class ProvidingFunctionValueBinding final : public AbstractValueBinding<T> {
	private: const ProviderFunctionT provide;

	public: ProvidingFunctionValueBinding(BindingKey && key_, ProviderFunctionT provide_)
		: AbstractValueBinding<T>(std::move(key_))
		, provide(provide_) {}

	public: T get(const Injector * ) const override {
		return provide();
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename T, typename ProviderT>
class ProvidingClassValueBinding final : public AbstractValueBinding<T> {
	private: std::unique_ptr<ProviderT> provide;

	public: explicit ProvidingClassValueBinding(BindingKey && key_)
		: AbstractValueBinding<T>(std::move(key_)) {}

	public: T get(const Injector * ) const override {
		return (*provide)();
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return ProviderT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// Compilation error? Did you forget to add an injection macro to your class?
		provide = std::unique_ptr<ProviderT>(ProviderT::BALAU_newHeapInstance(injector));
	}
};

template <typename T, typename ProviderT>
class ProvidingClassInstanceValueBinding final : public AbstractValueBinding<T> {
	private: std::shared_ptr<ProviderT> provide;

	public: ProvidingClassInstanceValueBinding(BindingKey && key_, std::shared_ptr<ProviderT> provide_)
		: AbstractValueBinding<T>(std::move(key_))
		, provide(std::move(provide_)) {}

	public: T get(const Injector * ) const override {
		return (*provide)();
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename DerivedT, typename DeleterT>
class InstantiatingUniquePtrBinding final : public AbstractUniquePtrBinding<BaseT, DeleterT> {
	public: explicit InstantiatingUniquePtrBinding(BindingKey && key_)
		: AbstractUniquePtrBinding<BaseT, DeleterT>(std::move(key_)) {}

	public: std::unique_ptr<BaseT, DeleterT> get(const Injector * injector) const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return std::unique_ptr<BaseT, DeleterT>(DerivedT::BALAU_newHeapInstance(*injector));
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return DerivedT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename DeleterT>
class ProvidingFunctionUniquePtrBinding final : public AbstractUniquePtrBinding<BaseT, DeleterT> {
	private: using UniquePtrProviderFunction = std::function<std::unique_ptr<BaseT> ()>;

	private: const UniquePtrProviderFunction provide;

	public: ProvidingFunctionUniquePtrBinding(BindingKey && key_, UniquePtrProviderFunction provide_)
		: AbstractUniquePtrBinding<BaseT, DeleterT>(std::move(key_))
		, provide(provide_) {}

	public: std::unique_ptr<BaseT, DeleterT> get(const Injector * ) const override {
		return provide();
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override {
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename ProviderT, typename DeleterT>
class ProvidingClassUniquePtrBinding final : public AbstractUniquePtrBinding<BaseT, DeleterT> {
	private: std::unique_ptr<ProviderT> provide;

	public: explicit ProvidingClassUniquePtrBinding(BindingKey && key_)
		: AbstractUniquePtrBinding<BaseT, DeleterT>(std::move(key_)) {}

	public: std::unique_ptr<BaseT, DeleterT> get(const Injector * ) const override {
		return (*provide)();
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return ProviderT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override { // TDDO
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// Compilation error? Did you forget to add an injection macro to your class?
		provide = std::unique_ptr<ProviderT>(ProviderT::BALAU_newHeapInstance(injector));
	}
};

template <typename BaseT, typename ProviderT, typename DeleterT>
class ProvidingClassInstanceUniquePtrBinding final : public AbstractUniquePtrBinding<BaseT, DeleterT> {
	private: std::shared_ptr<ProviderT> provide;

	public: ProvidingClassInstanceUniquePtrBinding(BindingKey && key_, std::shared_ptr<ProviderT> provide_)
		: AbstractUniquePtrBinding<BaseT, DeleterT>(std::move(key_))
		, provide(std::move(provide_)) {}

	public: std::unique_ptr<BaseT, DeleterT> get(const Injector * ) const override {
		return (*provide)();
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT>
class StandardReferenceBinding final : public AbstractReferenceBinding<BaseT> {
	private: BaseT & reference;

	public: StandardReferenceBinding(BindingKey && key_, BaseT & reference_)
		: AbstractReferenceBinding<BaseT>(std::move(key_))
		, reference(reference_) {}

	public: BaseT & get(const Injector * ) const override {
		return reference;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override {
		return AbstractBinding::key;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename DerivedT, typename DeleterT>
class ThreadLocalSingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	// Mutable in order to allow lazy evaluation.
	private: mutable Concurrent::ThreadLocalInstance<std::shared_ptr<BaseT>> storage;

	public: explicit ThreadLocalSingletonBinding(BindingKey && key_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * injector) const override {
		return storage(
			[&injector] () {
				// Compilation error? Did you forget to add an injection macro to your class?
				return new std::shared_ptr<BaseT>(DerivedT::BALAU_newHeapInstance(*injector), DeleterT());
			}
		);
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return DerivedT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override {
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return true;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT>
class ProvidedSingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	private: const std::shared_ptr<BaseT> instance;

	public: ProvidedSingletonBinding(BindingKey && key_, std::shared_ptr<BaseT> instance_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_))
		, instance(std::move(instance_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * ) const override {
		return instance;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename ProviderT>
class ProvidingClassSingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	private: std::shared_ptr<BaseT> instance;

	public: explicit ProvidingClassSingletonBinding(BindingKey && key_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * ) const override {
		return instance;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return ProviderT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// Compilation error? Did you forget to add an injection macro to your class?
		std::unique_ptr<ProviderT> provide = std::unique_ptr<ProviderT>(ProviderT::BALAU_newHeapInstance(injector));
		instance = (*provide)();
	}
};

template <typename BaseT, typename ProviderT>
class ProvidingClassInstanceSingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	private: std::shared_ptr<ProviderT> provide;
	private: std::shared_ptr<BaseT> instance;

	public: ProvidingClassInstanceSingletonBinding(BindingKey && key_, std::shared_ptr<ProviderT> provide_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_))
		, provide(std::move(provide_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * ) const override {
		return instance;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override { // TODO
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		instance = (*provide)();
		provide.reset(); // Single use provider.
	}
};

template <typename BaseT>
class ProvidingFunctionSingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	private: const std::function<std::shared_ptr<BaseT> ()> provide;
	private: std::shared_ptr<BaseT> instance;

	public: ProvidingFunctionSingletonBinding(BindingKey && key_, const std::function<std::shared_ptr<BaseT> ()> & provide_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_))
		, provide(provide_) {}

	public: std::shared_ptr<BaseT> get(const Injector * ) const override {
		return instance;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		return {};
	}

	private: BindingKey determineFinalBindingKey() const override {
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		instance = provide();
	}
};

template <typename BaseT, typename DerivedT, typename DeleterT>
class LazySingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	// Mutable in order to allow lazy evaluation.
	private: mutable Concurrent::LazyValue<std::shared_ptr<BaseT>> instance;

	public: explicit LazySingletonBinding(BindingKey && key_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * injector) const override {
		return instance(
			[&injector] () {
				// Compilation error? Did you forget to add an injection macro to your class?
				return std::shared_ptr<BaseT>(DerivedT::BALAU_newHeapInstance(*injector), DeleterT());
			}
		);
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return DerivedT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override {
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename DerivedT>
class LazySingletonBinding<BaseT, DerivedT, std::default_delete<DerivedT>> final : public AbstractSharedPtrBinding<BaseT> {
	// Mutable in order to allow lazy evaluation.
	private: mutable Concurrent::LazyValue<std::shared_ptr<BaseT>> instance;

	public: explicit LazySingletonBinding(BindingKey && key_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * injector) const override {
		return instance(
			[&injector] () {
				// Compilation error? Did you forget to add an injection macro to your class?
				return std::shared_ptr<BaseT>(DerivedT::BALAU_newHeapInstance(*injector));
			}
		);
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return DerivedT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override {
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// NOP
	}
};

template <typename BaseT, typename DerivedT, typename DeleterT>
class EagerSingletonBinding final : public AbstractSharedPtrBinding<BaseT> {
	private: std::shared_ptr<BaseT> instance;

	public: explicit EagerSingletonBinding(BindingKey && key_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * ) const override {
		return instance;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return DerivedT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override {
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// Compilation error? Did you forget to add an injection macro to your class?
		instance = std::shared_ptr<BaseT>(DerivedT::BALAU_newHeapInstance(injector), DeleterT());
	}
};

template <typename BaseT, typename DerivedT>
class EagerSingletonBinding<BaseT, DerivedT, std::default_delete<DerivedT>> final : public AbstractSharedPtrBinding<BaseT> {
	private: std::shared_ptr<BaseT> instance;

	public: explicit EagerSingletonBinding(BindingKey && key_)
		: AbstractSharedPtrBinding<BaseT>(std::move(key_)) {}

	public: std::shared_ptr<BaseT> get(const Injector * ) const override {
		return instance;
	}

	private: std::vector<BindingKey> getDependencyKeys() const override {
		// Compilation error? Did you forget to add an injection macro to your class?
		return DerivedT::BALAU_getDependencyKeys();
	}

	private: BindingKey determineFinalBindingKey() const override {
		// Promotes weak promotions.
		return BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>), AbstractBinding::key.getName());
	}

	private: bool isThreadLocalBinding() const override {
		return false;
	}

	private: void instantiateIfEager(Injector & injector) override {
		// Compilation error? Did you forget to add an injection macro to your class?
		instance = std::shared_ptr<BaseT>(DerivedT::BALAU_newHeapInstance(injector));
	}
};

/////////// Non-parameterised abstract binding builder base class. ////////////

// Used during binding construction.
class BindingBuilderBase {
	public: virtual ~BindingBuilderBase() = default;

	//
	// Construct a BindingBuilderBase with a partial key.
	// The meta-type will be set on the second fluent call.
	//
	protected: explicit BindingBuilderBase(std::string && name) : key(std::move(name)) {}

	friend class ::Balau::Injector;
	friend class ::Balau::EnvironmentConfiguration;
	friend class EnvironmentConfigurationBuilder;
	friend class ::Balau::LoggingSystem::LoggingState;

	// Called by the injector in order to convert the candidate into a binding.
	public: virtual std::unique_ptr<AbstractBinding> build() = 0;

	public: const BindingKey & getKey() const {
		return key;
	}

	protected: template <typename T> void setKeyType() {
		key.setType(typeid(T));
	}

	// Set by the constructor in part and via the setKeyType() method for the remainder.
	protected: BindingKey key;
};

} // namespace Impl

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING
