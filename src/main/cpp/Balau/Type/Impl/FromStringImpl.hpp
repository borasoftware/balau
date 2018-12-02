//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__FROM_STRING_IMPL
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__FROM_STRING_IMPL

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Type/Character.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>

namespace Balau {

namespace Impl {

// Checks that the string is not empty and throws a conversion exception if it is.
inline void validateFromStringValue(const std::string & value) {
	if (value.empty()) {
		ThrowBalauException(
			Balau::Exception::ConversionException, "Attempt to convert value from empty UTF-8 string."
		);
	}
}

// Checks that the string is not empty and throws a conversion exception if it is.
inline void validateFromString16Value(const std::u16string & value) {
	if (value.empty()) {
		ThrowBalauException(
			Balau::Exception::ConversionException, "Attempt to convert value from empty UTF-16 string."
		);
	}
}

// Checks that the string is not empty and throws a conversion exception if it is.
inline void validateFromString32Value(const std::u32string & value) {
	if (value.empty()) {
		ThrowBalauException(
			Balau::Exception::ConversionException, "Attempt to convert value from empty UTF-32 string."
		);
	}
}

// Checks that the character is valid and throws a conversion exception if it is not.
inline void validateCharacter(char32_t c) {
	if (!Balau::Character::isValid(c)) {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, "Invalid character or unsuccessful conversion during fromString call."
		);
	}
}

inline void validateIntegerConversion(const std::string & value, const char * strEnd) {
	if (strEnd != value.c_str() + value.length()) {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, ::toString("String to integer conversion of \"", value, "\" failed.")
		);
	}
}

template <typename DstT, typename SrcT> void checkLowerLimit(SrcT n) {
	const SrcT lowerLimit = std::numeric_limits<DstT>::lowest();

	if (n < lowerLimit) {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, ::toString(
				  "String to numeric conversion of \""
				, n
				, "\" is out of bounds for type "
				, boost::core::demangle(typeid(DstT).name())
				, "."
			)
		);
	}
}

template <typename DstT, typename SrcT> void checkUpperLimit(SrcT n) {
	const SrcT upperLimit = std::numeric_limits<DstT>::max();

	if (n > upperLimit) {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, ::toString(
				  "String to numeric conversion of \""
				, n
				, "\" is out of bounds for type "
				, boost::core::demangle(typeid(DstT).name())
				, "."
			)
		);
	}
}

template <typename DstT, typename SrcT> inline DstT safeNumericCast(SrcT n) {
	checkLowerLimit<DstT, SrcT>(n);
	checkUpperLimit<DstT, SrcT>(n);
	return static_cast<DstT>(n);
}

inline long stringToLong(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	long n = std::strtol(value.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

inline long long stringToLongLong(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	long long n = std::strtoll(value.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

inline unsigned long stringToUnsignedLong(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	unsigned long n = std::strtoul(value.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

inline unsigned long long stringToUnsignedLongLong(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	unsigned long long n = std::strtoull(value.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

inline float stringToFloat(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	float n = std::strtof(value.c_str(), &strEnd);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

inline double stringToDouble(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	double n = std::strtod(value.c_str(), &strEnd);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

inline long double stringToLongDouble(const std::string & value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	long double n = std::strtold(value.c_str(), &strEnd);
	Impl::validateIntegerConversion(value, strEnd);
	return n;
}

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__FROM_STRING_IMPL
