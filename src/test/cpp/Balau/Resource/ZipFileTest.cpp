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
#include <Balau/Resource/ZipFile.hpp>

namespace Balau {

using Testing::is;

namespace Resource {


struct ZipFileTest : public Testing::TestGroup<ZipFileTest> {
	ZipFileTest() {
		registerTest(&ZipFileTest::iteration, "iteration");
	}

	void iteration() {
		ZipFile root(TestResources::SourceTestResourcesFolder / "Zips" / "ZipFile.zip");
		auto iterator = root.recursiveZipFileIterator();

		size_t regularEntryCount = 0;
		size_t regularEntryCumulativeSize = 0;
		size_t directoryEntryCount = 0;

		while (iterator.hasNext()) {
			auto entry = iterator.next();

			if (entry.isEntryFile()) {
				++regularEntryCount;
				regularEntryCumulativeSize += entry.size();
			} else if (entry.isEntryDirectory()) {
				++directoryEntryCount;
			}
		}

		AssertThat(regularEntryCount, is(11U));
		AssertThat(directoryEntryCount, is(4U));

		AssertThat(regularEntryCumulativeSize, is(95U));
	}
};

} // namespace Resource

} // namespace Balau
