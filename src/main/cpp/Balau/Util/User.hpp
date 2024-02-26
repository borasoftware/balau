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
/// @file User.hpp
///
/// Utilities for accessing user information.
///

#ifndef COM_BORA_SOFTWARE_BALAU_UTIL_USER
#define COM_BORA_SOFTWARE_BALAU_UTIL_USER

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>

#include <pwd.h>

namespace Balau::Exception {

///
/// Thrown when there is an issue locating the user in the system.
///
class UserNotFoundException : public BalauException {
	public: UserNotFoundException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "UserNotFound", text) {}

	public: UserNotFoundException(const std::string & st, const std::string & text)
		: BalauException(st, "UserNotFound", text) {}
};

} // namespace Balau::Exception

namespace Balau::Util {

///
/// Utilities for accessing user information.
///
struct User final {
	///
	/// Get the name of the user running the process.
	///
	///
	static std::string getUserName() {
		#if BOOST_OS_UNIX
			const uid_t uid = geteuid();
			const passwd * pw = getpwuid(uid);

			if (pw) {
				return std::string(pw->pw_name);
			} else {
				ThrowBalauException(
					  Exception::UserNotFoundException
					, "Failed to obtain the user name: user entry not found in passwd."
				);
			}
		#elif BOOST_OS_WINDOWS
			#error "The Windows platform is not yet implemented."
		#else
			#error "Platform not implemented."
		#endif
	}

	///
	/// Get the home directory of the user running the process.
	///
	/// @throw NotFoundException if the home directory could not be located
	///
	static Resource::File getHomeDirectory() {
		// Best effort approach.
		#if BOOST_OS_UNIX
			const char * home = std::getenv("HOME");

			if (home != nullptr && !Util::Strings::trim(home).empty()) {
				return Resource::File(Util::Strings::trim(home));
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
							  Exception::UserNotFoundException
							, "Failed to locate the user home directory: user entry not found in passwd."
						);
					} else if (errno == ERANGE) {
						// Give up at 1MB buffer size.
						if (bufferSize >= 1048576) {
							ThrowBalauException(
								  Exception::UserNotFoundException
								, "Failed to locate the user home directory with buffer size of 1MB."
							);
						}

						bufferSize *= 2;
						buffer.resize(bufferSize, 0);
						repeat = true;
					} else {
						ThrowBalauException(
							  Exception::UserNotFoundException
							, "Failed to locate the user home directory: getpwnam_r raised an error, errno = " +
							  ::toString(errno)
						);
					}
				}
			} while (!repeat);

			if (result == nullptr || result->pw_dir == nullptr) {
				ThrowBalauException(
					  Exception::UserNotFoundException
					, ::toString("Failed to locate the user home directory: getpwnam_r raised an error, errno = ", errno)
				);
			}

			return Resource::File(result->pw_dir);
		#elif BOOST_OS_WINDOWS
			#error "The Windows platform is not yet implemented."

			// TODO verify
			const char * up = std::getenv("USERPROFILE");

			if (up != nullptr && !Util::Strings::trim(up).empty()) {
				return Resource::File(up);
			}

			const char * hd = std::getenv("HOMEDRIVE");
			const char * hp = std::getenv("HOMEPATH");

			if (hd != nullptr && hp != nullptr && !Util::Strings::trim(hd).empty() && !std::string(hp).empty()) {
				// TODO
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

#endif // COM_BORA_SOFTWARE_BALAU_UTIL_USER
