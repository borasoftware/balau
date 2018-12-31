// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__ECHO_WS_WEB_APP_TEST
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__ECHO_WS_WEB_APP_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Network::Http::WsWebApps {

struct EchoingWsWebAppTest : public Testing::TestGroup<EchoingWsWebAppTest> {
	explicit EchoingWsWebAppTest(Testing::TestRunner & runner) : TestGroup(runner) {
		// WIP registerTest(&EchoingWsWebAppTest::test, "test");
	}

	void test();
};

} // namespace Balau::Network::Http::WsWebApps

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__ECHO_WS_WEB_APP_TEST
