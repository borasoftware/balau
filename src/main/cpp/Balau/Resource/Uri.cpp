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
#include "Uri.hpp"
#include "File.hpp"

#include "Http.hpp"
#include "Https.hpp"

#ifdef BALAU_ENABLE_LIBZIP
	#include "ZipFile.hpp"
	#include "ZipEntry.hpp"
#endif

#include "../Type/Character.hpp"

namespace Balau::Resource {

void fromString(std::unique_ptr<Uri> & uri, std::string_view value) {
	const auto scheme = value.substr(0, value.find(':'));
	std::string path;

	if (scheme == "http") {
		uri.reset(new Http(value));
		return;
	} else if (scheme == "https") {
		uri.reset(new Https(value));
		return;
	}

	if (scheme == "file") {
		auto s = value.substr(value.find(':') + 1);
		path = std::string(s.data(), s.length());
	} else { // Implicit file schema or zip archive.
		path = std::string(value);
	}

	#ifdef BALAU_ENABLE_LIBZIP
		if (Util::Strings::endsWith(path, ".zip")) {
			uri.reset(new ZipFile(File(path)));
		} else {
			uri.reset(new File(path));
		}
	#else
		uri.reset(new File(path));
	#endif
}

} // namespace Balau::Resource
