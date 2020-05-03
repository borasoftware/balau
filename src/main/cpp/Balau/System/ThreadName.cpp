// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ThreadName.hpp"

#include <Balau/Util/Strings.hpp>

namespace Balau::System {

thread_local std::string ThreadName::threadName;

const std::string & ThreadName::getName() {
	return threadName;
}

void ThreadName::setName(const std::string & name) {
	threadName = toString(Util::Strings::trim(name));
}

} // namespace Balau::System
