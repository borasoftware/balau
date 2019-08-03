// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__ZIP_TEST
#define COM_BORA_SOFTWARE__BALAU_UTIL__ZIP_TEST

#include <Balau/Util/Zip.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Util {

struct CompressionTest : public Testing::TestGroup<CompressionTest> {
	CompressionTest() {
		registerTest(&CompressionTest::fingerprintTest,       "fingerprintTest");
		registerTest(&CompressionTest::unzipperTest,          "unzipperTest");
		registerTest(&CompressionTest::encryptedUnzipperTest, "encryptedUnzipperTest");
		registerTest(&CompressionTest::zipperTest,            "zipperTest");
		registerTest(&CompressionTest::encryptedZipperTest,   "encryptedZipperTest");
		registerTest(&CompressionTest::libzipSoakTest,        "libzipSoakTest");
		registerTest(&CompressionTest::libzipSoakTest2,       "libzipSoakTest2");
		registerTest(&CompressionTest::libzipSoakTest3,       "libzipSoakTest3");
	}

	void fingerprintTest();
	void unzipperTest();
	void encryptedUnzipperTest();
	void zipperTest();
	void encryptedZipperTest();
	void libzipSoakTest();
	void libzipSoakTest2();
	void libzipSoakTest3();
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__ZIP_TEST