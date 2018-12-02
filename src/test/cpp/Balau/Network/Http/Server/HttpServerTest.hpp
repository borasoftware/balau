// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_TEST
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Network {

namespace Http {

struct HttpServerTest : public Testing::TestGroup<HttpServerTest> {
	explicit HttpServerTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&HttpServerTest::injectedInstantiation, "injectedInstantiation");
	}

	void injectedInstantiation();
};

} // namespace Http

} // namespace Network

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER__HTTP_SERVER_TEST
