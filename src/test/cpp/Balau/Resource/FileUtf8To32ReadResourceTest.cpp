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

struct FileUtf8To32ReadResourceTest : public Testing::TestGroup<FileUtf8To32ReadResourceTest> {
	explicit FileUtf8To32ReadResourceTest() {
		registerTest(&FileUtf8To32ReadResourceTest::test, "test");
	}

	void test() {
		File file(TestResources::SourceTestFolder / "resources" / "Util" / "readLinesToVector.logconf");

		auto fileReadResource = file.getUtf8To32ReadResource();
		auto uriReadResource = file.utf8To32ReadResource();

		std::u32istream & fileReadStream = fileReadResource.readStream();
		std::u32istream & uriReadStream = uriReadResource->readStream();

		const std::u32string expectedStart = U"# @formatter:off\n";
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
