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
