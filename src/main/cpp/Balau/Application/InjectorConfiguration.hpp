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
/// @file InjectorConfiguration.hpp
///
/// The abstract base class for all Injector configuration classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_CONFIGURATION
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_CONFIGURATION

#include <Balau/Application/Impl/Binding.hpp>

namespace Balau {

class Injector;
class ApplicationConfiguration;
class EnvironmentConfiguration;

///
/// Injector configurations specify injector bindings.
///
class InjectorConfiguration {
	///
	/// Configure the injector configuration.
	///
	public: virtual void configure() const = 0;

	public: virtual ~InjectorConfiguration() = default;

	////////////////////////// Private implementation /////////////////////////

	// Builds the binding builders from the configuration.
	private: virtual std::vector<std::shared_ptr<Impl::BindingBuilderBase>> build() const = 0;

	private: virtual std::vector<const InjectorConfiguration*> getExtraConfiguration() const = 0;

	friend class ::Balau::Injector;

	// Run configure and build stages.
	private: std::vector<std::shared_ptr<Impl::BindingBuilderBase>> execute() const {
		configure();
		return build();
	}

};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_CONFIGURATION
