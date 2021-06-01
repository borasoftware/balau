// @formatter:off
//
// Balau core C++ library
//
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
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::is;

namespace Resource {

struct ZipEntryByteReadResourceTest : public Testing::TestGroup<ZipEntryByteReadResourceTest> {
	ZipEntryByteReadResourceTest() {
		RegisterTestCase(test);
	}

	void test() {
		File file = TestResources::SourceTestResourcesFolder / "Zips" / "ZipFile.zip";
		ZipFile zipFile(file);

		auto iterator = zipFile.recursiveIterator();

		const std::map<std::string, std::string> expectedEntries = {
			  std::make_pair(std::string("a/aa"),     "aaaaaaaa\n")
			, std::make_pair(std::string("a/ab"),     "aaaabbbb\n")
			, std::make_pair(std::string("c/ca"),     "ccccaaaa\n")
			, std::make_pair(std::string("e/ed/eda"), "eeeeddddaaaa\n")
			, std::make_pair(std::string("e/ed/edb"), "eeeeddddbbbb\n")
			, std::make_pair(std::string("e/ea"),     "eeeeaaaa\n")
			, std::make_pair(std::string("e/eb"),     "eeeebbbb\n")
			, std::make_pair(std::string("e/ec"),     "eeeecccc\n")
			, std::make_pair(std::string("b"),        "bbbb\n")
			, std::make_pair(std::string("d"),        "dddd\n")
			, std::make_pair(std::string("f"),        "ffff\n")
		};

		while (iterator->hasNext()) {
			auto next = iterator->next();
			auto & zipEntry = * dynamic_cast<ZipEntry *>(next.get());
			auto p = zipEntry.name();

			if (!Util::Strings::endsWith(p, "/")) {
				auto iter = expectedEntries.find(p);

				AssertThat(iter == expectedEntries.end(), is(false));

				const auto & expected = iter->second;

				auto zipEntryReadResource = zipEntry.getByteReadResource();
				auto uriReadResource = zipEntry.byteReadResource();

				std::istream & zipEntryReadStream = zipEntryReadResource.readStream();
				std::istream & uriReadStream = uriReadResource->readStream();

				auto actualZipEntryData = ::toString(zipEntryReadStream);
				auto actualUriData = ::toString(uriReadStream);

				AssertThat(actualZipEntryData, is(expected));
				AssertThat(actualUriData, is(expected));
			}
		}
	}
};

} // namespace Resource

} // namespace Balau
