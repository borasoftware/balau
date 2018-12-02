// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "EmailSendingHttpWebAppTest.hpp"
#include "../../../../../TestResources.hpp"

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/CannedHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Type/OnScopeExit.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isGreaterThan;
using Testing::startsWith;

namespace Network {

namespace Http {

namespace HttpWebApps {

void assertResponse(const CharVectorResponse & response) {
	const auto & header = response.base();
	auto result = header.result();
	auto reason = header.reason();
	auto chunked = response.chunked();
	auto hasContentLength = response.has_content_length();
	auto keepAlive = response.keep_alive();
	auto needEof = response.need_eof();
	auto version = response.version();

	std::cout << result << std::endl;
	std::cout << reason << std::endl;
	assertThat(chunked, is(false));
	assertThat(hasContentLength, is(true));
	assertThat(keepAlive, is(true));
	assertThat(needEof, is(false));
	assertThat(version, is(11U));

	auto payloadSize = response.payload_size();

	assertThat(payloadSize.is_initialized(), is(true));
	assertThat(payloadSize.value(), isGreaterThan(0ULL));

	const std::vector<char> & actualBody = response.body();

	assertThat(actualBody, is(Util::Vectors::toCharVector("<html><body><p>Ok</p></body></html>")));
}

const unsigned short emailSendingHttpWebAppTestPortStart = 43260;

// The logic in this function matches the parameters sent in the post request.
std::string createTestEmailBody(const EmailSendingHttpWebApp::ParameterMap & parameters) {
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

bool EmailSendingHttpWebAppTest::checkEnvironmentVariableSourcedString(const std::string & envVar,
                                                                       const std::string & value) {
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

void EmailSendingHttpWebAppTest::test() {
	if (!checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_HOST",            TestResources::BalauTestEmailHost)
	  || !checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_PORT",            TestResources::BalauTestEmailPort)
	  || !checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_SENDER_USER",     TestResources::BalauTestEmailSenderUser)
	  || !checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_SENDER_PASSWORD", TestResources::BalauTestEmailSenderPassword)
	  || !checkEnvironmentVariableSourcedString("BALAU_TEST_EMAIL_TO",              TestResources::BalauTestEmailTo)
	) {
		return;
	}

	auto documentRoot = Resource::File("/home/nicholas/Bora/PendingArea/website/borasoftware.com");

	auto cannedSuccess = std::shared_ptr<HttpWebApp>(
		new CannedHttpWebApp("text/html", "", "<html><body><p>Ok</p></body></html>")
	);

	auto cannedFailure = std::shared_ptr<HttpWebApp>(
		new CannedHttpWebApp("text/html", "", "<html><body><p>Error</p></body></html>")
	);

	unsigned short emailPort;
	fromString(emailPort, TestResources::BalauTestEmailPort);

	std::shared_ptr<HttpWebApp> emailHandler = std::shared_ptr<HttpWebApp>(
		new EmailSendingHttpWebApp(
			  createTestEmailBody
			, cannedSuccess
			, cannedFailure
			, TestResources::BalauTestEmailHost
			, emailPort
			, TestResources::BalauTestEmailSenderUser
			, TestResources::BalauTestEmailSenderPassword
			, "BalauTest"
			, "Test subject"
			, TestResources::BalauTestEmailTo
			, TestResources::BalauTestEmailTo
		)
	);

	RoutingHttpWebApp::Routing routing(routingNode<FileServingHttpWebApp>("", documentRoot));

	// The email post request handler at path "actions/contact".
	routing.add(
		  routingNode("actions")
		, RoutingHttpWebApp::Node::child(
			RoutingHttpWebApp::Value(
				  "contact"
				, emailHandler, emailHandler, emailHandler
			)
		)
	);

	std::shared_ptr<HttpServer> server;
	auto handler = std::shared_ptr<HttpWebApp>(new RoutingHttpWebApp(std::move(routing)));

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, &handler] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(emailSendingHttpWebAppTestPortStart, 50)
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
	const std::string pagesPath = "/actions/contact";
	const std::string body = "Name=TestName&Message=This is a test message.";

	auto response = client.post(pagesPath, body);

	assertResponse(response);
}

} // namespace HttpWebApps

} // namespace Http

} // namespace Network

} // namespace Balau
