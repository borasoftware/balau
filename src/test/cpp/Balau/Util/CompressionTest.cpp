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

#ifdef BALAU_LIBZIP_ENABLED

#include <zip.h>

#endif // BALAU_LIBZIP_ENABLED

namespace Balau {

using Testing::is;
using Testing::isNot;
using Testing::isGreaterThan;

namespace Util {

#ifdef BALAU_LIBZIP_ENABLED

const Resource::File resDir  = TestResources::BalauSourceTestResourcesFolder; // NOLINT
const Resource::File testDir = TestResources::BalauTestResultsFolder; // NOLINT

const Resource::File zipFile                 = resDir / "Zips" / "ZipFile.zip"; // NOLINT
const Resource::File encryptedZipFile        = resDir / "Zips" / "EncryptedZipFile.zip"; // NOLINT

const Resource::File mutatedZipFile          = testDir / "Zips" / "MutatedZipFile.zip"; // NOLINT
const Resource::File mutatedEncryptedZipFile = testDir / "Zips" / "MutatedEncryptedZipFile.zip"; // NOLINT

const std::string testPassword = "testPW"; // NOLINT

static inline std::chrono::system_clock::time_point tp(int64_t s) {
	return std::chrono::system_clock::time_point(std::chrono::nanoseconds(s * 1000000000UL));
}

static inline std::pair<std::string, std::vector<char>> mpv(std::string s, std::vector<char> v) {
	return std::make_pair<std::string, std::vector<char>>(std::move(s), std::move(v));
}

static inline std::pair<std::string, std::string> mps(std::string s1, std::string s2) {
	return std::make_pair<std::string, std::string>(std::move(s1), std::move(s2));
}

void CompressionTest::fingerprintTest() {
	AssertThat(
		  "ZipFileTest.zip has changed."
		, Hashing::md5(zipFile)
		, is("e4aab2f79683a428c7811f3492485320")
	);

	AssertThat(
		  "EncryptedUnzipperTest.zip has changed."
		, Hashing::md5(encryptedZipFile)
		, is("1b55fc4799b7d9cf2ac5ad765c7d8040")
	);
}

static void unzipperTestImpl(const Resource::File & file,
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

	AssertThat(unzipper.isOpen(), is(true));
	AssertThat(unzipper.readArchiveComment(), is(""));
	AssertThat(unzipper.entryCount(), is((long long) expectedEntryNames.size()));

	const auto names = unzipper.entryNames();
	AssertThat(std::set<std::string>(names.begin(), names.end()), is(expectedEntryNames));

	for (auto & name : expectedEntryNames) {
		AssertThat(unzipper.hasEntry(name), is(true));
	}

	AssertThat(unzipper.hasEntry("not-found"), is(false));
	AssertThat(unzipper.hasEntry("a/b/c/not-found"), is(false));

	std::set<long long> indices;

	for (auto expectedInfo : expectedEntryInfos) {
		ZipEntryInfo actualInfo {};
		unzipper.getEntryInfo(expectedInfo.name, actualInfo);

		// Zero out the index.. just check that all indices were returned.
		AssertThat(indices.find(actualInfo.index) == indices.end(), is(true));
		indices.insert(actualInfo.index);
		actualInfo.index = 0;
		actualInfo.modificationTime = std::chrono::system_clock::time_point();
		expectedInfo.modificationTime = std::chrono::system_clock::time_point();

		// Zero the encryption method in the expected if the test is not using encryption.
		if (pw.empty()) {
			expectedInfo.encryptionMethod = 0;
		}

		AssertThat(actualInfo, is(expectedInfo));
	}

	for (auto & name : expectedEntryNames) {
		AssertThat(unzipper.readEntryComment(name), is(""));

		if (!Strings::endsWith(name, "/")) {
			AssertThat(unzipper.readEntryAsBytes(name), is(expectedEntryBytes.at(name)));
			AssertThat(unzipper.readEntryAsString(name), is(expectedEntryStrings.at(name)));
		}
	}

	unzipper.close();

	AssertThat(unzipper.isOpen(), is(false));
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

const std::string archiveComment = "This is an archive comment."; // NOLINT
const std::string newDirectory = "q/w/e/r/"; // NOLINT
const std::string newEntryExistingDirectoryFromFile = "q/w/e/r/newEntry.zip"; // NOLINT
const std::string newEntryExistingDirectoryFromBytes = "q/w/e/r/newBytes"; // NOLINT
const std::string newEntryExistingDirectoryFromString = "q/w/e/r/newString"; // NOLINT
const std::vector<char> bytes {1, 2, 3, 4, 5, 6, 7, 8, 9}; // NOLINT
const std::string str = "qwertyuiop"; // NOLINT
const std::string newEntryExistingDirectoryFromStringRenamed = "q/w/e/r/newStringRenamed"; // NOLINT

void zipperTestImpl2(const Resource::File & file, bool commit, const std::string & pw) {
	Zipper zipper;
	zipper.open(file, true);

	AssertThat(zipper.isOpen(), is(true));
	AssertThat(zipper.readArchiveComment(), is(""));

	zipper.putArchiveComment(archiveComment);

	AssertThat(zipper.readArchiveComment(), is(archiveComment));

	zipper.deleteComment();

	AssertThat(zipper.readArchiveComment(), is(""));
	AssertThat(zipper.hasEntry(newDirectory), is(false));

	zipper.putDirectory(newDirectory);

	AssertThat(zipper.hasEntry(newDirectory), is(true));
	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromFile), is(false));

	zipper.putEntry(newEntryExistingDirectoryFromFile, zipFile);

	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromFile), is(true));
	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromBytes), is(false));

	zipper.putEntry(newEntryExistingDirectoryFromBytes, bytes);

	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromBytes), is(true));
	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromString), is(false));

	zipper.putEntry(newEntryExistingDirectoryFromString, str);

	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromString), is(true));

	zipper.renameEntry(newEntryExistingDirectoryFromString, newEntryExistingDirectoryFromStringRenamed);

	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromString), is(false));
	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromStringRenamed), is(true));

	zipper.deleteEntry(newEntryExistingDirectoryFromStringRenamed);

	AssertThat(zipper.hasEntry(newEntryExistingDirectoryFromStringRenamed), is(false));

	if (commit) {
		zipper.commit();

		// Check commit.

		Unzipper unzipper;
		unzipper.open(file, true);

		AssertThat(unzipper.hasEntry(newDirectory), is(true));
		AssertThat(unzipper.hasEntry(newEntryExistingDirectoryFromFile), is(true));
		AssertThat(unzipper.hasEntry(newEntryExistingDirectoryFromBytes), is(true));

		unzipper.close();

		AssertThat(unzipper.isOpen(), is(false));
	} else {
		zipper.close();
	}

	AssertThat(zipper.isOpen(), is(false));
}

void zipperTestImpl1(const Resource::File & srcFile, Resource::File dstFile, const std::string & pw) {
	// Remove any existing file from a previous partial run.
	dstFile.removeFile();

	// Remove the file created in this run on scope exit.
	OnScopeExit removeTestFile([&dstFile] () { dstFile.removeFile(); });

	dstFile.getParentDirectory().createDirectories();
	Files::copy(srcFile, dstFile);

	AssertThat(dstFile.exists(), is(true));

	auto md5Hash = Hashing::md5(dstFile);

	// First try with rollback.
	zipperTestImpl2(dstFile, false, pw);

	// Verify that the file is not modified.
	auto newMd5Hash = Hashing::md5(dstFile);

	AssertThat(md5Hash, is(newMd5Hash));

	// Then try with commit.
	zipperTestImpl2(dstFile, true, pw);

	newMd5Hash = Hashing::md5(dstFile);

	AssertThat(md5Hash, isNot(newMd5Hash));
}

void CompressionTest::zipperTest() {
	zipperTestImpl1(zipFile, mutatedZipFile, "");
}

void CompressionTest::encryptedZipperTest() {
	zipperTestImpl1(encryptedZipFile, mutatedEncryptedZipFile, testPassword);
}

void CompressionTest::libzipSoakTest() {
	const size_t iterationCount = 1;
	std::vector<std::thread> threads;

	for (size_t m = 0; m < 2; m++) {
		threads.emplace_back(
			[this] () {
				for (size_t m = 0; m < iterationCount; m++) {
					unzipperTest();
				}
			}
		);

	}

//	threads.emplace_back(
//		[this] () {
//			for (size_t m = 0; m < iterationCount; m++) {
//				unzipperTest();
//				zipperTest();
//			}
//		}
//	);
//
//	threads.emplace_back(
//		[this] () {
//			for (size_t m = 0; m < iterationCount; m++) {
//				encryptedUnzipperTest();
//			}
//		}
//	);
//
//	threads.emplace_back(
//		[this] () {
//			for (size_t m = 0; m < iterationCount; m++) {
//				encryptedUnzipperTest();
//				encryptedZipperTest();
//			}
//		}
//	);

	for (auto & thread : threads) {
		thread.join();
	}
}

static void unzipperTestImpl2() {
	int error = 0;
	zip_t * archive = zip_open("../../src/test/resources/Zips/ZipFile.zip", ZIP_RDONLY | ZIP_CHECKCONS, &error);
	if (error) { throw std::exception(); }
	zip_discard(archive);
}

void CompressionTest::libzipSoakTest2() {
	std::thread t1(unzipperTestImpl2);
	std::thread t2(unzipperTestImpl2);
	t1.join();
	t2.join();
}

static void unzipperTestImpl3() {
	const Resource::File zipFile = resDir / "Zips" / "ZipFile.zip";
	const auto path = zipFile.toRawString().c_str();
	int error = 0;
	zip_t * archive = zip_open(path, ZIP_RDONLY | ZIP_CHECKCONS, &error);
	if (error) { throw std::exception(); }
	struct zip_stat sb {};
	zip_stat_init(&sb);
	const long long count = zip_get_num_entries(archive, 0);

	if (count < 0) { throw std::exception(); }

	zip_discard(archive);
}

void CompressionTest::libzipSoakTest3() {
	std::thread t1([this] () { unzipperTestImpl2(); });
	std::thread t2([this] () { unzipperTestImpl2(); });

	t1.join();
	t2.join();
}

#endif // BALAU_LIBZIP_ENABLED

} // namespace Util

} // namespace Balau
