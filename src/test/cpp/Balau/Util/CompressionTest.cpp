// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CompressionTest.hpp"
#include "../../TestResources.hpp"

#include <Balau/Util/Hashing.hpp>
#include <Balau/Type/OnScopeExit.hpp>

#include <zip.h>

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isNot;
using Testing::isGreaterThan;

namespace Util {

const Resource::File resDir  = TestResources::BalauSourceTestResourcesFolder;
const Resource::File testDir = TestResources::BalauTestResultsFolder;

const Resource::File zipFile                 = resDir / "Zips" / "ZipFile.zip";
const Resource::File encryptedZipFile        = resDir / "Zips" / "EncryptedZipFile.zip";

const Resource::File mutatedZipFile          = testDir / "Zips" / "MutatedZipFile.zip";
const Resource::File mutatedEncryptedZipFile = testDir / "Zips" / "MutatedEncryptedZipFile.zip";

const std::string testPassword = "testPW";

inline std::chrono::system_clock::time_point tp(int64_t s) {
	return std::chrono::system_clock::time_point(std::chrono::nanoseconds(s * 1000000000UL));
}

inline std::pair<std::string, std::vector<char>> mpv(std::string s, std::vector<char> v) {
	return std::make_pair<std::string, std::vector<char>>(std::move(s), std::move(v));
}

inline std::pair<std::string, std::string> mps(std::string s1, std::string s2) {
	return std::make_pair<std::string, std::string>(std::move(s1), std::move(s2));
}

void CompressionTest::fingerprintTest() {
	assertThat(
		  "ZipFileTest.zip has changed."
		, Hashing::md5(zipFile)
		, is("e4aab2f79683a428c7811f3492485320")
	);

	assertThat(
		  "EncryptedUnzipperTest.zip has changed."
		, Hashing::md5(encryptedZipFile)
		, is("1b55fc4799b7d9cf2ac5ad765c7d8040")
	);
}

void unzipperTestImpl(const Resource::File & file,
                      const std::string & pw,
                      const std::vector<ZipEntryInfo> & expectedEntryInfos) {
	const std::set<std::string> expectedEntryNames = {
		  "a/"
		, "a/aa"
		, "a/ab"
		, "b"
		, "c/"
		, "c/ca"
		, "d"
		, "e/"
		, "e/ea"
		, "e/eb"
		, "e/ec"
		, "e/ed/"
		, "e/ed/eda"
		, "e/ed/edb"
		, "f"
	};

	const std::map<std::string, std::vector<char>> expectedEntryBytes = {
		  mpv("a/",       {})
		, mpv("a/aa",     { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', '\n' })
		, mpv("a/ab",     { 'a', 'a', 'a', 'a', 'b', 'b', 'b', 'b', '\n' })
		, mpv("b",        { 'b', 'b', 'b', 'b', '\n' })
		, mpv("c/",       {})
		, mpv("c/ca",     { 'c', 'c', 'c', 'c', 'a', 'a', 'a', 'a', '\n' })
		, mpv("d",        { 'd', 'd', 'd', 'd', '\n' })
		, mpv("e/",       {})
		, mpv("e/ea",     { 'e', 'e', 'e', 'e', 'a', 'a', 'a', 'a', '\n' })
		, mpv("e/eb",     { 'e', 'e', 'e', 'e', 'b', 'b', 'b', 'b', '\n' })
		, mpv("e/ec",     { 'e', 'e', 'e', 'e', 'c', 'c', 'c', 'c', '\n' })
		, mpv("e/ed/",    {})
		, mpv("e/ed/eda", { 'e', 'e', 'e', 'e', 'd', 'd', 'd', 'd', 'a', 'a', 'a', 'a', '\n' })
		, mpv("e/ed/edb", { 'e', 'e', 'e', 'e', 'd', 'd', 'd', 'd', 'b', 'b', 'b', 'b', '\n' })
		, mpv("f",        { 'f', 'f', 'f', 'f', '\n' })
	};

	const std::map<std::string, std::string> expectedEntryStrings = {
		  mps("a/",       "")
		, mps("a/aa",     "aaaaaaaa\n")
		, mps("a/ab",     "aaaabbbb\n")
		, mps("b",        "bbbb\n")
		, mps("c/",       "")
		, mps("c/ca",     "ccccaaaa\n")
		, mps("d",        "dddd\n")
		, mps("e/",       "")
		, mps("e/ea",     "eeeeaaaa\n")
		, mps("e/eb",     "eeeebbbb\n")
		, mps("e/ec",     "eeeecccc\n")
		, mps("e/ed/",    "")
		, mps("e/ed/eda", "eeeeddddaaaa\n")
		, mps("e/ed/edb", "eeeeddddbbbb\n")
		, mps("f",        "ffff\n")
	};

	Unzipper unzipper;
	unzipper.open(file, true, pw);

	assertThat(unzipper.isOpen(), is(true));
	assertThat(unzipper.readArchiveComment(), is(""));
	assertThat(unzipper.entryCount(), is((long long) expectedEntryNames.size()));

	const auto names = unzipper.entryNames();
	assertThat(std::set<std::string>(names.begin(), names.end()), is(expectedEntryNames));

	for (auto & name : expectedEntryNames) {
		assertThat(unzipper.hasEntry(name), is(true));
	}

	assertThat(unzipper.hasEntry("not-found"), is(false));
	assertThat(unzipper.hasEntry("a/b/c/not-found"), is(false));

	std::set<long long> indices;

	for (auto expectedInfo : expectedEntryInfos) {
		ZipEntryInfo actualInfo {};
		unzipper.getEntryInfo(expectedInfo.name, actualInfo);

		// Zero out the index.. just check that all indices were returned.
		assertThat(indices.find(actualInfo.index) == indices.end(), is(true));
		indices.insert(actualInfo.index);
		actualInfo.index = 0;
		actualInfo.modificationTime = std::chrono::system_clock::time_point();
		expectedInfo.modificationTime = std::chrono::system_clock::time_point();

		// Zero the encryption method in the expected if the test is not using encryption.
		if (pw.empty()) {
			expectedInfo.encryptionMethod = 0;
		}

		assertThat(actualInfo, is(expectedInfo));
	}

	for (auto & name : expectedEntryNames) {
		assertThat(unzipper.readEntryComment(name), is(""));

		if (!Strings::endsWith(name, "/")) {
			assertThat(unzipper.readEntryAsBytes(name), is(expectedEntryBytes.at(name)));
			assertThat(unzipper.readEntryAsString(name), is(expectedEntryStrings.at(name)));
		}
	}

	unzipper.close();

	assertThat(unzipper.isOpen(), is(false));
}

void CompressionTest::unzipperTest() {
	const std::vector<ZipEntryInfo> expectedEntryInfos = {
		// name, index (not checked), uncompressedSize, compressedSize, modificationTime, crc, compressionMethod, encryptionMethod
		  { "a/",       0,  0,  0, tp(1534335970),          0, 0, 0 }
		, { "a/aa",     0,  9,  6, tp(1534335960), 2916574902, 8, 0 }
		, { "a/ab",     0,  9,  9, tp(1534335970), 3517537810, 0, 0 }
		, { "b",        0,  5,  5, tp(1534335910), 1219630029, 0, 0 }
		, { "c/",       0,  0,  0, tp(1534335996),          0, 0, 0 }
		, { "c/ca",     0,  9,  9, tp(1534335996),  458488175, 0, 0 }
		, { "d",        0,  5,  5, tp(1534335918), 2957927045, 0, 0 }
		, { "e/",       0,  0,  0, tp(1534336040),          0, 0, 0 }
		, { "e/ea",     0,  9,  9, tp(1534336010),  464478021, 0, 0 }
		, { "e/eb",     0,  9,  9, tp(1534336022), 1741776865, 0, 0 }
		, { "e/ec",     0,  9,  9, tp(1534336026), 4208226882, 0, 0 }
		, { "e/ed/",    0,  0,  0, tp(1534336062),          0, 0, 0 }
		, { "e/ed/eda", 0, 13, 12, tp(1534336056), 4048045758, 8, 0 }
		, { "e/ed/edb", 0, 13, 12, tp(1534336062), 2369158682, 8, 0 }
		, { "f",        0,  5,  5, tp(1534335924), 1362448258, 0, 0 }
	};

	unzipperTestImpl(zipFile, "", expectedEntryInfos);
}

void CompressionTest::encryptedUnzipperTest() {
	const std::vector<ZipEntryInfo> expectedEntryInfos = {
		// name, index (not checked), uncompressedSize, compressedSize, modificationTime, crc, compressionMethod, encryptionMethod
		  { "a/",       0,  0,  0, tp(1534335970),          0, 0, 0 }
		, { "a/aa",     0,  9, 18, tp(1534335960), 2916574902, 8, ZIP_EM_TRAD_PKWARE }
		, { "a/ab",     0,  9, 21, tp(1534335970), 3517537810, 0, ZIP_EM_TRAD_PKWARE }
		, { "b",        0,  5, 17, tp(1534335910), 1219630029, 0, ZIP_EM_TRAD_PKWARE }
		, { "c/",       0,  0,  0, tp(1534335996),          0, 0, 0 }
		, { "c/ca",     0,  9, 21, tp(1534335996),  458488175, 0, ZIP_EM_TRAD_PKWARE }
		, { "d",        0,  5, 17, tp(1534335918), 2957927045, 0, ZIP_EM_TRAD_PKWARE }
		, { "e/",       0,  0,  0, tp(1534336040),          0, 0, 0 }
		, { "e/ea",     0,  9, 21, tp(1534336010),  464478021, 0, ZIP_EM_TRAD_PKWARE }
		, { "e/eb",     0,  9, 21, tp(1534336022), 1741776865, 0, ZIP_EM_TRAD_PKWARE }
		, { "e/ec",     0,  9, 21, tp(1534336026), 4208226882, 0, ZIP_EM_TRAD_PKWARE }
		, { "e/ed/",    0,  0,  0, tp(1534336062),          0, 0, 0 }
		, { "e/ed/eda", 0, 13, 24, tp(1534336056), 4048045758, 8, ZIP_EM_TRAD_PKWARE }
		, { "e/ed/edb", 0, 13, 24, tp(1534336062), 2369158682, 8, ZIP_EM_TRAD_PKWARE }
		, { "f",        0,  5, 17, tp(1534335924), 1362448258, 0, ZIP_EM_TRAD_PKWARE }
	};

	unzipperTestImpl(encryptedZipFile, testPassword, expectedEntryInfos);
}

const std::string archiveComment = "This is an archive comment.";
const std::string newDirectory = "q/w/e/r/";
const std::string newEntryExistingDirectoryFromFile = "q/w/e/r/newEntry.zip";
const std::string newEntryExistingDirectoryFromBytes = "q/w/e/r/newBytes";
const std::string newEntryExistingDirectoryFromString = "q/w/e/r/newString";
const std::vector<char> bytes {1, 2, 3, 4, 5, 6, 7, 8, 9};
const std::string str = "qwertyuiop";
const std::string newEntryExistingDirectoryFromStringRenamed = "q/w/e/r/newStringRenamed";

void zipperTestImpl2(const Resource::File & file, bool commit, const std::string & pw) {
	Zipper zipper;
	zipper.open(file, true);

	assertThat(zipper.isOpen(), is(true));
	assertThat(zipper.readArchiveComment(), is(""));

	zipper.putArchiveComment(archiveComment);

	assertThat(zipper.readArchiveComment(), is(archiveComment));

	zipper.deleteComment();

	assertThat(zipper.readArchiveComment(), is(""));
	assertThat(zipper.hasEntry(newDirectory), is(false));

	zipper.putDirectory(newDirectory);

	assertThat(zipper.hasEntry(newDirectory), is(true));
	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromFile), is(false));

	zipper.putEntry(newEntryExistingDirectoryFromFile, zipFile);

	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromFile), is(true));
	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromBytes), is(false));

	zipper.putEntry(newEntryExistingDirectoryFromBytes, bytes);

	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromBytes), is(true));
	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromString), is(false));

	zipper.putEntry(newEntryExistingDirectoryFromString, str);

	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromString), is(true));

	zipper.renameEntry(newEntryExistingDirectoryFromString, newEntryExistingDirectoryFromStringRenamed);

	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromString), is(false));
	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromStringRenamed), is(true));

	zipper.deleteEntry(newEntryExistingDirectoryFromStringRenamed);

	assertThat(zipper.hasEntry(newEntryExistingDirectoryFromStringRenamed), is(false));

	if (commit) {
		zipper.commit();

		// Check commit.

		Unzipper unzipper;
		unzipper.open(file, true);

		assertThat(unzipper.hasEntry(newDirectory), is(true));
		assertThat(unzipper.hasEntry(newEntryExistingDirectoryFromFile), is(true));
		assertThat(unzipper.hasEntry(newEntryExistingDirectoryFromBytes), is(true));

		unzipper.close();

		assertThat(unzipper.isOpen(), is(false));
	} else {
		zipper.close();
	}

	assertThat(zipper.isOpen(), is(false));
}

void zipperTestImpl1(const Resource::File & srcFile, Resource::File dstFile, const std::string & pw) {
	// Remove any existing file from a previous partial run.
	dstFile.removeFile();

	// Remove the file created in this run on scope exit.
	OnScopeExit removeTestFile([&dstFile] () { dstFile.removeFile(); });

	dstFile.getParentDirectory().createDirectories();
	Files::copy(srcFile, dstFile);

	assertThat(dstFile.exists(), is(true));

	auto md5Hash = Hashing::md5(dstFile);

	// First try with rollback.
	zipperTestImpl2(dstFile, false, pw);

	// Verify that the file is not modified.
	auto newMd5Hash = Hashing::md5(dstFile);

	assertThat(md5Hash, is(newMd5Hash));

	// Then try with commit.
	zipperTestImpl2(dstFile, true, pw);

	newMd5Hash = Hashing::md5(dstFile);

	assertThat(md5Hash, isNot(newMd5Hash));
}

void CompressionTest::zipperTest() {
	zipperTestImpl1(zipFile, mutatedZipFile, "");
}

void CompressionTest::encryptedZipperTest() {
	zipperTestImpl1(encryptedZipFile, mutatedEncryptedZipFile, testPassword);
}

} // namespace Util

} // namespace Balau
