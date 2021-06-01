// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
		//RegisterTestCase(test);
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
