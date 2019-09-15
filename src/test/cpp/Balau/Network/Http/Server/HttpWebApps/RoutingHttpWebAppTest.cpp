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
#include <Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Type/OnScopeExit.hpp>
#include <Balau/Logging/Logger.hpp>

namespace {

const unsigned short routingHttpWebAppTestPortStart = 43254;

} // namespace

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;

namespace Network::Http::HttpWebApps {

struct RoutingHttpWebAppTest : public Testing::TestGroup<RoutingHttpWebAppTest> {
	RoutingHttpWebAppTest() {
		registerTest(&RoutingHttpWebAppTest::test, "test");
	}

	static void assertResponse(const CharVectorResponse & response, const Resource::File & filePath) {
		const auto & header = response.base();
		auto reason = header.reason();
		auto result = header.result();
		auto chunked = response.chunked();
		auto hasContentLength = response.has_content_length();
		auto keepAlive = response.keep_alive();
		auto needEof = response.need_eof();
		auto payloadSize = response.payload_size();
		auto version = response.version();

		AssertThat(reason, is("OK"));
		AssertThat(result, is(Status::ok));
		AssertThat(chunked, is(false));
		AssertThat(hasContentLength, is(true));
		AssertThat(keepAlive, is(true));
		AssertThat(needEof, is(false));
		AssertThat(payloadSize.is_initialized(), is(true));
		AssertThat(payloadSize.value(), isGreaterThan(0ULL));
		AssertThat(version, is(11U));

		const std::vector<char> & actualBody = response.body();
		const std::vector<char> expectedBody = Util::Files::readToVector(filePath);

		AssertThat(actualBody, is(expectedBody));
	}

	Logger & logger = Logger::getLogger("TestMain"); // NOLINT

	void test() {
		const auto documentRoot = TestResources::SourceFolder / "doc";

		RoutingHttpWebApp::Routing routing;

		routing.add(routingNode<FileServingHttpWebApp>("manual", Resource::File(documentRoot)))
			.add(routingNode<FileServingHttpWebApp>("bdml",  Resource::File(documentRoot)));

		auto handler = std::shared_ptr<HttpWebApp>(new RoutingHttpWebApp(std::move(routing)));

		std::shared_ptr<HttpServer> server;

		const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
			[&server, &handler] () {
				auto endpoint = makeEndpoint(
					  "127.0.0.1"
					, Testing::NetworkTesting::getFreeTcpPort(routingHttpWebAppTestPortStart, 50)
				);

				auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

				server = std::shared_ptr<HttpServer>(
					new HttpServer(clock, "BalauTest", endpoint, "RoutingHandler", 4, handler)
				);

				server->startAsync();
				return server->getPort();
			}
		);

		HttpClient client("localhost", port);

		const std::string bdmlPath = "/manual/index.bdml";
		const std::string cssPath = "/bdml/css/bdml.css";

		auto bdmlResponse = client.get(bdmlPath);
		auto cssResponse = client.get(cssPath);

		assertResponse(bdmlResponse, documentRoot / bdmlPath);

		logger.info("Received bdml page: {}", bdmlPath);

		assertResponse(cssResponse, documentRoot / cssPath);

		logger.info("Received css page: {}", bdmlPath);
	}
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau
