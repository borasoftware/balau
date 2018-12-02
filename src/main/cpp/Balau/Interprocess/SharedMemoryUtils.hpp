// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file SharedMemoryUtils.hpp
///
/// Interprocess shared memory utilities.
///

#ifndef COM_BORA_SOFTWARE__BALAU_INTERPROCESS__SHARED_MEMORY_UTILS
#define COM_BORA_SOFTWARE__BALAU_INTERPROCESS__SHARED_MEMORY_UTILS

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
/// Used to select an interprocess queue/object constructor that creates only.
///
extern const CreateOnlySelector CreateOnly;

///
/// Used to select an interprocess queue/object constructor that opens or creates.
///
extern const OpenOrCreateSelector OpenOrCreate;

///
/// Used to select an interprocess queue/object constructor that opens only.
///
extern const OpenOnlySelector OpenOnly;

///
/// Used to select an interprocess queue/object constructor that opens ready only.
///
extern const OpenReadOnlySelector OpenReadOnly;

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

#endif // COM_BORA_SOFTWARE__BALAU_INTERPROCESS__SHARED_MEMORY_UTILS
