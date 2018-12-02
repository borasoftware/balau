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
	explicit PropertyParserTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&PropertyParserTest::test1, "test1");
		registerTest(&PropertyParserTest::test2, "test2");
		registerTest(&PropertyParserTest::test3, "test3");
		registerTest(&PropertyParserTest::test4, "test4");
		registerTest(&PropertyParserTest::normalisation, "normalisation");
	}

	void test1();
	void test2();
	void test3();
	void test4();
	void normalisation();
};

} // namespace Css

} // namespace Lang

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_PARSER__PROPERTY_PARSER_TEST
