// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__HTTPS_CLIENT_TEST
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__HTTPS_CLIENT_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Network {

namespace Http {

struct HttpsClientTest : public Testing::TestGroup<HttpsClientTest> {
	explicit HttpsClientTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&HttpsClientTest::getRequest, "getRequest");
		registerTest(&HttpsClientTest::headRequest, "headRequest");
		registerTest(&HttpsClientTest::postRequest, "postRequest");
	}

	void getRequest();
	void headRequest();
	void postRequest();
};

} // namespace Http

} // namespace Network

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__HTTPS_CLIENT_TEST
