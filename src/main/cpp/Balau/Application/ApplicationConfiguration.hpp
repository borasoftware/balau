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

	////////////////////////// Private implementation /////////////////////////

	private: std::vector<std::shared_ptr<Impl::BindingBuilderBase>> build() const override {
		// Move the vector because it is not used anymore after this call from the injector.
		return std::move(builders);
	}

	public: std::vector<const InjectorConfiguration *> getExtraConfiguration() const override {
		return extraConfiguration;
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
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__APPLICATION_CONFIGURATION
