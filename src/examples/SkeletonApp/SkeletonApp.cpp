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
