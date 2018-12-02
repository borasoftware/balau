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
/// @file UriDispatcher.hpp
///
/// Dispatcher interface for URIs.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_DISPATCHER
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_DISPATCHER

namespace Balau::Resource {

class File;
class Http;
class Https;
class ZipEntry;
class ZipFile;
class StringUri;

///
/// Visitor interface for URIs.
///
class UriDispatcher {
	///
	/// Visit a File URI.
	///
	/// @param object the URI
	///
	public: virtual void dispatch(const File & object) = 0;

	///
	/// Visit an HTTP URI.
	///
	/// @param object the URI
	///
	public: virtual void dispatch(const Http & object) = 0;

	///
	/// Visit an HTTPS URI.
	///
	/// @param object the URI
	///
	public: virtual void dispatch(const Https & object) = 0;

	///
	/// Visit a zip entry URI.
	///
	/// @param object the URI
	///
	public: virtual void dispatch(const ZipEntry & object) = 0;

	///
	/// Visit an immediate string URI.
	///
	/// @param object the URI
	///
	public: virtual void dispatch(const StringUri & object) = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_DISPATCHER
