// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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
#include <Balau/Resource/UriComponents.hpp>
#include <TestResources.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::throws;

namespace Resource {

struct UriComponentsTest : public Testing::TestGroup<UriComponentsTest> {
	UriComponentsTest() {
		RegisterTest(uriParse_full);
		RegisterTest(uriParse_noUserinfo);
		RegisterTest(uriParse_noAuthority);
		RegisterTest(uriParse_noPort);
		RegisterTest(uriParse_noQuery);
		RegisterTest(uriParse_noFragment);
		RegisterTest(uriParse_noQueryNoFragment);
		RegisterTest(uriParse_noPortNoPathNoQueryNoFragment);
		RegisterTest(uriParse_noPortNoPathNoFragment);
		RegisterTest(uriParse_noPortNoPathNoQuery);
		RegisterTest(uriParse_noHostNoPortNoPathNoQuery);
		RegisterTest(uriParse_ipv6Host);
		RegisterTest(uriParse_ipv6HostNoPort);
		RegisterTest(uriParse_ipv6HostEmptyPath);
		RegisterTest(uriParse_ipv6HostNoPortNoPathNoQueryNoFragment);
		RegisterTest(uriParse_ipv4Host);
		RegisterTest(uriParse_ipv4HostNoPort);
		RegisterTest(uriParse_ipv4HostEmptyPath);
		RegisterTest(uriParse_ipv4HostNoPortNoPathNoQueryNoFragment);
		RegisterTest(uriParse_mailto);
		RegisterTest(uriParse_full1000);
		RegisterTest(uriParse_invalidUris);
		RegisterTest(uriParse_RCF3986Example1);
		RegisterTest(uriParse_RCF3986Example2);
		RegisterTest(uriParse_RCF3986Example3);
		RegisterTest(uriParse_RCF3986Example4);
		RegisterTest(uriParse_RCF3986Example5);
		RegisterTest(uriParse_RCF3986Example6);
		RegisterTest(uriParse_RCF3986Example7);
		RegisterTest(uriParse_RCF3986Example8);

		RegisterTest(normalizePath);
	}

	static void uriComponentsAssertions(const UriComponents & actual,
	                                    const UriComponents & expected,
	                                    bool hasAuthority,
	                                    bool hasUserInfo,
	                                    bool hasHost,
	                                    bool hasPort,
	                                    bool hasQuery,
	                                    bool hasFragment,
	                                    const std::string & expectedScheme,
	                                    const std::string & expectedUserInfo,
	                                    const std::string & expectedHost,
	                                    const std::string & expectedPath,
	                                    const std::string & expectedQuery,
	                                    const std::string & expectedFragment) {
		const std::string_view scheme   = actual.scheme();
		const std::string_view userInfo = actual.userInfo();
		const std::string_view host     = actual.host();
		const unsigned short port       = actual.port();
		const std::string_view path     = actual.path();
		const std::string_view query    = actual.query();
		const std::string_view fragment = actual.fragment();

		// Private field assertions.
		AssertThat("uri",            actual.uri,            is(expected.uri));
		AssertThat("userinfoOffset", actual.userinfoOffset, is(expected.userinfoOffset));
		AssertThat("hostOffset",     actual.hostOffset,     is(expected.hostOffset));
		AssertThat("portOffset",     actual.portOffset,     is(expected.portOffset));
		AssertThat("pathOffset",     actual.pathOffset,     is(expected.pathOffset));
		AssertThat("queryOffset",    actual.queryOffset,    is(expected.queryOffset));
		AssertThat("fragmentOffset", actual.fragmentOffset, is(expected.fragmentOffset));

		// Public API assertions.
		AssertThat("hasAuthority", actual.hasAuthority(), is(hasAuthority));
		AssertThat("hasUserInfo",  actual.hasUserInfo(),  is(hasUserInfo));
		AssertThat("hasHost",      actual.hasHost(),      is(hasHost));
		AssertThat("hasPort",      actual.hasPort(),      is(hasPort));
		AssertThat("hasQuery",     actual.hasQuery(),     is(hasQuery));
		AssertThat("hasFragment",  actual.hasFragment(),  is(hasFragment));

		AssertThat("scheme",   scheme,   is(expectedScheme));
		AssertThat("userInfo", userInfo, is(expectedUserInfo));
		AssertThat("host",     host,     is(expectedHost));
		AssertThat("port",     port,     is(expected.port()));
		AssertThat("path",     path,     is(expectedPath));
		AssertThat("query",    query,    is(expectedQuery));
		AssertThat("fragment", fragment, is(expectedFragment));
	}

	///////////////////////////////////////////////////////////////////////////////

	void uriParse_full() {
		const std::string http = "http://usr1@localhost:12345/path/to/doc?query-string#frag-string";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 12, 22, 27, 40, 53, 12345 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, true, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr1", "localhost", "/path/to/doc", "query-string", "frag-string"
		);
	}

	void uriParse_noUserinfo() {
		const std::string http = "http://localhost:12345/path/to/doc?query-string#frag-string";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, 17, 22, 35, 48, 12345 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, true, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "localhost", "/path/to/doc", "query-string", "frag-string"
		);
	}

	void uriParse_noAuthority() {
		const std::string http = "http:/path/to/doc?query-string#frag-string";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, -1, -1, 5, 18, 31, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "", "/path/to/doc", "query-string", "frag-string"
		);
	}

	void uriParse_noPort() {
		const std::string http = "http://localhost/path/to/doc?query-string#frag-string";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 16, 29, 42, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "localhost", "/path/to/doc", "query-string", "frag-string"
		);
	}

	void uriParse_noQuery() {
		const std::string http = "http://localhost/path/to/doc#frag-string";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 16, -1, 29, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "localhost", "/path/to/doc", "", "frag-string"
		);
	}

	void uriParse_noFragment() {
		const std::string http = "http://localhost/path/to/doc?query-string";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 16, 29, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, true, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "localhost", "/path/to/doc", "query-string", ""
		);
	}

	void uriParse_noQueryNoFragment() {
		const std::string http = "http://localhost/path/to/doc.html";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 16, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "localhost", "/path/to/doc.html", "", ""
		);
	}

	void uriParse_noPortNoPathNoQueryNoFragment() {
		const std::string http = "http://borasoftware.com";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 23, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "borasoftware.com", "", "", ""
		);
	}

	void uriParse_noPortNoPathNoFragment() {
		const std::string http = "http://borasoftware.com?blah";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 23, 24, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, true, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "borasoftware.com", "", "blah", ""
		);
	}

	void uriParse_noPortNoPathNoQuery() {
		const std::string http = "http://borasoftware.com#blah";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 23, -1, 24, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "borasoftware.com", "", "", "blah"
		);
	}

	void uriParse_noHostNoPortNoPathNoQuery() {
		const std::string http = "http://";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, -1, -1, 7, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "", "", "", ""
		);
	}

	void uriParse_ipv6Host() {
		const std::string http = "http://usr123@[1::1]:12345/a/path?query-string#fragment";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 14, 21, 26, 34, 47, 12345 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, true, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr123", "[1::1]", "/a/path", "query-string", "fragment"
		);
	}

	void uriParse_ipv6HostNoPort() {
		const std::string http = "http://usr123@[1::1]/a/path?query-string#fragment";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 14, -1, 20, 28, 41, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, false, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr123", "[1::1]", "/a/path", "query-string", "fragment"
		);
	}

	void uriParse_ipv6HostEmptyPath() {
		const std::string http = "http://usr123@[1::1]:12345?query-string#fragment";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 14, 21, 26, 27, 40, 12345 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, true, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr123", "[1::1]", "", "query-string", "fragment"
		);
	}

	void uriParse_ipv6HostNoPortNoPathNoQueryNoFragment() {
		const std::string http = "http://[1::1]";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 13, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "[1::1]", "", "", ""
		);
	}

	void uriParse_ipv4Host() {
		const std::string http = "http://usr123@123.45.67.89:12345/a/path?query-string#fragment";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 14, 27, 32, 40, 53, 12345 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, true, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr123", "123.45.67.89", "/a/path", "query-string", "fragment"
		);
	}

	void uriParse_ipv4HostNoPort() {
		const std::string http = "http://usr123@123.45.67.89/a/path?query-string#fragment";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 14, -1, 26, 34, 47, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, false, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr123", "123.45.67.89", "/a/path", "query-string", "fragment"
		);
	}

	void uriParse_ipv4HostEmptyPath() {
		const std::string http = "http://usr123@123.45.67.89:12345?query-string#fragment";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, 7, 14, 27, 32, 33, 46, 12345 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, true, true, true, true, true
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "usr123", "123.45.67.89", "", "query-string", "fragment"
		);
	}

	void uriParse_ipv4HostNoPortNoPathNoQueryNoFragment() {
		const std::string http = "http://123.45.67.89";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 4, -1, 7, -1, 19, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "123.45.67.89", "", "", ""
		);
	}

	void uriParse_mailto() {
		const std::string http = "mailto:someone@example.com?subject=This%20is%20the%20subject&cc=someone_else@example.com&body=This%20is%20the%20body";

		uriComponentsAssertions(
			UriComponents(http)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { http, 6, -1, -1, -1, 7, 27, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, true, false
			// scheme, userInfo, host, port, path, query, fragment
			, "mailto", "", "", "someone@example.com", "subject=This%20is%20the%20subject&cc=someone_else@example.com&body=This%20is%20the%20body", ""
		);
	}

	void uriParse_full1000() {
		const std::string http = "http://usr1@localhost:12345/path/to/doc?query-string#frag-string";
		size_t sum = 0U;

		for (size_t m = 0U; m < 1000U; m++) {
			UriComponents actual(http);
			sum += actual.port();
		}

		AssertThat(sum, is(1000U * UriComponents(http).port()));
	}

	void uriParse_invalidUris() {
		AssertThat([] () { UriComponents(""); }, throws<Exception::InvalidUriException>());
		AssertThat([] () { UriComponents("0http://example.com"); }, throws<Exception::InvalidUriException>());
		AssertThat([] () { UriComponents("h$$p://example.com"); }, throws<Exception::InvalidUriException>());
		AssertThat([] () { UriComponents("https://[1::3"); }, throws<Exception::InvalidUriException>());
		AssertThat([] () { UriComponents("https://usr:password@example.com"); }, throws<Exception::InvalidUriException>());
		AssertThat([] () { UriComponents("http://:80"); }, throws<Exception::InvalidUriException>());
	}

	void uriParse_RCF3986Example1() {
		const std::string r = "ftp://ftp.is.co.za/rfc/rfc1808.txt";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 3, -1, 6, -1, 18, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "ftp", "", "ftp.is.co.za", "/rfc/rfc1808.txt", "", ""
		);
	}

	void uriParse_RCF3986Example2() {
		const std::string r = "http://www.ietf.org/rfc/rfc2396.txt";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 4, -1, 7, -1, 19, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "http", "", "www.ietf.org", "/rfc/rfc2396.txt", "", ""
		);
	}

	void uriParse_RCF3986Example3() {
		const std::string r = "ldap://[2001:db8::7]/c=GB?objectClass?one";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 4, -1, 7, -1, 20, 26, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, false, true, false
			// scheme, userInfo, host, port, path, query, fragment
			, "ldap", "", "[2001:db8::7]", "/c=GB", "objectClass?one", ""
		);
	}

	void uriParse_RCF3986Example4() {
		const std::string r = "mailto:John.Doe@example.com";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 6, -1, -1, -1, 7, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "mailto", "", "", "John.Doe@example.com", "", ""
		);
	}

	void uriParse_RCF3986Example5() {
		const std::string r = "news:comp.infosystems.www.servers.unix";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 4, -1, -1, -1, 5, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "news", "", "", "comp.infosystems.www.servers.unix", "", ""
		);
	}

	void uriParse_RCF3986Example6() {
		const std::string r = "tel:+1-816-555-1212";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 3, -1, -1, -1, 4, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "tel", "", "", "+1-816-555-1212", "", ""
		);
	}

	void uriParse_RCF3986Example7() {
		const std::string r = "telnet://192.0.2.16:80/";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 6, -1, 9, 20, 22, -1, -1, 80 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, true, false, true, true, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "telnet", "", "192.0.2.16", "/", "", ""
		);
	}

	void uriParse_RCF3986Example8() {
		const std::string r = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";

		uriComponentsAssertions(
			UriComponents(r)
			// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
			, UriComponents { r, 3, -1, -1, -1, 4, -1, -1, 0 }
			// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
			, false, false, false, false, false, false
			// scheme, userInfo, host, port, path, query, fragment
			, "urn", "", "", "oasis:names:specification:docbook:dtd:xml:4.1.2", "", ""
		);
	}

	void normalizePath() {
		AssertThat(UriComponents::normalizePath(""),                                       is(""));
		AssertThat(UriComponents::normalizePath("one"),                                    is("one"));
		AssertThat(UriComponents::normalizePath("one/two"),                                is("one/two"));
		AssertThat(UriComponents::normalizePath("one/../two"),                             is("two"));
		AssertThat(UriComponents::normalizePath("one/../two/.."),                          is(""));
		AssertThat(UriComponents::normalizePath("one/two/../.."),                          is(""));
		AssertThat(UriComponents::normalizePath("one/two/three/four/../../../five"),       is("one/five"));
		AssertThat(UriComponents::normalizePath("one/two/../three/four/five/../../six"),   is("one/three/six"));

	    AssertThat(UriComponents::normalizePath("/"),                                      is("/"));
		AssertThat(UriComponents::normalizePath("/one"),                                   is("/one"));
		AssertThat(UriComponents::normalizePath("/one/two"),                               is("/one/two"));
		AssertThat(UriComponents::normalizePath("/one/../two"),                            is("/two"));
		AssertThat(UriComponents::normalizePath("/one/../two/.."),                         is("/"));
		AssertThat(UriComponents::normalizePath("/one/two/../.."),                         is("/"));
		AssertThat(UriComponents::normalizePath("/one/two/three/four/../../../five"),      is("/one/five"));
		AssertThat(UriComponents::normalizePath("/one/two/../three/four/five/../../six"),  is("/one/three/six"));

		AssertThat(UriComponents::normalizePath("/one/"),                                  is("/one"));
		AssertThat(UriComponents::normalizePath("/one/two/"),                              is("/one/two"));
		AssertThat(UriComponents::normalizePath("/one/../two/"),                           is("/two"));
		AssertThat(UriComponents::normalizePath("/one/../two/../"),                        is("/"));
		AssertThat(UriComponents::normalizePath("/one/two/../../"),                        is("/"));
		AssertThat(UriComponents::normalizePath("/one/two/three/four/../../../five/"),     is("/one/five"));
		AssertThat(UriComponents::normalizePath("/one/two/../three/four/five/../../six/"), is("/one/three/six"));
	}
};

} // namespace Resource

} // namespace Balau
