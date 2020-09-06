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

#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::startsWith;
using Testing::endsWith;

namespace Resource {

struct FileByteReadResourceTest : public Testing::TestGroup<FileByteReadResourceTest> {
	FileByteReadResourceTest() {
		RegisterTestCase(test);
	}

	void test() {
		File file(TestResources::SourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");

		auto fileReadResource = file.getByteReadResource();
		auto uriReadResource = file.byteReadResource();

		std::istream & fileReadStream = fileReadResource.readStream();
		std::istream & uriReadStream = uriReadResource->readStream();

		const std::string expectedStart = "# @formatter:off\n";
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
