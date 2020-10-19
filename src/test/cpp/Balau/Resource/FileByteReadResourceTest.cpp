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

struct FileByteReadResourceTest : public Testing::TestGroup<FileByteReadResourceTest> {
	FileByteReadResourceTest() {
		RegisterTest(test);
	}

	void test() {
		File file(TestResources::SourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");

		auto fileReadResource = file.getByteReadResource();
		auto uriReadResource = file.byteReadResource();

		std::istream & fileReadStream = fileReadResource.readStream();
		std::istream & uriReadStream = uriReadResource->readStream();

		const std::string expectedStart = "##\n## Balau core C++ library\n## Copyright (C) 2008 Bora Software (contact@borasoftware.com)\n";
		const std::string expectedEnd = "balau.network   = level: debug\n";

		auto actualFileData = ::toString(fileReadStream);
		auto actualUriData = ::toString(uriReadStream);

		AssertThat(actualFileData, startsWith(expectedStart));
		AssertThat(actualUriData, startsWith(expectedStart));

		AssertThat(actualFileData, endsWith(expectedEnd));
		AssertThat(actualUriData, endsWith(expectedEnd));
	}
};

} // namespace Resource

} // namespace Balau
