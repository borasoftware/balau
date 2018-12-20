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
	const auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			class Wiring : public ApplicationConfiguration {
				public: void configure() const override {
					bind<System::Clock>().toSingleton<System::SystemClock>();
					bind<HttpServer>().toSingleton();
				}
			};

			class EnvConfig : public EnvironmentConfiguration {
				public: EnvConfig(const Resource::Uri & input) : EnvironmentConfiguration(input) {}

				public: void configure() const override {
					const unsigned short testPortStart = 43270;

					value<bool>("http.server.register.signal.handler", false);

					group("http.server"
						, value<std::string>("logging.ns", "http.server")
						, value<std::string>("access.log", "stream: stdout")
						, value<std::string>("error.log", "stream: stderr")
						, value<std::string>("server.id", "Test Server")
						, value<int>("worker.count", 2)
						, value<Endpoint>("listen", makeEndpoint("127.0.0.1", Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)))

						, group("mime.types"
							, value<std::string>("text/html", "html")
						)

						, group("http"
							, group("files"
								, value<std::string>("location")
								, value<std::string>("log.ns", "http.server.files")
								, value<std::string>("access.log", "stream: stdout")
								, value<std::string>("error.log", "stream: stderr")
								, unique<Resource::Uri>("root")
								, value<std::string>("index", "index.html")
							)
						)
					);
				}

				private: const Resource::File documentRoot;
			};

			const Resource::StringUri env(
				R"(
					# Main HTTP server environment configuration.
					http.server {
						http {
							files {
								location = /
								root = )" + documentRoot.toUriString() + R"(
							}
						}
					}
				)"
			);

			auto injector = Injector::create(Wiring(), EnvConfig(env));

			server = injector->getShared<HttpServer>();
			server->start();
			return server->getPort();
		}
	);

	OnScopeExit stopServer([&server] () { server->stop(); });

	assertThat(server->isRunning(), is(true));

	HttpClient client("localhost", port);

	std::string path = "/manual/index.bdml";
	Response<CharVectorBody> response = client.get(path);

	assertResponse(response, "OK", Status::ok, false, true);

	auto payloadSize = response.payload_size();

	assertThat(payloadSize.is_initialized(), is(true));
	assertThat(payloadSize.value(), isGreaterThan(0ULL));

	const std::vector<char> & actualBody = response.body();
	const std::vector<char> expectedBody = Util::Files::readToVector(documentRoot / path);

	assertThat(actualBody, is(expectedBody));

	std::string path2 = "/";
	Response<CharVectorBody> response2 = client.get(path2);

	assertThat(response2.base().result(), is(Status::not_found));
}

} // namespace Http

} // namespace Network

} // namespace Balau
