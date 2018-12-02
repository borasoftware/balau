// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PropertyParserTest.hpp"

#include <Balau/Lang/Property/Util/PropertyAstToString.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>
#include <Balau/Resource/StringUri.hpp>
#include <Balau/Util/Files.hpp>

#include "../../../../TestResources.hpp"

namespace Balau {

using Testing::assertFail;
using Testing::assertThat;
using Testing::is;

namespace Lang {

namespace Property {

using namespace AST;

void PropertyParserTest_test(const std::string & expected) {
	Resource::StringUri input(expected);
	Properties properties = PropertyParserService::parse(input);
	std::string actual = toString(properties);
	assertThat(actual, is(expected));
}

const std::string expected1 = 1 + R"Property(
http.server.worker.count = 8

file.serve {
	location = /
	document.root = file:src/doc
	cache.ttl = 3600
}

)Property";

const std::string expected2 = 1 + R"Property(
\{ = 8
abc = \{ brackets }
)Property";

const std::string expected3 = 1 + R"Property(
# A hierarchical property file that has comments,
# escaped characters, and line continuation.

\#a\ complexly\ named\ property\# = \{ a value with curly brackets \}

prop = a value with ## hash !! and excl

group.config {
	# Use of line continuation.
	files = file1.txt \
	      , file2.txt \
	      , file3.txt
}
)Property";

const std::string expected4 = 1 + R"Property(
# An HTTPS include directive.
@https://borasoftware.com/doc/examples/hprops.properties

# An absolute path include directive.
@/etc/balau/default-sites/default.site

# A relative path include directive.
@extra-sites/special.site
)Property";

void PropertyParserTest::test1() {
	PropertyParserTest_test(expected1);
}

void PropertyParserTest::test2() {
	PropertyParserTest_test(expected2);
}

void PropertyParserTest::test3() {
	PropertyParserTest_test(expected3);
}

void PropertyParserTest::test4() {
	PropertyParserTest_test(expected4);
}

void PropertyParserTest::normalisation() {
	assertThat(PropertyNode::normalise("\\{"), is("{"));
	assertThat(PropertyNode::normalise("abc\\\n  def\\\n  ghi"), is("abcdefghi"));
}

} // namespace Property

} // namespace Lang

} // namespace Balau
