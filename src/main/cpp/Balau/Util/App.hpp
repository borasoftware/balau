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

///
/// @file App.hpp
///
/// Utilities for accessing application information.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__APP
#define COM_BORA_SOFTWARE__BALAU_UTIL__APP

#include <Balau/Util/Impl/AppImpl.hpp>

namespace Balau::Util {

///
/// Utilities for accessing application information.
///
struct App final {
	///
	/// Get the user data directory for the specified application group/name.
	///
	/// The directory is fixed at the first call this function and will not change
	/// for the life of the process.
	///
	/// The directory is read-write and specific to the user running the process.
	///
	/// For Unix type OSes, the directory is "${XDG_DATA_HOME}/appGroup/appName" or
	/// "${HOME}/.local/share/appGroup/appName" if ${XDG_DATA_HOME} is not defined.
	///
	/// For Windows type OSes, the directory is "${HOME}/AppData/Roaming/appGroup/appName/share".
	///
	/// @param appGroup the application group name (normally the company name without spaces)
	/// @param appName the application group name (without spaces)
	/// @throw NotFoundException if the user data directory could not be located
	///
	static Resource::File getUserApplicationDataDirectory(const std::string & appGroup, const std::string & appName) {
		static Resource::File instance = Impl::AppImpl::getUserApplicationDataDirectory(appGroup, appName);
		return instance;
	}

	///
	/// Get the global data directory for the specified application group/name.
	///
	/// The directory is fixed at the first call this function and will not change
	/// for the life of the process.
	///
	/// The directory is normally read-only.
	///
	/// For Unix type OSes, the directory is "${PROGRAM_LOCATION}/../share/appGroup/appName/data".
	/// For Windows type OSes, the directory is "${PROGRAM_LOCATION}/data".
	///
	/// @param appGroup the application group name (normally the company name without spaces)
	/// @param appName the application group name (without spaces)
	/// @throw NotFoundException if the global data directory could not be located
	///
	static Resource::File getGlobalApplicationDataDirectory(const std::string & appGroup, const std::string & appName) {
		static Resource::File instance = Impl::AppImpl::getGlobalApplicationDataDirectory(appGroup, appName);
		return instance;
	}

	///
	/// Get the user config directory for the specified application group/name.
	///
	/// The directory is fixed at the first call this function and will not change
	/// for the life of the process.
	///
	/// The directory is read-write and specific to the user running the process.
	///
	/// For Unix type OSes, the directory is "${XDG_CONFIG_HOME}/appGroup/appName" or
	/// "${HOME}/.config/appGroup/appName" if ${XDG_CONFIG_HOME} is not defined.
	///
	/// For Windows type OSes, the directory is "${HOME}/AppData/Roaming/appGroup/appName/config".
	///
	/// @param appGroup the application group name (normally the company name without spaces)
	/// @param appName the application group name (without spaces)
	/// @throw NotFoundException if the user config directory could not be located
	///
	static Resource::File getUserApplicationConfigDirectory(const std::string & appGroup, const std::string & appName) {
		static Resource::File instance = Impl::AppImpl::getUserApplicationConfigDirectory(appGroup, appName);
		return instance;
	}

	///
	/// Get the global config directory for the specified application group/name.
	///
	/// The directory is fixed at the first call this function and will not change
	/// for the life of the process.
	///
	/// The directory is normally read-only.
	///
	/// For Unix type OSes, the directory is "${PROGRAM_LOCATION}/../share/appGroup/appName/config".
	/// For Windows type OSes, the directory is "${PROGRAM_LOCATION}/config".
	///
	/// @param appGroup the application group name (normally the company name without spaces)
	/// @param appName the application group name (without spaces)
	/// @throw NotFoundException if the user config directory could not be located
	///
	static Resource::File getGlobalApplicationConfigDirectory(const std::string & appGroup, const std::string & appName) {
		static Resource::File instance = Impl::AppImpl::getGlobalApplicationConfigDirectory(appGroup, appName);
		return instance;
	}

	///
	/// Get the runtime data directory for the current application.
	///
	/// The directory is fixed at the first call this function and will not change
	/// for the life of the process.
	///
	/// The directory is read-write for the user that owns the process.
	///
	/// The directory returned depends on file system availability. Specifically, the directory:
	///  - must be owned by the user;
	///  - must have access rights restricted to the user only (for Unix OSes, the access mode must be 0700);
	///  - must be on a local file system and not shared with any other system;
	///  - must be fully-featured by the standards of the operating system;
	///
	/// This function will create the directory on the first call, if the directory does not already exist.
	///
	/// For Unix type OSes, the directory is "${XDG_RUNTIME_DIR}/appGroup/appName" or
	/// "${TMPDIR}/appGroup/appName" or "/tmp/${USER}/appGroup/appName" depending on
	/// the existence of the XDG_RUNTIME_DIR and TMPDIR environment variables.
	///
	/// For Windows type OSes, the directory is "GetTempPath() / appGroup / appName".
	///
	/// This function does not implement any logic to handle:
	/// - the lifetime of the directory being bound to the logged in user;
	/// - the removal of the directory when the user fully logs out;
	/// - possible loss of data due to periodic clean-ups of the temporary directory by other processes.
	///
	/// Applications should provide suitable logic to deal with these requirements, including updating file
	/// access timestamps every 6 hours and/or setting the sticky bit of each file.
	///
	/// @param appGroup the application group name (normally the company name without spaces)
	/// @param appName the application group name (without spaces)
	/// @throw NotFoundException if the user config directory could not be located
	///
	static Resource::File getApplicationRuntimeDataDirectory(const std::string & appGroup, const std::string & appName) {
		static Resource::File instance = Impl::AppImpl::getApplicationRuntimeDataDirectory(appGroup, appName);
		return instance;
	}

	///////////////////////////////////////////////////////////////////////////

	App() = delete;
	App(const App &) = delete;
	App & operator = (const App &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__APP
