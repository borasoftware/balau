// @formatter:off
//
// Balau core C++ library
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

#include <Balau/Network/Http/Client/WsClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/FailingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/WsWebApps/EchoingWsWebApp.hpp>
#include <Balau/Network/Http/Server/WsWebApps/RoutingWsWebApp.hpp>

namespace Balau {

using Testing::is;

namespace Network::Http::WsWebApps {

struct EchoingWsWebAppTest : public Testing::TestGroup<EchoingWsWebAppTest> {
	EchoingWsWebAppTest() {
		// WIP RegisterTest(test);
	}

	void test() {
		const unsigned short testPortStart = 47291;
		auto documentRoot = TestResources::SourceFolder / "doc";

		HttpWebApps::RoutingHttpWebApp::Routing httpRouting(HttpWebApps::routingNode<HttpWebApps::FailingHttpWebApp>(""));

		RoutingWsWebApp::Routing wsRouting(WsWebApps::routingNode<WsWebApps::EchoingWsWebApp>(""));

		std::shared_ptr<HttpServer> server;
		auto httpHandler = std::shared_ptr<HttpWebApp>(new HttpWebApps::RoutingHttpWebApp(std::move(httpRouting)));
		auto wsHandler = std::shared_ptr<WsWebApp>(new WsWebApps::RoutingWsWebApp(std::move(wsRouting)));

		const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
			[&server, &httpHandler, &wsHandler] () {
				auto endpoint = makeEndpoint(
					"127.0.0.1", Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
				);

				auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

				server = std::shared_ptr<HttpServer>(
					new HttpServer(clock, "BalauTest", endpoint, "RoutingHandler", 4, httpHandler, wsHandler)
				);

				server->startAsync();
				return server->getPort();
			}
		);

		WsClient client("localhost", port);

		const std::string path = "/";
		const std::string testData = "qwertyuiopasdfghjklzxcvbnm";

		auto data = boost::asio::buffer(std::string(testData));

		auto response = client.send<Buffer>(path, data);

		AssertThat(std::string_view((char *) response.data().data(), response.size()), is(testData));
	}
};

} // namespace Network::Http::WsWebApps

} // namespace Balau
