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
#include <TestResources.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::is;

namespace Util {

const std::vector<std::string> expectedLines = {
	  "##"
	, "## Balau core C++ library"
	, "## Copyright (C) 2008 Bora Software (contact@borasoftware.com)"
	, "##"
	, "## Licensed under the Apache License, Version 2.0 (the \"License\");"
	, "## you may not use this file except in compliance with the License."
	, "## You may obtain a copy of the License at"
	, "##"
	, "##     http://www.apache.org/licenses/LICENSE-2.0"
	, "##"
	, "## Unless required by applicable law or agreed to in writing, software"
	, "## distributed under the License is distributed on an \"AS IS\" BASIS,"
	, "## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
	, "## See the License for the specific language governing permissions and"
	, "## limitations under the License."
	, "##"
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

struct StreamsTest : public Testing::TestGroup<StreamsTest> {
	StreamsTest() {
		RegisterTest(readLinesToVector);
		RegisterTest(consume);
	}

	void readLinesToVector() {
		Resource::File file(TestResources::SourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");
		auto fileReadResource = file.getByteReadResource();
		std::istream & fileReadStream = fileReadResource.readStream();

		std::vector<std::string> actual = Streams::readLinesToVector(fileReadStream);

		const auto & expected = expectedLines;

		AssertThat(actual, is(expected));
	}

	void consume() {
		Resource::File file(TestResources::SourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");
		auto fileReadResource = file.getByteReadResource();
		std::istream & src = fileReadResource.readStream();

		std::ostringstream dst;

		Streams::consume(dst, src);

		const std::string expected = Strings::joinContainer("\n", expectedLines) + "\n";
		const std::string actual = dst.str();

		AssertThat(actual, is(expected));
	}
};

} // namespace Util

} // namespace Balau
