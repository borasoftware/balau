// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__STRINGS_TEST
#define COM_BORA_SOFTWARE__BALAU_UTIL__STRINGS_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Util {

struct StringsTest : public Testing::TestGroup<StringsTest> {
	StringsTest() {
		registerTest(&StringsTest::toUpper_string,                       "toUpper_string");
		registerTest(&StringsTest::toUpper_u32string,                    "toUpper_u32string");
		registerTest(&StringsTest::toLower_string,                       "toLower_string");
		registerTest(&StringsTest::toLower_u32string,                    "toLower_u32string");
		registerTest(&StringsTest::equalsIgnoreCase_string_string,       "equalsIgnoreCase_string_string");
		registerTest(&StringsTest::equalsIgnoreCase_u32string_u32string, "equalsIgnoreCase_u32string_u32string");
		registerTest(&StringsTest::startsWith_string_string,             "startsWith_string_string");
		registerTest(&StringsTest::startsWith_string_char,               "startsWith_string_char");
		registerTest(&StringsTest::startsWith_u32string_u32string,       "startsWith_u32string_u32string");
		registerTest(&StringsTest::startsWith_u32string_char32_t,        "startsWith_u32string_char32_t");
		registerTest(&StringsTest::endsWith_string_string,               "endsWith_string_string");
		registerTest(&StringsTest::endsWith_string_char,                 "endsWith_string_char");
		registerTest(&StringsTest::endsWith_u32string_u32string,         "endsWith_u32string_u32string");
		registerTest(&StringsTest::endsWith_u32string_char32_t,          "endsWith_u32string_char32_t");
		registerTest(&StringsTest::contains_string_string,               "contains_string_string");
		registerTest(&StringsTest::contains_string_char,                 "contains_string_char");
		registerTest(&StringsTest::contains_u16string_u16string,         "contains_u16string_u16string");
		registerTest(&StringsTest::contains_u16string_char16_t,          "contains_u16string_char16_t");
		registerTest(&StringsTest::contains_u32string_u32string,         "contains_u32string_u32string");
		registerTest(&StringsTest::contains_u32string_char32_t,          "contains_u32string_char32_t");
		registerTest(&StringsTest::occurrences_string_string,            "occurrences_string_string");
		registerTest(&StringsTest::occurrences_u32string_u32string,      "occurrences_u32string_u32string");
		registerTest(&StringsTest::occurrences_string_regex,             "occurrences_string_regex");
		registerTest(&StringsTest::lineLengths,                          "lineLengths");
		registerTest(&StringsTest::lastIndexOf_string_string,            "lastIndexOf_string_string");
		registerTest(&StringsTest::lastIndexOf_u32string_u32string,      "lastIndexOf_u32string_u32string");
		registerTest(&StringsTest::codePointCount_string,                "codePointCount_string");
		registerTest(&StringsTest::codePointCount_u32string,             "codePointCount_u32string");
		registerTest(&StringsTest::append_string_char,                   "append_string_char");
		registerTest(&StringsTest::append_string_char32_t,               "append_string_char32_t");
		registerTest(&StringsTest::append_u32string_char32_t,            "append_u32string_char32_t");
		registerTest(&StringsTest::padLeft_string_char,                  "padLeft_string_char");
		registerTest(&StringsTest::padLeft_string_char32_t,              "padLeft_string_char32_t");
		registerTest(&StringsTest::padLeft_u32string_char32_t,           "padLeft_u32string_char32_t");
		registerTest(&StringsTest::padRight_string_char,                 "padRight_string_char");
		registerTest(&StringsTest::padRight_string_char32_t,             "padRight_string_char32_t");
		registerTest(&StringsTest::padRight_u32string_char32_t,          "padRight_u32string_char32_t");
		registerTest(&StringsTest::trim_string,                          "trim_string");
		registerTest(&StringsTest::trim_u32string,                       "trim_u32string");
		registerTest(&StringsTest::trimLeft_string,                      "trimLeft_string");
		registerTest(&StringsTest::trimLeft_u32string,                   "trimLeft_u32string");
		registerTest(&StringsTest::trimRight_string,                     "trimRight_string");
		registerTest(&StringsTest::trimRight_u32string,                  "trimRight_u32string");
		registerTest(&StringsTest::replaceAll_string_string,             "replaceAll_string_string");
		registerTest(&StringsTest::replaceAll_u32string_u32string,       "replaceAll_u32string_u32string");
		registerTest(&StringsTest::join_string,                          "join_string");
		registerTest(&StringsTest::join_u32string,                       "join_u32string");
		registerTest(&StringsTest::split_string_string,                  "split_string_string");
		registerTest(&StringsTest::split_u32string_u32string,            "split_u32string_u32string");
		registerTest(&StringsTest::split_string_regex,                   "split_string_regex");
		registerTest(&StringsTest::replaceAll_string_regex,              "replaceAll_string_regex");
	}

	void toUpper_string();
	void toUpper_u32string();
	void toLower_string();
	void toLower_u32string();
	void equalsIgnoreCase_string_string();
	void equalsIgnoreCase_u32string_u32string();
	void startsWith_string_string();
	void startsWith_string_char();
	void startsWith_u32string_u32string();
	void startsWith_u32string_char32_t();
	void endsWith_string_string();
	void endsWith_string_char();
	void endsWith_u32string_u32string();
	void endsWith_u32string_char32_t();
	void contains_string_string();
	void contains_string_char();
	void contains_u16string_u16string();
	void contains_u16string_char16_t();
	void contains_u32string_u32string();
	void contains_u32string_char32_t();
	void occurrences_string_string();
	void occurrences_u32string_u32string();
	void occurrences_string_regex();
	void lineLengths();
	void lastIndexOf_string_string();
	void lastIndexOf_u32string_u32string();
	void codePointCount_string();
	void codePointCount_u32string();
	void append_string_char();
	void append_string_char32_t();
	void append_u32string_char32_t();
	void padLeft_string_char();
	void padLeft_string_char32_t();
	void padLeft_u32string_char32_t();
	void padRight_string_char();
	void padRight_string_char32_t();
	void padRight_u32string_char32_t();
	void trim_string();
	void trim_u32string();
	void trimLeft_string();
	void trimLeft_u32string();
	void trimRight_string();
	void trimRight_u32string();
	void replaceAll_string_string();
	void replaceAll_u32string_u32string();
	void join_string();
	void join_u32string();
	void split_string_string();
	void split_u32string_u32string();
	void split_string_regex();
	void replaceAll_string_regex();
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__STRINGS_TEST
