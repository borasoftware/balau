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

///
/// @file InjectorConfiguration.hpp
///
/// The abstract base class for all Injector configuration classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_CONFIGURATION
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_CONFIGURATION

#include <Balau/Application/Impl/Binding.hpp>
#include <Balau/Application/Impl/StaticSingletonBuilder.hpp>

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

	public: virtual std::vector<const InjectorConfiguration*> getExtraConfiguration() const = 0;
	public: virtual std::list<std::function<void (const Injector& )>> getPostConstructionCalls() const = 0;
	public: virtual std::list<std::function<void ()>> getPreDestructionCalls() const = 0;
	public: virtual std::list<std::unique_ptr<StaticSingletonRegistrationBase>> getStaticSingletonPostConstructionCalls() const = 0;

	// Run configure and build stages.
	public: std::vector<std::shared_ptr<Impl::BindingBuilderBase>> execute() const {
		configure();
		return build();
	}

};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_CONFIGURATION
