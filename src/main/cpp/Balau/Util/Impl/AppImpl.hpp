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
#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__APP_IMPL
#define COM_BORA_SOFTWARE__BALAU_UTIL__APP_IMPL

#include <Balau/Util/User.hpp>

namespace Balau::Util::Impl {

struct AppImpl final {
	static Resource::File getProgramLocation() {
		#ifdef BALAU_LINUX_PLATFORM
			std::error_code ec;
			auto location = std::filesystem::read_symlink("/proc/self/exe", ec);

			if (ec) {
				ThrowBalauException(Exception::NotFoundException, "Failed to locate the program location.");
			}

			return Resource::File(location);
		#elif defined BALAU_WINDOWS_PLATFORM
			#error "The Windows platform is not yet implemented."
		#else
			#error "Platform not implemented."
		#endif

	}

	static Resource::File getUserApplicationDataDirectory(const std::string & appGroup, const std::string & appName) {
		// Best effort approach.
		#ifdef BALAU_LINUX_PLATFORM
			const char * xdg = std::getenv("XDG_DATA_HOME");

			if (xdg != nullptr && !Util::Strings::trim(xdg).empty()) {
				return Resource::File(xdg) / appGroup / appName;
			}

			auto home = User::getHomeDirectory();
			return home / ".local" / "share" / appGroup / appName;
		#elif defined BALAU_WINDOWS_PLATFORM
			#error "The Windows platform is not yet implemented."

			// TODO implement
			//CHAR szPath[MAX_PATH];
			//SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)));
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getGlobalApplicationDataDirectory(const std::string & appGroup, const std::string & appName) {
		const auto programLocation = getProgramLocation();
		const Resource::File programLocationDirectory = getProgramLocation().getParentDirectory();

		#ifdef BALAU_LINUX_PLATFORM
			const Resource::File parentDirectory = programLocationDirectory.getParentDirectory();
			return parentDirectory / "share" / appGroup / appName / "data";
		#elif defined BALAU_WINDOWS_PLATFORM
			return programLocationDirectory / "data";
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getUserApplicationConfigDirectory(const std::string & appGroup, const std::string & appName) {
		// Best effort approach.
		#ifdef BALAU_LINUX_PLATFORM
			const char * xdg = std::getenv("XDG_CONFIG_HOME");

			if (xdg != nullptr && !Util::Strings::trim(xdg).empty()) {
				return Resource::File(Util::Strings::trim(xdg)) / appGroup / appName;
			}

			auto home = User::getHomeDirectory();
			return home / ".config" / appGroup / appName;
		#elif defined BALAU_WINDOWS_PLATFORM
			#error "The Windows platform is not yet implemented."

			// TODO implement
			//CHAR szPath[MAX_PATH];
			//SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)));
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getGlobalApplicationConfigDirectory(const std::string & appGroup, const std::string & appName) {
		const auto programLocation = getProgramLocation();

		const Resource::File programLocationDirectory = programLocation.getParentDirectory();

		#ifdef BALAU_LINUX_PLATFORM
			const Resource::File parentDirectory = programLocationDirectory.getParentDirectory();
			return parentDirectory / "share" / appGroup / appName / "config";
		#elif defined BALAU_WINDOWS_PLATFORM
			return programLocationDirectory / "config";
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getApplicationRuntimeDataDirectory(const std::string & appGroup, const std::string & appName) {
		#ifdef BALAU_LINUX_PLATFORM
			Resource::File dir;

			const char * xdg = std::getenv("XDG_RUNTIME_DIR");

			if (xdg != nullptr && !Util::Strings::trim(xdg).empty()) {
				dir = Resource::File(Util::Strings::trim(xdg)) / appGroup / appName;
			} else {
				const char * tmpdir = std::getenv("TMPDIR");

				if (tmpdir != nullptr && !Util::Strings::trim(tmpdir).empty()) {
					dir = Resource::File(Util::Strings::trim(tmpdir)) / appGroup / appName;
				} else {
					auto username = User::getUserName();
					dir = Resource::File("/tmp") / Util::Strings::trim(username) / appGroup / appName;
				}
			}

			dir.createDirectories();
			return dir;
		#elif defined BALAU_WINDOWS_PLATFORM
			#error "The Windows platform is not yet implemented."
		#else
			#error "Platform not implemented."
		#endif
	}

	///////////////////////////////////////////////////////////////////////////

	AppImpl() = delete;
	AppImpl(const AppImpl &) = delete;
	AppImpl & operator = (const AppImpl &) = delete;
};

} // namespace Balau::Util::Impl

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__APP_IMPL
