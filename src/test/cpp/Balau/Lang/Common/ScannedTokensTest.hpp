// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LANG__SCANNED_TOKENS_TEST
#define COM_BORA_SOFTWARE__BALAU_LANG__SCANNED_TOKENS_TEST

#include "Balau/Lang/Common/ScannedTokens.hpp"
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Lang {

struct ScannedTokensTest : public Testing::TestGroup<ScannedTokensTest> {
	ScannedTokensTest() {
		registerTest(&ScannedTokensTest::randomAccessBuilding, "randomAccessBuilding");
		registerTest(&ScannedTokensTest::scannerApiConsumeAndPutBack, "scannerApiConsumeAndPutBack");
	}

	void randomAccessBuilding();
	void scannerApiConsumeAndPutBack();
};

} // namespace Lang

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_LANG__SCANNED_TOKENS_TEST
