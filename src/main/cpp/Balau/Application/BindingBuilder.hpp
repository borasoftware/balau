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
/// @file BindingBuilder.hpp
///
/// An injector binding candidate created via the injector configuration.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__BINDING_BUILDER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__BINDING_BUILDER

#include <Balau/Application/Impl/Binding.hpp>
#include <Balau/Application/Impl/InjectorLogger.hpp>

namespace Balau {

namespace Impl {

template <typename ValueT> class ValuePropertyBindingBuilderFactory;
template <typename ValueT> class UniquePropertyBindingBuilderFactory;

} // namespace Impl

///
/// The clone function type for unique pointer prototypes.
///
template <typename BaseT>
using UniquePropertyCloner = std::function<std::unique_ptr<BaseT> (const std::unique_ptr<const BaseT> &)>;

///
/// An injector binding candidate created via the injector configuration.
///
template <typename BaseT> class BindingBuilder final : public Impl::BindingBuilderBase {
	private: using UniquePtrProviderFunction = std::function<std::unique_ptr<BaseT> ()>;

	///
	/// Bind a concrete type.
	///
	/// A new instance of the type will be created each time a value is requested.
	///
	public: void toValue() {
		warnInvalidConstQualifier(Impl::BindingMetaType::Value);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Value, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new InstantiatingValueBindingSupplier());
	}

	///
	/// Bind a concrete type to a prototype value.
	///
	/// A copy of the prototype value will be created via copy semantics each
	/// time a value is requested.
	///
	public: template <typename ValueT = BaseT> void toValue(ValueT prototype) {
		warnInvalidConstQualifier(Impl::BindingMetaType::Value);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Value, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new PrototypeBindingSupplier(prototype));
	}

	///
	/// Bind a concrete type to a value provider function.
	///
	/// The provider will be called each time a value is requested.
	///
	public: template <typename ProviderFunctionT> void toValueProvider(ProviderFunctionT provider) {
		warnInvalidConstQualifier(Impl::BindingMetaType::Value);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Value, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingFunctionValueBindingSupplier<ProviderFunctionT>(provider));
	}

	///
	/// Bind a concrete type to a value provider class.
	///
	/// The provider will be called each time a value is requested.
	///
	public: template <typename ProviderT> void toValueProvider() {
		warnInvalidConstQualifier(Impl::BindingMetaType::Value);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Value, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingClassValueBindingSupplier<ProviderT>());
	}

	///
	/// Bind a concrete type to a value provider class instance.
	///
	/// The provider instance is supplied as a shared pointer, allowing the caller to maintain shared ownership if required.
	///
	/// The provider will be called each time a value is requested.
	///
	public: template <typename ProviderT> void toValueProvider(std::shared_ptr<ProviderT> provider) {
		warnInvalidConstQualifier(Impl::BindingMetaType::Value);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Value, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingClassInstanceValueBindingSupplier<ProviderT>(std::move(provider)));
	}

	///
	/// Bind an interface to an implementing class.
	///
	/// A new instance of the polymorphic type will be created each time a value is requested.
	///
	public: template <typename DerivedT> void toUnique() {
		warnInvalidConstQualifier(Impl::BindingMetaType::Unique);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Unique, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new InstantiatingUniquePtrBindingSupplier<DerivedT>());
	}

	///
	/// Bind an interface to a unique pointer provider function.
	///
	/// The provider will be called each time a new polymorphic instance is requested.
	///
	public: void toUniqueProvider(UniquePtrProviderFunction provider) {
		warnInvalidConstQualifier(Impl::BindingMetaType::Unique);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Unique, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingFunctionUniquePtrBindingSupplier(provider));
	}

	///
	/// Bind an interface to a unique pointer provider class.
	///
	/// The provider will be called each time a new polymorphic instance is requested.
	///
	public: template <typename ProviderT> void toUniqueProvider() {
		warnInvalidConstQualifier(Impl::BindingMetaType::Unique);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Unique, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingClassUniquePtrBindingSupplier<ProviderT>());
	}

	///
	/// Bind an interface to a unique pointer provider class instance.
	///
	/// The provider instance is supplied as a shared pointer, allowing the caller to maintain shared ownership if required.
	///
	/// The provider will be called each time a new polymorphic instance is requested.
	///
	public: template <typename ProviderT> void toUniqueProvider(std::shared_ptr<ProviderT> provider) {
		warnInvalidConstQualifier(Impl::BindingMetaType::Unique);
		using U = typename std::remove_const<BaseT>::type;
		using T = Impl::BindingKeyType<Impl::BindingMetaType::Unique, U>;
		setKeyType<T>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingClassInstanceUniquePtrBindingSupplier<ProviderT>(std::move(provider)));
	}

	///
	/// Bind a reference type to a reference value.
	///
	/// The reference will be provided each time it is requested.
	///
	public: void toReference(BaseT & reference) {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Reference, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ReferenceBindingSupplier(reference));
	}

	///
	/// Bind an interface to a concrete type with thread-local singleton semantics.
	///
	/// The thread-local singleton will be instantiated lazily for each new calling thread.
	///
	public: template <typename DerivedT> void toThreadLocal() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ThreadLocalSingletonBindingSupplier<DerivedT>());
	}

	///
	/// Bind a concrete type with thread-local singleton semantics.
	///
	/// The thread-local singleton will be instantiated lazily for each new calling thread.
	///
	public: void toThreadLocal() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ThreadLocalSingletonBindingSupplier<BaseT>());
	}

	///
	/// Bind an interface to a concrete singleton type.
	///
	/// The singleton will be instantiated lazily.
	///
	public: template <typename DerivedT> void toSingleton() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new LazySingletonBindingSupplier<DerivedT, std::default_delete<DerivedT>>());
	}

	///
	/// Bind an interface to a concrete singleton type, using the supplied deleter type.
	///
	/// The singleton will be instantiated lazily.
	///
	public: template <typename DerivedT, typename DeleterT> void toSingleton() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new LazySingletonBindingSupplier<DerivedT, DeleterT>());
	}

	///
	/// Bind a concrete singleton type.
	///
	/// The singleton will be instantiated lazily.
	///
	public: void toSingleton() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new LazySingletonBindingSupplier<BaseT, std::default_delete<BaseT>>());
	}

	///
	/// Bind an interface to a specific instance.
	///
	/// This binding call allows the instance to be created and configured
	/// before being bound.
	///
	/// The injector has shared ownership of the supplied pointer.
	///
	public: void toSingleton(std::shared_ptr<BaseT> instance) {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidedSingletonBindingSupplier(instance));
	}

	///
	/// Bind an interface to a specific instance, via a pointer container type syntax.
	///
	/// The injector takes ownership of the supplied pointer.
	///
	/// Do not use this binding call in order to bind an owned raw pointer,
	/// as such a call will result in a defective application. Only use this
	/// call as if you are initialising a pointer container with a new object.
	///
	public: void toSingleton(BaseT * instance) {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		toSingleton(std::shared_ptr<BaseT>(instance));
	}

	///
	/// Bind an interface to a specific instance, via a pointer container type syntax and using the supplied deleter type.
	///
	/// The injector takes ownership of the supplied pointer.
	///
	/// Do not use this binding call in order to bind an owned raw pointer,
	/// as such a call will result in a defective application. Only use this
	/// call as if you are initialising a pointer container with a new object.
	///
	public: template <typename DeleterT> void toSingleton(BaseT * instance) {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		toSingleton(std::shared_ptr<BaseT>(instance, DeleterT()));
	}

	///
	/// Bind an interface to a singleton provider class.
	///
	/// The provider will be called once when the singleton is first requested.
	///
	public: template <typename ProviderT> void toSingletonProvider() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingClassSingletonBindingSupplier<ProviderT>());
	}

	///
	/// Bind an interface to a singleton provider class instance.
	///
	/// The provider instance is supplied as a shared pointer, allowing the caller to maintain shared ownership if required.
	///
	/// The provider will be called once when the singleton is first requested.
	///
	public: template <typename ProviderT> void toSingletonProvider(std::shared_ptr<ProviderT> provider) {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingClassInstanceSingletonBindingSupplier<ProviderT>(std::move(provider)));
	}

	///
	/// Bind an interface to a singleton provider function.
	///
	/// The provider will be called once when the singleton is first requested.
	///
	public: void toSingletonProvider(std::function<std::shared_ptr<BaseT> ()> & provider) {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new ProvidingFunctionSingletonBindingSupplier(provider));
	}

	///
	/// Bind an interface to a concrete singleton type.
	///
	/// The singleton will be instantiated eagerly.
	///
	public: template <typename DerivedT> void toEagerSingleton() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new EagerSingletonBindingSupplier<DerivedT, std::default_delete<DerivedT>>());
	}

	///
	/// Bind an interface to a concrete singleton type, using the supplied deleter type.
	///
	/// The singleton will be instantiated eagerly.
	///
	public: template <typename DerivedT, typename DeleterT> void toEagerSingleton() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new EagerSingletonBindingSupplier<DerivedT, DeleterT>());
	}

	///
	/// Bind a concrete singleton type.
	///
	/// The singleton will be instantiated eagerly.
	///
	public: void toEagerSingleton() {
		setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, BaseT>>();
		supplier = std::unique_ptr<BindingSupplier>(new EagerSingletonBindingSupplier<BaseT, std::default_delete<BaseT>>());
	}

	////////////////////////// Private implementation /////////////////////////

	public: BindingBuilder(const BindingBuilder & rhs) = delete;
	public: BindingBuilder & operator = (const BindingBuilder & rhs) = delete;

	// Binding suppliers use candidate information to create bindings.
	private: class BindingSupplier {
		public: virtual ~BindingSupplier() = default;

		public: virtual std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const = 0;
	};

	private: class InstantiatingValueBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(new Impl::InstantiatingValueBinding<BaseT>(std::move(key)));
		}
	};

	private: class PrototypeBindingSupplier : public BindingSupplier {
		private: const BaseT prototype;

		public: explicit PrototypeBindingSupplier(const BaseT & prototype_)
			: prototype(prototype_) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(new Impl::PrototypeBinding<BaseT>(std::move(key), prototype));
		}
	};

	private: template <typename ProviderFunctionT>
	class ProvidingFunctionValueBindingSupplier : public BindingSupplier {
		private: const ProviderFunctionT provide;

		public: explicit ProvidingFunctionValueBindingSupplier(ProviderFunctionT provide_)
			: provide(provide_) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingFunctionValueBinding<BaseT, ProviderFunctionT>(std::move(key), provide)
			);
		}
	};

	private: template <typename ProviderT> class ProvidingClassValueBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingClassValueBinding<BaseT, ProviderT>(std::move(key))
			);
		}
	};

	private: template <typename ProviderT> class ProvidingClassInstanceValueBindingSupplier : public BindingSupplier {
		public: explicit ProvidingClassInstanceValueBindingSupplier(std::shared_ptr<ProviderT> && provider_) noexcept
			: provider(std::move(provider_)) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingClassInstanceValueBinding<BaseT, ProviderT>(std::move(key), provider)
			);
		}

		std::shared_ptr<ProviderT> provider;
	};

	private: class ReferenceBindingSupplier : public BindingSupplier {
		private: BaseT & reference;

		public: explicit ReferenceBindingSupplier(BaseT & reference_)
			: reference(reference_) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::StandardReferenceBinding<BaseT>(std::move(key), reference)
			);
		}
	};

	private: template <typename DerivedT> class InstantiatingUniquePtrBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::InstantiatingUniquePtrBinding<BaseT, DerivedT>(std::move(key))
			);
		}
	};

	private: class ProvidingFunctionUniquePtrBindingSupplier : public BindingSupplier {
		private: const UniquePtrProviderFunction provide;

		public: explicit ProvidingFunctionUniquePtrBindingSupplier(UniquePtrProviderFunction provide_)
			: provide(provide_) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingFunctionUniquePtrBinding<BaseT>(std::move(key), provide)
			);
		}
	};

	private: template <typename ProviderT> class ProvidingClassUniquePtrBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingClassUniquePtrBinding<BaseT, ProviderT>(std::move(key))
			);
		}
	};

	private: template <typename ProviderT> class ProvidingClassInstanceUniquePtrBindingSupplier : public BindingSupplier {
		public: explicit ProvidingClassInstanceUniquePtrBindingSupplier(std::shared_ptr<ProviderT> && provider_) noexcept
			: provider(std::move(provider_)) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingClassInstanceUniquePtrBinding<BaseT, ProviderT>(std::move(key), provider)
			);
		}

		private: std::shared_ptr<ProviderT> provider;
	};

	private: template <typename DerivedT> class ThreadLocalSingletonBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ThreadLocalSingletonBinding<BaseT, DerivedT>(std::move(key))
			);
		}
	};

	private: class ProvidedSingletonBindingSupplier : public BindingSupplier {
		private: const std::shared_ptr<BaseT> instance;

		public: explicit ProvidedSingletonBindingSupplier(std::shared_ptr<BaseT> instance_)
			: instance(std::move(instance_)) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidedSingletonBinding<BaseT>(std::move(key), instance)
			);
		}
	};

	private: template <typename ProviderT> class ProvidingClassSingletonBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingClassSingletonBinding<BaseT, ProviderT>(std::move(key))
			);
		}
	};

	private: template <typename ProviderT> class ProvidingClassInstanceSingletonBindingSupplier : public BindingSupplier {
		public: explicit ProvidingClassInstanceSingletonBindingSupplier(std::shared_ptr<ProviderT> && provider_) noexcept
			: provider(std::move(provider_)) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingClassInstanceSingletonBinding<BaseT, ProviderT>(std::move(key), provider)
			);
		}

		private: std::shared_ptr<ProviderT> provider;
	};

	private: class ProvidingFunctionSingletonBindingSupplier : public BindingSupplier {
		public: explicit ProvidingFunctionSingletonBindingSupplier(const std::function<std::shared_ptr<BaseT> ()> & provider_) noexcept
			: provider(provider_) {}

		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidingFunctionSingletonBinding<BaseT>(std::move(key), provider)
			);
		}

		private: const std::function<std::shared_ptr<BaseT> ()> provider;
	};

	private: template <typename DerivedT, typename Deleter> class LazySingletonBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::LazySingletonBinding<BaseT, DerivedT, Deleter>(std::move(key))
			);
		}
	};

	private: template <typename DerivedT, typename Deleter> class EagerSingletonBindingSupplier : public BindingSupplier {
		public: std::unique_ptr<Impl::AbstractBinding> build(Impl::BindingKey && key) const override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::EagerSingletonBinding<BaseT, DerivedT, Deleter>(std::move(key))
			);
		}
	};

	private: void warnInvalidConstQualifier(Impl::BindingMetaType metaType) {
		if (!std::is_same<BaseT, typename std::remove_const<BaseT>::type>::value) {
			Impl::InjectorLogger::log().warn(
				  "{} binding key {} has been defined with a const type. This "
				  "qualifier will be removed from the key because {} types cannot "
				  "be const qualified. This will not affect binding semantics."
				, metaType
				, key
				, metaType
			);
		}
	}

	friend class ApplicationConfiguration;
	friend class EnvironmentConfiguration;
	template <typename ValueT> friend class Impl::ValuePropertyBindingBuilderFactory;
	template <typename ValueT> friend class Impl::UniquePropertyBindingBuilderFactory;

	private: explicit BindingBuilder(std::string && name)
		: Impl::BindingBuilderBase(std::move(name)) {}

	private: std::unique_ptr<Impl::AbstractBinding> build() override {
		return supplier->build(std::move(key));
	}

	private: std::unique_ptr<BindingSupplier> supplier;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__BINDING_BUILDER
