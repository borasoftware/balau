// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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
#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__FROM_STRING_IMPL
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__FROM_STRING_IMPL

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Type/Character.hpp>

#include <boost/core/demangle.hpp>

namespace Balau::Impl {

// Checks that the string is not empty and throws a conversion exception if it is.
inline void validateFromStringValue(std::string_view value) {
	if (value.empty()) {
		ThrowBalauException(
			Balau::Exception::ConversionException, "Attempt to convert value from empty UTF-8 string."
		);
	}
}

// Checks that the string is not empty and throws a conversion exception if it is.
inline void validateFromString16Value(std::u16string_view value) {
	if (value.empty()) {
		ThrowBalauException(
			Balau::Exception::ConversionException, "Attempt to convert value from empty UTF-16 string."
		);
	}
}

// Checks that the string is not empty and throws a conversion exception if it is.
inline void validateFromString32Value(std::u32string_view value) {
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
	using ::toString;

	if (strEnd != value.c_str() + value.length()) {
		ThrowBalauException(
			  Balau::Exception::ConversionException
			, toString("String to integer conversion of \"", value, "\" failed.")
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

//
// TODO These will be updated with std::from_chars calls such as:
//   std::from_chars_result result = std::from_chars(value.data(), value.data() + value.length(), base);
//

inline long stringToLong(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	long n = std::strtol(s.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline long stringToLong(std::string_view value, int base) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	long n = std::strtol(s.c_str(), &strEnd, base);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline long long stringToLongLong(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	long long n = std::strtoll(s.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline long long stringToLongLong(std::string_view value, int base) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	long long n = std::strtoll(s.c_str(), &strEnd, base);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline unsigned long stringToUnsignedLong(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	unsigned long n = std::strtoul(s.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline unsigned long stringToUnsignedLong(std::string_view value, int base) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	unsigned long n = std::strtoul(s.c_str(), &strEnd, base);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline unsigned long long stringToUnsignedLongLong(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	unsigned long long n = std::strtoull(s.c_str(), &strEnd, 10);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline unsigned long long stringToUnsignedLongLong(std::string_view value, int base) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	unsigned long long n = std::strtoull(s.c_str(), &strEnd, base);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline float stringToFloat(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	float n = std::strtof(s.c_str(), &strEnd);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline double stringToDouble(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	double n = std::strtod(s.c_str(), &strEnd);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

inline long double stringToLongDouble(std::string_view value) {
	Impl::validateFromStringValue(value);
	char * strEnd;
	const auto s = std::string(value);
	long double n = std::strtold(s.c_str(), &strEnd);
	Impl::validateIntegerConversion(s, strEnd);
	return n;
}

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__FROM_STRING_IMPL
