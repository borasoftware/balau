// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "HttpsClientTest.hpp"
#include "HttpClientTest.hpp" // For assertions.

#include <Balau/Network/Http/Client/HttpsClient.hpp>

namespace Balau {

namespace Network {

namespace Http {

void HttpsClientTest::getRequest() {
	HttpsClient client("borasoftware.com");

	try {
		Response<CharVectorBody> response = client.get("/test/testfile.html");
		HttpClientTest::assertResponse(response, "<!DOCTYPE html", "OK", Status::ok);
	} catch (const boost::system::system_error & e) {
		logLine(e.what());
		logLine(e.code().message());
		throw;
	}
}

void HttpsClientTest::headRequest() {
	HttpsClient client("borasoftware.com");

	try {
		Response<CharVectorBody> response = client.get("/test/testfile.html");
		HttpClientTest::assertResponse(response, nullptr, "OK", Status::ok);
	} catch (const boost::system::system_error & e) {
		logLine(e.what());
		logLine(e.code().message());
		throw;
	}
}

void HttpsClientTest::postRequest() {
	HttpsClient client("borasoftware.com");

	try {
		//Response<CharVectorBody> response = client.post("/", ""); // TODO
		// TODO
		//HttpClientTest::assertResponse(response, "<html>", "Moved Permanently", Status::ok);
	} catch (const boost::system::system_error & e) {
		logLine(e.what());
		logLine(e.code().message());
		throw;
	}
}

} // namespace Http

} // namespace Network

} // namespace Balau
