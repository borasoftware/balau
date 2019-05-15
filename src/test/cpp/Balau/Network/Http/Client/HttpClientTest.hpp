// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__HTTP_CLIENT_TEST
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__HTTP_CLIENT_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Network::Http {

struct HttpClientTest : public Testing::TestGroup<HttpClientTest> {
	HttpClientTest() {
		registerTest(&HttpClientTest::getRequest, "getRequest");
		registerTest(&HttpClientTest::headRequest, "headRequest");
		registerTest(&HttpClientTest::postRequest, "postRequest");
		registerTest(&HttpClientTest::newClient,   "newClient");
	}

	void getRequest();
	void headRequest();
	void postRequest();
	void newClient();

	static void assertResponse(Response<CharVectorBody> & response, const char * bodyStart, const char * reasonStr, Status status);
	static void assertResponse(EmptyResponse & response, const char * reasonStr, Status status);
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__HTTP_CLIENT_TEST
