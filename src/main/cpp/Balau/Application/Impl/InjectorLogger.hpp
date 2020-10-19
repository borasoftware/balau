// @formatter:off
//
// Balau core C++ library
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
