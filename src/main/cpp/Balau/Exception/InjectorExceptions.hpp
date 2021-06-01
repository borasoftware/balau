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
/// @file InjectorExceptions.hpp
///
/// %Balau exceptions for the injector framework.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__INJECTOR_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__INJECTOR_EXCEPTIONS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// Thrown when the supplied binding configuration has a cyclic dependency.
///
class CyclicDependencyException : public BalauException {
	public: CyclicDependencyException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "CyclicDependency", text) {}

	public: CyclicDependencyException(const std::string & st, const std::string & text)
		: BalauException(st, "CyclicDependency", text) {}
};

///
/// Base class of binding exceptions.
///
class BindingException : public BalauException {
	public: const Impl::BindingKey bindingKey;

	protected: BindingException(SourceCodeLocation location,
	                            const std::string & st,
	                            const std::string & name,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(location, st, name, toString(bindingKey_))
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}

	protected: BindingException(const std::string & st,
	                            const std::string & name,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(st, name, toString(bindingKey_))
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}

	protected: BindingException(SourceCodeLocation location,
	                            const std::string & st,
	                            const std::string & name,
	                            const std::string & str,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(location, st, name, ::toString(str, bindingKey_))
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}

	protected: BindingException(const std::string & st,
	                            const std::string & name,
	                            const std::string & str,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(st, name, ::toString(str, bindingKey_))
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}
};

inline bool operator == (const BindingException & lhs, const BindingException & rhs) {
	return lhs.message == rhs.message && lhs.bindingKey == rhs.bindingKey;
}

///
/// Thrown when an illegal injector binding configuration is attempted.
///
class DuplicateBindingException : public BindingException {
	public: DuplicateBindingException(SourceCodeLocation location,
	                                  const std::string & st,
	                                  const Impl::BindingKey & bindingKey_)
		: BindingException(location, st, "DuplicateBinding", bindingKey_) {}

	public: DuplicateBindingException(const std::string & st,
	                                  const Impl::BindingKey & bindingKey_)
		: BindingException(st, "DuplicateBinding", bindingKey_) {}
};

///
/// Thrown when no binding is found in the injector.
///
class NoBindingException : public BindingException {
	public: NoBindingException(SourceCodeLocation location, const std::string & st, const Impl::BindingKey & key_)
		: BindingException(location, st, "NoBinding", key_) {}

	public: explicit NoBindingException(const Impl::BindingKey & bindingKey_)
		: BindingException("", "NoBinding", bindingKey_) {}
};

///
/// Thrown when no binding is found in the injector.
///
class MissingDependencyException : public BindingException {
	public: MissingDependencyException(SourceCodeLocation location,
	                                   const std::string & st,
	                                   const Impl::BindingKey & dependentKey_,
	                                   const Impl::BindingKey & independentKey_)
		: BindingException(location, st, "MissingDependency", ::toString(independentKey_, " is required by "), dependentKey_) {}

	public: MissingDependencyException(const std::string & st,
	                                   const Impl::BindingKey & dependentKey_,
	                                   const Impl::BindingKey & independentKey_)
		: BindingException(st, "MissingDependency", ::toString(independentKey_, " is required by "), dependentKey_) {}
};

///
/// Thrown when an attempt is made to create a shared pointer binding to the injector.
///
/// Only weak pointers are supported in singleton bindings.
///
class SharedInjectorException : public BindingException {
	public: SharedInjectorException(SourceCodeLocation location,
	                                const std::string & st,
	                                const Impl::BindingKey & bindingKey_)
		: BindingException(location, st, "SharedInjector", bindingKey_) {}

	public: SharedInjectorException(const std::string & st,
	                                const Impl::BindingKey & bindingKey_)
		: BindingException(st, "SharedInjector", bindingKey_) {}
};

///
/// Thrown when there is an issue loading an environment property file.
///
class EnvironmentConfigurationException : public BalauException {
	public: EnvironmentConfigurationException(SourceCodeLocation location,
	                                          const std::string & st,
	                                          const std::string & text)
		: BalauException(location, st, "EnvironmentConfiguration", text) {}

	public: EnvironmentConfigurationException(const std::string & st,
	                                          const std::string & text)
		: BalauException(st, "EnvironmentConfiguration", text) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__INJECTOR_EXCEPTIONS
