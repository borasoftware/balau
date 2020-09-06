// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <TestResources.hpp>
#include <Balau/Network/Http/Client/HttpsClient.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::startsWith;
using Testing::throws;

namespace Network::Http {

struct HttpsClientTest : public Testing::TestGroup<HttpsClientTest> {
	HttpsClientTest() {
		RegisterTestCase(getRequest);
		RegisterTestCase(headRequest);
		RegisterTestCase(postRequest);
	}

	static void assertResponse(Response<CharVectorBody> & response,
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

	static void assertResponse(EmptyResponse & response, const char * reasonStr, Status status) {
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
		AssertThat(hasContentLength, is(false));
		AssertThat(keepAlive, is(true));
		AssertThat(needEof, is(false));
		AssertThat(version, is(11U));
	}

	void getRequest() {
		try {
			HttpsClient client("borasoftware.com");

			try {
				Response<CharVectorBody> response = client.get("/en/index.html");
				assertResponse(response, "<!DOCTYPE html", "OK", Status::ok);
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

	void headRequest() {
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

	void postRequest() {
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
};

} // namespace Network::Http

} // namespace Balau
