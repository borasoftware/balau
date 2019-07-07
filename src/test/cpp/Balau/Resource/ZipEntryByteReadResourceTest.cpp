// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ZipEntryByteReadResourceTest.hpp"

#include "../../TestResources.hpp"

#include <Balau/Resource/ZipFile.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::is;

namespace Resource {

void ZipEntryByteReadResourceTest::test() {
	File file = TestResources::BalauSourceTestResourcesFolder / "Zips" / "ZipFile.zip";
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

} // namespace Resource

} // namespace Balau
