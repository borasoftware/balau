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

#ifndef COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_LOGGER_ITEM_PARAMETERS
#define COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_LOGGER_ITEM_PARAMETERS

#include <Balau/Logging/Logger.hpp>
#include <Balau/Logging/Impl/LoggerAllocator.hpp>

#include <chrono>

namespace Balau::LoggingSystem {

// Parameters passed as single reference to log items during logging.
struct LoggerItemParameters {
	LoggerOStringStream & builder;
	const std::string & nameSpace; // NOLINT
	const std::string & ns; // NOLINT
	const LoggingLevel level;
	const char * location;
	const std::string_view message;
	const std::chrono::system_clock::time_point & timePoint;

	LoggerItemParameters(LoggerOStringStream & builder_,
	                     const std::string & nameSpace_,
	                     const std::string & ns_,
	                     const LoggingLevel level_,
	                     const char * location_,
	                     std::string_view message_,
	                     const std::chrono::system_clock::time_point & timePoint_)
		: builder(builder_)
		, nameSpace(nameSpace_)
		, ns(ns_)
		, level(level_)
		, location(location_)
		, message(message_)
		, timePoint(timePoint_) {}
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_LOGGER_ITEM_PARAMETERS
