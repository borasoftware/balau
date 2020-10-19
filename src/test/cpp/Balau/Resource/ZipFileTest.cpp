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
#include <Balau/Resource/ZipFile.hpp>

namespace Balau {

using Testing::is;

namespace Resource {


struct ZipFileTest : public Testing::TestGroup<ZipFileTest> {
	ZipFileTest() {
		RegisterTest(iteration);
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
