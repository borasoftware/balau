// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "UriComponents.hpp"

#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

std::string UriComponents::normalizePath(std::string_view path) {
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

//
// URI = scheme:[//[userinfo@]host[:port]]path[?query][#fragment]
//

UriComponents::UriComponents(const Uri & uri_) : UriComponents(uri_.toUriString()) {}

UriComponents::UriComponents(std::string uri_)
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
			Exception::InvalidUriException, "The URI string length exceeds " + ::toString(INT16_MAX) + "."
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

} // namespace Balau::Resource
