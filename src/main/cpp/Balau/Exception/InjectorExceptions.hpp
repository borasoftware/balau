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
	public: CyclicDependencyException(const char * file, int line, const std::string & st, const std::string & text)
		: BalauException(file, line, st, "CyclicDependency", text) {}
};

///
/// Base class of binding exceptions.
///
class BindingException : public BalauException {
	public: const Impl::BindingKey bindingKey;

	protected: BindingException(const char * file,
	                            int line,
	                            const std::string & st,
	                            const std::string & name,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(file, line, st, name, toString(bindingKey_))
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}

	protected: BindingException(const char * file,
	                            int line,
	                            const std::string & st,
	                            const std::string & name,
	                            const std::string & str,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(file, line, st, name, ::toString(str, bindingKey_))
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}
};

inline bool operator == (const BindingException & lhs, const BindingException & rhs) {
	return lhs.message == rhs.message && lhs.bindingKey == rhs.bindingKey;
}

///
/// Thrown when an illegal injector binding configuration is attempted.
///
class DuplicateBindingException : public BindingException {
	public: DuplicateBindingException(const char * file,
	                                  int line,
	                                  const std::string & st,
	                                  const Impl::BindingKey & bindingKey_)
		: BindingException(file, line, st, "DuplicateBinding", bindingKey_) {}
};

///
/// Thrown when no binding is found in the injector.
///
class NoBindingException : public BindingException {
	public: NoBindingException(const char * file, int line, const std::string & st, const Impl::BindingKey & key_)
		: BindingException(file, line, st, "NoBinding", key_) {}

	public: explicit NoBindingException(const Impl::BindingKey & bindingKey_)
		: BindingException("", 0, "", "NoBinding", bindingKey_) {}
};

///
/// Thrown when no binding is found in the injector.
///
class MissingDependencyException : public BindingException {
	public: MissingDependencyException(const char * file,
	                                   int line,
	                                   const std::string & st,
	                                   const Impl::BindingKey & dependentKey_,
	                                   const Impl::BindingKey & independentKey_)
		: BindingException(file, line, st, "MissingDependency", ::toString(independentKey_, " is required by "), dependentKey_) {}
};

///
/// Thrown when an attempt is made to create a shared pointer binding to the injector.
///
/// Only weak pointers are supported in singleton bindings.
///
class SharedInjectorException : public BindingException {
	public: SharedInjectorException(const char * file,
	                                int line,
	                                const std::string & st,
	                                const Impl::BindingKey & bindingKey_)
		: BindingException(file, line, st, "SharedInjector", bindingKey_) {}
};

///
/// Thrown when there is an issue loading an environment property file.
///
class EnvironmentConfigurationException : public BalauException {
	public: EnvironmentConfigurationException(const char * file,
	                                          int line,
	                                          const std::string & st,
	                                          const std::string & text)
		: BalauException(file, line, st, "EnvironmentConfiguration", text) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__INJECTOR_EXCEPTIONS
