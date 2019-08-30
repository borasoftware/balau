// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <Balau/Testing/TestRunner.hpp>

#include "../../TestResources.hpp"

#include <Balau/Resource/ZipFile.hpp>
#include <Balau/Util/Streams.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStatic"

namespace Balau {

using Testing::is;

namespace Resource {

struct ZipEntryUtf8To32ReadResourceTest : public Testing::TestGroup<ZipEntryUtf8To32ReadResourceTest> {
	explicit ZipEntryUtf8To32ReadResourceTest() {
		registerTest(&ZipEntryUtf8To32ReadResourceTest::test, "test");
	}

	void test() {
		File file = TestResources::BalauSourceTestResourcesFolder / "Zips" / "ZipFile.zip";
		ZipFile zipFile(file);

		auto iterator = zipFile.recursiveIterator();

		const std::map<std::string, std::u32string> expectedEntries = {
			  std::make_pair(std::string("a/aa"),     U"aaaaaaaa\n")
			, std::make_pair(std::string("a/ab"),     U"aaaabbbb\n")
			, std::make_pair(std::string("c/ca"),     U"ccccaaaa\n")
			, std::make_pair(std::string("e/ed/eda"), U"eeeeddddaaaa\n")
			, std::make_pair(std::string("e/ed/edb"), U"eeeeddddbbbb\n")
			, std::make_pair(std::string("e/ea"),     U"eeeeaaaa\n")
			, std::make_pair(std::string("e/eb"),     U"eeeebbbb\n")
			, std::make_pair(std::string("e/ec"),     U"eeeecccc\n")
			, std::make_pair(std::string("b"),        U"bbbb\n")
			, std::make_pair(std::string("d"),        U"dddd\n")
			, std::make_pair(std::string("f"),        U"ffff\n")
		};

		while (iterator->hasNext()) {
			auto next = iterator->next();
			auto & zipEntry = * dynamic_cast<ZipEntry *>(next.get());
			auto p = zipEntry.name();

			if (!Util::Strings::endsWith(p, "/")) {
				auto iter = expectedEntries.find(p);

				AssertThat(iter == expectedEntries.end(), is(false));

				const auto & expected = iter->second;

				auto zipEntryReadResource = zipEntry.getUtf8To32ReadResource();
				auto uriReadResource = zipEntry.utf8To32ReadResource();

				std::u32istream & zipEntryReadStream = zipEntryReadResource.readStream();
				std::u32istream & uriReadStream = uriReadResource->readStream();

				auto actualZipEntryData = toString32(zipEntryReadStream);
				auto actualUriData = toString32(uriReadStream);

				AssertThat(actualZipEntryData, is(expected));
				AssertThat(actualUriData, is(expected));
			}
		}
	}
};

} // namespace Resource

} // namespace Balau

#pragma clang diagnostic pop
