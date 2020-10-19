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

///
/// @file ToStringS.hpp
///
/// Pre-defined universal to-string functions for std::allocator based strings (i.e. std::string).
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_S
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_S

#include <Balau/Type/StdTypes.hpp>
#include <Balau/Type/Impl/ToStringImpl.hpp>

#include <codecvt>
#include <complex>
#include <forward_list>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <locale>

template <typename P1, typename P2, typename ... P>
std::string toString(const P1 & p1, const P2 & p2, const P & ... p);

template <typename P1, typename P2, typename ... P>
std::u16string toString16(const P1 & p1, const P2 & p2, const P & ... p);

template <typename P1, typename P2, typename ... P>
std::u32string toString32(const P1 & p1, const P2 & p2, const P & ... p);

///////////////////// Universal UTF-8 to-string function //////////////////////

///
/// Returns the supplied value as is.
///
inline std::string toString(const std::string & value) {
	return value;
}

///
/// Creates a string from the string view.
///
inline std::string toString(const std::string_view & value) {
	return std::string(value);
}

///
/// Convert the supplied UTF-16 string to a UTF-8 string.
///
inline std::string toString(const std::u16string & value) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convertor;
	return convertor.to_bytes(value);
}

///
/// Creates a UTF-8 string from the UTF-16 string view.
///
/// Currently this materialises the string view into a string.
///
inline std::string toString(const std::u16string_view & value) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convertor;
	return convertor.to_bytes(std::u16string(value));
}

///
/// Convert the supplied UTF-32 string to a UTF-8 string.
///
inline std::string toString(const std::u32string & value) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convertor;
	return convertor.to_bytes(value);
}

///
/// Creates a UTF-8 string from the UTF-32 string view.
///
/// Currently this materialises the string view into a string.
///
inline std::string toString(const std::u32string_view & value) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convertor;
	return convertor.to_bytes(std::u32string(value));
}

///
/// Returns the supplied value as a UTF-8 string.
///
inline std::string toString(char value) {
	std::string str;
	str += value;
	return str;
}

///
/// Convert the supplied code point to a UTF-8 string.
///
inline std::string toString(char16_t value) {
	std::u16string str;
	str += value;
	return toString(str);
}

///
/// Convert the supplied code point to a UTF-8 string.
///
inline std::string toString(char32_t value) {
	std::u32string str;
	str += value;
	return toString(str);
}

///
/// Convert the supplied signed char to a UTF-8 string.
///
inline std::string toString(signed char value) {
	return Balau::Impl::toString<std::allocator<char>>("%d", (int) value);
}

///
/// Convert the supplied short to a UTF-8 string.
///
inline std::string toString(short value) {
	return Balau::Impl::toString<std::allocator<char>>("%d", (int) value);
}

///
/// Convert the supplied int to a UTF-8 string.
///
inline std::string toString(int value) {
	return Balau::Impl::toString<std::allocator<char>>("%d", value);
}

///
/// Convert the supplied long to a UTF-8 string.
///
inline std::string toString(long value) {
	return Balau::Impl::toString<std::allocator<char>>("%ld", value);
}

///
/// Convert the supplied long long to a UTF-8 string.
///
inline std::string toString(long long value) {
	return Balau::Impl::toString<std::allocator<char>>("%lld", value);
}

///
/// Convert the supplied signed char to a UTF-8 string.
///
inline std::string toString(unsigned char value) {
	return Balau::Impl::toString<std::allocator<char>>("%u", (unsigned int) value);
}

///
/// Convert the supplied short to a UTF-8 string.
///
inline std::string toString(unsigned short value) {
	return Balau::Impl::toString<std::allocator<char>>("%u", (unsigned int) value);
}

///
/// Convert the supplied unsigned int to a UTF-8 string.
///
inline std::string toString(unsigned int value) {
	return Balau::Impl::toString<std::allocator<char>>("%u", value);
}

///
/// Convert the supplied unsigned long to a UTF-8 string.
///
inline std::string toString(unsigned long value) {
	return Balau::Impl::toString<std::allocator<char>>("%lu", value);
}

///
/// Convert the supplied unsigned long long to a UTF-8 string.
///
inline std::string toString(unsigned long long value) {
	return Balau::Impl::toString<std::allocator<char>>("%llu", value);
}

///
/// Convert the supplied float to a UTF-8 string.
///
inline std::string toString(float value) {
	return std::to_string(value);
}

///
/// Convert the supplied double to a UTF-8 string.
///
inline std::string toString(double value) {
	return std::to_string(value);
}

///
/// Convert the supplied long double to a UTF-8 string.
///
inline std::string toString(long double value) {
	return std::to_string(value);
}

///
/// Convert the supplied complex to a UTF-8 string.
///
template <typename T> inline std::string toString(const std::complex<T> & value) {
	return toString(value.real(), " + j", value.imag());
}

///
/// Convert the supplied boolean to a UTF-8 string.
///
inline std::string toString(bool value) {
	return value ? "true" : "false";
}

///
/// Returns the supplied const char * as a UTF-8 string.
///
inline std::string toString(const char * value) {
	return std::string(value);
}

std::u16string toString16(const char16_t * value);
std::u32string toString32(const char32_t * value);

///
/// Returns the supplied const char16_t * as a UTF-8 string.
///
inline std::string toString(const char16_t * value) {
	return toString(toString16(value));
}

///
/// Returns the supplied const char32_t * as a UTF-8 string.
///
inline std::string toString(const char32_t * value) {
	return toString(toString32(value));
}

///
/// Convert the supplied pointer to a UTF-8 string.
///
/// The pointer is converted to an unsigned long long integer value and toString
/// is called on the value.
///
inline std::string toString(const void * ptr) {
	return toString((size_t) ptr);
}

///
/// Convert the supplied nullptr value to a UTF-8 string.
///
/// The returned value is zero in order to match the values
/// returned for other pointer to-string functions.
///
inline std::string toString(std::nullptr_t) {
	return "0";
}

///
/// Return the name of the type index.
///
inline std::string toString(std::type_index value) {
	return value.name();
}

///
/// A toString function that creates the UTF-8 string by calling the function.
///
inline std::string toString(const std::function<std::string ()> & f) {
	return f();
}

///
/// Convert the supplied std::pair value to a UTF-8 string.
///
template <typename F, typename S>
inline std::string toString(const std::pair<F, S> & p) {
	return std::string("{ ") + toString(p.first) + ", " + toString(p.second) + " }";
}

///
/// Helper for container to UTF-8 string functions.
///
/// This helper function can be used for custom container types if required.
///
template <typename ... T, template <typename ...> class C>
inline std::string toStringHelper(const C<T ...> & c)  {
	std::string builder = "{";
	std::string prefix = " ";
	bool empty = true;

	for (const auto & e : c) {
		builder += prefix + toString(e);
		prefix = ", ";
		empty = false;
	}

	if (!empty) {
		builder += " ";
	}

	builder += "}";

	return builder;
}

///
/// Define a UTF-8 to-string function for a container that has a single template parameter.
///
#define BALAU_CONTAINER1_TO_STRING(CONTAINER_TYPE)             \
	template <typename T>                                      \
	inline std::string toString(const CONTAINER_TYPE<T> & c) { \
		return toStringHelper(c);                              \
	}

///
/// Define a UTF-8 to-string function for a container that has two template parameters.
///
#define BALAU_CONTAINER2_TO_STRING(CONTAINER_TYPE)                \
	template <typename T, typename U>                             \
	inline std::string toString(const CONTAINER_TYPE<T, U> & c) { \
		return toStringHelper(c);                                 \
	}

///
/// Define a UTF-8 to-string function for a container that has three template parameters.
///
#define BALAU_CONTAINER3_TO_STRING(CONTAINER_TYPE)                   \
	template <typename T, typename U, typename V>                    \
	inline std::string toString(const CONTAINER_TYPE<T, U, V> & c) { \
		return toStringHelper(c);                                    \
	}

///
/// Define a UTF-8 to-string function for a container that has four template parameters.
///
#define BALAU_CONTAINER4_TO_STRING(CONTAINER_TYPE)                      \
	template <typename T, typename U, typename V, typename W>           \
	inline std::string toString(const CONTAINER_TYPE<T, U, V, W> & c) { \
		return toStringHelper(c);                                       \
	}

///
/// Define a UTF-8 to-string function for a container that has five template parameters.
///
#define BALAU_CONTAINER5_TO_STRING(CONTAINER_TYPE)                         \
	template <typename T, typename U, typename V, typename W, typename X>  \
	inline std::string toString(const CONTAINER_TYPE<T, U, V, W, X> & c) { \
		return toStringHelper(c);                                          \
	}

BALAU_CONTAINER2_TO_STRING(std::deque)
BALAU_CONTAINER2_TO_STRING(std::forward_list)
BALAU_CONTAINER2_TO_STRING(std::list)
BALAU_CONTAINER2_TO_STRING(std::map)
BALAU_CONTAINER3_TO_STRING(std::multiset)
BALAU_CONTAINER4_TO_STRING(std::multimap)
BALAU_CONTAINER3_TO_STRING(std::priority_queue)
BALAU_CONTAINER2_TO_STRING(std::queue)
BALAU_CONTAINER2_TO_STRING(std::set)
BALAU_CONTAINER2_TO_STRING(std::stack)
BALAU_CONTAINER5_TO_STRING(std::unordered_map)
BALAU_CONTAINER5_TO_STRING(std::unordered_multimap)
BALAU_CONTAINER4_TO_STRING(std::unordered_multiset)
BALAU_CONTAINER3_TO_STRING(std::unordered_set)
BALAU_CONTAINER2_TO_STRING(std::vector)

///
/// Print the supplied array as a UTF-8 string.
///
/// @return the supplied array as a UTF-8 string
///
template <typename T, size_t N> inline std::string toString(const std::array<T, N> & c) {
	std::string builder = "{";
	std::string prefix = " ";
	bool empty = true;

	for (size_t m = 0; m < N; m++) {
		builder += prefix + toString(c[m]);
		prefix = ", ";
		empty = false;
	}

	if (!empty) {
		builder += " ";
	}

	builder += "}";

	return builder;
}

///
/// Concatenates the to-string values of the input arguments.
///
/// Calls toString on each input argument and concatenates them together to
/// form a single UTF-8 string.
///
template <typename P1, typename P2, typename ... P>
inline std::string toString(const P1 & p1, const P2 & p2, const P & ... p) {
	return toString(p1) + toString(p2) + (std::string() + ... + toString(p));
}

///////////////////// Universal UTF-16 to-string function /////////////////////

///
/// Convert the supplied UTF-8 string to a UTF-16 string.
///
inline std::u16string toString16(const std::string & value) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convertor;
	return convertor.from_bytes(value);
}

///
/// Convert the supplied UTF-8 string view to a UTF-16 string.
///
/// Currently this materialises the string view into a string.
///
inline std::u16string toString16(const std::string_view & value) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convertor;
	return convertor.from_bytes(std::string(value));
}

///
/// Returns the supplied value as is.
///
inline std::u16string toString16(const std::u16string & value) {
	return value;
}

///
/// Creates a UTF-16 string from the UTF-16 string view.
///
inline std::u16string toString16(const std::u16string_view & value) {
	return std::u16string(value);
}

///
/// Convert the supplied UTF-32 string to a UTF-16 string.
///
inline std::u16string toString16(const std::u32string & value) {
	return toString16(toString(value));
}

///
/// Convert the supplied ascii character to a UTF-16 string.
///
inline std::u16string toString16(char value) {
	std::string str;
	str += value;
	return toString16(str);
}

///
/// Convert the supplied UTF-16 character to a UTF-16 string.
///
inline std::u16string toString16(char16_t & value) {
	std::u16string str;
	str += value;
	return str;
}

///
/// Convert the supplied code point to a UTF-16 string.
///
inline std::u16string toString16(char32_t & value) {
	std::u32string str;
	str += value;
	return toString16(str);
}

///
/// Convert the supplied signed char to a UTF-16 string.
///
inline std::u16string toString16(signed char value) {
	return toString16(toString(value));
}

///
/// Convert the supplied short to a UTF-16 string.
///
inline std::u16string toString16(short value) {
	return toString16(toString(value));
}

///
/// Convert the supplied int to a UTF-16 string.
///
inline std::u16string toString16(int value) {
	return toString16(toString(value));
}

///
/// Convert the supplied long to a UTF-16 string.
///
inline std::u16string toString16(long value) {
	return toString16(toString(value));
}

///
/// Convert the supplied long long to a UTF-16 string.
///
inline std::u16string toString16(long long value) {
	return toString16(toString(value));
}

///
/// Convert the supplied unsigned char to a UTF-16 string.
///
inline std::u16string toString16(unsigned char value) {
	return toString16(toString(value));
}

///
/// Convert the supplied unsigned short to a UTF-16 string.
///
inline std::u16string toString16(unsigned short value) {
	return toString16(toString(value));
}

///
/// Convert the supplied unsigned int to a UTF-16 string.
///
inline std::u16string toString16(unsigned int value) {
	return toString16(toString(value));
}

///
/// Convert the supplied unsigned long to a UTF-16 string.
///
inline std::u16string toString16(unsigned long value) {
	return toString16(toString(value));
}

///
/// Convert the supplied unsigned long long to a UTF-16 string.
///
inline std::u16string toString16(unsigned long long value) {
	return toString16(toString(value));
}

///
/// Convert the supplied float to a UTF-16 string.
///
inline std::u16string toString16(float value) {
	return toString16(toString(value));
}

///
/// Convert the supplied double to a UTF-16 string.
///
inline std::u16string toString16(double value) {
	return toString16(toString(value));
}

///
/// Convert the supplied long double to a UTF-16 string.
///
inline std::u16string toString16(long double value) {
	return toString16(toString(value));
}

///
/// Convert the supplied complex to a UTF-16 string.
///
template <typename T> inline std::u16string toString16(const std::complex<T> & value) {
	return toString16(value.real(), " + j", value.imag());
}

///
/// Convert the supplied boolean to a UTF-16 string.
///
inline std::u16string toString16(bool value) {
	return value ? u"true" : u"false";
}

///
/// Returns the supplied const char * as a UTF-16 string.
///
inline std::u16string toString16(const char * value) {
	return toString16(toString((value)));
}

///
/// Returns the supplied const char16_t * as a UTF-16 string.
///
inline std::u16string toString16(const char16_t * value) {
	return std::u16string(value);
}

///
/// Returns the supplied const char32_t * as a UTF-16 string.
///
inline std::u16string toString16(const char32_t * value) {
	return toString16(toString32(value));
}

///
/// Convert the supplied pointer to a UTF-16 string.
///
/// The pointer is converted to an unsigned long long integer value and toString16
/// is called on the value.
///
inline std::u16string toString16(const void * ptr) {
	return toString16((size_t) ptr);
}

///
/// Convert the supplied nullptr value to a UTF-16 string.
///
/// The returned value is zero in order to match the values
/// returned for other pointer to-string functions.
///
inline std::u16string toString16(std::nullptr_t) {
	return u"0";
}

///
/// Return the name of the type index.
///
inline std::u16string toString16(std::type_index value) {
	return toString16(value.name());
}

///
/// A toString16 function that creates the UTF-16 string by calling the function.
///
inline std::u16string toString16(const std::function<std::u16string ()> & f) {
	return f();
}

///
/// Convert the supplied std::pair value to a UTF-16 string.
///
template <typename F, typename S>
inline std::u16string toString16(const std::pair<F, S> & p) {
	return std::u16string(u"{ ") + toString16(p.first) + u", " + toString16(p.second) + u" }";
}

///
/// Helper for container to UTF-16 string functions.
///
/// This helper function can be used for custom container types if required.
///
template <typename ... T, template <typename ...> class C>
inline std::u16string toString16Helper(const C<T ...> & c)  {
	std::u16string builder = u"{";
	std::u16string prefix = u" ";
	bool empty = true;

	for (const auto & e : c) {
		builder += prefix + toString16(e);
		prefix = u", ";
		empty = false;
	}

	if (!empty) {
		builder += u" ";
	}

	builder += u"}";

	return builder;
}

///
/// Define a UTF-16 to-string function for a container that has a single template parameter.
///
#define BALAU_CONTAINER1_TO_STRING16(CONTAINER_TYPE)                \
	template <typename T>                                           \
	inline std::u16string toString16(const CONTAINER_TYPE<T> & c) { \
		return toString16Helper(c);                                 \
	}

///
/// Define a UTF-16 to-string function for a container that has two template parameters.
///
#define BALAU_CONTAINER2_TO_STRING16(CONTAINER_TYPE)                   \
	template <typename T, typename U>                                  \
	inline std::u16string toString16(const CONTAINER_TYPE<T, U> & c) { \
		return toString16Helper(c);                                    \
	}

///
/// Define a UTF-16 to-string function for a container that has three template parameters.
///
#define BALAU_CONTAINER3_TO_STRING16(CONTAINER_TYPE)                      \
	template <typename T, typename U, typename V>                         \
	inline std::u16string toString16(const CONTAINER_TYPE<T, U, V> & c) { \
		return toString16Helper(c);                                       \
	}

///
/// Define a UTF-16 to-string function for a container that has four template parameters.
///
#define BALAU_CONTAINER4_TO_STRING16(CONTAINER_TYPE)                         \
	template <typename T, typename U, typename V, typename W>                \
	inline std::u16string toString16(const CONTAINER_TYPE<T, U, V, W> & c) { \
		return toString16Helper(c);                                          \
	}

///
/// Define a UTF-16 to-string function for a container that has five template parameters.
///
#define BALAU_CONTAINER5_TO_STRING16(CONTAINER_TYPE)                            \
	template <typename T, typename U, typename V, typename W, typename X>       \
	inline std::u16string toString16(const CONTAINER_TYPE<T, U, V, W, X> & c) { \
		return toString16Helper(c);                                             \
	}

BALAU_CONTAINER2_TO_STRING16(std::deque)
BALAU_CONTAINER2_TO_STRING16(std::forward_list)
BALAU_CONTAINER2_TO_STRING16(std::list)
BALAU_CONTAINER2_TO_STRING16(std::map)
BALAU_CONTAINER3_TO_STRING16(std::multiset)
BALAU_CONTAINER4_TO_STRING16(std::multimap)
BALAU_CONTAINER3_TO_STRING16(std::priority_queue)
BALAU_CONTAINER2_TO_STRING16(std::queue)
BALAU_CONTAINER2_TO_STRING16(std::set)
BALAU_CONTAINER2_TO_STRING16(std::stack)
BALAU_CONTAINER5_TO_STRING16(std::unordered_map)
BALAU_CONTAINER5_TO_STRING16(std::unordered_multimap)
BALAU_CONTAINER4_TO_STRING16(std::unordered_multiset)
BALAU_CONTAINER3_TO_STRING16(std::unordered_set)
BALAU_CONTAINER2_TO_STRING16(std::vector)

///
/// Concatenates the to-string values of the input arguments.
///
/// Calls toString16 on each input argument and concatenates them together to
/// form a single UTF-16 string.
///
template <typename P1, typename P2, typename ... P>
inline std::u16string toString16(const P1 & p1, const P2 & p2, const P & ... p) {
	return toString16(p1) + toString16(p2) + (std::u16string() + ... + toString16(p));
}

///////////////////// Universal UTF-32 to-string function /////////////////////

///
/// Convert the supplied UTF-8 string to a UTF-32 string.
///
inline std::u32string toString32(const std::string & value) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convertor;
	return convertor.from_bytes(value);
}

///
/// Convert the supplied UTF-8 string view to a UTF-32 string.
///
/// Currently this materialises the string view into a string.
///
inline std::u32string toString32(const std::string_view & value) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convertor;
	return convertor.from_bytes(std::string(value));
}

///
/// Convert the supplied UTF-16 string to a UTF-32 string.
///
inline std::u32string toString32(const std::u16string & value) {
	return toString32(toString(value));
}

///
/// Convert the supplied UTF-16 string view to a UTF-32 string.
///
/// Currently this materialises the string view into a string.
///
inline std::u32string toString32(const std::u16string_view & value) {
	return toString32(toString(value));
}

///
/// Returns the supplied value as is.
///
inline std::u32string toString32(const std::u32string & value) {
	return value;
}

///
/// Creates a UTF-32 string from the UTF-32 string view.
///
inline std::u32string toString32(const std::u32string_view & value) {
	return std::u32string(value);
}

///
/// Returns the supplied ascii character as a UTF-32 string.
///
inline std::u32string toString32(char value) {
	std::string str;
	str += value;
	return toString32(str);
}

///
/// Convert the supplied UTF-16 character to a UTF-32 string.
///
inline std::u32string toString32(char16_t & value) {
	std::u16string str;
	str += value;
	return toString32(str);
}

///
/// Convert the supplied code point to a UTF-32 string.
///
inline std::u32string toString32(char32_t & value) {
	std::u32string str;
	str += value;
	return str;
}

///
/// Convert the supplied signed char to a UTF-32 string.
///
inline std::u32string toString32(signed char value) {
	return toString32(toString(value));
}

///
/// Convert the supplied short to a UTF-32 string.
///
inline std::u32string toString32(short value) {
	return toString32(toString(value));
}

///
/// Convert the supplied int to a UTF-32 string.
///
inline std::u32string toString32(int value) {
	return toString32(toString(value));
}

///
/// Convert the supplied long to a UTF-32 string.
///
inline std::u32string toString32(long value) {
	return toString32(toString(value));
}

///
/// Convert the supplied long long to a UTF-32 string.
///
inline std::u32string toString32(long long value) {
	return toString32(toString(value));
}

///
/// Convert the supplied unsigned char to a UTF-32 string.
///
inline std::u32string toString32(unsigned char value) {
	return toString32(toString(value));
}

///
/// Convert the supplied unsigned short to a UTF-32 string.
///
inline std::u32string toString32(unsigned short value) {
	return toString32(toString(value));
}

///
/// Convert the supplied unsigned int to a UTF-32 string.
///
inline std::u32string toString32(unsigned int value) {
	return toString32(toString(value));
}

///
/// Convert the supplied unsigned long to a UTF-32 string.
///
inline std::u32string toString32(unsigned long value) {
	return toString32(toString(value));
}

///
/// Convert the supplied unsigned long long to a UTF-32 string.
///
inline std::u32string toString32(unsigned long long value) {
	return toString32(toString(value));
}

///
/// Convert the supplied float to a UTF-32 string.
///
inline std::u32string toString32(float value) {
	return toString32(toString(value));
}

///
/// Convert the supplied double to a UTF-32 string.
///
inline std::u32string toString32(double value) {
	return toString32(toString(value));
}

///
/// Convert the supplied long double to a UTF-32 string.
///
inline std::u32string toString32(long double value) {
	return toString32(toString(value));
}

///
/// Convert the supplied complex to a UTF-32 string.
///
template <typename T> inline std::u32string toString32(const std::complex<T> & value) {
	return toString32(value.real(), " + j", value.imag());
}

///
/// Convert the supplied boolean to a UTF-32 string.
///
inline std::u32string toString32(bool value) {
	return value ? U"true" : U"false";
}

///
/// Returns the supplied const char * as a UTF-32 string.
///
inline std::u32string toString32(const char * value) {
	return toString32(toString((value)));
}

///
/// Returns the supplied const char16_t * as a UTF-32 string.
///
inline std::u32string toString32(const char16_t * value) {
	return toString32(toString16((value)));
}

///
/// Returns the supplied const char32_t * as a UTF-32 string.
///
inline std::u32string toString32(const char32_t * value) {
	return std::u32string(value);
}

///
/// Convert the supplied pointer to a UTF-32 string.
///
/// The pointer is converted to an unsigned long long integer value and toString32
/// is called on the value.
///
inline std::u32string toString32(const void * ptr) {
	return toString32((size_t) ptr);
}

///
/// Convert the supplied nullptr value to a UTF-32 string.
///
/// The returned value is zero in order to match the values
/// returned for other pointer to-string functions.
///
inline std::u32string toString32(std::nullptr_t) {
	return U"0";
}

///
/// Return the name of the type index.
///
inline std::u32string toString32(std::type_index value) {
	return toString32(value.name());
}

///
/// A toString32 function that creates the UTF-32 string by calling the function.
///
inline std::u32string toString32(const std::function<std::u32string ()> & f) {
	return f();
}

///
/// Convert the supplied std::pair value to a UTF-32 string.
///
template <typename F, typename S>
inline std::u32string toString32(const std::pair<F, S> & p) {
	return std::u32string(U"{ ") + toString32(p.first) + U", " + toString32(p.second) + U" }";
}

///
/// Helper for container to UTF-8 string functions.
///
/// This helper function can be used for custom container types if required.
///
template <typename ... T, template <typename ...> class C>
inline std::u32string toString32Helper(const C<T ...> & c)  {
	std::u32string builder = U"{";
	std::u32string prefix = U" ";
	bool empty = true;

	for (const auto & e : c) {
		builder += prefix + toString32(e);
		prefix = U", ";
		empty = false;
	}

	if (!empty) {
		builder += U" ";
	}

	builder += U"}";

	return builder;
}

///
/// Define a UTF-32 to-string function for a container that has a single template parameter.
///
#define BALAU_CONTAINER1_TO_STRING32(CONTAINER_TYPE)                \
	template <typename T>                                           \
	inline std::u32string toString32(const CONTAINER_TYPE<T> & c) { \
		return toString32Helper(c);                                 \
	}

///
/// Define a UTF-32 to-string function for a container that has two template parameters.
///
#define BALAU_CONTAINER2_TO_STRING32(CONTAINER_TYPE)                   \
	template <typename T, typename U>                                  \
	inline std::u32string toString32(const CONTAINER_TYPE<T, U> & c) { \
		return toString32Helper(c);                                    \
	}

///
/// Define a UTF-32 to-string function for a container that has three template parameters.
///
#define BALAU_CONTAINER3_TO_STRING32(CONTAINER_TYPE)                      \
	template <typename T, typename U, typename V>                         \
	inline std::u32string toString32(const CONTAINER_TYPE<T, U, V> & c) { \
		return toString32Helper(c);                                       \
	}

///
/// Define a UTF-32 to-string function for a container that has four template parameters.
///
#define BALAU_CONTAINER4_TO_STRING32(CONTAINER_TYPE)                         \
	template <typename T, typename U, typename V, typename W>                \
	inline std::u32string toString32(const CONTAINER_TYPE<T, U, V, W> & c) { \
		return toString32Helper(c);                                          \
	}

///
/// Define a UTF-32 to-string function for a container that has five template parameters.
///
#define BALAU_CONTAINER5_TO_STRING32(CONTAINER_TYPE)                            \
	template <typename T, typename U, typename V, typename W, typename X>       \
	inline std::u32string toString32(const CONTAINER_TYPE<T, U, V, W, X> & c) { \
		return toString32Helper(c);                                             \
	}

BALAU_CONTAINER2_TO_STRING32(std::deque)
BALAU_CONTAINER2_TO_STRING32(std::forward_list)
BALAU_CONTAINER2_TO_STRING32(std::list)
BALAU_CONTAINER2_TO_STRING32(std::map)
BALAU_CONTAINER3_TO_STRING32(std::multiset)
BALAU_CONTAINER4_TO_STRING32(std::multimap)
BALAU_CONTAINER3_TO_STRING32(std::priority_queue)
BALAU_CONTAINER2_TO_STRING32(std::queue)
BALAU_CONTAINER2_TO_STRING32(std::set)
BALAU_CONTAINER2_TO_STRING32(std::stack)
BALAU_CONTAINER5_TO_STRING32(std::unordered_map)
BALAU_CONTAINER5_TO_STRING32(std::unordered_multimap)
BALAU_CONTAINER4_TO_STRING32(std::unordered_multiset)
BALAU_CONTAINER3_TO_STRING32(std::unordered_set)
BALAU_CONTAINER2_TO_STRING32(std::vector)

///
/// Concatenates the to-string values of the input arguments.
///
/// Calls toString32 on each input argument and concatenates them together to
/// form a single UTF-32 string.
///
template <typename P1, typename P2, typename ... P>
inline std::u32string toString32(const P1 & p1, const P2 & p2, const P & ... p) {
	return toString32(p1) + toString32(p2) + (std::u32string() + ... + toString32(p));
}

/////////////// Universal to-string function - templated version //////////////

///
/// UTF-8 specialisation of ToString<CharT, AllocatorT> with std::allocator.
///
/// Convert the supplied object to a std::string by calling toString.
///
template <> struct ToString<char, std::allocator<char>> {
	template <typename T> std::string operator () (const T & object) const {
		return toString(object);
	}
};

///
/// UTF-16 specialisation of ToString<CharT, AllocatorT> with std::allocator.
///
/// Convert the supplied object to a std::u16string by calling toString16.
///
template <> struct ToString<char16_t, std::allocator<char16_t>> {
	template <typename T> std::u16string operator () (const T & object) const {
		return toString16(object);
	}
};

///
/// UTF-32 specialisation of ToString<CharT, AllocatorT> with std::allocator.
///
/// Convert the supplied object to a std::u32string by calling toString32.
///
template <> struct ToString<char32_t, std::allocator<char32_t>> {
	template <typename T> std::u32string operator () (const T & object) const {
		return toString32(object);
	}
};

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_S
