//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file FromString.hpp
///
/// Pre-defined universal from-string functions.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__FROM_STRING
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__FROM_STRING

#include <Balau/Type/Impl/FromStringImpl.hpp>
#include <Balau/Util/Strings.hpp>

///////////////////// Universal UTF-8 from-string function //////////////////////

///
/// Sets the destination value to the supplied value.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString(std::string & destination, std::string_view value) {
	destination = value;
}

///
/// Sets the destination value to a UTF-16 version of the supplied UTF-8 string.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString(std::u16string & destination, std::string_view value) {
	destination = boost::locale::conv::utf_to_utf<char16_t, char>(std::string(value));
}

///
/// Sets the destination value to a UTF-32 version of the supplied UTF-8 string.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString(std::u32string & destination, std::string_view value) {
	destination = boost::locale::conv::utf_to_utf<char32_t, char>(std::string(value));
}

///
/// Sets the destination value to the first UTF-8 character in the supplied UTF-8 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string contains invalid UTF-8 text;
///  - the code point does not fit in an ascii char.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(char & destination, std::string_view value) {
	Balau::Impl::validateFromStringValue(value);
	int offset = 0;
	char32_t c = Balau::Character::getNextUtf8Safe(value, offset);
	Balau::Impl::validateCharacter(c);

	if (c > 255) {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, toString("Cannot convert code point ", (long long) c, " to ascii char.")
		);
	}

	destination = static_cast<char>(c);
}

///
/// Sets the destination value to the first code point in the supplied UTF-8 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string contains invalid UTF-8 text.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(char32_t & destination, std::string_view value) {
	Balau::Impl::validateFromStringValue(value);
	int offset = 0;
	destination = Balau::Character::getNextUtf8Safe(value, offset);
	Balau::Impl::validateCharacter(destination);
}

///
/// Converts the supplied string to a signed char value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(signed char & destination, std::string_view value) {
	long n = Balau::Impl::stringToLong(value);
	destination = Balau::Impl::safeNumericCast<signed char>(n);
}

///
/// Converts the supplied string to a signed short value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(short & destination, std::string_view value) {
	long n = Balau::Impl::stringToLong(value);
	destination = Balau::Impl::safeNumericCast<short>(n);
}

///
/// Converts the supplied string to a signed int value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(int & destination, std::string_view value) {
	long n = Balau::Impl::stringToLong(value);
	destination = Balau::Impl::safeNumericCast<int>(n);
}

///
/// Converts the supplied string to a signed long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(long & destination, std::string_view value) {
	destination = Balau::Impl::stringToLong(value);
}

///
/// Converts the supplied string to a signed long long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(long long & destination, std::string_view value) {
	destination = Balau::Impl::stringToLongLong(value);
}

///
/// Converts the supplied string to a unsigned char value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(unsigned char & destination, std::string_view value) {
	unsigned long n = Balau::Impl::stringToUnsignedLong(value);
	destination = Balau::Impl::safeNumericCast<unsigned char>(n);
}

///
/// Converts the supplied string to a unsigned short value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(unsigned short & destination, std::string_view value) {
	unsigned long n = Balau::Impl::stringToUnsignedLong(value);
	destination = Balau::Impl::safeNumericCast<unsigned short>(n);
}

///
/// Converts the supplied string to a unsigned int value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(unsigned int & destination, std::string_view value) {
	unsigned long n = Balau::Impl::stringToUnsignedLong(value);
	destination = Balau::Impl::safeNumericCast<unsigned int>(n);
}

///
/// Converts the supplied string to a unsigned long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(unsigned long & destination, std::string_view value) {
	destination = Balau::Impl::stringToUnsignedLong(value);
}

///
/// Converts the supplied string to a unsigned long long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(unsigned long long & destination, std::string_view value) {
	destination = Balau::Impl::stringToUnsignedLongLong(value);
}

///
/// Converts the supplied string to a single precision floating point value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(float & destination, std::string_view value) {
	destination = Balau::Impl::stringToFloat(value);
}

///
/// Converts the supplied string to a double precision floating point value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(double & destination, std::string_view value) {
	destination = Balau::Impl::stringToDouble(value);
}

///
/// Converts the supplied string to a long double precision floating point value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(long double & destination, std::string_view value) {
	destination = Balau::Impl::stringToLongDouble(value);
}

///
/// Converts the supplied string to a boolean value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string does not contain one of the case insensitive strings "false" or "true".
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString(bool & destination, std::string_view value) {
	const std::string lower = Balau::Util::Strings::toLower(value);

	if (lower == "true") {
		destination = true;
	} else if (lower == "false") {
		destination = false;
	} else {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, toString("Cannot convert \"", value, "\" to boolean.")
		);
	}
}

///////////////////// Universal UTF-16 from-string function /////////////////////

///
/// Sets the destination value to a UTF-8 version of the supplied UTF-16 string.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString16(std::string & destination, std::u16string_view value) {
	destination = boost::locale::conv::utf_to_utf<char, char16_t>(std::u16string(value));
}

///
/// Sets the destination value to the supplied value.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString16(std::u16string & destination, std::u16string_view value) {
	destination = value;
}

///
/// Sets the destination value to a UTF-32 version of the supplied UTF-16 string.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString16(std::u32string & destination, std::u16string_view value) {
	destination = boost::locale::conv::utf_to_utf<char32_t, char16_t>(std::u16string(value));
}

///
/// Sets the destination value to the first character in the supplied UTF-16 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the code point does not fit in an ascii char.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(char & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to the first code point in the supplied UTF-16 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(char32_t & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed char value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(signed char & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	long n = Balau::Impl::stringToLong(valueStr);
	destination = Balau::Impl::safeNumericCast<signed char>(n);
}

///
/// Converts the supplied string to a signed short value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(short & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed int value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(int & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(long & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed long long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(long long & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned char value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(unsigned char & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned short value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(unsigned short & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned int value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(unsigned int & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(unsigned long & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned long long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(unsigned long long & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to Convert the supplied float to a UTF-16 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(float & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to Convert the supplied double to a UTF-16 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(double & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to Convert the supplied long double to a UTF-16 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(long double & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a boolean value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string does not contain one of the case insensitive strings "false" or "true".
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString16(bool & destination, std::u16string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///////////////////// Universal UTF-32 from-string function /////////////////////

///
/// Sets the destination value to a UTF-8 version of the supplied UTF-32 string.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString32(std::string & destination, std::u32string_view value) {
	destination = boost::locale::conv::utf_to_utf<char, char32_t>(std::u32string(value));
}

///
/// Sets the destination value to a UTF-16 version of the supplied UTF-32 string.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString32(std::u16string & destination, std::u32string_view value) {
	destination = boost::locale::conv::utf_to_utf<char16_t, char32_t>(std::u32string(value));
}

///
/// Sets the destination value to the supplied value.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
///
inline void fromString32(std::u32string & destination, std::u32string_view value) {
	destination = value;
}

///
/// Sets the destination value to the first character in the supplied UTF-32 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the code point does not fit in an ascii char.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(char & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to the first code point in the supplied UTF-32 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(char32_t & destination, std::u32string_view value) {
	Balau::Impl::validateFromString32Value(value);
	destination = value[0];
}

///
/// Converts the supplied string to a signed char value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(signed char & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed short value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(short & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed int value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(int & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(long & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a signed long long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(long long & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned char value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(unsigned char & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned short value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(unsigned short & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned int value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(unsigned int & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(unsigned long & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a unsigned long long value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(unsigned long long & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to Convert the supplied float to a UTF-32 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(float & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to Convert the supplied double to a UTF-32 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(double & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Sets the destination value to Convert the supplied long double to a UTF-32 string.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string is empty;
///  - the string to number conversion fails;
///  - the resulting numeric value is out of the valid range for the numeric type.
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(long double & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

///
/// Converts the supplied string to a boolean value.
///
/// A ConversionException will be thrown under the following circumstances:
///  - the string does not contain one of the case insensitive strings "false" or "true".
///
/// @param destination the destination value that is set via assignment
/// @param value the string input
/// @throw ConversionException when the conversion fails
///
inline void fromString32(bool & destination, std::u32string_view value) {
	std::string valueStr = toString(value);
	fromString(destination, valueStr);
}

/////////////// Universal from-string function - templated version //////////////

///
/// Template class based from-string function for use in template classes.
///
template <typename> struct FromString;

///
/// UTF-8 specialisation of FromString<T>.
///
/// Converts the supplied std::string to an object of type T by calling fromString.
///
template <> struct FromString<char> {
	///
	/// @param destination the destination value that is set via assignment
	/// @param value the string input
	/// @throw ConversionException when the conversion fails
	///
	template <typename T> void operator () (T & destination, std::string_view value) const {
		fromString(destination, value);
	}
};

///
/// UTF-16 specialisation of FromString<T>.
///
/// Converts the supplied std::u16string to an object of type T by calling fromString16.
///
template <> struct FromString<char16_t> {
	///
	/// @param destination the destination value that is set via assignment
	/// @param value the string input
	/// @throw ConversionException when the conversion fails
	///
	template <typename T> void operator () (T & destination, std::u16string_view value) const {
		fromString16(destination, value);
	}
};

///
/// UTF-32 specialisation of FromString<T>.
///
/// Converts the supplied std::u32string to an object of type T by calling fromString32.
///
template <> struct FromString<char32_t> {
	///
	/// @param destination the destination value that is set via assignment
	/// @param value the string input
	/// @throw ConversionException when the conversion fails
	///
	template <typename T> void operator () (T & destination, std::u32string_view value) const {
		fromString32(destination, value);
	}
};

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__FROM_STRING
