// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileServingHttpWebAppTest.hpp"
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

	AssertThat(reason, is(expectedReason));
	AssertThat(result, is(expectedStatus));
	AssertThat(chunked, is(false));
	AssertThat(hasContentLength, is(expectedHasContentLength));
	AssertThat(keepAlive, is(true));
	AssertThat(needEof, is(expectedNeedEof));
	AssertThat(version, is(11U));
}

void FileServingHttpWebAppTest::getFile() {
	const unsigned short testPortStart = 43240;
	auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1", Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock, "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	HttpClient client("localhost", port);
	const std::string path = "/manual/index.bdml";
	Response<CharVectorBody> response = client.get(path);

	assertResponse(response, "OK", Status::ok, false, true);

	auto payloadSize = response.payload_size();

	AssertThat(payloadSize.is_initialized(), is(true));
	AssertThat(payloadSize.value(), isGreaterThan(10000ULL));

	const std::vector<char> & actualBody = response.body();
	const std::vector<char> expectedBody = Util::Files::readToVector(documentRoot / path);

	AssertThat(actualBody, is(expectedBody));
}


void FileServingHttpWebAppTest::getNotFoundFile() {
	const unsigned short testPortStart = 47241;
	auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock, "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	HttpClient client("localhost", port);

	const std::string path = "/manual/_--blah--_.html";
	Response<CharVectorBody> response = client.get(path);

	assertResponse(response, "Not Found", Status::not_found, false, true);
}

void FileServingHttpWebAppTest::headFile() {
	const unsigned short testPortStart = 43242;
	auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock, "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	HttpClient client("localhost", port);

	const std::string path = "/manual/index.bdml";
	Response<EmptyBody> response = client.head(path);

	assertResponse(response, "OK", Status::ok, true, false);
}

void FileServingHttpWebAppTest::headNotFoundFile() {
	const unsigned short testPortStart = 43243;
	auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock, "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	HttpClient client("localhost", port);

	const std::string path = "/manual/_--blah--_.html";
	Response<EmptyBody> response = client.head(path);

	assertResponse(response, "Not Found", Status::not_found, true, false);
}

void FileServingHttpWebAppTest::getFile1000Serial() {
	const unsigned short testPortStart = 43244;
	auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock, "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	HttpClient client("localhost", port);
	size_t bytesTransferred = 0;

	for (size_t m = 0; m < 50; m++) {
		Response<CharVectorBody> response = client.get("/manual/index.bdml");

		assertResponse(response, "OK", Status::ok, false, true);
		AssertThat(response.payload_size().is_initialized(), is(true));
		AssertThat(response.payload_size().value(), isGreaterThan(10000ULL));

		bytesTransferred += response.payload_size().value();
	}

	logLine(
		::toString(
			  "FileServingHttpWebAppTest::getFile1000Serial - "
			, bytesTransferred
			, " bytes ("
			, Util::PrettyPrint::byteValue(bytesTransferred, 1)
			, ") transferred."
		)
	);
}

struct GetFile1000ParallelClientState {
	std::atomic_ullong bytesTransferred { 0 };
	unsigned short port {};
};

void getFile1000ParallelClientFunction(GetFile1000ParallelClientState * state) {
	HttpClient client("localhost", state->port);

	for (size_t m = 0; m < 100; m++) {
		Response<CharVectorBody> response = client.get("/manual/index.bdml");

		assertResponse(response, "OK", Status::ok, false, true);
		AssertThat(response.payload_size().is_initialized(), is(true));
		AssertThat(response.payload_size().value(), isGreaterThan(10000ULL));

		state->bytesTransferred += response.payload_size().value();
	}
}

void FileServingHttpWebAppTest::getFile1000Parallel() {
	const unsigned short testPortStart = 43245;
	auto documentRoot = TestResources::BalauSourceFolder / "doc";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock, "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	std::vector<std::thread> clientThreads;
	GetFile1000ParallelClientState state;
	state.port = port;
	const size_t threadCount = 10;

	for (size_t m = 0; m < threadCount; m++) {
		clientThreads.emplace_back(getFile1000ParallelClientFunction, &state);
	}

	for (size_t m = 0; m < threadCount; m++) {
		clientThreads[m].join();
	}

	logLine(
		::toString(
			"FileServingHttpWebAppTest::getFile1000Parallel - "
			, state.bytesTransferred
			, " bytes ("
			, Util::PrettyPrint::byteValue(state.bytesTransferred, 1)
			, ") transferred."
		)
	);
}

void FileServingHttpWebAppTest::getFiles() {
	const unsigned short testPortStart = 43546;
	auto documentRoot = TestResources::BalauSourceFolder / "doc" / "manual";

	std::shared_ptr<HttpServer> server;

	const unsigned short port = Testing::NetworkTesting::initialiseWithFreeTcpPort(
		[&server, documentRoot] () {
			auto endpoint = makeEndpoint(
				"127.0.0.1"
				, Testing::NetworkTesting::getFreeTcpPort(testPortStart, 50)
			);

			auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

			server = std::shared_ptr<HttpServer>(
				new HttpServer(clock,  "BalauTest", endpoint, "FileHandler", 4, documentRoot)
			);

			server->startAsync();
			return server->getPort();
		}
	);

	HttpClient client("localhost", port);

	size_t fileCount = 0;
	size_t bytesTransferred = 0;
	auto iterator = documentRoot.recursiveFileIterator();

	while (iterator.hasNext()) {
		auto file = iterator.next();

		if (file.isRegularFile()) {
			auto relativePath = file.relative(documentRoot);
			auto relativePathString = toString(relativePath);
			auto urlPath = "/" + relativePathString;

			Response<CharVectorBody> response = client.get(urlPath);

			assertResponse(response, "OK", Status::ok, false, true);
			AssertThat(response.payload_size().is_initialized(), is(true));

			++fileCount;
			bytesTransferred += response.payload_size().value();
		}
	}

	AssertThat(fileCount, isGreaterThan(38U));
	AssertThat(fileCount, isLessThan(100U));

	AssertThat(bytesTransferred, isGreaterThan(100000U));
	AssertThat(bytesTransferred, isLessThan(100000000U));
}

} // namespace Network::Http::HttpWebApps

} // namespace Balau
