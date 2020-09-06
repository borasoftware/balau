// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>
#include <Balau/Lang/Property/Util/PropertyAstToString.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>
#include <Balau/Resource/StringUri.hpp>

namespace Balau {

using Testing::is;

namespace Lang::Property {

using namespace AST;

void PropertyParserTest_test(const std::string & expected) {
	Resource::StringUri input(expected);
	Properties properties = PropertyParserService::parse(input);
	std::string actual = toString(properties);
	AssertThat(actual, is(expected));
}

const std::string simpleHierarchyText = 1 + R"Property(
http.server.worker.count = 8

file.serve {
	location = /
	document.root = file:src/doc
	cache.ttl = 3600
}

)Property";

const std::string specialSymbolNamesText = 1 + R"Property(
\{ = 8
abc = \{ brackets }
)Property";

const std::string complexNamesText = 1 + R"Property(
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

const std::string rootIncludesText = 1 + R"Property(
# An HTTPS include directive.
@https://borasoftware.com/doc/examples/hprops.properties

# An absolute path include directive.
@/etc/balau/default-sites/default.site

# A relative path include directive.
@extra-sites/special.site
)Property";

struct PropertyParserTest : public Testing::TestGroup<PropertyParserTest> {
	PropertyParserTest() {
		RegisterTestCase(simpleHierarchy);
		RegisterTestCase(specialSymbolNames);
		RegisterTestCase(complexNames);
		RegisterTestCase(rootIncludes);
		RegisterTestCase(normalisation);
	}

	void simpleHierarchy() {
		PropertyParserTest_test(simpleHierarchyText);
	}

	void specialSymbolNames() {
		PropertyParserTest_test(specialSymbolNamesText);
	}

	void complexNames() {
		PropertyParserTest_test(complexNamesText);
	}

	void rootIncludes() {
		PropertyParserTest_test(rootIncludesText);
	}

	void normalisation() {
		AssertThat(PropertyNode::normalise("\\{"), is("{"));
		AssertThat(PropertyNode::normalise("abc\\\n  def\\\n  ghi"), is("abcdefghi"));
	}
};

} // namespace Lang::Property

} // namespace Balau
