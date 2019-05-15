// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Uri.hpp"
#include "File.hpp"
#include "Http.hpp"
#include "Https.hpp"
#include "ZipFile.hpp"
#include "ZipEntry.hpp"
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
	} else if (scheme == "file") {
		auto s = value.substr(value.find(':') + 1);
		path = std::string(s.data(), s.length());
	} else { // Implicit file schema or zip archive.
		path = std::string(value);
	}

	if (Util::Strings::endsWith(path, ".zip")) {
		uri.reset(new ZipFile(File(path)));
	} else {
		uri.reset(new File(path));
	}
}

} // namespace Balau::Resource
