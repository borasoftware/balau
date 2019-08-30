// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <Balau/Testing/TestRunner.hpp>
#include <Balau/Util/Random.hpp>

namespace Balau {

using Testing::is;

namespace Util {

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

	void toUpper_string() {
		AssertThat(Strings::toUpper("weifuhaw"), is(std::string("WEIFUHAW")));
	}
	
	void toUpper_u32string() {
		AssertThat(Strings::toUpper(U"weifuhaw"), is(std::u32string(U"WEIFUHAW")));
	}
	
	void toLower_string() {
		AssertThat(Strings::toLower("WEIFUHAW"), is(std::string("weifuhaw")));
	}
	
	void toLower_u32string() {
		AssertThat(Strings::toLower(U"WEIFUHAW"), is(std::u32string(U"weifuhaw")));
	}
	
	void equalsIgnoreCase_string_string() {
		AssertThat(Strings::equalsIgnoreCase("WEIFUHAW", std::string("weifuhaw")), is(true));
		AssertThat(Strings::equalsIgnoreCase("WeIfUHaW", std::string("weifuhaw")), is(true));
		AssertThat(Strings::equalsIgnoreCase("EeIfUHaW", std::string("weifuhaw")), is(false));
	}
	
	void equalsIgnoreCase_u32string_u32string() {
		AssertThat(Strings::equalsIgnoreCase(U"WEIFUHAW", std::u32string(U"weifuhaw")), is(true));
		AssertThat(Strings::equalsIgnoreCase(U"WeIfUHaW", std::u32string(U"weifuhaw")), is(true));
		AssertThat(Strings::equalsIgnoreCase(U"EeIfUHaW", std::u32string(U"weifuhaw")), is(false));
	}
	
	void startsWith_string_string() {
		AssertThat(Strings::startsWith("qwertyuiop", "qwer"), is(true));
		AssertThat(Strings::startsWith("qwertyuiop", "qwertiu"), is(false));
	}
	
	void startsWith_string_char() {
		AssertThat(Strings::startsWith("qwertyuiop", 'q'), is(true));
		AssertThat(Strings::startsWith("wertyuiop", 'q'), is(false));
	}
	
	void startsWith_u32string_u32string() {
		AssertThat(Strings::startsWith(U"qwertyuiop", U"qwer"), is(true));
		AssertThat(Strings::startsWith(U"qwertyuiop", U"qwertiu"), is(false));
	}
	
	void startsWith_u32string_char32_t() {
		AssertThat(Strings::startsWith(U"qwertyuiop", U'q'), is(true));
		AssertThat(Strings::startsWith(U"wertyuiop", U'q'), is(false));
	}
	
	void endsWith_string_string() {
		AssertThat(Strings::endsWith("qwertyuiop", "uiop"), is(true));
		AssertThat(Strings::endsWith("qwertyuiop", "uipp"), is(false));
	}
	
	void endsWith_string_char() {
		AssertThat(Strings::endsWith("qwertyuiop", 'p'), is(true));
		AssertThat(Strings::endsWith("qwertyuio", 'p'), is(false));
	}
	
	void endsWith_u32string_u32string() {
		AssertThat(Strings::endsWith(U"qwertyuiop", U"uiop"), is(true));
		AssertThat(Strings::endsWith(U"qwertyuiop", U"uipp"), is(false));
	}
	
	void endsWith_u32string_char32_t() {
		AssertThat(Strings::endsWith(U"qwertyuiop", U'p'), is(true));
		AssertThat(Strings::endsWith(U"qwertyuio", U'p'), is(false));
	}
	
	void contains_string_string() {
		AssertThat(Strings::contains("qwertyuiop", "tyu"), is(true));
		AssertThat(Strings::contains("qwertyuiop", "ttyu"), is(false));
	}
	
	void contains_string_char() {
		AssertThat(Strings::contains("qwertyuiop", 't'), is(true));
		AssertThat(Strings::contains("qwertyuiop", 'z'), is(false));
	}
	
	void contains_u16string_u16string() {
		AssertThat(Strings::contains(u"qwertyuiop", u"tyu"), is(true));
		AssertThat(Strings::contains(u"qwertyuiop", u"ttyu"), is(false));
	}
	
	void contains_u16string_char16_t() {
		AssertThat(Strings::contains(u"qwertyuiop", u't'), is(true));
		AssertThat(Strings::contains(u"qwertyuiop", u'z'), is(false));
	}
	
	void contains_u32string_u32string() {
		AssertThat(Strings::contains(U"qwertyuiop", U"tyu"), is(true));
		AssertThat(Strings::contains(U"qwertyuiop", U"ttyu"), is(false));
	}
	
	void contains_u32string_char32_t() {
		AssertThat(Strings::contains(U"qwertyuiop", U't'), is(true));
		AssertThat(Strings::contains(U"qwertyuiop", U'z'), is(false));
	}
	
	void occurrences_string_string() {
		AssertThat(Strings::occurrences(std::string(""), std::string(".")), is(0U));
		AssertThat(Strings::occurrences(std::string("..."), std::string(".")), is(3U));
		AssertThat(Strings::occurrences(std::string("abfcjijffaaaiuygdafaagaajaaaaka"), std::string("aa")), is(5U));
	}
	
	void occurrences_u32string_u32string() {
		AssertThat(Strings::occurrences(std::u32string(U""), std::u32string(U".")), is(0U));
		AssertThat(Strings::occurrences(std::u32string(U"..."), std::u32string(U".")), is(3U));
		AssertThat(Strings::occurrences(std::u32string(U"abfcjijffaaaiuygdafaagaajaaaaka"), std::u32string(U"aa")), is(5U));
	}
	
	void occurrences_string_regex() {
		AssertThat(Strings::occurrences("", std::regex("\\.")), is(0U));
		AssertThat(Strings::occurrences("...", std::regex("\\.")), is(3U));
		AssertThat(Strings::occurrences("abfcjijffaaaiuygdafaagaajaaaaka", std::regex("aa")), is(5U));
		AssertThat(Strings::occurrences("abfcjijffaaaiuygdafaagaajaaaaka", std::regex("aa|f")), is(9U));
		AssertThat(Strings::occurrences("abf\ncj\n\rijf\r\n\nfa\r\r\raaiu\n\r\n\raa", std::regex("\n\r|\r\n|\n|\r")), is(9U));
		AssertThat(Strings::occurrences("abf\n", std::regex("\n\r|\r\n|\n|\r")), is(1U));
	}
	
	void lineLengths() {
		AssertThat(Strings::lineLengths("qwerty"), is(std::vector<size_t> { 6U }));
		AssertThat(Strings::lineLengths("qwerty\npoiu"), is(std::vector<size_t> { 6U, 4U }));
		AssertThat(Strings::lineLengths("qwerty\npoiu\r\n"), is(std::vector<size_t> { 6U, 4U }));
		AssertThat(Strings::lineLengths("qwerty\npoiu\r\ncvb"), is(std::vector<size_t> { 6U, 4U, 3U }));
		AssertThat(Strings::lineLengths("qwerty\npoiu\r\ncvb", false), is(std::vector<size_t> { 6U, 4U }));
		AssertThat(Strings::lineLengths("qwerty\npoiu\r\ncvb\r"), is(std::vector<size_t> { 6U, 4U, 3U }));
	}
	
	void lastIndexOf_string_string() {
		AssertThat(Strings::lastIndexOf("qabcweabcrtabcypoiabcucvb", "abc"), is(18U));
	}
	
	void lastIndexOf_u32string_u32string() {
		AssertThat(Strings::lastIndexOf(U"qabcweabcrtabcypoiabcucvb", U"abc"), is(18U));
	}
	
	void codePointCount_string() {
		AssertThat(Strings::codePointCount("qabcweabcrtabcypoiabcucvb"), is(25U));
	}
	
	void codePointCount_u32string() {
		AssertThat(Strings::codePointCount(U"qabcweabcrtabcypoiabcucvb"), is(25U));
	}
	
	void append_string_char() {
		std::string expected("abccc");
		std::string actual("ab");
		Strings::append(actual, 'c', 3);
		AssertThat(actual, is(expected));
	}
	
	void append_string_char32_t() {
		std::string expected("abccc");
		std::string actual("ab");
		Strings::append(actual, U'c', 3);
		AssertThat(actual, is(expected));
	}
	
	void append_u32string_char32_t() {
		std::u32string expected(U"abccc");
		std::u32string actual(U"ab");
		Strings::append(actual, U'c', 3);
		AssertThat(actual, is(expected));
	}
	
	void padLeft_string_char() {
		AssertThat(Strings::padLeft("qwerty", 10), is("    qwerty"));
		AssertThat(Strings::padLeft("qwerty", 10, 'X'), is("XXXXqwerty"));
	}
	
	void padLeft_string_char32_t() {
		AssertThat(Strings::padLeft("qwerty", 10, U'λ'), is(u8"λλλλqwerty"));
	}
	
	void padLeft_u32string_char32_t() {
		AssertThat(Strings::padLeft(U"qwerty", 10), is(U"    qwerty"));
		AssertThat(Strings::padLeft(U"qwerty", 10, U'λ'), is(U"λλλλqwerty"));
	}
	
	void padRight_string_char() {
		AssertThat(Strings::padRight("qwerty", 10), is("qwerty    "));
		AssertThat(Strings::padRight("qwerty", 10, 'X'), is("qwertyXXXX"));
	}
	
	void padRight_string_char32_t() {
		AssertThat(Strings::padRight("qwerty", 10, U'λ'), is(u8"qwertyλλλλ"));
	}
	
	void padRight_u32string_char32_t() {
		AssertThat(Strings::padRight(U"qwerty", 10), is(U"qwerty    "));
		AssertThat(Strings::padRight(U"qwerty", 10, U'λ'), is(U"qwertyλλλλ"));
	}
	
	void trim_string() {
		AssertThat(Strings::trim("  qwerty   "), is(std::string("qwerty")));
	}
	
	void trim_u32string() {
		AssertThat(Strings::trim(U"  qwerty   "), is(std::u32string(U"qwerty")));
	}
	
	void trimLeft_string() {
		AssertThat(Strings::trimLeft("  qwerty   "), is(std::string("qwerty   ")));
	}
	
	void trimLeft_u32string() {
		AssertThat(Strings::trimLeft(U"  qwerty   "), is(std::u32string(U"qwerty   ")));
	}
	
	void trimRight_string() {
		AssertThat(Strings::trimRight("  qwerty   "), is(std::string("  qwerty")));
	}
	
	void trimRight_u32string() {
		AssertThat(Strings::trimRight(U"  qwerty   "), is(std::u32string(U"  qwerty")));
	}
	
	void replaceAll_string_string() {
		AssertThat(Strings::replaceAll("ABCDEFCDEFGGDEDEFGTH", "DEF", "ZZ"), is(std::string("ABCZZCZZGGDEZZGTH")));
	}
	
	void replaceAll_u32string_u32string() {
		AssertThat(Strings::replaceAll(U"ABCDEFCDEFGGDEDEFGTH", U"DEF", U"ZZ"), is(std::u32string(U"ABCZZCZZGGDEZZGTH")));
	}
	
	void replaceAll_string_regex() {
		AssertThat(Strings::replaceAll("ABCDEFCDEFFGGDEDEFGTH", std::regex("DEFF?"), "ZZ"), is(std::string("ABCZZCZZGGDEZZGTH")));
		AssertThat(Strings::replaceAll("ABC", std::regex("DEFF?"), "ZZ"), is(std::string("ABC")));
		AssertThat(Strings::replaceAll("", std::regex("ccc"), "DD"), is(std::string("")));
	}
	
	void join_string() {
		AssertThat(Strings::join("X", "ab", "cd", "ef"), is("abXcdXef"));
		AssertThat(Strings::join("X", "ab", "cd"), is("abXcd"));
		AssertThat(Strings::join("X", "ab"), is("ab"));
	}
	
	void join_u32string() {
		AssertThat(Strings::join(U"X", U"ab", U"cd", U"ef"), is(U"abXcdXef"));
		AssertThat(Strings::join(U"X", U"ab", U"cd"), is(U"abXcd"));
		AssertThat(Strings::join(U"X", U"ab"), is(U"ab"));
	}
	
	void split_string_string() {
		AssertThat(Strings::split("ABCDEFCDEFGGDEDEFGTH", "DEF"), is(std::vector<std::string_view> { "ABC", "C", "GGDE", "GTH" }));
		AssertThat(Strings::split("ABCDEF", "DEF", true),  is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("ABCDEF", "DEF", false), is(std::vector<std::string_view> { "ABC", "" }));
		AssertThat(Strings::split("DEFABC", "DEF", true),  is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("DEFABC", "DEF", false), is(std::vector<std::string_view> { "", "ABC" }));
		AssertThat(Strings::split("DEFABCDEF", "DEF", true),  is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("DEFABCDEF", "DEF", false), is(std::vector<std::string_view> { "", "ABC", "" }));
	}
	
	void split_u32string_u32string() {
		AssertThat(Strings::split(U"ABCDEFCDEFGGDEDEFGTH", U"DEF"), is(std::vector<std::u32string_view> { U"ABC", U"C", U"GGDE", U"GTH" }));
		AssertThat(Strings::split(U"ABCDEF", U"DEF", true),  is(std::vector<std::u32string_view> { U"ABC" }));
		AssertThat(Strings::split(U"ABCDEF", U"DEF", false), is(std::vector<std::u32string_view> { U"ABC", U"" }));
		AssertThat(Strings::split(U"DEFABC", U"DEF", true),  is(std::vector<std::u32string_view> { U"ABC" }));
		AssertThat(Strings::split(U"DEFABC", U"DEF", false), is(std::vector<std::u32string_view> { U"", U"ABC" }));
		AssertThat(Strings::split(U"DEFABCDEF", U"DEF", true),  is(std::vector<std::u32string_view> { U"ABC" }));
		AssertThat(Strings::split(U"DEFABCDEF", U"DEF", false), is(std::vector<std::u32string_view> { U"", U"ABC", U"" }));
	}
	
	void split_string_regex() {
		AssertThat(Strings::split("ABCDEFCDEFFGGDEDEFGTH", std::regex("DEFF?")), is(std::vector<std::string_view> { "ABC", "C", "GGDE", "GTH" }));
		AssertThat(Strings::split("ABC", std::regex("DEFF?")), is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("ABCDEF", std::regex("DEF"), false, true),  is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("ABCDEF", std::regex("DEF"), false, false), is(std::vector<std::string_view> { "ABC", "" }));
		AssertThat(Strings::split("DEFABC", std::regex("DEF"), false, true),  is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("DEFABC", std::regex("DEF"), false, false), is(std::vector<std::string_view> { "", "ABC" }));
		AssertThat(Strings::split("DEFABCDEF", std::regex("DEF"), false, true),  is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("DEFABCDEF", std::regex("DEF"), false, false), is(std::vector<std::string_view> { "", "ABC", "" }));
		AssertThat(Strings::split("", std::regex("ccc"), false, true), is(std::vector<std::string_view> {}));
		AssertThat(Strings::split("", std::regex("ccc"), false, false), is(std::vector<std::string_view> {}));
	
		AssertThat(Strings::split("ABCDEFCDEFFGGDEDEFGTH", std::regex("DEFF?"), true), is(std::vector<std::string_view> { "ABC", "DEF", "C", "DEFF", "GGDE", "DEF", "GTH" }));
		AssertThat(Strings::split("ABC", std::regex("DEFF?"), true), is(std::vector<std::string_view> { "ABC" }));
		AssertThat(Strings::split("ABCDEF", std::regex("DEF"), true, true),  is(std::vector<std::string_view> { "ABC", "DEF" }));
		AssertThat(Strings::split("ABCDEF", std::regex("DEF"), true, false), is(std::vector<std::string_view> { "ABC", "DEF", "" }));
		AssertThat(Strings::split("DEFABC", std::regex("DEF"), true, true),  is(std::vector<std::string_view> { "DEF", "ABC" }));
		AssertThat(Strings::split("DEFABC", std::regex("DEF"), true, false), is(std::vector<std::string_view> { "", "DEF", "ABC" }));
		AssertThat(Strings::split("DEFABCDEF", std::regex("DEF"), true, true),  is(std::vector<std::string_view> { "DEF", "ABC", "DEF" }));
		AssertThat(Strings::split("DEFABCDEF", std::regex("DEF"), true, false), is(std::vector<std::string_view> { "", "DEF", "ABC", "DEF", "" }));
		AssertThat(Strings::split("", std::regex("ccc"), true, true), is(std::vector<std::string_view> {}));
		AssertThat(Strings::split("", std::regex("ccc"), true, false), is(std::vector<std::string_view> {}));
	}
};

} // namespace Util

} // namespace Balau
