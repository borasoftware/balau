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
	public: CyclicDependencyException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "CyclicDependency", text) {}
};

///
/// Base class of binding exceptions.
///
class BindingException : public BalauException {
	public: const Impl::BindingKey bindingKey;

	protected: BindingException(const char * file,
	                            int line,
	                            const std::string & name,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(
			  file
			, line
			, name
			, ::toString(bindingKey_)
		)
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}

	protected: BindingException(const char * file,
	                            int line,
	                            const std::string & name,
	                            const std::string & str,
	                            const Impl::BindingKey & bindingKey_)
		: BalauException(
			file
			, line
			, name
			, ::toString(str, bindingKey_)
		)
		, bindingKey(bindingKey_.getType(), std::string(bindingKey_.getName())) {}
};

inline bool operator == (const BindingException & lhs, const BindingException & rhs) {
	return lhs.message == rhs.message && lhs.bindingKey == rhs.bindingKey;
}

///
/// Thrown when an illegal injector binding configuration is attempted.
///
class DuplicateBindingException : public BindingException {
	public: DuplicateBindingException(const char * file, int line, const Impl::BindingKey & bindingKey_)
		: BindingException(file, line, "DuplicateBinding", bindingKey_) {}
};

///
/// Thrown when no binding is found in the injector.
///
class NoBindingException : public BindingException {
	public: NoBindingException(const char * file, int line, const Impl::BindingKey & key_)
		: BindingException(file, line, "NoBinding", key_) {}

	public: explicit NoBindingException(const Impl::BindingKey & bindingKey_)
		: BindingException("", 0, "NoBinding", bindingKey_) {}
};

inline std::string toString(const NoBindingException & e) {
	return e.what();
}

///
/// Thrown when no binding is found in the injector.
///
class MissingDependencyException : public BindingException {
	public: MissingDependencyException(const char * file,
	                                   int line,
	                                   const Impl::BindingKey & dependentKey_,
	                                   const Impl::BindingKey & independentKey_)
		: BindingException(file, line, "MissingDependency", ::toString(independentKey_, " is required by "), dependentKey_) {}
};

inline std::string toString(const MissingDependencyException & e) {
	return e.what();
}

///
/// Thrown when an attempt is made to create a shared pointer binding to the injector.
///
/// Only weak pointers are supported in singleton bindings.
///
class SharedInjectorException : public BindingException {
	public: SharedInjectorException(const char * file, int line, const Impl::BindingKey & bindingKey_)
		: BindingException(file, line, "SharedInjector", bindingKey_) {}
};

///
/// Thrown when there is an issue loading an environment property file.
///
class EnvironmentConfigurationException : public BalauException {
	public: EnvironmentConfigurationException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "EnvironmentConfiguration", text) {}
};

inline std::string toString(const SharedInjectorException & e) {
	return e.what();
}

} // namespace Balau::Exception

inline std::string toString(const Balau::Exception::CyclicDependencyException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::DuplicateBindingException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::NoBindingException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::MissingDependencyException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::SharedInjectorException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::EnvironmentConfigurationException & e) {
	return e.what();
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__INJECTOR_EXCEPTIONS
