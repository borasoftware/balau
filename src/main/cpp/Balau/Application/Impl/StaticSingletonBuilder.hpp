// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__STATIC_SINGLETON_BUILDER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__STATIC_SINGLETON_BUILDER

#include <memory>
#include <string>

namespace Balau {

struct StaticSingletonRegistrationBase {
	virtual ~StaticSingletonRegistrationBase() = default;
};

template <typename T> struct StaticSingletonRegistration : public StaticSingletonRegistrationBase {
	StaticSingletonRegistration(std::shared_ptr<T> * ptrPtr_, std::string name_)
		: ptrPtr(ptrPtr_)
		, name(std::move(name_)) {}

	std::shared_ptr<T> * ptrPtr;
	std::string name;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__STATIC_SINGLETON_BUILDER
