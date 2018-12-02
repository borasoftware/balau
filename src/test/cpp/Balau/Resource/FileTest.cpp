// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileTest.hpp"
#include "../../TestResources.hpp"

namespace Balau {

using Testing::assertThat;
using Testing::isGreaterThan;
using Testing::isLessThan;

namespace Resource {

void FileTest::iteration() {
	File root(TestResources::BalauSourceFolder / "doc" / "manual");
	auto iterator = root.recursiveFileIterator();

	size_t regularFileCount = 0;
	size_t regularFileCumulativeSize = 0;
	size_t directoryFileCount = 0;

	while (iterator.hasNext()) {
		auto file = iterator.next();

		if (file.isRegularFile()) {
			++regularFileCount;
			regularFileCumulativeSize += file.size();
		} else if (file.isRegularDirectory()) {
			++directoryFileCount;
		}
	}

	assertThat(regularFileCount, isGreaterThan(38U));
	assertThat(directoryFileCount, isGreaterThan(10U));

	assertThat(regularFileCount, isLessThan(100U));
	assertThat(directoryFileCount, isLessThan(100U));

	assertThat(regularFileCumulativeSize, isGreaterThan(100000U));
	assertThat(regularFileCumulativeSize, isLessThan(100000000U));
}

} // namespace Resource

} // namespace Balau
