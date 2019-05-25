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
/// @file User.hpp
///
/// Utilities for accessing user information.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__USER
#define COM_BORA_SOFTWARE__BALAU_UTIL__USER

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>

#include <pwd.h>

namespace Balau::Util {

///
/// Utilities for accessing user information.
///
struct User final {
	///
	/// Get the home directory of the user running the process.
	///
	/// @throw NotFoundException if the home directory could not be located
	///
	static Resource::File getHomeDirectory() {
		// Best effort approach.
		#if BOOST_OS_UNIX
			std::string h = std::getenv("HOME");

			if (!h.empty()) {
				return Resource::File(h);
			}

			passwd pwd {};
			passwd * result;
			int s;
			size_t bufferSize = 512;
			std::vector<char> buffer(bufferSize);
			bool repeat = false;

			do {
				s = getpwuid_r(getuid(), &pwd, buffer.data(), bufferSize, &result);

				if (result == nullptr) {
					if (s == 0) {
						ThrowBalauException(
							  Exception::NotFoundException
							, "Failed to locate the user home directory: user entry not found in passwd."
						);
					} else if (errno == ERANGE) {
						// Give up at 1MB buffer size.
						if (bufferSize >= 1048576) {
							ThrowBalauException(
								  Exception::NotFoundException
								, "Failed to locate the user home directory with buffer size of 1MB."
							);
						}

						bufferSize *= 2;
						buffer.resize(bufferSize, 0);
						repeat = true;
					} else {
						ThrowBalauException(
							  Exception::NotFoundException
							, "Failed to locate the user home directory: getpwnam_r raised an error, errno = " +
							  ::toString(errno)
						);
					}
				}
			} while (!repeat);

			if (result == nullptr || result->pw_dir == nullptr) {
				ThrowBalauException(
					  Exception::NotFoundException
					, ::toString("Failed to locate the user home directory: getpwnam_r raised an error, errno = ", errno)
				);
			}

			return Resource::File(result->pw_dir);
		#elif BOOST_OS_WINDOWS
			std::string h = std::getenv("USERPROFILE");

			if (!h.empty()) {
				return Resource::File(h);
			}

			h = std::getenv("HOMEDRIVE") + std::getenv("HOMEPATH");

			if (!h.empty()) {
				return Resource::File(h);
			}
		#else
			#error "Platform not implemented."
		#endif
	}

	///////////////////////////////////////////////////////////////////////////

	User() = delete;
	User(const User &) = delete;
	User & operator = (const User &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__USER
