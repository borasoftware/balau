// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpServerTest.hpp"
#include "../../../../TestResources.hpp"

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/WsWebApps/NullWsWebApp.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/System/SystemClock.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Type/OnScopeExit.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isGreaterThan;

namespace Network {

namespace Http {

template <typename ResponseT> void assertResponse(const ResponseT & response,
                                                  const char * expectedReason,
                                                  Status expectedStatus,
                                                  bool expectedNeedEof,
                                                  bool expectedHasContentLength) {
	const auto & header = response.base();
	auto reason = header.reason();
	auto result = header.result();
	auto chunked = response.chunked();
	auto hasContentLength = response.has_content_length();
	auto keepAlive = response.keep_alive();
	auto needEof = response.need_eof();
	auto version = response.version();

	assertThat(reason, is(expectedReason));
	assertThat(result, is(expectedStatus));
	assertThat(chunked, is(false));
	assertThat(hasContentLength, is(expectedHasContentLength));
	assertThat(keepAlive, is(true));
	assertThat(needEof, is(expectedNeedEof));
	assertThat(version, is(11U));
}

void HttpServerTest::injectedInstantiation() {
	const Resource::File documentRoot = TestResources::BalauSourceFolder / "doc";
	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			class Wiring : public ApplicationConfiguration {
				public: Wiring(Resource::File documentRoot_) : documentRoot(documentRoot_) {}

				public: void configure() const override {
					const unsigned short testPortStart = 43270;

					bind<System::Clock>().toSingleton<System::SystemClock>();

					bind<std::string>("httpServerIdentification").toValue("Balau Test");
					bind<std::string>("httpServerLoggingNamespace").toValue("balau.server");

					bind<TCP::endpoint>("httpServerEndpoint").toValue(
						makeEndpoint(
							"127.0.0.1", Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
						)
					);

					bind<std::string>("httpServerThreadName").toValue("HttpTest");
					bind<size_t>("httpServerWorkerCount").toValue(2U);

					bind<HttpWebApp>("httpHandler").toSingleton(new HttpWebApps::FileServingHttpWebApp(documentRoot));
					bind<WsWebApp>("webSocketHandler").toSingleton(new WsWebApps::NullWsWebApp());
					bind<MimeTypes>("mimeTypes").toSingleton(MimeTypes::defaultMimeTypes);

					bind<HttpServer>().toSingleton();
				}

				private: const Resource::File documentRoot;
			};

			auto injector = Injector::create(Wiring(documentRoot));

			server = injector->getShared<HttpServer>();
			server->start();
			return server->getPort();
		}
	);

	OnScopeExit stopServer([&server] () { server->stop(); });

	assertThat(server->isRunning(), is(true));

	HttpClient client("localhost", port);

	const std::string path = "/manual/index.bdml";
	Response<CharVectorBody> response = client.get(path);

	assertResponse(response, "OK", Status::ok, false, true);

	auto payloadSize = response.payload_size();

	assertThat(payloadSize.is_initialized(), is(true));
	assertThat(payloadSize.value(), isGreaterThan(0ULL));

	const std::vector<char> & actualBody = response.body();
	const std::vector<char> expectedBody = Util::Files::readToVector(documentRoot / path);

	assertThat(actualBody, is(expectedBody));
}

} // namespace Http

} // namespace Network

} // namespace Balau
