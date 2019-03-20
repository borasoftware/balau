// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpClientTest.hpp"

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Resource/Http.hpp>
#include <Balau/Resource/Https.hpp>
#include <Balau/Util/Vectors.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::startsWith;
using Testing::throws;

namespace Network {

namespace Http {

void HttpClientTest::getRequest() {
	HttpClient client("borasoftware.com");

	try {
		Response<CharVectorBody> response = client.get("/");
		const std::string expectedBody = "<html>\r\n<head><title>301 Moved Permanently</title></head>";
		assertResponse(response, expectedBody.c_str(), "Moved Permanently", Status::moved_permanently);
	} catch (const boost::system::system_error & e) {
		logLine(e.what());
		logLine(e.code().message());
		throw;
	}
}

void HttpClientTest::headRequest() {
	HttpClient client("borasoftware.com");

	try {
		Response<CharVectorBody> response = client.get("/");
		assertResponse(response, nullptr, "Moved Permanently", Status::moved_permanently);
	} catch (const boost::system::system_error & e) {
		logLine(e.what());
		logLine(e.code().message());
		throw;
	}
}

void HttpClientTest::postRequest() {
	HttpClient client("borasoftware.com");

	try {
		Response<CharVectorBody> response = client.post("/", "");
		const std::string expectedBody = "<html>\r\n<head><title>301 Moved Permanently</title></head>";
		assertResponse(response, expectedBody.c_str(), "Moved Permanently", Status::moved_permanently);
	} catch (const boost::system::system_error & e) {
		logLine(e.what());
		logLine(e.code().message());
		throw;
	}
}

void HttpClientTest::newClient() {
	Resource::Http http("http://borasoftware.com");
	auto httpClient1 = HttpClient::newClient(http);

	Resource::Https https("https://borasoftware.com");
	auto httpsClient1 = HttpClient::newClient(https);

	Resource::Http httpWithPort("http://borasoftware.com:80");
	auto httpClient2 = HttpClient::newClient(httpWithPort);

	Resource::Https httpsWithPort("https://borasoftware.com:443");
	auto httpsClient2 = HttpClient::newClient(httpsWithPort);

	AssertThat([] () { HttpClient::newClient("borasoftware.com"); }, throws<Exception::NetworkException>());
	AssertThat([] () { HttpClient::newClient("http://"); }, throws<Exception::NetworkException>());
	AssertThat([] () { HttpClient::newClient("http://:80"); }, throws<Exception::InvalidUriException>());
	AssertThat([] () { HttpClient::newClient("smtp://blah"); }, throws<Exception::NetworkException>());
}


void HttpClientTest::assertResponse(Response<CharVectorBody> & response,
                                    const char * bodyStart,
                                    const char * reasonStr,
                                    Status status) {
	auto header = response.base();
	auto reason = header.reason();
	auto result = header.result();
	auto chunked = response.chunked();
	auto hasContentLength = response.has_content_length();
	auto keepAlive = response.keep_alive();
	auto needEof = response.need_eof();
	auto version = response.version();

	AssertThat(reason, is(reasonStr));
	AssertThat(result, is(status));
	AssertThat(chunked, is(false));
	AssertThat(hasContentLength, is(true));
	AssertThat(keepAlive, is(true));
	AssertThat(needEof, is(false));
	AssertThat(version, is(11U));

	if (bodyStart != nullptr) {
		auto payloadSize = response.payload_size();

		AssertThat(payloadSize.is_initialized(), is(true));
		AssertThat(payloadSize.value(), isGreaterThan(0U));

		const std::vector<char> & actualBody = response.body();

		AssertThat(actualBody, startsWith(Util::Vectors::toCharVector(bodyStart)));
	}
}

} // namespace Http

} // namespace Network

} // namespace Balau
