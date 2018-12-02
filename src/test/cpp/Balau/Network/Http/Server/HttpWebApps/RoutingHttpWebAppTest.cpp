// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "RoutingHttpWebAppTest.hpp"
#include "../../../../../TestResources.hpp"

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Type/OnScopeExit.hpp>
#include <Balau/Logging/Logger.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isGreaterThan;

namespace Network {

namespace Http {

namespace HttpWebApps {

void assertResponse(const CharVectorResponse & response, const Resource::File & filePath) {
	const auto & header = response.base();
	auto reason = header.reason();
	auto result = header.result();
	auto chunked = response.chunked();
	auto hasContentLength = response.has_content_length();
	auto keepAlive = response.keep_alive();
	auto needEof = response.need_eof();
	auto payloadSize = response.payload_size();
	auto version = response.version();

	assertThat(reason, is("OK"));
	assertThat(result, is(Status::ok));
	assertThat(chunked, is(false));
	assertThat(hasContentLength, is(true));
	assertThat(keepAlive, is(true));
	assertThat(needEof, is(false));
	assertThat(payloadSize.is_initialized(), is(true));
	assertThat(payloadSize.value(), isGreaterThan(0ULL));
	assertThat(version, is(11U));

	const std::vector<char> & actualBody = response.body();
	const std::vector<char> expectedBody = Util::Files::readToVector(filePath);

	assertThat(actualBody, is(expectedBody));
}

const unsigned short routingHttpWebAppTestPortStart = 43254;

Logger & logger = Logger::getLogger("TestMain"); // NOLINT

void RoutingHttpWebAppTest::test() {
	const auto documentRoot = TestResources::BalauSourceFolder / "doc";

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

			server->start();
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

} // namespace HttpWebApps

} // namespace Http

} // namespace Network

} // namespace Balau
