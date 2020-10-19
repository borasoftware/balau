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

using Testing::startsWith;
using Testing::endsWith;

namespace Resource {

struct FileUtf8To32ReadResourceTest : public Testing::TestGroup<FileUtf8To32ReadResourceTest> {
	explicit FileUtf8To32ReadResourceTest() {
		RegisterTest(test);
	}

	void test() {
		File file(TestResources::SourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");

		auto fileReadResource = file.getUtf8To32ReadResource();
		auto uriReadResource = file.utf8To32ReadResource();

		std::u32istream & fileReadStream = fileReadResource.readStream();
		std::u32istream & uriReadStream = uriReadResource->readStream();

		const std::u32string expectedStart = U"##\n## Balau core C++ library\n## Copyright (C) 2008 Bora Software (contact@borasoftware.com)\n";
		const std::u32string expectedEnd = U"balau.network   = level: debug\n";

		auto actualFileData = toString32(fileReadStream);
		auto actualUriData = toString32(uriReadStream);

		AssertThat(actualFileData, startsWith(expectedStart));
		AssertThat(actualUriData, startsWith(expectedStart));

		AssertThat(actualFileData, endsWith(expectedEnd));
		AssertThat(actualUriData, endsWith(expectedEnd));
	}
};

} // namespace Resource

} // namespace Balau
