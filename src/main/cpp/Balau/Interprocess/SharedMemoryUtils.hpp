// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

///
/// @file SharedMemoryUtils.hpp
///
/// Interprocess shared memory utilities.
///

#ifndef COM_BORA_SOFTWARE_BALAU_INTERPROCESS_SHARED_MEMORY_UTILS
#define COM_BORA_SOFTWARE_BALAU_INTERPROCESS_SHARED_MEMORY_UTILS

#include <Balau/Type/UUID.hpp>
#include <Balau/Util/Strings.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

namespace Balau::Interprocess {

///
/// Type of CreateOnly constructor selector.
///
using CreateOnlySelector = boost::interprocess::create_only_t;

///
/// Type of OpenOrCreate constructor selector.
///
using OpenOrCreateSelector = boost::interprocess::open_or_create_t;

///
/// Type of OpenOrCreate constructor selector.
///
using OpenOnlySelector = boost::interprocess::open_only_t;

///
/// Type of OpenReadOnly constructor selector.
///
using OpenReadOnlySelector = boost::interprocess::open_read_only_t;

///////////////////////////////////////////////////////////////////////////////

///
/// Utilities for shared memory items.
///
struct SharedMemoryUtils {
	///
	/// Create a name prefix by canonicalising the application path.
	///
	static std::string namePrefixFromAppPath() {
		return Util::Strings::replaceAll(boost::dll::program_location().string(), "[:/\\\\]", "_");
	}

	///
	/// Create a name prefix by canonicalising a UUID.
	///
	static std::string namePrefixFromUUID() {
		return Util::Strings::replaceAll(UUID().asString(), "-", "_");
	}

	///////////////////////////////////////////////////////////////////////////

	public: SharedMemoryUtils() = delete;
	public: SharedMemoryUtils(const SharedMemoryUtils &) = delete;
	public: SharedMemoryUtils & operator = (const SharedMemoryUtils &) = delete;
};

} // namespace Balau::Interprocess

#endif // COM_BORA_SOFTWARE_BALAU_INTERPROCESS_SHARED_MEMORY_UTILS
