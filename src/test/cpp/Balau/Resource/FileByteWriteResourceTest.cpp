// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>

#include <Balau/Type/OnScopeExit.hpp>
#include <Balau/Util/Files.hpp>

namespace Balau {

using Testing::is;

namespace Resource {

struct FileByteWriteResourceTest : public Testing::TestGroup<FileByteWriteResourceTest> {
	FileByteWriteResourceTest() {
		RegisterTestCase(test);
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
