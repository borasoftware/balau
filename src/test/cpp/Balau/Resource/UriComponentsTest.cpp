// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "UriComponentsTest.hpp"

#include <Balau/Resource/Http.hpp>

namespace Balau {

using Testing::is;
using Testing::isGreaterThan;
using Testing::throws;

namespace Resource {

void UriComponentsTest::uriComponentsAssertions(const UriComponents & actual,
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

void UriComponentsTest::uriParse_full() {
	const Http http("http://usr1@localhost:12345/path/to/doc?query-string#frag-string");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 12, 22, 27, 40, 53, 12345 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, true, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr1", "localhost", "/path/to/doc", "query-string", "frag-string"
	);
}

void UriComponentsTest::uriParse_noUserinfo() {
	const Http http("http://localhost:12345/path/to/doc?query-string#frag-string");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, 17, 22, 35, 48, 12345 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, true, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "localhost", "/path/to/doc", "query-string", "frag-string"
	);
}

void UriComponentsTest::uriParse_noAuthority() {
	const Http http("http:/path/to/doc?query-string#frag-string");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, -1, -1, 5, 18, 31, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "", "/path/to/doc", "query-string", "frag-string"
	);
}

void UriComponentsTest::uriParse_noPort() {
	const Http http("http://localhost/path/to/doc?query-string#frag-string");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 16, 29, 42, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "localhost", "/path/to/doc", "query-string", "frag-string"
	);
}

void UriComponentsTest::uriParse_noQuery() {
	const Http http("http://localhost/path/to/doc#frag-string");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 16, -1, 29, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "localhost", "/path/to/doc", "", "frag-string"
	);
}

void UriComponentsTest::uriParse_noFragment() {
	const Http http("http://localhost/path/to/doc?query-string");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 16, 29, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, true, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "localhost", "/path/to/doc", "query-string", ""
	);
}

void UriComponentsTest::uriParse_noQueryNoFragment() {
	const Http http("http://localhost/path/to/doc.html");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 16, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "localhost", "/path/to/doc.html", "", ""
	);
}

void UriComponentsTest::uriParse_noPortNoPathNoQueryNoFragment() {
	const Http http("http://borasoftware.com");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 23, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "borasoftware.com", "", "", ""
	);
}

void UriComponentsTest::uriParse_noPortNoPathNoFragment() {
	const Http http("http://borasoftware.com?blah");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 23, 24, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, true, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "borasoftware.com", "", "blah", ""
	);
}

void UriComponentsTest::uriParse_noPortNoPathNoQuery() {
	const Http http("http://borasoftware.com#blah");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 23, -1, 24, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "borasoftware.com", "", "", "blah"
	);
}

void UriComponentsTest::uriParse_noHostNoPortNoPathNoQuery() {
	const Http http("http://");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, -1, -1, 7, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "", "", "", ""
	);
}

void UriComponentsTest::uriParse_ipv6Host() {
	const Http http("http://usr123@[1::1]:12345/a/path?query-string#fragment");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 14, 21, 26, 34, 47, 12345 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, true, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr123", "[1::1]", "/a/path", "query-string", "fragment"
	);
}

void UriComponentsTest::uriParse_ipv6HostNoPort() {
	const Http http("http://usr123@[1::1]/a/path?query-string#fragment");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 14, -1, 20, 28, 41, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, false, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr123", "[1::1]", "/a/path", "query-string", "fragment"
	);
}

void UriComponentsTest::uriParse_ipv6HostEmptyPath() {
	const Http http("http://usr123@[1::1]:12345?query-string#fragment");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 14, 21, 26, 27, 40, 12345 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, true, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr123", "[1::1]", "", "query-string", "fragment"
	);
}

void UriComponentsTest::uriParse_ipv6HostNoPortNoPathNoQueryNoFragment() {
	const Http http("http://[1::1]");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 13, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "[1::1]", "", "", ""
	);
}

void UriComponentsTest::uriParse_ipv4Host() {
	const Http http("http://usr123@123.45.67.89:12345/a/path?query-string#fragment");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 14, 27, 32, 40, 53, 12345 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, true, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr123", "123.45.67.89", "/a/path", "query-string", "fragment"
	);
}

void UriComponentsTest::uriParse_ipv4HostNoPort() {
	const Http http("http://usr123@123.45.67.89/a/path?query-string#fragment");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 14, -1, 26, 34, 47, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, false, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr123", "123.45.67.89", "/a/path", "query-string", "fragment"
	);
}

void UriComponentsTest::uriParse_ipv4HostEmptyPath() {
	const Http http("http://usr123@123.45.67.89:12345?query-string#fragment");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, 7, 14, 27, 32, 33, 46, 12345 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, true, true, true, true, true
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "usr123", "123.45.67.89", "", "query-string", "fragment"
	);
}

void UriComponentsTest::uriParse_ipv4HostNoPortNoPathNoQueryNoFragment() {
	const Http http("http://123.45.67.89");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 19, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "123.45.67.89", "", "", ""
	);
}

void UriComponentsTest::uriParse_mailto() {
	const Http http("mailto:someone@example.com?subject=This%20is%20the%20subject&cc=someone_else@example.com&body=This%20is%20the%20body");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 6, -1, -1, -1, 7, 27, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, true, false
		// scheme, userInfo, host, port, path, query, fragment
		, "mailto", "", "", "someone@example.com", "subject=This%20is%20the%20subject&cc=someone_else@example.com&body=This%20is%20the%20body", ""
	);
}

void UriComponentsTest::uriParse_full1000() {
	const Http http("http://usr1@localhost:12345/path/to/doc?query-string#frag-string");
	size_t sum = 0U;

	for (size_t m = 0U; m < 1000U; m++) {
		UriComponents actual(http);
		sum += actual.port();
	}

	AssertThat(sum, is(1000U * UriComponents(http).port()));
}

void UriComponentsTest::uriParse_invalidUris() {
	AssertThat([] () { UriComponents(""); }, throws<Exception::InvalidUriException>());
	AssertThat([] () { UriComponents("0http://example.com"); }, throws<Exception::InvalidUriException>());
	AssertThat([] () { UriComponents("h$$p://example.com"); }, throws<Exception::InvalidUriException>());
	AssertThat([] () { UriComponents("https://[1::3"); }, throws<Exception::InvalidUriException>());
	AssertThat([] () { UriComponents("https://usr:password@example.com"); }, throws<Exception::InvalidUriException>());
	AssertThat([] () { UriComponents("http://:80"); }, throws<Exception::InvalidUriException>());
}

void UriComponentsTest::uriParse_RCF3986Example1() {
	const Http ftp("ftp://ftp.is.co.za/rfc/rfc1808.txt");

	uriComponentsAssertions(
		UriComponents(ftp)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { ftp.toUriString(), 3, -1, 6, -1, 18, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "ftp", "", "ftp.is.co.za", "/rfc/rfc1808.txt", "", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example2() {
	const Http http("http://www.ietf.org/rfc/rfc2396.txt");

	uriComponentsAssertions(
		UriComponents(http)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { http.toUriString(), 4, -1, 7, -1, 19, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "http", "", "www.ietf.org", "/rfc/rfc2396.txt", "", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example3() {
	const Http ldap("ldap://[2001:db8::7]/c=GB?objectClass?one");

	uriComponentsAssertions(
		UriComponents(ldap)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { ldap.toUriString(), 4, -1, 7, -1, 20, 26, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, false, true, false
		// scheme, userInfo, host, port, path, query, fragment
		, "ldap", "", "[2001:db8::7]", "/c=GB", "objectClass?one", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example4() {
	const Http mailto("mailto:John.Doe@example.com");

	uriComponentsAssertions(
		UriComponents(mailto)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { mailto.toUriString(), 6, -1, -1, -1, 7, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "mailto", "", "", "John.Doe@example.com", "", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example5() {
	const Http news("news:comp.infosystems.www.servers.unix");

	uriComponentsAssertions(
		UriComponents(news)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { news.toUriString(), 4, -1, -1, -1, 5, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "news", "", "", "comp.infosystems.www.servers.unix", "", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example6() {
	const Http tel("tel:+1-816-555-1212");

	uriComponentsAssertions(
		UriComponents(tel)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { tel.toUriString(), 3, -1, -1, -1, 4, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "tel", "", "", "+1-816-555-1212", "", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example7() {
	const Http telnet("telnet://192.0.2.16:80/");

	uriComponentsAssertions(
		UriComponents(telnet)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { telnet.toUriString(), 6, -1, 9, 20, 22, -1, -1, 80 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, true, false, true, true, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "telnet", "", "192.0.2.16", "/", "", ""
	);
}

void UriComponentsTest::uriParse_RCF3986Example8() {
	const Http urn("urn:oasis:names:specification:docbook:dtd:xml:4.1.2");

	uriComponentsAssertions(
		UriComponents(urn)
		// uri, schemeEnd, userinfoOffset, hostOffset, portOffset, pathOffset, queryOffset, fragmentOffset, port
		, UriComponents { urn.toUriString(), 3, -1, -1, -1, 4, -1, -1, 0 }
		// hasAuthority, hasUserInfo, hasHost, hasPort, hasQuery, hasFragment
		, false, false, false, false, false, false
		// scheme, userInfo, host, port, path, query, fragment
		, "urn", "", "", "oasis:names:specification:docbook:dtd:xml:4.1.2", "", ""
	);
}

void UriComponentsTest::normalizePath() {
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

} // namespace Resource

} // namespace Balau
