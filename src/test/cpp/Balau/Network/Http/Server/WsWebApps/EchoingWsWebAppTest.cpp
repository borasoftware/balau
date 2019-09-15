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
		// WIP registerTest(&EchoingWsWebAppTest::test, "test");
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
