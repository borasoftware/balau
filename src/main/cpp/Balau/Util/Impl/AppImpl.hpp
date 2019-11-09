// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__APP_IMPL
#define COM_BORA_SOFTWARE__BALAU_UTIL__APP_IMPL

#include <Balau/Util/User.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

namespace Balau::Util::Impl {

struct AppImpl final {
	static Resource::File getUserApplicationDataDirectory(const std::string & appGroup, const std::string & appName) {
		// Best effort approach.
		#if BOOST_OS_UNIX
			const char * xdg = std::getenv("XDG_DATA_HOME");

			if (xdg != nullptr && !Util::Strings::trim(xdg).empty()) {
				return Resource::File(xdg) / appGroup / appName;
			}

			auto home = User::getHomeDirectory();
			return home / ".local" / "share" / appGroup / appName;
		#elif BOOST_OS_WINDOWS
			#error "The Windows platform is not yet implemented."

			// TODO implement
			//CHAR szPath[MAX_PATH];
			//SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)));
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getGlobalApplicationDataDirectory(const std::string & appGroup, const std::string & appName) {
		const boost::filesystem::path programLocation = boost::dll::program_location();

		if (programLocation.empty()) {
			ThrowBalauException(
				  Exception::NotFoundException
				, "Failed to locate the program location whilst determining the application's global data directory."
			);
		}

		const Resource::File programLocationDirectory(programLocation.parent_path());

		#if BOOST_OS_UNIX
			const Resource::File parentDirectory = programLocationDirectory.getParentDirectory();
			return parentDirectory / "share" / appGroup / appName / "data";
		#elif BOOST_OS_WINDOWS
			return programLocationDirectory / "data";
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getUserApplicationConfigDirectory(const std::string & appGroup, const std::string & appName) {
		// Best effort approach.
		#if BOOST_OS_UNIX
			const char * xdg = std::getenv("XDG_CONFIG_HOME");

			if (xdg != nullptr && !Util::Strings::trim(xdg).empty()) {
				return Resource::File(Util::Strings::trim(xdg)) / appGroup / appName;
			}

			auto home = User::getHomeDirectory();
			return home / ".config" / appGroup / appName;
		#elif BOOST_OS_WINDOWS
			#error "The Windows platform is not yet implemented."

			// TODO implement
			//CHAR szPath[MAX_PATH];
			//SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)));
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getGlobalApplicationConfigDirectory(const std::string & appGroup, const std::string & appName) {
		const boost::filesystem::path programLocation = boost::dll::program_location();

		if (programLocation.empty()) {
			ThrowBalauException(
				  Exception::NotFoundException
				, "Failed to locate the program location whilst determining the application's global config directory."
			);
		}

		const Resource::File programLocationDirectory(programLocation.parent_path());

		#if BOOST_OS_UNIX
			const Resource::File parentDirectory = programLocationDirectory.getParentDirectory();
			return parentDirectory / "share" / appGroup / appName / "config";
		#elif BOOST_OS_WINDOWS
			return programLocationDirectory / "config";
		#else
			#error "Platform not implemented."
		#endif
	}

	static Resource::File getApplicationRuntimeDataDirectory(const std::string & appGroup, const std::string & appName) {
		#if BOOST_OS_UNIX
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
		#elif BOOST_OS_WINDOWS
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