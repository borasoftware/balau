// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Service1.hpp"
#include "Service2.hpp"

#include <Balau/Application/Injector.hpp>
#include <Balau/Logging/Logger.hpp>
#include <Balau/System/ThreadName.hpp>

using namespace Balau;

Logger & logger = Logger::getLogger("main");

int main() {
	System::ThreadName::setName("Main");

	BalauLogInfo(logger, "Skeleton application starting up.");

	// TODO work out what this skeleton app should do, create services, create injector configuration

	return 0;
}
