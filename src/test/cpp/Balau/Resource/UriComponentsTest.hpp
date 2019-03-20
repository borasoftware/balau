// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_COMPONENTS_TEST
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_COMPONENTS_TEST

#include <Balau/Resource/UriComponents.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Resource {

struct UriComponentsTest : public Testing::TestGroup<UriComponentsTest> {
	UriComponentsTest() {
		registerTest(&UriComponentsTest::uriParse_full,                                  "uriParse_full");
		registerTest(&UriComponentsTest::uriParse_noUserinfo,                            "uriParse_noUserinfo");
		registerTest(&UriComponentsTest::uriParse_noAuthority,                           "uriParse_noAuthority");
		registerTest(&UriComponentsTest::uriParse_noPort,                                "uriParse_noPort");
		registerTest(&UriComponentsTest::uriParse_noQuery,                               "uriParse_noQuery");
		registerTest(&UriComponentsTest::uriParse_noFragment,                            "uriParse_noFragment");
		registerTest(&UriComponentsTest::uriParse_noQueryNoFragment,                     "uriParse_noQueryNoFragment");
		registerTest(&UriComponentsTest::uriParse_noPortNoPathNoQueryNoFragment,         "uriParse_noPortNoPathNoQueryNoFragment");
		registerTest(&UriComponentsTest::uriParse_noPortNoPathNoFragment,                "uriParse_noPortNoPathNoFragment");
		registerTest(&UriComponentsTest::uriParse_noPortNoPathNoQuery,                   "uriParse_noPortNoPathNoQuery");
		registerTest(&UriComponentsTest::uriParse_noHostNoPortNoPathNoQuery,             "uriParse_noHostNoPortNoPathNoQuery");
		registerTest(&UriComponentsTest::uriParse_ipv6Host,                              "uriParse_ipv6Host");
		registerTest(&UriComponentsTest::uriParse_ipv6HostNoPort,                        "uriParse_ipv6HostNoPort");
		registerTest(&UriComponentsTest::uriParse_ipv6HostEmptyPath,                     "uriParse_ipv6HostEmptyPath");
		registerTest(&UriComponentsTest::uriParse_ipv6HostNoPortNoPathNoQueryNoFragment, "uriParse_ipv6HostNoPortNoPathNoQueryNoFragment");
		registerTest(&UriComponentsTest::uriParse_ipv4Host,                              "uriParse_ipv4Host");
		registerTest(&UriComponentsTest::uriParse_ipv4HostNoPort,                        "uriParse_ipv4HostNoPort");
		registerTest(&UriComponentsTest::uriParse_ipv4HostEmptyPath,                     "uriParse_ipv4HostEmptyPath");
		registerTest(&UriComponentsTest::uriParse_ipv4HostNoPortNoPathNoQueryNoFragment, "uriParse_ipv4HostNoPortNoPathNoQueryNoFragment");
		registerTest(&UriComponentsTest::uriParse_mailto,                                "uriParse_mailto");
		registerTest(&UriComponentsTest::uriParse_full1000,                              "uriParse_full1000");
		registerTest(&UriComponentsTest::uriParse_invalidUris,                           "uriParse_invalidUris");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example1,                       "uriParse_RCF3986Example1");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example2,                       "uriParse_RCF3986Example2");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example3,                       "uriParse_RCF3986Example3");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example4,                       "uriParse_RCF3986Example4");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example5,                       "uriParse_RCF3986Example5");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example6,                       "uriParse_RCF3986Example6");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example7,                       "uriParse_RCF3986Example7");
		registerTest(&UriComponentsTest::uriParse_RCF3986Example8,                       "uriParse_RCF3986Example8");

		registerTest(&UriComponentsTest::normalizePath,                                  "normalizePath");
	}

	void uriParse_full();
	void uriParse_noUserinfo();
	void uriParse_noAuthority();
	void uriParse_noPort();
	void uriParse_noQuery();
	void uriParse_noFragment();
	void uriParse_noQueryNoFragment();
	void uriParse_noPortNoPathNoQueryNoFragment();
	void uriParse_noPortNoPathNoFragment();
	void uriParse_noPortNoPathNoQuery();
	void uriParse_noHostNoPortNoPathNoQuery();
	void uriParse_ipv6Host();
	void uriParse_ipv6HostNoPort();
	void uriParse_ipv6HostEmptyPath();
	void uriParse_ipv6HostNoPortNoPathNoQueryNoFragment();
	void uriParse_ipv4Host();
	void uriParse_ipv4HostNoPort();
	void uriParse_ipv4HostEmptyPath();
	void uriParse_ipv4HostNoPortNoPathNoQueryNoFragment();
	void uriParse_mailto();
	void uriParse_full1000();
	void uriParse_invalidUris();
	void uriParse_RCF3986Example1();
	void uriParse_RCF3986Example2();
	void uriParse_RCF3986Example3();
	void uriParse_RCF3986Example4();
	void uriParse_RCF3986Example5();
	void uriParse_RCF3986Example6();
	void uriParse_RCF3986Example7();
	void uriParse_RCF3986Example8();

	void normalizePath();

	void uriComponentsAssertions(const UriComponents & actual,
	                             const UriComponents & expected,
	                             bool hasAuthority,
	                             bool hasUserInfo,
	                             bool hasHost,
	                             bool hasPort,
	                             bool hasQuery,
	                             bool hasFragment,
	                             const std::string & scheme,
	                             const std::string & userInfo,
	                             const std::string & host,
	                             const std::string & path,
	                             const std::string & query,
	                             const std::string & fragment);
};

} // namespace Resource

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_COMPONENTS_TEST
