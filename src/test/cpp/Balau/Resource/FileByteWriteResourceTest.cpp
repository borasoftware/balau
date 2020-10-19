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

#include <Balau/Type/OnScopeExit.hpp>
#include <Balau/Util/Files.hpp>

namespace Balau {

using Testing::is;

namespace Resource {

struct FileByteWriteResourceTest : public Testing::TestGroup<FileByteWriteResourceTest> {
	FileByteWriteResourceTest() {
		RegisterTest(test);
	}

	static File prepWritePath(const std::string & testName, const std::string & text) {
		const std::string filename = std::string("FileByteWriteResourceTest-") + testName + ".log";
		File file = TestResources::TestResultsFolder / "Resource" / filename;
		const std::string fileUriStr = file.toUriString();

		file.getParentDirectory().createDirectories();
		AssertThat(file.getParentDirectory().exists(), is(true));
		file.removeFile();
		AssertThat(file.exists(), is(false));

		return file;
	}

	void test() {
		const std::string expected =
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
			"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

		File fileA = prepWritePath("test_a", expected);
		File fileB = prepWritePath("test_b", expected);
		OnScopeExit removeFileA([=] () mutable { fileA.removeFile(); });
		OnScopeExit removeFileB([=] () mutable { fileB.removeFile(); });

		auto fileWriteResource = fileA.getByteWriteResource();
		auto uriWriteResource = fileB.byteWriteResource();

		std::ostream & fileWriteStream = fileWriteResource.writeStream();
		std::ostream & uriWriteStream = uriWriteResource->writeStream();

		fileWriteStream << expected;
		uriWriteStream << expected;

		fileWriteStream.flush();
		uriWriteStream.flush();

		fileWriteResource.close();
		uriWriteResource->close();

		const std::string actualA = Util::Files::readToString(fileA);
		const std::string actualB = Util::Files::readToString(fileA);

		AssertThat(actualA, is(expected));
		AssertThat(actualB, is(expected));
	}
};

} // namespace Resource

} // namespace Balau
