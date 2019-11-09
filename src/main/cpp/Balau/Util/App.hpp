// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file App.hpp
///
/// Utilities for accessing application information.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__APP
#define COM_BORA_SOFTWARE__BALAU_UTIL__APP

#include <Balau/Util/User.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

namespace Balau::Util {

///
/// Utilities for accessing application information.
///
struct App final {
	///
	/// Get the global data directory for the specified application group/name.
	///
	/// This directory is normally read-only.
	///
	/// For Unix type OSes, the directory is "${PROGRAM_LOCATION}/../share/appGroup/appName".
	/// For Windows type OSes, the directory is "${PROGRAM_LOCATION}/share".
	///
	/// @param appGroup the application group name (normally the company name without spaces)
	/// @param appName the application group name (without spaces)
	/// @throw NotFoundException if the global data directory could not be located
	///
	static Resource::File getGlobalDataResourcesDirectory(const std::string & appGroup, const std::string & appName) {
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
			return parentDirectory / "share" / appGroup / appName;
		#elif BOOST_OS_WINDOWS
			return programLocationDirectory / "share";
		#else
			#error "Platform not implemented."
		#endif
	}

	///
	/// Get the user data directory for the specified application group/name.
	///
	/// This directory is read-write and specific to the user running the process.
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
		// Best effort approach.
		#if BOOST_OS_UNIX
			const char * xdg = std::getenv("XDG_DATA_HOME");

			if (xdg != nullptr && !std::string(xdg).empty()) {
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

	///
	/// Get the user config directory for the specified application group/name.
	///
	/// This directory is read-write and specific to the user running the process.
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
		// Best effort approach.
		#if BOOST_OS_UNIX
			const char * xdg = std::getenv("XDG_CONFIG_HOME");

			if (xdg != nullptr && !std::string(xdg).empty()) {
				return Resource::File(xdg) / appGroup / appName;
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

	///////////////////////////////////////////////////////////////////////////

	App() = delete;
	App(const User &) = delete;
	App & operator = (const User &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__APP
