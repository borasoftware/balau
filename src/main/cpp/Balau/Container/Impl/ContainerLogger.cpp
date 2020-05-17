// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ContainerLogger.hpp"

namespace Balau::Impl {

BalauLogger & ContainerLogger::log() {
	static BalauLogger instance("balau.container");
	return instance;
}

} // namespace Balau::Impl
