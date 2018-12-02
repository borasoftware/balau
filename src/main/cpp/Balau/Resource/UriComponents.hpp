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
/// @file UriComponents.hpp
///
/// A class representing the parsed components of a URI.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_COMPONENTS
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_COMPONENTS

#include <Balau/Dev/Assert.hpp>

namespace Balau::Resource {

class Uri;

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
	public: static std::string normalizePath(std::string_view path);

	///
	/// The URI string contained within the URI component structure.
	///
	public: const std::string uri;

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
	public: explicit UriComponents(const Uri & uri_);

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
	public: explicit UriComponents(const std::string & uri_);

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

	private: short schemeEndOffset;
	private: short userinfoOffset;
	private: short hostOffset;
	private: short portOffset;
	private: short pathOffset;
	private: short queryOffset;
	private: short fragmentOffset;
	private: unsigned short parsedPort;
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
inline std::string toString(const UriComponents & uriComponents) {
	return uriComponents.uri;
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_COMPONENTS
