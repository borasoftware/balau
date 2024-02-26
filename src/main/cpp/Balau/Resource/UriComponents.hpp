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
/// @file UriComponents.hpp
///
/// A class representing the parsed components of a URI.
///

#ifndef COM_BORA_SOFTWARE_BALAU_RESOURCE_URI_COMPONENTS
#define COM_BORA_SOFTWARE_BALAU_RESOURCE_URI_COMPONENTS

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Type/ToString.hpp>
#include <Balau/Util/Strings.hpp>

namespace Balau::Resource {

///
/// Parses a URI and provides views into the URI's components.
///
/// A URI has the following syntax:
///
///    <strong>URI = scheme:[//authority]path[?query][\#fragment]</strong>
///
/// where the authority component divides into three sub-components:
///
///    <strong>authority = [userinfo@]host[:port]</strong>
///
/// A constructed %UriComponents structure contains indices indicating the start
/// offsets of each component in the parsed URI string. These are used to create
/// string views into the internal URI string. The %UriComponents instance also
/// contains the parsed port if one was included in the URI.
///
/// The parsing of the URI in the constructor performs some initial validation in
/// order to ensure that the URI is well formed, but, with the exception of the
/// scheme and port, does not perform full validation of each URI component. Such
/// full validation should be performed by the consuming application.
///
/// Note that deprecated userinfo passwords are not supported in this URI parser.
/// Hence attempting to parse a URI with the form scheme://usr:pw\@example.com will
/// result in an InvalidUriException being thrown.
///
struct UriComponents {
	///
	/// Remove instances of "." and "blah/.." and remove any trailing slashes.
	///
	/// @param path the path to normalize
	/// @return the normalized path.
	///
	public: static std::string normalizePath(std::string_view path) {
		const auto pathComponents = Util::Strings::split(path, "/");
		const bool absolute = Util::Strings::startsWith(path, "/");
		std::vector<std::string_view> newPathComponents;
		std::vector<size_t> pendingComponents;
		size_t index = 0;
		bool lastWasDotDot = false;

		for (const auto & pathComponent : pathComponents) {
			if (pathComponent == "..") {
				if (!pendingComponents.empty()) {
					pendingComponents.pop_back();
					lastWasDotDot = true;
				} else {
					newPathComponents.push_back(pathComponent);
				}
			} else {
				if (lastWasDotDot) {
					// Commit pending components.
					for (size_t componentIndex : pendingComponents) {
						newPathComponents.push_back(pathComponents[componentIndex]);
					}

					pendingComponents.clear();
					lastWasDotDot = false;
				}

				pendingComponents.push_back(index);
			}

			++index;
		}

		// Commit remaining pending components.
		for (size_t componentIndex : pendingComponents) {
			newPathComponents.push_back(pathComponents[componentIndex]);
		}

		return (absolute ? "/" : "") + Util::Strings::joinContainer("/", newPathComponents);
	}

	///
	/// Get the URI string contained within the URI component structure.
	///
	public: const std::string & getUri() const {
		return uri;
	}

	///
	/// Parse the URI into its components.
	///
	/// The parsing performs some initial validation in order to ensure that the
	/// URI is well formed, but, with the exception of the scheme and port, does
	/// not perform full validation of each component. Such full validation should
	/// be performed by the consuming application.
	///
	/// @param uri_ the URI to parse
	/// @throw InvalidUriException if the URI is invalid.
	///
	public: explicit UriComponents(const Uri & uri_) : UriComponents(uri_.toUriString()) {}

	///
	/// Parse the URI string in the supplied URI into its components.
	///
	/// The parsing performs some initial validation in order to ensure that the
	/// URI is well formed, but, with the exception of the scheme and port, does
	/// not perform full validation of each component. Such full validation should
	/// be performed by the consuming application.
	///
	/// @param uri_ the URI string to parse
	/// @throw InvalidUriException if the URI is invalid.
	///
	public: explicit UriComponents(std::string uri_)
		: uri(std::move(uri_))
		, schemeEndOffset(-1)
		, userinfoOffset(-1)
		, hostOffset(-1)
		, portOffset(-1)
		, pathOffset(-1)
		, queryOffset(-1)
		, fragmentOffset(-1) {
		if (uri.length() > INT16_MAX) {
			ThrowBalauException(
				  Exception::InvalidUriException
				, "The URI string length exceeds " + ::toString(INT16_MAX) + "."
			);
		}

		if (uri.empty()) {
			ThrowBalauException(Exception::InvalidUriException, "The URI is empty.");
		}

		const int length = (int) uri.length();
		char32_t c = 0;
		int offset = 0;

		//
		// Mandatory scheme section.
		//
		// The scheme begins with a letter and is followed by any combination
		// of letters, digits, plus (+), period (.), or hyphen (-).
		//

		c = Character::getNextUtf8(uri, offset);

		if (!Character::isAlpha(c)) {
			ThrowBalauException(
				Exception::InvalidUriException
			, "Invalid scheme (does not start with a letter)."
			);
		}

		while (offset < length) {
			c = Character::getNextUtf8(uri, offset);

			if (c == U':') { // End of scheme.
				break;
			} else if (!Character::isAlphaOrDecimal(c) && c != U'+' && c != U'.' && c != U'-') {
				ThrowBalauException(
					Exception::InvalidUriException
				, std::string("Invalid scheme (invalid character at index ") + ::toString(offset) + ")."
				);
			}
		}

		schemeEndOffset = short(offset - 1);
		int userinfoEnd = -1;

		// Optional authority section.
		if (length >= offset + 2 && uri[offset] == '/' && uri[offset + 1] == '/') {
			offset += 2; // Skip double forward slash.

			if (length == offset) {
				//
				// "The authority component is preceded by a double slash ("//") and is
				//  terminated by the next slash ("/"), question mark ("?"), or number
				//  sign ("#") character, or by the end of the URI."
				//
				// The "or by the end of the URI" clause indicates that "blah://" is valid
				// and thus the URI is valid and the authority section / path are empty.
				//
				pathOffset = short(offset);
			} else {
				while (offset < length) {
					c = Character::getNextUtf8(uri, offset);

					if (hostOffset == -1 && c == U'[') { // [] enclosed IPv6 address.
						hostOffset = short(offset - 1);

						while (offset < length && c != U']') {
							c = Character::getNextUtf8(uri, offset);
						}

						if (c != U']') { // No closing ']'?
							ThrowBalauException(
								Exception::InvalidUriException
							, "[] based host was found with no closing ]."
							);
						} else if (offset == length) { // End of URI.
							break;
						}
					} else if (c == U'/') { // End of host, no port, start of path.
						pathOffset = short(offset - 1);
						break;
					} else if (c == U'?' || c == U'#') { // End of host, no port, no path, start of query/fragment.
						--offset;
						pathOffset = short(offset);
						break;
					} else if (userinfoEnd == -1 && c == U'@') { // End of userinfo.
						userinfoEnd = offset - 1;
						userinfoOffset = short(schemeEndOffset + 3);
					} else if (c == U':' && (userinfoEnd != -1 || offset > schemeEndOffset + 2)) { // Start of port.
						if (userinfoEnd == -1) {
							// No user info, so all text between the scheme and the port is the host.
							hostOffset = short(schemeEndOffset + 3);
						}

						portOffset = short(offset);

						while (offset < length) {
							c = Character::getNextUtf8(uri, offset);

							if (c == U'@') { // Deprecated password.
								ThrowBalauException(
									Exception::InvalidUriException
								, "Passwords in user information are not supported."
								);
							} else if (c == U'/' || c == U'?' || c == U'#') { // End of port.
								--offset;
								pathOffset = short(offset);
								break;
							}
						}

						break;
					}
				}

				if (hostOffset == -1) {
					if (userinfoOffset != -1 && offset > userinfoEnd + 1) {
						hostOffset = short(userinfoEnd + 1);
					} else if (offset > schemeEndOffset + 2) {
						hostOffset = short(schemeEndOffset + 3);
					}
				}
			}
		}

		// If there is an authority section, there must be a non empty host component.
		if (userinfoOffset != -1  || portOffset != -1) {
			if (hostOffset == -1) {
				ThrowBalauException(
					Exception::InvalidUriException, "Authority section does not contain host information."
				);
			} else if (portOffset != -1) {
				if (portOffset == hostOffset + 1) {
					ThrowBalauException(
						Exception::InvalidUriException, "Authority section does not contain host information."
					);
				}
			} else if (offset == hostOffset + 1) {
				ThrowBalauException(
					Exception::InvalidUriException, "Authority section does not contain host information."
				);
			}
		}

		if (pathOffset == -1) {
			// Either:
			//  - no port, no query, no fragment; or
			//  - no host, no port, no query, no fragment
			pathOffset = short(offset);
		}

		// Mandatory path section.
		while (offset < length) {
			c = Character::getNextUtf8(uri, offset);

			if (c == U'?' || c == U'#') { // End of path.
				break;
			}
		}

		// Optional query section.
		if (offset < length && c == U'?') {
			queryOffset = short(offset);

			while (offset < length) {
				c = Character::getNextUtf8(uri, offset);

				if (c == U'#') { // End of query.
					break;
				}
			}
		}

		// Optional fragment section.
		if (offset < length) {
			fragmentOffset = short(offset);
		}

		// Pre-convert the port into an unsigned short if it is set.
		if (portOffset != -1) {
			const unsigned long p = std::stoul(uri.substr((size_t) portOffset));

			if (p > UINT16_MAX) {
				ThrowBalauException(Exception::InvalidUriException, std::string("Invalid port number: ") + ::toString(p));
			}

			parsedPort = (unsigned short) p;
		} else {
			parsedPort = (unsigned short) 0;
		}
	}

	///
	/// Create a URI components instance by copying the supplied instance.
	///
	public: UriComponents(const UriComponents & ) = default;

	///
	/// Create a URI components instance by moving the contents of the supplied instance.
	///
	public: UriComponents(UriComponents && ) = default;

	///
	/// Assign a URI components instance by copying the supplied instance.
	///
	/// @return the current object
	///
	public: UriComponents & operator = (const UriComponents & ) = default;

	///
	/// Assign a URI components instance by moving the contents of the supplied instance.
	///
	/// @return the current object
	///
	public: UriComponents & operator = (UriComponents && ) = default;

	///
	/// Returns true if the URI has an authority section.
	///
	/// @return true if the URI has an authority section
	///
	public: bool hasAuthority() const {
		return hostOffset != -1;
	}

	///
	/// Returns true if the URI has user info.
	///
	/// @return true if the URI has user info
	///
	public: bool hasUserInfo() const {
		return userinfoOffset != -1;
	}

	///
	/// Returns true if the URI has a host entry.
	///
	/// @return true if the URI has a host entry
	///
	public: bool hasHost() const {
		return hostOffset != -1;
	}

	///
	/// Returns true if the URI has port information.
	///
	/// @return true if the URI has port information
	///
	public: bool hasPort() const {
		return portOffset != -1;
	}

	///
	/// Returns true if the URI has a query.
	///
	/// @return true if the URI has a query
	///
	public: bool hasQuery() const {
		return queryOffset != -1;
	}

	///
	/// Returns true if the URI has a fragment.
	///
	/// @return true if the URI has a fragment
	///
	public: bool hasFragment() const {
		return fragmentOffset != -1;
	}

	///
	/// Obtain a string view to the scheme.
	///
	/// @return a string view to the scheme
	///
	public: std::string_view scheme() const {
		return std::string_view(uri.c_str(), (size_t) schemeEndOffset);
	}

	///
	/// Obtain a string view to the user info.
	///
	/// If there is no userinfo in the URI, an empty string will be returned.
	///
	/// @return a string view to the user info
	///
	public: std::string_view userInfo() const {
		if (!hasUserInfo()) {
			return std::string_view();
		}

		const int endOffset = hostOffset - 1; // "@"
		Assert::assertion(endOffset > 0);
		return std::string_view(uri.c_str() + userinfoOffset, (size_t) (endOffset - userinfoOffset));
	}

	///
	/// Obtain a string view to the host.
	///
	/// If there is no host in the URI, an empty string will be returned.
	///
	/// @return a string view to the host
	///
	public: std::string_view host() const {
		if (!hasHost()) {
			return std::string_view();
		}

		const int endOffset = hasPort()
			? portOffset - 1 // ":"
			: pathOffset;

		Assert::assertion(endOffset > 0);
		return std::string_view(uri.c_str() + hostOffset, (size_t) (endOffset - hostOffset));
	}

	///
	/// Obtain the port. If there is no port in the URI, zero will be returned.
	///
	/// @return the port number or zero if the port was not set.
	///
	public: unsigned short port() const {
		return parsedPort;
	}

	///
	/// Obtain a string view to the path.
	///
	/// If the path is empty, an empty string will be returned.
	///
	/// @return a string view to the path
	///
	public: std::string_view path() const {
		const int endOffset = hasQuery()
			? queryOffset - 1 // "?"
			: hasFragment()
			? fragmentOffset - 1 // "#"
			: (int) uri.length();

		Assert::assertion(endOffset > 0);
		return std::string_view(uri.c_str() + pathOffset, (size_t) (endOffset - pathOffset));
	}

	///
	/// Obtain a string view to the query.
	///
	/// If there is no query in the URI, an empty string will be returned.
	///
	/// @return a string view to the query
	///
	public: std::string_view query() const {
		if (!hasQuery()) {
			return std::string_view();
		}

		const int endOffset = hasFragment()
			? fragmentOffset - 1 // "#"
			: (int) uri.length();

		Assert::assertion(endOffset > 0);
		return std::string_view(uri.c_str() + queryOffset, (size_t) (endOffset - queryOffset));
	}

	///
	/// Obtain a string view to the fragment.
	///
	/// If there is no fragment in the URI, an empty string will be returned.
	///
	/// @return a string view to the fragment
	///
	public: std::string_view fragment() const {
		if (!hasFragment()) {
			return std::string_view();
		}

		return std::string_view(uri.c_str() + fragmentOffset, (size_t) (uri.length() - fragmentOffset));
	}

	///
	/// Obtain a string view to the path-query-fragment.
	///
	/// This is the usual string used in a client request call.
	///
	/// @return a string view to the path-query-fragment
	///
	public: std::string_view pathQueryFragment() const {
		return std::string_view(uri.c_str() + pathOffset, (size_t) (uri.length() - pathOffset));
	}

	////////////////////////// Private implementation /////////////////////////

	friend struct UriComponentsTest;

	// For testing.
	private: UriComponents(std::string uri_,
	                       short schemeEndOffset_,
	                       short userinfoOffset_,
	                       short hostOffset_,
	                       short portOffset_,
	                       short pathOffset_,
	                       short queryOffset_,
	                       short fragmentOffset_,
	                       unsigned short parsedPort_)
		: uri(std::move(uri_))
		, schemeEndOffset(schemeEndOffset_)
		, userinfoOffset(userinfoOffset_)
		, hostOffset(hostOffset_)
		, portOffset(portOffset_)
		, pathOffset(pathOffset_)
		, queryOffset(queryOffset_)
		, fragmentOffset(fragmentOffset_)
		, parsedPort(parsedPort_) {}

	private: std::string uri;
	private: short schemeEndOffset;
	private: short userinfoOffset;
	private: short hostOffset;
	private: short portOffset;
	private: short pathOffset;
	private: short queryOffset;
	private: short fragmentOffset;
	private: unsigned short parsedPort;

	friend bool operator == (const UriComponents & lhs, const UriComponents & rhs);
};

///
/// Returns true if the two URI component instances are equal.
///
/// This function will not normalise the URIs.
///
/// @return true if the two URI component instances are equal
///
inline bool operator == (const UriComponents & lhs, const UriComponents & rhs) {
	return lhs.uri == rhs.uri;
}

///
/// Print the URI components as a UTF-8 string.
///
/// This function does not normalise the URI.
///
/// @return a UTF-8 string representing the URI components
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const UriComponents & uriComponents) {
	return toString<AllocatorT>(uriComponents.getUri());
}

///
/// Print the URI components as a UTF-8 string.
///
/// This function does not normalise the URI.
///
/// @return a UTF-8 string representing the URI components
///
inline std::string toString(const UriComponents & uriComponents) {
	return uriComponents.getUri();
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE_BALAU_RESOURCE_URI_COMPONENTS
