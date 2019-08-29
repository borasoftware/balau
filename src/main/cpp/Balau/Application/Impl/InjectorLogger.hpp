// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_LOGGER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_LOGGER

#include <Balau/Logging/Impl/BalauLogger.hpp>

namespace Balau::Impl {

class InjectorLogger {
	public: static BalauLogger & log() {
		static BalauLogger instance("balau.injector");
		return instance;
	}
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_LOGGER
