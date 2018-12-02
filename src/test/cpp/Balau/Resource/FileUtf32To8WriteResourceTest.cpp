// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileUtf32To8WriteResourceTest.hpp"
#include "../../TestResources.hpp"

#include <Balau/Type/OnScopeExit.hpp>
#include <Balau/Util/Files.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

namespace Resource {

File FileUtf32To8WriteResourceTest::prepWritePath(const std::string & testName, const std::string & text) {
	const std::string filename = std::string("FileUtf32To8WriteResourceTest-") + testName + ".log";
	File file = TestResources::BalauTestResultsFolder / "Resource" / filename;
	const std::string fileUriStr = file.toUriString();

	file.getParentDirectory().createDirectories();
	assertThat(file.getParentDirectory().exists(), is(true));
	file.removeFile();
	assertThat(file.exists(), is(false));

	return file;
}

void FileUtf32To8WriteResourceTest::test() {
	const std::string expected =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
		"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

	File fileA = prepWritePath("test_a", expected);
	File fileB = prepWritePath("test_b", expected);
	OnScopeExit removeFileA([=] () mutable { fileA.removeFile(); });
	OnScopeExit removeFileB([=] () mutable { fileB.removeFile(); });

	auto fileWriteResource = fileA.getUtf32To8WriteResource();
	auto uriWriteResource = fileB.utf32To8WriteResource();

	std::u32ostream & fileWriteStream = fileWriteResource.writeStream();
	std::u32ostream & uriWriteStream = uriWriteResource->writeStream();

	fileWriteStream << toString32(expected);
	uriWriteStream << toString32(expected);

	fileWriteStream.flush();
	uriWriteStream.flush();

	fileWriteResource.close();
	uriWriteResource->close();

	const std::string actualA = Util::Files::readToString(fileA);
	const std::string actualB = Util::Files::readToString(fileA);

	assertThat(actualA, is(expected));
	assertThat(actualB, is(expected));
}

} // namespace Resource

} // namespace Balau
