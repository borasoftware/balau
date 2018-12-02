// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ZipFileTest.hpp"
#include "../../TestResources.hpp"

#include <Balau/Resource/ZipFile.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

namespace Resource {

void ZipFileTest::iteration() {
	ZipFile root(TestResources::BalauSourceTestResourcesFolder / "Zips" / "ZipFile.zip");
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

	assertThat(regularEntryCount, is(11U));
	assertThat(directoryEntryCount, is(4U));

	assertThat(regularEntryCumulativeSize, is(95U));
}

} // namespace Resource

} // namespace Balau
