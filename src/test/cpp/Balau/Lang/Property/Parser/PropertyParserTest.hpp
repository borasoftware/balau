// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER_TEST
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Lang {

namespace Property {

struct PropertyParserTest : public Testing::TestGroup<PropertyParserTest> {
	PropertyParserTest() {
		registerTest(&PropertyParserTest::simpleHierarchy,    "simpleHierarchy");
		registerTest(&PropertyParserTest::specialSymbolNames, "specialSymbolNames");
		registerTest(&PropertyParserTest::complexNames,       "complexNames");
		registerTest(&PropertyParserTest::rootIncludes,       "rootIncludes");
		registerTest(&PropertyParserTest::normalisation,      "normalisation");
	}

	void simpleHierarchy();
	void specialSymbolNames();
	void complexNames();
	void rootIncludes();
	void normalisation();
};

} // namespace Css

} // namespace Lang

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER_TEST
