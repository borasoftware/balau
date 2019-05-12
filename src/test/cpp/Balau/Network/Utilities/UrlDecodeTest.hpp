// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_DECODE_TEST
#define COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_DECODE_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Network {

struct UrlDecodeTest : public Testing::TestGroup<UrlDecodeTest> {
	UrlDecodeTest() {
		registerTest(&UrlDecodeTest::decodeTest, "decodeTest");
		registerTest(&UrlDecodeTest::splitAndDecodeTest, "splitAndDecodeTest");
	}

	void decodeTest();
	void splitAndDecodeTest();
};

} // namespace Balau::Network

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_DECODE_TEST
