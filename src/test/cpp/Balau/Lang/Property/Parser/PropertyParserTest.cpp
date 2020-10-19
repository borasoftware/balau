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
		RegisterTest(simpleHierarchy);
		RegisterTest(specialSymbolNames);
		RegisterTest(complexNames);
		RegisterTest(rootIncludes);
		RegisterTest(normalisation);
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
