// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file Strings.hpp
///
/// Utilities for strings.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__STRINGS
#define COM_BORA_SOFTWARE__BALAU_UTIL__STRINGS

#include <Balau/Type/Character.hpp>
#include <Balau/Type/ToString.hpp>
#include <Balau/Util/Impl/StringsImpl.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <regex>

namespace Balau::Util {

///
/// Utilities for strings.
///
/// The utilities use and return std::string_view objects where possible.
///
/// In order to support multiple string types, a two stage typename
/// deduction / type conversion approach is used for many of the functions.
///
struct Strings final {
	/////////////////////////////// Examination ///////////////////////////////

	///
	/// Does the string start with the specified prefix?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename PrefixT>
	static bool startsWith(const StringT<CharT, T ...> & str, const PrefixT & prefix) {
		return startsWith(std::basic_string_view<CharT>{str}, std::basic_string_view<CharT>{prefix});
	}

	///
	/// Does the string start with the specified prefix?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT>
	static bool startsWith(const StringT<CharT, T ...> & str, CharT prefix) {
		return startsWith(std::basic_string_view<CharT>{str}, prefix);
	}

	///
	/// Does the string start with the specified prefix?
	///
	template <typename CharT, typename PrefixT>
	static bool startsWith(const CharT * str, const PrefixT & prefix) {
		return startsWith(std::basic_string_view<CharT>{str}, std::basic_string_view<CharT>{prefix});
	}

	///
	/// Does the string start with the specified prefix?
	///
	template <typename CharT>
	static bool startsWith(const CharT * str, CharT prefix) {
		return startsWith(std::basic_string_view<CharT>{str}, prefix);
	}

	///
	/// Does the string start with the specified prefix?
	///
	template <typename CharT>
	static bool startsWith(const std::basic_string_view<CharT> & str,
	                       const std::basic_string_view<CharT> & prefix) {
		return str.find(prefix) == 0;
	}

	///
	/// Does the string start with the specified prefix?
	///
	template <typename CharT>
	static bool startsWith(const std::basic_string_view<CharT> & str, CharT prefix) {
		return str.find(prefix) == 0;
	}

	///
	/// Does the string start with the specified regular expression?
	///
	static bool startsWithRegex(const std::string & str, const std::regex & prefix) {
		std::smatch match;

		if (!std::regex_search(str, match, prefix)) {
			return false;
		}

		return match.position(0) == 0;
	}

	///////////////////////////////////

	///
	/// Does the string match the specified regular expression?
	///
	static bool matches(const std::string & str, const std::regex & re) {
		std::smatch match;
		return std::regex_match(str, match, re);
	}

	///////////////////////////////////

	///
	/// Does the string end with the specified suffix?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename SuffixT>
	static bool endsWith(const StringT<CharT, T ...> & str, const SuffixT & suffix) {
		return endsWith(std::basic_string_view<CharT>{str}, std::basic_string_view<CharT>{suffix});
	}

	///
	/// Does the string end with the specified suffix?
	///
	template <typename CharT, typename SuffixT>
	static bool endsWith(const CharT * str, const SuffixT & suffix) {
		return endsWith(std::basic_string_view<CharT>{str}, std::basic_string_view<CharT>{suffix});
	}

	///
	/// Does the first string end with the specified suffix?
	///
	template <typename CharT>
	static bool endsWith(const std::basic_string_view<CharT> & str, const std::basic_string_view<CharT> & suffix) {
		return str.length() >= suffix.length() && str.rfind(suffix) == str.length() - suffix.length();
	}

	///
	/// Does the UTF-8 string end with the specified ascii char?
	///
	static bool endsWith(const char * str, const char suffix) {
		return endsWith(std::string_view{str}, suffix);
	}

	///
	/// Does the UTF-8 string end with the specified ascii char?
	///
	static bool endsWith(const std::string_view & str, const char suffix) {
		return str.length() >= 1 && str[str.length() - 1] == suffix;
	}

	///
	/// Does the UTF-32 string end with the specified ascii char?
	///
	static bool endsWith(const char32_t * str, const char32_t suffix) {
		return endsWith(std::u32string_view{str}, suffix);
	}

	///
	/// Does the UTF-32 string end with the specified code point?
	///
	static bool endsWith(const std::u32string_view & str, const char32_t suffix) {
		return str.length() >= 1 && str[str.length() - 1] == suffix;
	}

	///////////////////////////////////

	///
	/// Does the first string contain the second string?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename SubstrT>
	static bool contains(const StringT<CharT, T ...> & str, const SubstrT & substring) {
		return str.find(substring) != StringT<CharT, T ...>::npos;
	}

	///
	/// Does the first string contain the second string?
	///
	template <typename CharT, typename SubstrT>
	static bool contains(const CharT * str, const SubstrT & substring) {
		return contains(std::basic_string_view<CharT>{str}, substring);
	}

	///
	/// Does the string contain the ascii char (UTF-8) / UTF-16 char (UTF-16) / code point (UTF-32)?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT>
	static bool contains(const StringT<CharT, T ...> & str, const CharT & c) {
		return str.find(c) != StringT<CharT, T ...>::npos;
	}

	///
	/// Does the string contain the ascii char (UTF-8) / UTF-16 char (UTF-16) / code point (UTF-32)?
	///
	template <typename CharT>
	static bool contains(const CharT * str, const CharT & c) {
		return std::basic_string_view<CharT>{str}.find(c) != std::basic_string_view<CharT>::npos;
	}

	///////////////////////////////////

	///
	/// How many non-overlapping occurrences of the second string are found in the first string?
	///
	template <typename CharT, typename SubstrT>
	static size_t occurrences(const CharT * str, const SubstrT & substring) {
		return occurrences(std::basic_string_view<CharT>{str}, std::basic_string_view<CharT>{substring});
	}

	///
	/// How many non-overlapping occurrences of the second string are found in the first string?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename SubstrT>
	static size_t occurrences(const StringT<CharT, T ...> & str, const SubstrT & substring) {
		size_t index = 0;
		size_t count = 0;

		while ((index = str.find(substring, index)) != StringT<CharT, T ...>::npos) {
			index += Balau::Impl::length(substring);
			++count;
		}

		return count;
	}

	///
	/// Count the number of non-overlapping occurrences of the supplied regular expression.
	///
	/// @todo u32 regex?
	///
	static size_t occurrences(const std::string & str, const std::regex & regex) {
		return occurrences(std::string_view{str}, regex);
	}

	///
	/// Count the number of non-overlapping occurrences of the supplied regular expression.
	///
	/// @todo u32 regex?
	///
	static size_t occurrences(const char * str, const std::regex & regex) {
		return occurrences(std::string_view{str}, regex);
	}

	///
	/// Count the number of non-overlapping occurrences of the supplied regular expression.
	///
	/// @todo u32 regex?
	///
	static size_t occurrences(const std::string_view & str, const std::regex & regex) {
		size_t count = 0;
		std::cregex_iterator iter(str.data(), str.data() + str.length(), regex);
		std::cregex_iterator end;

		while (iter != end) {
			++count;
			++iter;
		}

		return count;
	}

	///////////////////////////////////

	///
	/// Ignoring case, is the first string equal to the second string?
	///
	/// @todo Remove requirement for new string allocations.
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename SubstrT>
	static bool equalsIgnoreCase(const StringT<CharT, T ...> & lhs, const SubstrT & rhs) {
		return toLower(lhs) == toLower(rhs);
	}

	///
	/// Ignoring case, is the first string equal to the second string?
	///
	/// @todo Remove requirement for new string allocations.
	///
	template <typename CharT, typename SubstrT>
	static bool equalsIgnoreCase(const CharT * lhs, const SubstrT & rhs) {
		return toLower(std::basic_string_view<CharT>{lhs}) == toLower(std::basic_string_view<CharT>{rhs});
	}

	///////////////////////////////////

	///
	/// Determine the lengths of the lines in bytes, given the line break regular expression.
	///
	/// If the text ends with some non-line end characters, these characters form the
	/// last line and are counted in the line length results if the includeExtraTextAsLine
	/// boolean is true (the default).
	///
	/// @todo u32 regex?
	///
	template <typename ... T, template <typename ...> class StringT>
	static std::vector<size_t> lineLengths(const StringT<char, T ...> & text,
	                                       const std::regex & lineBreakRegex,
	                                       bool includeExtraTextAsLine = true) {
		std::vector<size_t> lengths;
		// A character iterator is used here in order to allow different string types.
		std::cregex_iterator iter(text.data(), text.data() + text.length(), lineBreakRegex);
		std::cregex_iterator end;
		long lineStart = 0;

		while (iter != end) {
			lengths.emplace_back(iter->position() - lineStart);
			lineStart = iter->position() + iter->length();
			++iter;
		}

		if (includeExtraTextAsLine && (long long) text.length() > lineStart) {
			lengths.emplace_back(text.length() - lineStart);
		}

		return lengths;
	}

	///
	/// Given the supplied multi-line text string, determine the length of each line in bytes.
	///
	/// If the text ends with some non-line end characters, these characters form the
	/// last line and are counted in the line length results if the includeExtraTextAsLine
	/// boolean is true (the default).
	///
	/// This function matches the following line endings: CRLN, LNCR, LN, CR.
	/// If a different end of line marker regular expression is required,
	/// use the alternative overload and supply the regular expression.
	///
	/// @todo u32 regex?
	///
	template <typename ... T, template <typename ...> class StringT>
	static std::vector<size_t> lineLengths(const StringT<char, T ...> & text, bool includeExtraTextAsLine = true) {
		return lineLengths(std::string_view{text}, StringsRegexImpl<0>::defaultLineBreakRegex, includeExtraTextAsLine);
	}

	///
	/// Given the supplied multi-line text string, determine the length of each line in bytes.
	///
	/// If the text ends with some non-line end characters, these characters form the
	/// last line and are counted in the line length results if the includeExtraTextAsLine
	/// boolean is true (the default).
	///
	/// This function matches the following line endings: CRLN, LNCR, LN, CR.
	/// If a different end of line marker regular expression is required,
	/// use the alternative overload and supply the regular expression.
	///
	/// @todo u32 regex?
	///
	static std::vector<size_t> lineLengths(const char * text, bool includeExtraTextAsLine = true) {
		return lineLengths(std::string_view{text}, StringsRegexImpl<0>::defaultLineBreakRegex, includeExtraTextAsLine);
	}

	///////////////////////////////////

	///
	/// Get the character (UTF-8) / code point (UTF-32) position of the last index of the specified sub-string.
	///
	/// If the sub-string is not found npos is returned.
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename SubstrT>
	static size_t lastIndexOf(const StringT<CharT, T ...> & str, const SubstrT & substring) {
		return std::basic_string_view<CharT>{str}.rfind(std::basic_string_view<CharT>{substring});
	}

	///
	/// Get the character (UTF-8) / code point (UTF-32) position of the last index of the specified sub-string.
	///
	/// If the sub-string is not found npos is returned.
	///
	template <typename CharT, typename SubstrT>
	static size_t lastIndexOf(const CharT * str, const SubstrT & substring) {
		return std::basic_string_view<CharT>{str}.rfind(std::basic_string_view<CharT>{substring});
	}

	///////////////////////////////////

	///
	/// Count the number of code points in the supplied UTF-8 string.
	///
	/// This is an O(n) operation.
	///
	static size_t codePointCount(const std::string_view & str) {
		int offset = 0;
		size_t codePointCount = 0;

		while (offset < (int32_t) str.length()) {
			Character::advanceUtf8(str, offset);
			++codePointCount;
		}

		return codePointCount;
	}

	///
	/// Count the number of code points in the supplied UTF-32 string.
	///
	/// This is an O(1) operation.
	///
	static size_t codePointCount(const std::u32string_view & str) {
		return str.length();
	}

	///////////////////////////////// Mutation ////////////////////////////////

	///
	/// Convert the supplied UTF-8 string to uppercase.
	///
	///
	static std::string toUpper(const std::string_view & s) {
		std::string destination = std::string(s);
		int getOffset = 0;
		int setOffset = 0;

		while (getOffset < (int) s.length()) {
			char32_t c = Character::getNextUtf8(s, getOffset);
			char32_t upper = Character::toUpper(c);

			// Are all uppercase code points the same number of bytes as lowercase
			// code points? The algorithm currently assumes not. If this is not the
			// case, then this conditional code is not required.
			const size_t charByteCount = Character::utf8ByteCount(upper);
			if ((size_t) setOffset + charByteCount > destination.length()) {
				destination.resize((size_t) setOffset + charByteCount);
			}

			Character::setUtf8AndAdvanceOffset(destination, setOffset, upper);
		}

		// Ditto.
		if ((size_t) setOffset < destination.length()) {
			destination.resize((size_t) setOffset);
		}

		return destination;
	}

	///
	/// Convert the supplied UTF-32 string to uppercase.
	///
	static std::u32string toUpper(const std::u32string_view & s) {
		std::u32string destination = std::u32string(s);

		for (char32_t & m : destination) {
			m = Character::toUpper(m);
		}

		return destination;
	}

	///////////////////////////////////

	///
	/// Convert the supplied UTF-8 string to lowercase.
	///
	static std::string toLower(const std::string_view & s) {
		std::string destination = std::string(s);
		int getOffset = 0;
		int setOffset = 0;

		while (getOffset < (int) s.length()) {
			char32_t c = Character::getNextUtf8(s, getOffset);
			char32_t lower  = Character::toLower(c);

			// Are all lowercase code points the same number of bytes as uppercase
			// code points? The algorithm currently assumes not. If this is not the
			// case, then this conditional code is not required.
			const size_t charByteCount = Character::utf8ByteCount(lower);
			if ((size_t) setOffset + charByteCount > destination.length()) {
				destination.resize((size_t) setOffset + charByteCount);
			}

			Character::setUtf8AndAdvanceOffset(destination, setOffset, lower);
		}

		// Ditto.
		if ((size_t) setOffset < destination.length()) {
			destination.resize((size_t) setOffset);
		}

		return destination;
	}

	///
	/// Convert the supplied UTF-32 string to lowercase.
	///
	static std::u32string toLower(const std::u32string_view & s) {
		std::u32string destination = std::u32string{s};

		for (char32_t & m : destination) {
			m = Character::toLower(m);
		}

		return destination;
	}

	///////////////////////////////////

	///
	/// Appends count times the supplied ascii character to the supplied UTF-8 string (in place version).
	///
	/// @param str the UTF-8 string to append in place
	/// @param c the ascii character
	/// @param count the number of times to append the ascii character
	///
	static void append(std::string & str, char c, size_t count) {
		const size_t start = str.length();
		const size_t end = str.length() + count;
		str.resize(end);

		for (size_t m = start; m < end; m++) {
			str[m] = c;
		}
	}

	///
	/// Appends count times the supplied code point to the supplied UTF-8 string (in place version).
	///
	/// @param str the UTF-8 string to append in place
	/// @param c the code point
	/// @param count the number of times to append the code point
	///
	static void append(std::string & str, char32_t c, size_t count) {
		const size_t charByteCount = Character::utf8ByteCount(c);
		int offset = (int) str.length();
		const size_t end = str.length() + count * charByteCount;
		str.resize(end);

		for (size_t m = 0; m < count; m++) {
			Character::setUtf8AndAdvanceOffset(str, offset, c);
		}
	}

	///
	/// Appends count times the supplied code point to the supplied UTF-32 string (in place version).
	///
	/// @param str the UTF-32 string to append in place
	/// @param c the code point
	/// @param count the number of times to append the code point
	///
	static void append(std::u32string & str, char32_t c, size_t count) {
		const size_t start = str.length();
		const size_t end = str.length() + count;
		str.resize(end);

		for (size_t m = start; m < end; m++) {
			str[m] = c;
		}
	}

	///////////////////////////////////

	///
	/// Returns the supplied UTF-8 string with count times the supplied ascii character appended to it.
	///
	/// @param str the UTF-8 string to append onto
	/// @param c the ascii character
	/// @param count the number of times to append the ascii character
	/// @return the appended UTF-8 string
	///
	static std::string append(const std::string_view & str, char c, size_t count) {
		std::string ret{str};
		append(ret, c, count);
		return ret;
	}

	///
	/// Returns the supplied UTF-8 string with count times the supplied code point appended to it.
	///
	/// @param str the UTF-8 string to append in place
	/// @param c the code point
	/// @param count the number of times to append the code point
	/// @return the appended UTF-8 string
	///
	static std::string append(const std::string & str, char32_t c, size_t count) {
		std::string ret{str};
		append(ret, c, count);
		return ret;
	}

	///
	/// Returns the supplied UTF-32 string with count times the supplied code point appended to it.
	///
	/// @param str the UTF-32 string to append in place
	/// @param c the code point
	/// @param count the number of times to append the code point
	/// @return the appended UTF-32 string
	///
	static std::u32string append(const std::u32string & str, char32_t c, size_t count) {
		std::u32string ret{str};
		append(ret, c, count);
		return ret;
	}

	///////////////////////////////////

	///
	/// Left pad the supplied UTF-8 string up to the specified width in code points.
	///
	/// The supplied ascii character is used for padding. The default character is a space.
	///
	static std::string padLeft(const std::string_view & str, unsigned int width, char c = ' ') {
		const size_t count = codePointCount(str);

		if (width <= count) {
			return std::string{str};
		}

		std::string ret;
		append(ret, c, width - count);
		ret.append(std::string(str.data(), str.length()));
		return ret;
	}

	///
	/// Left pad the supplied UTF-8 string up to the specified width in code points.
	///
	/// The supplied code point is used for padding.
	///
	static std::string padLeft(const std::string_view & str, unsigned int width, char32_t c) {
		const size_t count = codePointCount(str);

		if (width <= count) {
			return std::string{str};
		}

		std::string ret;
		append(ret, c, width - count);
		ret.append(std::string(str.data(), str.length()));
		return ret;
	}

	///
	/// Left pad the supplied UTF-32 string up to the specified width in code points.
	///
	/// The supplied code point is used for padding. The default character is a space.
	///
	static std::u32string padLeft(const std::u32string_view & str, unsigned int width, char32_t c = U' ') {
		if (width <= str.length()) {
			return std::u32string{str};
		}

		std::u32string ret;
		append(ret, c, width - str.length());
		ret.append(std::u32string(str.data(), str.length()));
		return ret;
	}

	///////////////////////////////////

	///
	/// Right pad the supplied UTF-8 string up to the specified width in code points.
	///
	/// The supplied ascii character is used for padding. The default character is a space.
	///
	static std::string padRight(const std::string_view & str, unsigned int width, char c = ' ') {
		const size_t count = codePointCount(str);

		if (width <= count) {
			return std::string{str};
		}

		std::string ret = std::string{str};
		append(ret, c, width - count);
		return ret;
	}

	///
	/// Right pad the supplied UTF-8 string up to the specified width in code points.
	///
	/// The supplied code point is used for padding.
	///
	static std::string padRight(const std::string_view & str, unsigned int width, char32_t c) {
		const size_t count = codePointCount(str);

		if (width <= count) {
			return std::string(str);
		}

		std::string ret = std::string{str};
		append(ret, c, width - count);
		return ret;
	}

	///
	/// Right pad the supplied UTF-32 string up to the specified width in code points.
	///
	/// The supplied code point is used for padding. The default character is a space.
	///
	static std::u32string padRight(const std::u32string_view & str, unsigned int width, char32_t c = U' ') {
		if (width <= str.length()) {
			return std::u32string(str);
		}

		std::u32string ret = std::u32string{str};
		append(ret, c, width - str.length());
		return ret;
	}

	///////////////////////////////////

	///
	/// Trim whitespace from the beginning and end of the supplied UTF-8 string.
	///
	static std::string_view trim(const std::string_view & input) {
		int offset = 0;
		int startPosition = offset;

		while (offset < (int) input.length() && Character::isSpace(Character::getNextUtf8(input, offset))) {
			startPosition = offset;
		}

		offset = (int) input.length();
		int endPosition = offset;

		while (endPosition > 0 && Character::isSpace(Character::getPreviousUtf8(input, offset))) {
			endPosition = offset;
		}

		if (endPosition <= startPosition) {
			return std::string_view(input.data(), 0);
		}

		const auto length = endPosition - startPosition;
		return std::string_view(input.data() + (size_t) startPosition, (size_t) length);
	}

	///
	/// Trim whitespace from the beginning and end of the supplied UTF-32 string.
	///
	static std::u32string_view trim(const std::u32string_view & input) {
		size_t startPosition = 0;

		while (startPosition < input.length() && Character::isSpace(input[startPosition])) {
			++startPosition;
		}

		size_t endPosition = input.length();

		while (endPosition > 0 && Character::isSpace(input[endPosition - 1])) {
			--endPosition;
		}

		if (endPosition <= startPosition) {
			return std::u32string_view(input.data(), 0);
		}

		return std::u32string_view(input.data() + startPosition, endPosition - startPosition);
	}

	///////////////////////////////////

	///
	/// Trim whitespace from the beginning of the supplied UTF-8 string.
	///
	static std::string_view trimLeft(const std::string_view & input) {
		int offset = 0;
		int startPosition = offset;

		while (offset < (int) input.length() && Character::isSpace(Character::getNextUtf8(input, offset))) {
			startPosition = offset;
		}

		return std::string_view(input.data() + startPosition, input.length() - startPosition);
	}

	///
	/// Trim whitespace from the beginning of the supplied UTF-32 string.
	///
	static std::u32string_view trimLeft(const std::u32string_view & input) {
		size_t startPosition = 0;

		while (startPosition < input.length() && Character::isSpace(input[startPosition])) {
			++startPosition;
		}

		return std::u32string_view(input.data() + startPosition, input.length() - startPosition);
	}

	///////////////////////////////////

	///
	/// Trim whitespace from the end of the supplied UTF-8 string.
	///
	static std::string_view trimRight(const std::string_view & input) {
		auto offset = (int) input.length();
		int endPosition = offset;

		while (endPosition > 0 && Character::isSpace(Character::getPreviousUtf8(input, offset))) {
			endPosition = offset;
		}

		if (endPosition <= 0) {
			return std::string_view(input.data(), 0);
		}

		return std::string_view(input.data(), (size_t) endPosition);
	}

	///
	/// Trim whitespace from the end of the supplied UTF-32 string.
	///
	static std::u32string_view trimRight(const std::u32string_view & input) {
		size_t endPosition = input.length();

		while (endPosition > 0 && Character::isSpace(input[endPosition - 1])) {
			--endPosition;
		}

		if (endPosition <= 0) {
			return std::u32string_view(input.data(), 0);
		}

		return std::u32string_view(input.data(), (size_t) endPosition);
	}

	///////////////////////////////////

	///
	/// Simplify whitespace (conflate blanks) from the supplied UTF-8 string.
	///
	/// Multiple blanks (tab/space) are conflated to single blanks and whitespace from
	/// the start and end of the string is removed.
	///
	/// As this is a destructive process, a new string is returned.
	///
	static std::string simplify(const std::string_view & input) {
		static const std::regex conflater("[\t ]+");
		auto trimmed = trim(input);
		return replaceAll(trimmed, conflater, " ");
	}

	///////////////////////////////////

	///
	/// Replace all occurrences of the specified string with the supplied replacement.
	///
	template <typename CharT,
	          typename ... T,
	          template <typename ...> class StringT,
	          typename MatchT,
	          typename ReplacementT>
	static std::basic_string<CharT>
	replaceAll(const StringT<CharT, T ...> & input, const MatchT & match, const ReplacementT & replacement) {
		return replaceAll(
			  std::basic_string_view<CharT>{input}
			, std::basic_string_view<CharT>{match}
			, std::basic_string_view<CharT>{replacement}
		);
	}

	///
	/// Replace all occurrences of the specified string with the supplied replacement (specified allocator version).
	///
	template <typename CharT,
	          typename AllocatorT,
	          typename ... T,
	          template <typename ...> class StringT,
	          typename MatchT,
	          typename ReplacementT>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>
	replaceAll(const StringT<CharT, T ...> & input, const MatchT & match, const ReplacementT & replacement) {
		return replaceAll(
			  std::basic_string_view<CharT>{input}
			, std::basic_string_view<CharT>{match}
			, std::basic_string_view<CharT>{replacement}
		);
	}

	///
	/// Replace all occurrences of the specified string with the supplied replacement.
	///
	template <typename CharT, typename AllocatorT, typename MatchT, typename ReplacementT>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> replaceAll(const CharT * input,
	                                           const MatchT & match,
	                                           const ReplacementT & replacement) {
		return replaceAll(
			  std::basic_string_view<CharT>{input}
			, std::basic_string_view<CharT>{match}
			, std::basic_string_view<CharT>{replacement}
		);
	}

	///
	/// Replace all occurrences of the specified string with the supplied replacement.
	///
	template <typename CharT, typename AllocatorT>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>
	replaceAll(std::basic_string_view<CharT> input,
	           std::basic_string_view<CharT> match,
	           std::basic_string_view<CharT> replacement,
	           size_t * count = nullptr) {
		std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> ret(input);
		auto m = std::string(match.data(), match.length());
		auto r = std::string(replacement.data(), replacement.length());
		Balau::Impl::replaceAllImpl(ret, m, r, count);
		return ret;
	}

	///
	/// Replace all occurrences of the specified string with the supplied replacement.
	///
	template <typename CharT>
	static std::basic_string<CharT> replaceAll(std::basic_string_view<CharT> input,
	                                           std::basic_string_view<CharT> match,
	                                           std::basic_string_view<CharT> replacement,
	                                           size_t * count = nullptr) {
		std::basic_string<CharT> ret(input);
		auto m = std::string(match.data(), match.length());
		auto r = std::string(replacement.data(), replacement.length());
		Balau::Impl::replaceAllImpl(ret, m, r, count);
		return ret;
	}

	///
	/// Replace all occurrences of the specified string with the supplied replacement.
	///
	static std::string replaceAll(std::string_view input,
	                              std::string_view match,
	                              std::string_view replacement,
	                              size_t * count = nullptr) {
		std::string ret(input);
		auto m = std::string(match.data(), match.length());
		auto r = std::string(replacement.data(), replacement.length());
		Balau::Impl::replaceAllImpl(ret, m, r, count);
		return ret;
	}

	///
	/// Replace all occurrences of the specified string with the supplied replacement.
	///
	static std::u32string replaceAll(std::u32string_view input,
	                                 std::u32string_view match,
	                                 std::u32string_view replacement,
	                                 size_t * count = nullptr) {
		std::u32string ret(input);
		auto m = std::u32string(match.data(), match.length());
		auto r = std::u32string(replacement.data(), replacement.length());
		Balau::Impl::replaceAllImpl(ret, m, r, count);
		return ret;
	}

	///
	/// Replace all occurrences of the regular expression in the input string with the replacement string.
	///
	/// @todo UTF-32 version?
	///
	template <typename ReplacementT>
	static std::string replaceAll(const char * input, const std::regex & regex, const ReplacementT & replacement) {
		return replaceAll(std::string_view{input}, regex, replacement);
	}

	///
	/// Replace all occurrences of the regular expression in the input string with the replacement string.
	///
	/// @todo UTF-32 version?
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename ReplacementT>
	static std::string replaceAll(const StringT<CharT, T ...> & input,
	                              const std::regex & regex,
	                              const ReplacementT & replacement) {
		std::ostringstream stream;

		auto begin = std::cregex_iterator(Strings::begin(input), Strings::end(input), regex);
		auto end = std::cregex_iterator();

		if (begin == end) {
			return std::string{input};
		}

		std::cmatch match;

		for (std::cregex_iterator m = begin; m != end; m++) {
			match = *m;
			stream << match.prefix();
			stream << replacement;
		}

		stream << match.suffix();

		return stream.str();
	}

	/////////////////////////////// Manipulation //////////////////////////////

	///
	/// Call to-string on each of the objects and join the resulting strings together with the delimiter.
	///
	/// @param delimiter the string delimiter
	/// @param p1 the first object
	/// @param p subsequent objects
	///
	template <typename AllocatorT, typename CharT, typename ... T, template <typename ...> class StringT, typename P1, typename ... P>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> join(StringT<CharT, T ...> delimiter, const P1 & p1, const P & ... p) {
		return (::ToString<CharT, AllocatorT>()(p1) + ... + (delimiter + ::ToString<CharT, AllocatorT>()(p)));
	}

	///
	/// Call to-string on each of the objects and join the resulting strings together with the delimiter.
	///
	/// @param delimiter the string delimiter
	/// @param p1 the first object
	/// @param p subsequent objects
	///
	template <typename CharT, typename ... T, template <typename ...> class StringT, typename P1, typename ... P>
	static std::basic_string<CharT> join(StringT<CharT, T ...> delimiter, const P1 & p1, const P & ... p) {
		return (::ToString<CharT, std::allocator<CharT>>()(p1) + ... + (delimiter + ::ToString<CharT, std::allocator<CharT>>()(p)));
	}

	///
	/// Call to-string on each of the objects and join the resulting strings together with the delimiter.
	///
	/// @param delimiter the string delimiter
	/// @param p1 the first object
	/// @param p subsequent objects
	///
	template <typename AllocatorT, typename CharT, typename P1, typename ... P>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> join(const CharT * delimiter, const P1 & p1, const P & ... p) {
		return (::ToString<CharT, AllocatorT>()(p1) + ... + (delimiter + ::ToString<CharT, AllocatorT>()(p)));
	}

	///
	/// Call to-string on each of the objects and join the resulting strings together with the delimiter.
	///
	/// @param delimiter the string delimiter
	/// @param p1 the first object
	/// @param p subsequent objects
	///
	template <typename CharT, typename P1, typename ... P>
	static std::basic_string<CharT> join(const CharT * delimiter, const P1 & p1, const P & ... p) {
		return (::ToString<CharT, std::allocator<CharT>>()(p1) + ... + (delimiter + ::ToString<CharT, std::allocator<CharT>>()(p)));
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the delimiter.
	///
	/// Note that the delimiter in the parameter list of this function is reversed
	/// compared to the other join functions. This is done merely to ensure the
	/// correct template function is selected for containers.
	///
	/// @param container the container of objects
	/// @param delimiter the UTF-8 string delimiter
	///
	template <typename AllocatorT, typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>
	joinContainer(const CharT * delimiter, const C<T ...> & container) {
		return joinContainer(std::basic_string_view<CharT>(delimiter), container);
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the delimiter.
	///
	/// Note that the delimiter in the parameter list of this function is reversed
	/// compared to the other join functions. This is done merely to ensure the
	/// correct template function is selected for containers.
	///
	/// @param container the container of objects
	/// @param delimiter the UTF-8 string delimiter
	///
	template <typename CharT, typename ... T, template <typename ...> class C> static std::basic_string<CharT>
	joinContainer(const CharT * delimiter, const C<T ...> & container) {
		return joinContainer(std::basic_string_view<CharT>(delimiter), container);
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the delimiter.
	///
	/// Note that the delimiter in the parameter list of this function is reversed
	/// compared to the other join functions. This is done merely to ensure the
	/// correct template function is selected for containers.
	///
	/// @param container the container of objects
	/// @param delimiter the UTF-8 string delimiter
	///
	template <typename AllocatorT, typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>
	joinContainer(const std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> & delimiter, const C<T ...> & container) {
		return joinContainer(std::basic_string_view<CharT>(delimiter), container);
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the delimiter.
	///
	/// Note that the delimiter in the parameter list of this function is reversed
	/// compared to the other join functions. This is done merely to ensure the
	/// correct template function is selected for containers.
	///
	/// @param container the container of objects
	/// @param delimiter the UTF-8 string delimiter
	///
	template <typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT> joinContainer(const std::basic_string<CharT> & delimiter, const C<T ...> & container) {
		return joinContainer(std::basic_string_view<CharT>(delimiter), container);
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the delimiter.
	///
	/// @param container the container of objects
	/// @param delimiter the UTF-8 string view delimiter
	///
	template <typename AllocatorT, typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> joinContainer(std::basic_string_view<CharT> delimiter, const C<T ...> & container) {
		std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> builder;
		std::basic_string_view<CharT> d;

		for (const auto & e : container) {
			builder.append(std::basic_string<CharT>(d.data(), d.length()));
			builder.append(::ToString<CharT, std::allocator<CharT>>()(e));
			d = delimiter;
		}

		return builder;
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the delimiter.
	///
	/// @param container the container of objects
	/// @param delimiter the UTF-8 string view delimiter
	///
	template <typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT> joinContainer(std::basic_string_view<CharT> delimiter, const C<T ...> & container) {
		std::basic_string<CharT> builder;
		std::basic_string_view<CharT> d;

		for (const auto & e : container) {
			builder.append(std::basic_string<CharT>(d.data(), d.length()));
			builder.append(::ToString<CharT, std::allocator<CharT>>()(e));
			d = delimiter;
		}

		return builder;
	}

	///////////////////////////////////

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the prefix and suffix.
	///
	/// Print the prefix before each string and the suffix after each string.
	///
	/// @param container the container of objects
	/// @param prefix the string prefix
	/// @param suffix the string suffix
	///
	template <typename AllocatorT,
	          typename CharT,
	          typename ... T,
	          template <typename ...> class C,
	          typename ... U,
	          template <typename ...> class PrefixT,
	          typename ... V,
	          template <typename ...> class SuffixT>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>
	prefixSuffixJoin(const C<T ...> & container, const PrefixT<CharT, U ...> & prefix, const SuffixT<CharT, V ...> & suffix) {
		std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> builder;

		for (const auto & e : container) {
			builder.append(prefix + ::ToString<CharT, AllocatorT>()(e) + suffix);
		}

		return builder;
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the prefix and suffix.
	///
	/// Print the prefix before each string and the suffix after each string.
	///
	/// @param container the container of objects
	/// @param prefix the string prefix
	/// @param suffix the string suffix
	///
	template <typename CharT,
	          typename ... T,
	          template <typename ...> class C,
	          typename ... U,
	          template <typename ...> class PrefixT,
	          typename ... V,
	          template <typename ...> class SuffixT>
	static std::string prefixSuffixJoin(const C<T ...> & container,
	                                    const PrefixT<CharT, U ...> & prefix,
	                                    const SuffixT<CharT, V ...> & suffix) {
		return prefixSuffixJoin<std::allocator<CharT>>(container, prefix, suffix);
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the prefix and suffix.
	///
	/// Print the prefix before each string and the suffix after each string.
	///
	/// @param container the container of objects
	/// @param prefix the string prefix
	/// @param suffix the string suffix
	///
	template <typename AllocatorT, typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>
	prefixSuffixJoin(const C<T ...> & container, const CharT * prefix, const CharT * suffix) {
		std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> builder;

		for (const auto & e : container) {
			builder.append(
				  std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>{prefix}
				+ ::ToString<CharT, AllocatorT>()(e)
				+ suffix
			);
		}

		return builder;
	}

	///
	/// Call to-string on each of the objects in the container and join the resulting strings together with the prefix and suffix.
	///
	/// Print the prefix before each string and the suffix after each string.
	///
	/// @param container the container of objects
	/// @param prefix the string prefix
	/// @param suffix the string suffix
	///
	template <typename CharT, typename ... T, template <typename ...> class C>
	static std::basic_string<CharT> prefixSuffixJoin(const C<T ...> & container, const CharT * prefix, const CharT * suffix) {
		return prefixSuffixJoin<std::allocator<CharT>>(container, prefix, suffix);
	}

	///////////////////////////////////

	///
	/// Split the input string on each of the occurrences of the specified delimiter regular expression.
	///
	/// The input string must remain valid after the call, or the resulting string
	/// views will be invalid.
	///
	/// @param input the input string to split
	/// @param delimiter the regular expression delimiter marking each split
	/// @param returnDelimiters when true, the delimiter matches are also returned
	/// @param compress false returns empty strings where multiple delimiters touch
	///
	static std::vector<std::string_view> split(const std::string_view & input,
	                                           const std::regex & delimiter,
	                                           bool returnDelimiters = false,
	                                           bool compress = true) {
		std::vector<std::string_view> elements;
		auto begin = std::cregex_iterator(input.data(), input.data() + input.length(), delimiter);
		auto end = std::cregex_iterator();

		if (begin == end) {
			if (!input.empty()) {
				elements.emplace_back(input.data(), input.length());
			}

			return elements;
		}

		long currentStart = 0;

		for (auto i = begin; i != end; ++i) {
			std::cmatch match = *i;

			const auto pos = match.position(0);
			const auto len = match.length(0);

			if (pos > currentStart || !compress) {
				elements.emplace_back(input.data() + currentStart, pos - currentStart);
			}

			currentStart = pos + len;

			if (returnDelimiters) {
				elements.emplace_back(input.data() + pos, len);
			}

			auto j = i;
			++j;

			if (j == end && (currentStart < (long long) input.length() || !compress)) {
				elements.emplace_back(input.data() + currentStart, input.length() - currentStart);
			}
		}

		return elements;
	}

	///
	/// Split the input string on each of the occurrences of the specified delimiter.
	///
	/// @param input the input string to split
	/// @param delimiter the string delimiter marking each split
	/// @param compress false returns empty strings where multiple delimiters touch
	///
	static std::vector<std::string_view> split(const std::string_view & input,
	                                           const std::string_view & delimiter,
	                                           bool compress = true) {
		return splitImpl(input, delimiter, compress);
	}

	///
	/// Split the input string on each of the occurrences of the specified delimiter.
	///
	/// @param input the input string to split
	/// @param delimiter the string delimiter marking each split
	/// @param compress false returns empty strings where multiple delimiters touch
	///
	static std::vector<std::u32string_view> split(const std::u32string_view & input,
	                                              const std::u32string_view & delimiter,
	                                              bool compress = true) {
		return splitImpl(input, delimiter, compress);
	}

	///////////////////////////////////

	///
	/// Split the input string on each of the occurrences of the specified delimiter regular expression and subsequently trim the resulting strings.
	///
	/// The input string must remain valid after the call, or the resulting string
	/// views will be invalid.
	///
	/// @param input the input string to split
	/// @param delimiter the regular expression delimiter marking each split
	/// @param returnDelimiters when true, the delimiter matches are also returned
	/// @param compress false returns empty strings where multiple delimiters touch
	///
	static std::vector<std::string_view> splitAndTrim(const std::string_view & input,
	                                                  const std::regex & delimiter,
	                                                  bool returnDelimiters = false,
	                                                  bool compress = true) {
		auto elements = split(input, delimiter, returnDelimiters, compress);

		for (auto & element : elements) {
			element = trim(element);
		}

		return elements;
	}

	///
	/// Split the input string on each of the occurrences of the specified delimiter and subsequently trim the resulting strings.
	///
	/// @param input the input string to split
	/// @param delimiter the string delimiter marking each split
	/// @param compress false returns empty strings where multiple delimiters touch
	///
	static std::vector<std::string_view> splitAndTrim(const std::string_view & input,
	                                                  const std::string_view & delimiter,
	                                                  bool compress = true) {
		auto elements = split(input, delimiter, compress);

		for (auto & element : elements) {
			element = trim(element);
		}

		return elements;
	}

	///
	/// Split the input string on each of the occurrences of the specified delimiter and subsequently trim the resulting strings.
	///
	/// @param input the input string to split
	/// @param delimiter the string delimiter marking each split
	/// @param compress false returns empty strings where multiple delimiters touch
	///
	static std::vector<std::u32string_view> splitAndTrim(const std::u32string_view & input,
	                                                     const std::u32string_view & delimiter,
	                                                     bool compress = true) {
		auto elements = split(input, delimiter, compress);

		for (auto & element : elements) {
			element = trim(element);
		}

		return elements;
	}

	//////////////////////////////// Conversion ///////////////////////////////

	///
	/// Convert the container of string views to a container of strings.
	///
	///
	template <typename CharT, typename ... T, template <typename ...> class ContainerT>
	static ContainerT<std::basic_string<CharT>> toStringContainer(const ContainerT<std::basic_string_view<CharT>, T ...> & input) {
		ContainerT<std::basic_string<CharT>> ret;
		std::transform(input.begin(), input.end(), std::back_inserter(ret), [] (auto s) { return std::string(s); });
		return ret;
	}

	////////////////////////// Private implementation /////////////////////////

	Strings() = delete;
	Strings(const Strings &) = delete;
	Strings & operator = (const Strings &) = delete;

	private: template <typename CharT>
	static std::vector<std::basic_string_view<CharT>> splitImpl(std::basic_string_view<CharT> input,
	                                                            std::basic_string_view<CharT> delimiter,
	                                                            bool compress = true) {
		std::vector<std::basic_string_view<CharT>> elements;
		const size_t delimiterLength = Balau::Impl::length(delimiter);

		if (Balau::Impl::length(delimiter) == 0) {
			elements.emplace_back(input.data(), input.length());
			return elements;
		}

		std::string builder;
		size_t previousIndex = 0;

		for (size_t currentIndex = 0; currentIndex < input.length(); ++currentIndex) {
			if (input[currentIndex] == delimiter[0]) {
				if (delimiterFound(input, currentIndex, delimiter)) {
					if (previousIndex != currentIndex || !compress) {
						elements.emplace_back(input.data() + previousIndex, currentIndex - previousIndex);
					}

					currentIndex += delimiterLength - 1;
					previousIndex = currentIndex + 1;
				}
			}
		}

		// TODO finish
		if (previousIndex != input.length() || !compress) {
			elements.emplace_back(input.data() + previousIndex, input.length() - previousIndex);
		}

		return elements;
	}

	// Used in splitImpl method.
	private: template <typename CharT, typename ... T, template <typename ...> class StringT, typename DelimiterT>
	static bool delimiterFound(const StringT<CharT, T ...> & input, size_t currentIndex, const DelimiterT & delimiter) {
		size_t inputIndex = currentIndex;
		size_t delimiterIndex = 0;
		const size_t delimiterLength = Balau::Impl::length(delimiter);

		while (inputIndex < input.length() && delimiterIndex < delimiterLength) {
			if (input[inputIndex] != delimiter[delimiterIndex]) {
				return false;
			}

			++inputIndex;
			++delimiterIndex;
		}

		return delimiterIndex == delimiterLength;
	}

	private: template <typename CharT, typename AllocatorT> static auto begin(const std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> & str) {
		return str.data();
	}

	private: template <typename CharT> static auto begin(const CharT * str) {
		return str;
	}

	private: template <typename CharT> static auto begin(std::basic_string_view<CharT> str) {
		return str.data();
	}

	private: template <typename CharT, typename AllocatorT> static auto end(const std::basic_string<CharT, std::char_traits<CharT>, AllocatorT> & str) {
		return str.data() + str.length();
	}

	private: template <typename CharT> static auto end(const CharT * str) {
		return str + Balau::Impl::length(str);
	}

	private: template <typename CharT> static auto end(std::basic_string_view<CharT> str) {
		return str.data() + str.length();
	}

	private: template <int> struct StringsRegexImpl {
		static const std::regex defaultLineBreakRegex;
	};
};

template <int Unused> const std::regex Strings::StringsRegexImpl<Unused>::defaultLineBreakRegex = std::regex("\n\r|\r\n|\n|\r");

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__STRINGS
