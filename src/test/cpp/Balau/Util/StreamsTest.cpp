// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StreamsTest.hpp"
#include "../../TestResources.hpp"

#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

namespace Util {

const std::vector<std::string> expectedLines = {
	  "# @formatter:off"
	, "#"
	, "# Balau core C++ library"
	, "#"
	, "# Copyright (C) 2008 Bora Software (contact@borasoftware.com)"
	, "#"
	, "# Licensed under the Boost Software License - Version 1.0 - August 17th, 2003."
	, "# See the LICENSE file for the full license text."
	, "#"
	, ""
	, "# Example logging configuration."
	, ""
	, ".          = level: info"
	, "           , format: \"%Y-%m-%d %H:%M:%S [%thread] %LEVEL - %ns - %message\""
	, ""
	, "com.borasoftware   = level: info"
	, ""
	, "com.borasoftware.a = level: debug"
	, ""
	, "# Logging namespaces used in tests."
	, ""
	, "balau.container = level: debug"
	, "balau.injector  = level: debug"
	, "balau.network   = level: debug"
};

void StreamsTest::readLinesToVector() {
	Resource::File file(TestResources::BalauSourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");
	auto fileReadResource = file.getByteReadResource();
	std::istream & fileReadStream = fileReadResource.readStream();

	std::vector<std::string> actual = Streams::readLinesToVector(fileReadStream);

	const auto & expected = expectedLines;

	assertThat(actual, is(expected));
}

void StreamsTest::consume() {
	Resource::File file(TestResources::BalauSourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");
	auto fileReadResource = file.getByteReadResource();
	std::istream & src = fileReadResource.readStream();

	std::ostringstream dst;

	Streams::consume(dst, src);

	const std::string expected = Strings::joinContainer("\n", expectedLines) + "\n";
	const std::string actual = dst.str();

	assertThat(actual, is(expected));
}

} // namespace Util

} // namespace Balau
