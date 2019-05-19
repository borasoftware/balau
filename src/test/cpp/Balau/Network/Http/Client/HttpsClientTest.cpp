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

namespace Balau::Network::Http {

void HttpsClientTest::getRequest() {
	try {
		HttpsClient client("borasoftware.com");

		try {
			Response<CharVectorBody> response = client.get("/test/testfile.html");
			HttpClientTest::assertResponse(response, "<!DOCTYPE html", "OK", Status::ok);
		} catch (const boost::system::system_error & e) {
			logLine(e.what());
			logLine(e.code().message());
			throw;
		}
	} catch (const boost::system::system_error & e) {
		if (e.code() == boost::system::errc::device_or_resource_busy) {
			// Ignore due to no network available.
			ignore();
		} else {
			throw;
		}
	}
}

void HttpsClientTest::headRequest() {
	// TODO finish
	ignore();
	return;

//	try {
//		HttpsClient client("borasoftware.com");
//
//		try {
//			EmptyResponse response = client.head("/test/testfile.html");
//			HttpClientTest::assertResponse(response, "OK", Status::ok);
//		} catch (const boost::system::system_error & e) {
//			logLine(e.what());
//			logLine(e.code().message());
//			throw;
//		}
//	} catch (const boost::system::system_error & e) {
//		if (e.code() == boost::system::errc::device_or_resource_busy) {
//			// Ignore due to no network available.
//			ignore();
//		} else {
//			throw;
//		}
//	}
}

void HttpsClientTest::postRequest() {
	try {
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
	} catch (const boost::system::system_error & e) {
		if (e.code() == boost::system::errc::device_or_resource_busy) {
			// Ignore due to no network available.
			ignore();
		} else {
			throw;
		}
	}
}

} // namespace Balau::Network::Http
