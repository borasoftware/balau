// @formatter:off
//
// Balau core C++ library
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
#include <Balau/Resource/Http.hpp>
#include <Balau/Resource/Https.hpp>
#include <Balau/Util/Vectors.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::startsWith;
using Testing::throws;

namespace Network::Http {

struct HttpClientTest : public Testing::TestGroup<HttpClientTest> {
	HttpClientTest() {
		RegisterTest(getRequest);
		RegisterTest(headRequest);
		RegisterTest(postRequest);
		RegisterTest(newClient);
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

		AssertThat(reason.to_string(), is(reasonStr));
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

		AssertThat(reason.to_string(), is(reasonStr));
		AssertThat(result, is(status));
		AssertThat(chunked, is(false));
		AssertThat(hasContentLength, is(false));
		AssertThat(keepAlive, is(true));
		AssertThat(needEof, is(false));
		AssertThat(version, is(11U));
	}

	void getRequest() {
		try {
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
	//		HttpClient client("borasoftware.com");
	//
	//		try {
	//			Response<CharVectorBody> response = client.get("/");
	//			assertResponse(response, nullptr, "Moved Permanently", Status::moved_permanently);
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
		} catch (const boost::system::system_error & e) {
			if (e.code() == boost::system::errc::device_or_resource_busy) {
				// Ignore due to no network available.
				ignore();
			} else {
				throw;
			}
		}
	}

	void newClient() {
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
};

} // namespace Network::Http

} // namespace Balau
