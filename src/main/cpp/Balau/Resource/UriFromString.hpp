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
/// @file Uri.hpp
///
/// FromString functions for URIs.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_FROM_STRING
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_FROM_STRING

#include <Balau/Resource/Uri.hpp>

#ifdef BALAU_ENABLE_HTTP
	#include <Balau/Resource/Http.hpp>
#endif

#ifdef BALAU_ENABLE_ZIP
	#include <Balau/Resource/ZipFile.hpp>
	#include <Balau/Resource/ZipEntry.hpp>
#endif

#include <Balau/Type/Character.hpp>

namespace Balau::Resource {

///
/// Create a unique pointer URI from the supplied string.
///
/// @param uri the unique pointer container to place the new URI instance into
/// @param value the input string
///
inline void fromString(std::unique_ptr<Uri> & uri, std::string_view value) {
	const auto scheme = value.substr(0, value.find(':'));
	std::string path;

	#ifdef BALAU_ENABLE_HTTP
		if (scheme == "http") {
			uri.reset(new Http(value));
			return;
		} else if (scheme == "https") {
			uri.reset(new Https(value));
			return;
		}
	#endif

	if (scheme == "file") {
		auto s = value.substr(value.find(':') + 1);
		path = std::string(s.data(), s.length());
	} else { // Implicit file schema or zip archive.
		path = std::string(value);
	}

	#ifdef BALAU_ENABLE_ZIP
		if (Util::Strings::endsWith(path, ".zip")) {
			uri.reset(new ZipFile(File(path)));
		} else {
			uri.reset(new File(path));
		}
	#else
		uri.reset(new File(path));
	#endif
}

///
/// Create a shared pointer URI from the supplied string.
///
/// @param uri the shared pointer container to place the new URI instance into
/// @param value the input string
///
inline void fromString(std::shared_ptr<Uri> & uri, std::string_view value) {
	std::unique_ptr<Uri> ptr;
	fromString(ptr, value);
	uri = std::shared_ptr<Uri>(std::move(ptr));
}

///
/// Create a shared const pointer URI from the supplied string.
///
/// @param uri the shared pointer container to place the new URI instance into
/// @param value the input string
///
inline void fromString(std::shared_ptr<const Uri> & uri, std::string_view value) {
	std::unique_ptr<Uri> ptr;
	fromString(ptr, value);
	uri = std::shared_ptr<const Uri>(std::move(ptr));
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_FROM_STRING
