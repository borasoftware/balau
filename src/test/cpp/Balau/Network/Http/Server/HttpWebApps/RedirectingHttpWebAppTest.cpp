// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "RedirectingHttpWebAppTest.hpp"
#include "../../../../../TestResources.hpp"

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Type/OnScopeExit.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::isLessThan;

namespace Network::Http::HttpWebApps {

static void assertResponse(const CharVectorResponse & response, Status expectedStatus, const std::string & redirectionPath) {
	const auto & header = response.base();
	auto result = header.result();

	AssertThat(result, is(expectedStatus));
	AssertThat(response[Field::location], is(redirectionPath));
}

const size_t redirectingHttpWebAppTestPortStart = 13245;

void RedirectingHttpWebAppTest::injectedInstantiation() {
	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server] () {
			class Wiring : public ApplicationConfiguration {
				public: void configure() const override {
					bind<System::Clock>().toSingleton<System::SystemClock>();
					bind<HttpServer>().toSingleton();
				}
			};

			class EnvConfig : public EnvironmentConfiguration {
				public: EnvConfig(const std::shared_ptr<Resource::Uri> & env)
					: EnvironmentConfiguration({ env }) {}

				public: void configure() const override {
					value<bool>("http.server.register.signal.handler", false);

					group("http.server"
						, value<std::string>("logging.ns", "http.server")
						, value<std::string>("access.log", "stream: stdout")
						, value<std::string>("error.log", "stream: stderr")
						, value<std::string>("server.id", "Test Server")
						, value<int>("worker.count", 2)
						, value<Endpoint>("listen", makeEndpoint("127.0.0.1",Testing::NetworkTesting::getFreeTcpPort(redirectingHttpWebAppTestPortStart, 100)))
						, group("mime.types"
							, value<std::string>("text/html", "html")
						)
						, group("http"
							, group("redirections"
								, value<std::string>("location")
							)
						)
					);
				}

				private: const Resource::File documentRoot;
			};

			const auto env = std::shared_ptr<Resource::Uri>(
				new Resource::StringUri(
					R"(
						http.server {
							http {
								redirections {
									location = /redirect/

									matches {
										^/redirect/other/(.*)$     = 1 302 /test/other/$1
										^/redirect/(.*)/(.*)/(.*)$ = 2 302 /flip/$2/$1/$3
										^/redirect/(.*)$           = 3 301 /test/$1
									}
								}
							}
						}
					)"
				)
			);

			auto injector = Injector::create(Wiring(), EnvConfig(env));

			server = injector->getShared<HttpServer>();
			server->startAsync();
			return server->getPort();
		}
	);

	OnScopeExit stopServer([&server] () { server->stop(); });

	AssertThat(server->isRunning(), is(true));

	HttpClient client("localhost", port);

	// ^/redirect/other/(.*)$ = /test/other/$1
	assertResponse(client.get("/redirect/other/file.html"), Status::found, "/test/other/file.html");
	assertResponse(client.get("/redirect/other/indirect/file.html"), Status::found, "/test/other/indirect/file.html");

	// ^/redirect/(.*)/(.*)/(.*)$ = /flip/$2/$1/$3
	assertResponse(client.get("/redirect/first/second/file.html"), Status::found, "/flip/second/first/file.html");
	assertResponse(client.get("/redirect/third/fourth/file.html"), Status::found, "/flip/fourth/third/file.html");

	// ^/redirect/(.*)$ = 301 /test/$1
	assertResponse(client.get("/redirect/file.html"), Status::moved_permanently, "/test/file.html");
	assertResponse(client.get("/redirect/more/file.html"), Status::moved_permanently, "/test/more/file.html");
}

} // namespace Network::Http::HttpWebApps

} // namespace Balau
