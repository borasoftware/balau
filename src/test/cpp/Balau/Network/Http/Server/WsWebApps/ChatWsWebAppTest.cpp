// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <TestResources.hpp>

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Type/OnScopeExit.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::isLessThan;

namespace Network::Http::WsWebApps {

struct ChatWsWebAppTest : public Testing::TestGroup<ChatWsWebAppTest> {
	ChatWsWebAppTest() {
		// Work in progress.
		//registerTest(&ChatWsWebAppTest::test, "test");
	}

	void test() {
		// Work in progress.

	//	const unsigned short testPortStart = 43340;
	//
	//	std::shared_ptr<HttpServer> server;
	//
	//	std::shared_ptr<HttpWebApp> httpHandler;
	//	std::shared_ptr<WsWebApp> wsHandler;
	//
	//	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
	//		[&server, &httpHandler, &wsHandler, testPortStart] () {
	//			auto endpoint = makeEndpoint(
	//				"127.0.0.1", Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
	//			);
	//
	//			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());
	//
	//			server = std::shared_ptr<HttpServer>(
	//				new HttpServer(clock, "BalauTest", endpoint, "WsTestHandler", 4, httpHandler, wsHandler)
	//			);
	//
	//			server->startAsync();
	//			return server->getPort();
	//		}
	//	);
	}
};

} // namespace Network::Http::WsWebApps

} // namespace Balau
