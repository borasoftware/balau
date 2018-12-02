// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StringsTest.hpp"

#include <Balau/Util/Random.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

namespace Util {

void StringsTest::toUpper_string() {
	assertThat(Strings::toUpper("weifuhaw"), is(std::string("WEIFUHAW")));
}

void StringsTest::toUpper_u32string() {
	assertThat(Strings::toUpper(U"weifuhaw"), is(std::u32string(U"WEIFUHAW")));
}

void StringsTest::toLower_string() {
	assertThat(Strings::toLower("WEIFUHAW"), is(std::string("weifuhaw")));
}

void StringsTest::toLower_u32string() {
	assertThat(Strings::toLower(U"WEIFUHAW"), is(std::u32string(U"weifuhaw")));
}

void StringsTest::equalsIgnoreCase_string_string() {
	assertThat(Strings::equalsIgnoreCase("WEIFUHAW", std::string("weifuhaw")), is(true));
	assertThat(Strings::equalsIgnoreCase("WeIfUHaW", std::string("weifuhaw")), is(true));
	assertThat(Strings::equalsIgnoreCase("EeIfUHaW", std::string("weifuhaw")), is(false));
}

void StringsTest::equalsIgnoreCase_u32string_u32string() {
	assertThat(Strings::equalsIgnoreCase(U"WEIFUHAW", std::u32string(U"weifuhaw")), is(true));
	assertThat(Strings::equalsIgnoreCase(U"WeIfUHaW", std::u32string(U"weifuhaw")), is(true));
	assertThat(Strings::equalsIgnoreCase(U"EeIfUHaW", std::u32string(U"weifuhaw")), is(false));
}

void StringsTest::startsWith_string_string() {
	assertThat(Strings::startsWith("qwertyuiop", "qwer"), is(true));
	assertThat(Strings::startsWith("qwertyuiop", "qwertiu"), is(false));
}

void StringsTest::startsWith_string_char() {
	assertThat(Strings::startsWith("qwertyuiop", 'q'), is(true));
	assertThat(Strings::startsWith("wertyuiop", 'q'), is(false));
}

void StringsTest::startsWith_u32string_u32string() {
	assertThat(Strings::startsWith(U"qwertyuiop", U"qwer"), is(true));
	assertThat(Strings::startsWith(U"qwertyuiop", U"qwertiu"), is(false));
}

void StringsTest::startsWith_u32string_char32_t() {
	assertThat(Strings::startsWith(U"qwertyuiop", U'q'), is(true));
	assertThat(Strings::startsWith(U"wertyuiop", U'q'), is(false));
}

void StringsTest::endsWith_string_string() {
	assertThat(Strings::endsWith("qwertyuiop", "uiop"), is(true));
	assertThat(Strings::endsWith("qwertyuiop", "uipp"), is(false));
}

void StringsTest::endsWith_string_char() {
	assertThat(Strings::endsWith("qwertyuiop", 'p'), is(true));
	assertThat(Strings::endsWith("qwertyuio", 'p'), is(false));
}

void StringsTest::endsWith_u32string_u32string() {
	assertThat(Strings::endsWith(U"qwertyuiop", U"uiop"), is(true));
	assertThat(Strings::endsWith(U"qwertyuiop", U"uipp"), is(false));
}

void StringsTest::endsWith_u32string_char32_t() {
	assertThat(Strings::endsWith(U"qwertyuiop", U'p'), is(true));
	assertThat(Strings::endsWith(U"qwertyuio", U'p'), is(false));
}

void StringsTest::contains_string_string() {
	assertThat(Strings::contains("qwertyuiop", "tyu"), is(true));
	assertThat(Strings::contains("qwertyuiop", "ttyu"), is(false));
}

void StringsTest::contains_string_char() {
	assertThat(Strings::contains("qwertyuiop", 't'), is(true));
	assertThat(Strings::contains("qwertyuiop", 'z'), is(false));
}

void StringsTest::contains_u16string_u16string() {
	assertThat(Strings::contains(u"qwertyuiop", u"tyu"), is(true));
	assertThat(Strings::contains(u"qwertyuiop", u"ttyu"), is(false));
}

void StringsTest::contains_u16string_char16_t() {
	assertThat(Strings::contains(u"qwertyuiop", u't'), is(true));
	assertThat(Strings::contains(u"qwertyuiop", u'z'), is(false));
}

void StringsTest::contains_u32string_u32string() {
	assertThat(Strings::contains(U"qwertyuiop", U"tyu"), is(true));
	assertThat(Strings::contains(U"qwertyuiop", U"ttyu"), is(false));
}

void StringsTest::contains_u32string_char32_t() {
	assertThat(Strings::contains(U"qwertyuiop", U't'), is(true));
	assertThat(Strings::contains(U"qwertyuiop", U'z'), is(false));
}

void StringsTest::occurrences_string_string() {
	assertThat(Strings::occurrences(std::string(""), std::string(".")), is(0U));
	assertThat(Strings::occurrences(std::string("..."), std::string(".")), is(3U));
	assertThat(Strings::occurrences(std::string("abfcjijffaaaiuygdafaagaajaaaaka"), std::string("aa")), is(5U));
}

void StringsTest::occurrences_u32string_u32string() {
	assertThat(Strings::occurrences(std::u32string(U""), std::u32string(U".")), is(0U));
	assertThat(Strings::occurrences(std::u32string(U"..."), std::u32string(U".")), is(3U));
	assertThat(Strings::occurrences(std::u32string(U"abfcjijffaaaiuygdafaagaajaaaaka"), std::u32string(U"aa")), is(5U));
}

void StringsTest::occurrences_string_regex() {
	assertThat(Strings::occurrences("", std::regex("\\.")), is(0U));
	assertThat(Strings::occurrences("...", std::regex("\\.")), is(3U));
	assertThat(Strings::occurrences("abfcjijffaaaiuygdafaagaajaaaaka", std::regex("aa")), is(5U));
	assertThat(Strings::occurrences("abfcjijffaaaiuygdafaagaajaaaaka", std::regex("aa|f")), is(9U));
	assertThat(Strings::occurrences("abf\ncj\n\rijf\r\n\nfa\r\r\raaiu\n\r\n\raa", std::regex("\n\r|\r\n|\n|\r")), is(9U));
	assertThat(Strings::occurrences("abf\n", std::regex("\n\r|\r\n|\n|\r")), is(1U));
}

void StringsTest::lineLengths() {
	assertThat(Strings::lineLengths("qwerty"), is(std::vector<size_t> { 6U }));
	assertThat(Strings::lineLengths("qwerty\npoiu"), is(std::vector<size_t> { 6U, 4U }));
	assertThat(Strings::lineLengths("qwerty\npoiu\r\n"), is(std::vector<size_t> { 6U, 4U }));
	assertThat(Strings::lineLengths("qwerty\npoiu\r\ncvb"), is(std::vector<size_t> { 6U, 4U, 3U }));
	assertThat(Strings::lineLengths("qwerty\npoiu\r\ncvb", false), is(std::vector<size_t> { 6U, 4U }));
	assertThat(Strings::lineLengths("qwerty\npoiu\r\ncvb\r"), is(std::vector<size_t> { 6U, 4U, 3U }));
}

void StringsTest::lastIndexOf_string_string() {
	assertThat(Strings::lastIndexOf("qabcweabcrtabcypoiabcucvb", "abc"), is(18U));
}

void StringsTest::lastIndexOf_u32string_u32string() {
	assertThat(Strings::lastIndexOf(U"qabcweabcrtabcypoiabcucvb", U"abc"), is(18U));
}

void StringsTest::codePointCount_string() {
	assertThat(Strings::codePointCount("qabcweabcrtabcypoiabcucvb"), is(25U));
}

void StringsTest::codePointCount_u32string() {
	assertThat(Strings::codePointCount(U"qabcweabcrtabcypoiabcucvb"), is(25U));
}

void StringsTest::append_string_char() {
	std::string expected("abccc");
	std::string actual("ab");
	Strings::append(actual, 'c', 3);
	assertThat(actual, is(expected));
}

void StringsTest::append_string_char32_t() {
	std::string expected("abccc");
	std::string actual("ab");
	Strings::append(actual, U'c', 3);
	assertThat(actual, is(expected));
}

void StringsTest::append_u32string_char32_t() {
	std::u32string expected(U"abccc");
	std::u32string actual(U"ab");
	Strings::append(actual, U'c', 3);
	assertThat(actual, is(expected));
}

void StringsTest::padLeft_string_char() {
	assertThat(Strings::padLeft("qwerty", 10), is("    qwerty"));
	assertThat(Strings::padLeft("qwerty", 10, 'X'), is("XXXXqwerty"));
}

void StringsTest::padLeft_string_char32_t() {
	assertThat(Strings::padLeft("qwerty", 10, U'λ'), is(u8"λλλλqwerty"));
}

void StringsTest::padLeft_u32string_char32_t() {
	assertThat(Strings::padLeft(U"qwerty", 10), is(U"    qwerty"));
	assertThat(Strings::padLeft(U"qwerty", 10, U'λ'), is(U"λλλλqwerty"));
}

void StringsTest::padRight_string_char() {
	assertThat(Strings::padRight("qwerty", 10), is("qwerty    "));
	assertThat(Strings::padRight("qwerty", 10, 'X'), is("qwertyXXXX"));
}

void StringsTest::padRight_string_char32_t() {
	assertThat(Strings::padRight("qwerty", 10, U'λ'), is(u8"qwertyλλλλ"));
}

void StringsTest::padRight_u32string_char32_t() {
	assertThat(Strings::padRight(U"qwerty", 10), is(U"qwerty    "));
	assertThat(Strings::padRight(U"qwerty", 10, U'λ'), is(U"qwertyλλλλ"));
}

void StringsTest::trim_string() {
	assertThat(Strings::trim("  qwerty   "), is(std::string("qwerty")));
}

void StringsTest::trim_u32string() {
	assertThat(Strings::trim(U"  qwerty   "), is(std::u32string(U"qwerty")));
}

void StringsTest::trimLeft_string() {
	assertThat(Strings::trimLeft("  qwerty   "), is(std::string("qwerty   ")));
}

void StringsTest::trimLeft_u32string() {
	assertThat(Strings::trimLeft(U"  qwerty   "), is(std::u32string(U"qwerty   ")));
}

void StringsTest::trimRight_string() {
	assertThat(Strings::trimRight("  qwerty   "), is(std::string("  qwerty")));
}

void StringsTest::trimRight_u32string() {
	assertThat(Strings::trimRight(U"  qwerty   "), is(std::u32string(U"  qwerty")));
}

void StringsTest::replaceAll_string_string() {
	assertThat(Strings::replaceAll("ABCDEFCDEFGGDEDEFGTH", "DEF", "ZZ"), is(std::string("ABCZZCZZGGDEZZGTH")));
}

void StringsTest::replaceAll_u32string_u32string() {
	assertThat(Strings::replaceAll(U"ABCDEFCDEFGGDEDEFGTH", U"DEF", U"ZZ"), is(std::u32string(U"ABCZZCZZGGDEZZGTH")));
}

void StringsTest::replaceAll_string_regex() {
	assertThat(Strings::replaceAll("ABCDEFCDEFFGGDEDEFGTH", std::regex("DEFF?"), "ZZ"), is(std::string("ABCZZCZZGGDEZZGTH")));
	assertThat(Strings::replaceAll("ABC", std::regex("DEFF?"), "ZZ"), is(std::string("ABC")));
	assertThat(Strings::replaceAll("", std::regex("ccc"), "DD"), is(std::string("")));
}

void StringsTest::join_string() {
	assertThat(Strings::join("X", "ab", "cd", "ef"), is("abXcdXef"));
	assertThat(Strings::join("X", "ab", "cd"), is("abXcd"));
	assertThat(Strings::join("X", "ab"), is("ab"));
}

void StringsTest::join_u32string() {
	assertThat(Strings::join(U"X", U"ab", U"cd", U"ef"), is(U"abXcdXef"));
	assertThat(Strings::join(U"X", U"ab", U"cd"), is(U"abXcd"));
	assertThat(Strings::join(U"X", U"ab"), is(U"ab"));
}

void StringsTest::split_string_string() {
	assertThat(Strings::split("ABCDEFCDEFGGDEDEFGTH", "DEF"), is(std::vector<std::string_view> { "ABC", "C", "GGDE", "GTH" }));
	assertThat(Strings::split("ABCDEF", "DEF", true),  is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("ABCDEF", "DEF", false), is(std::vector<std::string_view> { "ABC", "" }));
	assertThat(Strings::split("DEFABC", "DEF", true),  is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("DEFABC", "DEF", false), is(std::vector<std::string_view> { "", "ABC" }));
	assertThat(Strings::split("DEFABCDEF", "DEF", true),  is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("DEFABCDEF", "DEF", false), is(std::vector<std::string_view> { "", "ABC", "" }));
}

void StringsTest::split_u32string_u32string() {
	assertThat(Strings::split(U"ABCDEFCDEFGGDEDEFGTH", U"DEF"), is(std::vector<std::u32string_view> { U"ABC", U"C", U"GGDE", U"GTH" }));
	assertThat(Strings::split(U"ABCDEF", U"DEF", true),  is(std::vector<std::u32string_view> { U"ABC" }));
	assertThat(Strings::split(U"ABCDEF", U"DEF", false), is(std::vector<std::u32string_view> { U"ABC", U"" }));
	assertThat(Strings::split(U"DEFABC", U"DEF", true),  is(std::vector<std::u32string_view> { U"ABC" }));
	assertThat(Strings::split(U"DEFABC", U"DEF", false), is(std::vector<std::u32string_view> { U"", U"ABC" }));
	assertThat(Strings::split(U"DEFABCDEF", U"DEF", true),  is(std::vector<std::u32string_view> { U"ABC" }));
	assertThat(Strings::split(U"DEFABCDEF", U"DEF", false), is(std::vector<std::u32string_view> { U"", U"ABC", U"" }));
}

void StringsTest::split_string_regex() {
	assertThat(Strings::split("ABCDEFCDEFFGGDEDEFGTH", std::regex("DEFF?")), is(std::vector<std::string_view> { "ABC", "C", "GGDE", "GTH" }));
	assertThat(Strings::split("ABC", std::regex("DEFF?")), is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("ABCDEF", std::regex("DEF"), false, true),  is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("ABCDEF", std::regex("DEF"), false, false), is(std::vector<std::string_view> { "ABC", "" }));
	assertThat(Strings::split("DEFABC", std::regex("DEF"), false, true),  is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("DEFABC", std::regex("DEF"), false, false), is(std::vector<std::string_view> { "", "ABC" }));
	assertThat(Strings::split("DEFABCDEF", std::regex("DEF"), false, true),  is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("DEFABCDEF", std::regex("DEF"), false, false), is(std::vector<std::string_view> { "", "ABC", "" }));
	assertThat(Strings::split("", std::regex("ccc"), false, true), is(std::vector<std::string_view> {}));
	assertThat(Strings::split("", std::regex("ccc"), false, false), is(std::vector<std::string_view> {}));

	assertThat(Strings::split("ABCDEFCDEFFGGDEDEFGTH", std::regex("DEFF?"), true), is(std::vector<std::string_view> { "ABC", "DEF", "C", "DEFF", "GGDE", "DEF", "GTH" }));
	assertThat(Strings::split("ABC", std::regex("DEFF?"), true), is(std::vector<std::string_view> { "ABC" }));
	assertThat(Strings::split("ABCDEF", std::regex("DEF"), true, true),  is(std::vector<std::string_view> { "ABC", "DEF" }));
	assertThat(Strings::split("ABCDEF", std::regex("DEF"), true, false), is(std::vector<std::string_view> { "ABC", "DEF", "" }));
	assertThat(Strings::split("DEFABC", std::regex("DEF"), true, true),  is(std::vector<std::string_view> { "DEF", "ABC" }));
	assertThat(Strings::split("DEFABC", std::regex("DEF"), true, false), is(std::vector<std::string_view> { "", "DEF", "ABC" }));
	assertThat(Strings::split("DEFABCDEF", std::regex("DEF"), true, true),  is(std::vector<std::string_view> { "DEF", "ABC", "DEF" }));
	assertThat(Strings::split("DEFABCDEF", std::regex("DEF"), true, false), is(std::vector<std::string_view> { "", "DEF", "ABC", "DEF", "" }));
	assertThat(Strings::split("", std::regex("ccc"), true, true), is(std::vector<std::string_view> {}));
	assertThat(Strings::split("", std::regex("ccc"), true, false), is(std::vector<std::string_view> {}));
}

} // namespace Util

} // namespace Balau
