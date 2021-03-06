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
#include <Balau/Network/Http/Server/HttpWebApps/CannedHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/FailingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Type/OnScopeExit.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::startsWith;

namespace Network::Http::HttpWebApps {

const unsigned short emailSendingHttpWebAppTestPortStart = 43260;
const unsigned short emailSendingHttpWebAppTestPortStart2 = 43280;

struct EmailSendingHttpWebAppTest : public Testing::TestGroup<EmailSendingHttpWebAppTest> {
	EmailSendingHttpWebAppTest() {
		RegisterTestCase(directInstantiation);
		RegisterTestCase(injectedInstantiation);
	}

	static void assertResponse(const CharVectorResponse & response, bool isRedirect) {
		const auto & header = response.base();
		auto result = header.result();
		//auto reason = header.reason();
		auto chunked = response.chunked();
		auto hasContentLength = response.has_content_length();
		auto keepAlive = response.keep_alive();
		auto needEof = response.need_eof();
		auto version = response.version();

		AssertThat(result, is(isRedirect ? boost::beast::http::status::found : boost::beast::http::status::ok));
		//std::cout << reason << std::endl;
		AssertThat(chunked, is(false));
		AssertThat(hasContentLength, is(!isRedirect));
		AssertThat(keepAlive, is(true));
		AssertThat(needEof, is(isRedirect));
		AssertThat(version, is(11U));

		if (!isRedirect) {
			auto payloadSize = response.payload_size();

			AssertThat(payloadSize.is_initialized(), is(true));
			AssertThat(payloadSize.value(), isGreaterThan(0ULL));

			const std::vector<char> & actualBody = response.body();

			AssertThat(actualBody, is(Util::Vectors::toCharVector("<html><body><p>Ok</p></body></html>")));
		}
	}

	// The logic in this function matches the parameters sent in the post request.
	static std::string createTestEmailBody(HttpSession & ,
	                                       const StringRequest & ,
	                                       std::map<std::string, std::string> & ,
	                                       const EmailSendingHttpWebApp::ParameterMap & parameters) {
		auto nameIter = parameters.find("Name");
		auto messageIter = parameters.find("Message");

		auto name = nameIter != parameters.end() ? nameIter->second : "";
		auto message = messageIter != parameters.end() ? messageIter->second : "";

		return "\r\n"
			"Name = " + ::toString(name) + "\r\n"
			"\r\n"
			"------- Message -------\r\n"
			"\r\n" +
			::toString(message) + "\r\n";
	}

	bool checkEnvironmentVariableSourcedString(const std::string & envVar, const std::string & value) {
		if (value.empty()) {
			logLine(
				::toString(
					"EmailSendingHttpWebAppTest::test: not running test because environment variable "
					, envVar
					, " is not set."
				)
			);

			return false;
		}

		return true;
	}

	bool checkEnvironmentVariables() {
		return checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_HOST",            TestResources::TestEmailHost)
			&& checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_PORT",            TestResources::TestEmailPort)
			&& checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_SENDER_USER",     TestResources::TestEmailSenderUser)
			&& checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_SENDER_PASSWORD", TestResources::TestEmailSenderPassword)
			&& checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_TO",              TestResources::TestEmailTo);
	}

	void directInstantiation() {
		if (!checkEnvironmentVariables()) {
			return;
		}

		auto cannedSuccess = std::shared_ptr<HttpWebApp>(
			new CannedHttpWebApp("text/html", "", "<html><body><p>Ok</p></body></html>")
		);

		auto cannedFailure = std::shared_ptr<HttpWebApp>(
			new CannedHttpWebApp("text/html", "", "<html><body><p>Error</p></body></html>")
		);

		unsigned short emailPort;
		fromString(emailPort, TestResources::TestEmailPort);

		std::shared_ptr<HttpWebApp> emailHandler = std::shared_ptr<HttpWebApp>(
			new EmailSendingHttpWebApp(
				  createTestEmailBody
				, cannedSuccess
				, cannedFailure
				, TestResources::TestEmailHost
				, emailPort
				, TestResources::TestEmailSenderUser
				, TestResources::TestEmailSenderPassword
				, "BalauTest"
				, "Test subject"
				, TestResources::TestEmailTo
				, TestResources::TestEmailTo
			)
		);

		RoutingHttpWebApp::Routing routing(routingNode<FailingHttpWebApp>(""));

		// The email post request handler at path "/1/send-message".
		routing.add(
			  routingNode("1")
			, RoutingHttpWebApp::Node::child(
				RoutingHttpWebApp::Value("send-message", emailHandler, emailHandler, emailHandler)
			)
		);

		std::shared_ptr<HttpServer> server;
		auto handler = std::shared_ptr<HttpWebApp>(new RoutingHttpWebApp(std::move(routing)));

		const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
			[&server, &handler] () {
				auto endpoint = makeEndpoint(
					"127.0.0.1", Testing::NetworkTesting::getFreeTcpPort(emailSendingHttpWebAppTestPortStart, 50)
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
		const std::string pagesPath = "/1/send-message";
		const std::string body = "Name=TestName&Message=This is a test message.";

		auto response = client.post(pagesPath, body);

		assertResponse(response, false);
	}

	void injectedInstantiation() {
		if (!checkEnvironmentVariables()) {
			return;
		}

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
					public: EnvConfig(const std::shared_ptr<Resource::Uri> & env, const std::shared_ptr<Resource::Uri> & creds)
						: EnvironmentConfiguration(std::vector<std::shared_ptr<Resource::Uri>> { env, creds }, {}) {}

					public: void configure() const override {
						value<bool>("http.server.register.signal.handler", false);

						group("http.server"
							, value<std::string>("logging.ns", "http.server")
							, value<std::string>("access.log", "stream: stdout")
							, value<std::string>("error.log", "stream: stderr")
							, value<std::string>("server.id", "Test Server"), value<int>("worker.count", 2)
							, value<Endpoint>("listen"
								, makeEndpoint("127.0.0.1",Testing::NetworkTesting::getFreeTcpPort(emailSendingHttpWebAppTestPortStart2, 50))
							)
							, group("mime.types"
								, value<std::string>("text/html", "html")
							)
							, group("http"
								, group("email.sender"
									, value<std::string>("location")
									, value<std::string>("log.ns", "http.server.email")
									, value<std::string>("info.log", "")
									, value<std::string>("error.log", "")
									, value<std::string>("host")
									, value<int>("port")
									, value<std::string>("user")
									, value<std::string>("password")
									, value<std::string>("subject")
									, value<std::string>("from")
									, value<std::string>("to")
									, value<std::string>("user-agent")
									, value<std::string>("success")
									, value<std::string>("failure")
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
									email.sender {
										location = /1/send-message
										host       = )" + TestResources::TestEmailHost + R"(
										port       = )" + TestResources::TestEmailPort + R"(
										user       = )" + TestResources::TestEmailSenderUser + R"(
										subject    = test message
										from       = )" + TestResources::TestEmailTo + R"(
										to         = )" + TestResources::TestEmailTo + R"(
										user-agent = BalauTest
										success    = /1/success.html
										failure    = /1/failed.html

										parameters {
											Name    = 1
											Email   = 2
											Message = 3
										}
									}
								}
							}
						)"
					)
				);

				const auto creds = std::shared_ptr<Resource::Uri>(
					new Resource::StringUri(
						R"(
							http.server {
								http {
									email.sender {
										password = )" + TestResources::TestEmailSenderPassword + R"(
									}
								}
							}
						)"
					)
				);

				auto injector = Injector::create(Wiring(), EnvConfig(env, creds));

				server = injector->getShared<HttpServer>();
				server->startAsync();
				return server->getPort();
			}
		);

		OnScopeExit stopServer([&server] () { server->stop(); });

		AssertThat(server->isRunning(), is(true));

		HttpClient client("localhost", port);
		const std::string pagesPath = "/1/send-message";
		const std::string body = "Name=TestName&Message=This is a test message.";

		auto response = client.post(pagesPath, body);

		assertResponse(response, true);
	}
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau
