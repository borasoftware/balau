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
/// @file ToStringA.hpp
///
/// Pre-defined universal to-string functions for strings with custom allocators.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_A
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_A

#include <Balau/Type/StdTypes.hpp>
#include <Balau/Type/Impl/ToStringImpl.hpp>

#include <complex>
#include <functional>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <unordered_map>

namespace Balau {

///
/// UTF-8 string type with selectable allocator.
///
template <typename AllocatorT>
using U8String = std::basic_string<char, std::char_traits<char>, AllocatorT>;

///
/// UTF-16 string type with selectable allocator.
///
template <typename AllocatorT>
using U16String = std::basic_string<char16_t, std::char_traits<char16_t>, AllocatorT>;

///
/// UTF-32 string type with selectable allocator.
///
template <typename AllocatorT>
using U32String = std::basic_string<char32_t, std::char_traits<char32_t>, AllocatorT>;

///
/// UTF-8 output string stream type with selectable allocator.
///
template <typename AllocatorT>
using U8OStringStream = std::basic_ostringstream<char, std::char_traits<char>, AllocatorT>;

///
/// UTF-16 output string stream type with selectable allocator.
///
template <typename AllocatorT>
using U16OStringStream = std::basic_ostringstream<char16_t, std::char_traits<char16_t>, AllocatorT>;

///
/// UTF-32 output string stream type with selectable allocator.
///
template <typename AllocatorT>
using U32OStringStream = std::basic_ostringstream<char32_t, std::char_traits<char32_t>, AllocatorT>;

}

template <typename AllocatorT, typename P1, typename P2, typename ... P>
Balau::U8String<AllocatorT> toString(const P1 & p1, const P2 & p2, const P & ... p);

template <typename AllocatorT, typename P1, typename P2, typename ... P>
Balau::U16String<AllocatorT> toString16(const P1 & p1, const P2 & p2, const P & ... p);

template <typename AllocatorT, typename P1, typename P2, typename ... P>
Balau::U32String<AllocatorT> toString32(const P1 & p1, const P2 & p2, const P & ... p);

///////////////////// Universal UTF-8 to-string function //////////////////////

///
/// Returns the supplied value as is.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Balau::U8String<AllocatorT> & value) {
	return value;
}

///
/// Converts the supplied string to Balau::U8String<AllocatorT>.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::string & value) {
	return Balau::U8String<AllocatorT>(value.data(), value.length());
}

///
/// Creates a string from the string view.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::string_view & value) {
	return Balau::U8String<AllocatorT>(value);
}

///
/// Convert the supplied UTF-16 string to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::u16string & value) {
	// TODO
	return Balau::U8String<AllocatorT>();
}

///
/// Creates a UTF-8 string from the UTF-16 string view.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::u16string_view & value) {
	// TODO
	return Balau::U8String<AllocatorT>();
}

///
/// Convert the supplied UTF-32 string to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::u32string & value) {
	// TODO
	return Balau::U8String<AllocatorT>();
}

///
/// Creates a UTF-8 string from the UTF-32 string view.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::u32string_view & value) {
	// TODO
	return Balau::U8String<AllocatorT>();
}

///
/// Returns the supplied value as a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(char value) {
	Balau::U8String<AllocatorT> str;
	str += value;
	return str;
}

///
/// Convert the supplied code point to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(char16_t value) {
	Balau::U16String<AllocatorT> str;
	str += value;
	return toString<AllocatorT>(str);
}

///
/// Convert the supplied code point to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(char32_t value) {
	std::u32string str;
	str += value;
	return toString<AllocatorT>(str);
}

///
/// Convert the supplied signed char to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(signed char value) {
	return Balau::Impl::toString<AllocatorT>("%d", (int) value);
}

///
/// Convert the supplied short to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(short value) {
	return Balau::Impl::toString<AllocatorT>("%d", (int) value);
}

///
/// Convert the supplied int to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(int value) {
	return Balau::Impl::toString<AllocatorT>("%d", (int) value);
}

///
/// Convert the supplied long to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(long value) {
	return Balau::Impl::toString<AllocatorT>("%ld", value);
}

///
/// Convert the supplied long long to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(long long value) {
	return Balau::Impl::toString<AllocatorT>("%lld", value);
}

///
/// Convert the supplied signed char to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(unsigned char value) {
	return Balau::Impl::toString<AllocatorT>("%u", (unsigned int) value);
}

///
/// Convert the supplied short to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(unsigned short value) {
	return Balau::Impl::toString<AllocatorT>("%u", (unsigned int) value);
}

///
/// Convert the supplied unsigned int to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(unsigned int value) {
	return Balau::Impl::toString<AllocatorT>("%u", value);
}

///
/// Convert the supplied unsigned long to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(unsigned long value) {
	return Balau::Impl::toString<AllocatorT>("%lu", value);
}

///
/// Convert the supplied unsigned long long to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(unsigned long long value) {
	return Balau::Impl::toString<AllocatorT>("%llu", value);
}

///
/// Convert the supplied float to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(float value) {
	return std::to_string(value);
}

///
/// Convert the supplied double to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(double value) {
	// TODO
	return Balau::U8String<AllocatorT>();
}

///
/// Convert the supplied long double to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(long double value) {
	// TODO
	return Balau::U8String<AllocatorT>();
}

///
/// Convert the supplied complex to a UTF-8 string.
///
template <typename T, typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::complex<T> & value) {
	return toString<AllocatorT>(value.real(), " + j", value.imag());
}

///
/// Convert the supplied boolean to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(bool value) {
	return value ? "true" : "false";
}

///
/// Returns the supplied const char * as a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const char * value) {
	return Balau::U8String<AllocatorT>(value);
}

template <typename AllocatorT>
std::u16string toString16(const char16_t * value);

template <typename AllocatorT>
std::u32string toString32(const char32_t * value);

///
/// Returns the supplied const char16_t * as a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const char16_t * value) {
	return toString<AllocatorT>(toString16<AllocatorT>(value));
}

///
/// Returns the supplied const char32_t * as a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const char32_t * value) {
	return toString<AllocatorT>(toString32<AllocatorT>(value));
}

///
/// Convert the supplied pointer to a UTF-8 string.
///
/// The pointer is converted to an unsigned long long integer value and toString
/// is called on the value.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const void * ptr) {
	return toString<AllocatorT>((size_t) ptr);
}

///
/// Convert the supplied nullptr value to a UTF-8 string.
///
/// The returned value is zero in order to match the values
/// returned for other pointer to-string functions.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(std::nullptr_t) {
	return "0";
}

///
/// Return the name of the type index.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(std::type_index value) {
	return value.name();
}

///
/// A toString function that creates the UTF-8 string by calling the function.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::function<Balau::U8String<AllocatorT> ()> & f) {
	return f();
}

///
/// Convert the supplied std::pair value to a UTF-8 string.
///
template <typename AllocatorT, typename F, typename S>
inline Balau::U8String<AllocatorT> toString(const std::pair<F, S> & p) {
	return Balau::U8String<AllocatorT>("{ ") + toString<AllocatorT>(p.first) + ", " + toString<AllocatorT>(p.second) + " }";
}

///
/// Helper for container to UTF-8 string functions.
///
/// This helper function can be used for custom container types if required.
///
template <typename AllocatorT, typename ... T, template <typename ...> class C>
inline Balau::U8String<AllocatorT> toStringHelper(const C<T ...> & c)  {
	Balau::U8String<AllocatorT> builder = "{";
	Balau::U8String<AllocatorT> prefix = " ";
	bool empty = true;

	for (const auto & e : c) {
		builder += prefix + toString<AllocatorT>(e);
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
#define BALAU_CONTAINER1_TO_STRING_A(CONTAINER_TYPE)                          \
	template <typename T, typename AllocatorT>                                 \
	inline Balau::U8String<AllocatorT> toString(const CONTAINER_TYPE<T> & c) { \
		return toStringHelper<AllocatorT>(c);                                  \
	}

///
/// Define a UTF-8 to-string function for a container that has two template parameters.
///
#define BALAU_CONTAINER2_TO_STRING_A(CONTAINER_TYPE)                             \
	template <typename AllocatorT, typename T, typename U>                        \
	inline Balau::U8String<AllocatorT> toString(const CONTAINER_TYPE<T, U> & c) { \
		return toStringHelper<AllocatorT>(c);                                     \
	}

///
/// Define a UTF-8 to-string function for a container that has three template parameters.
///
#define BALAU_CONTAINER3_TO_STRING_A(CONTAINER_TYPE)                                \
	template <typename AllocatorT, typename T, typename U, typename V>               \
	inline Balau::U8String<AllocatorT> toString(const CONTAINER_TYPE<T, U, V> & c) { \
		return toStringHelper<AllocatorT>(c);                                        \
	}

///
/// Define a UTF-8 to-string function for a container that has four template parameters.
///
#define BALAU_CONTAINER4_TO_STRING_A(CONTAINER_TYPE)                                   \
	template <typename AllocatorT, typename T, typename U, typename V, typename W>      \
	inline Balau::U8String<AllocatorT> toString(const CONTAINER_TYPE<T, U, V, W> & c) { \
		return toStringHelper<AllocatorT>(c);                                           \
	}

///
/// Define a UTF-8 to-string function for a container that has five template parameters.
///
#define BALAU_CONTAINER5_TO_STRING_A(CONTAINER_TYPE)                                          \
	template <typename AllocatorT, typename T, typename U, typename V, typename W, typename X> \
	inline Balau::U8String<AllocatorT> toString(const CONTAINER_TYPE<T, U, V, W, X> & c) {     \
		return toStringHelper<AllocatorT>(c);                                                  \
	}

BALAU_CONTAINER2_TO_STRING_A(std::deque)
BALAU_CONTAINER2_TO_STRING_A(std::forward_list)
BALAU_CONTAINER2_TO_STRING_A(std::list)
BALAU_CONTAINER2_TO_STRING_A(std::map)
BALAU_CONTAINER3_TO_STRING_A(std::multiset)
BALAU_CONTAINER4_TO_STRING_A(std::multimap)
BALAU_CONTAINER3_TO_STRING_A(std::priority_queue)
BALAU_CONTAINER2_TO_STRING_A(std::queue)
BALAU_CONTAINER2_TO_STRING_A(std::set)
BALAU_CONTAINER2_TO_STRING_A(std::stack)
BALAU_CONTAINER5_TO_STRING_A(std::unordered_map)
BALAU_CONTAINER5_TO_STRING_A(std::unordered_multimap)
BALAU_CONTAINER4_TO_STRING_A(std::unordered_multiset)
BALAU_CONTAINER3_TO_STRING_A(std::unordered_set)
BALAU_CONTAINER2_TO_STRING_A(std::vector)

///
/// Print the supplied array as a UTF-8 string.
///
/// @return the supplied array as a UTF-8 string
///
template <typename AllocatorT, typename T, size_t N>
inline Balau::U8String<AllocatorT> toString(const std::array<T, N> & c) {
	return toStringHelper<AllocatorT>(c);
}

///
/// Concatenates the to-string values of the input arguments.
///
/// Calls toString on each input argument and concatenates them together to
/// form a single UTF-8 string.
///
template <typename AllocatorT, typename P1, typename P2, typename ... P>
inline Balau::U8String<AllocatorT> toString(const P1 & p1, const P2 & p2, const P & ... p) {
	return toString<AllocatorT>(p1) + toString<AllocatorT>(p2) + (Balau::U8String<AllocatorT>() + ... + toString<AllocatorT>(p));
}

///////////////////// Universal UTF-16 to-string function /////////////////////

///
/// Convert the supplied UTF-8 string to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const Balau::U8String<AllocatorT> & value) {
	// TODO
	return Balau::U16String<AllocatorT>();
}

///
/// Convert the supplied UTF-8 string to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::string & value) {
	// TODO
	return Balau::U16String<AllocatorT>();
}

///
/// Convert the supplied UTF-8 string view to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::string_view & value) {
	return toString16<AllocatorT>(Balau::U8String<AllocatorT>(value));
}

///
/// Returns the supplied value as is.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const Balau::U16String<AllocatorT> & value) {
	return value;
}

///
/// Convert the std::u16string to a Balau::U16String<AllocatorT>.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::u16string & value) {
	return Balau::U16String<AllocatorT>(value.data(), value.length());
}

///
/// Creates a UTF-16 string from the UTF-16 string view.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::u16string_view & value) {
	return Balau::U16String<AllocatorT>(value);
}

///
/// Convert the supplied UTF-32 string to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::u32string & value) {
	// TODO
	return Balau::U16String<AllocatorT>();
}

///
/// Convert the supplied ascii character to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(char value) {
	Balau::U8String<AllocatorT> str;
	str += value;
	return toString16<AllocatorT>(str);
}

///
/// Convert the supplied UTF-16 character to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(char16_t & value) {
	Balau::U16String<AllocatorT> str;
	str += value;
	return str;
}

///
/// Convert the supplied code point to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(char32_t & value) {
	Balau::U32String<AllocatorT> str;
	str += value;
	return toString16<AllocatorT>(str);
}

///
/// Convert the supplied signed char to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(signed char value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied short to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(short value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied int to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(int value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied long to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(long value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied long long to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(long long value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned char to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(unsigned char value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned short to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(unsigned short value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned int to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(unsigned int value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned long to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(unsigned long value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned long long to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(unsigned long long value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied float to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(float value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied double to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(double value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied long double to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(long double value) {
	return toString16<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied complex to a UTF-16 string.
///
template <typename T, typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::complex<T> & value) {
	return toString16<AllocatorT>(value.real(), " + j", value.imag());
}

///
/// Convert the supplied boolean to a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(bool value) {
	return value ? u"true" : u"false";
}

///
/// Returns the supplied const char * as a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const char * value) {
	return toString16<AllocatorT>(toString<AllocatorT>((value)));
}

///
/// Returns the supplied const char16_t * as a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const char16_t * value) {
	return std::u16string(value);
}

///
/// Returns the supplied const char32_t * as a UTF-16 string.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const char32_t * value) {
	return toString16<AllocatorT>(toString32<AllocatorT>(value));
}

///
/// Convert the supplied pointer to a UTF-16 string.
///
/// The pointer is converted to an unsigned long long integer value and toString16
/// is called on the value.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const void * ptr) {
	return toString16<AllocatorT>((size_t) ptr);
}

///
/// Convert the supplied nullptr value to a UTF-16 string.
///
/// The returned value is zero in order to match the values
/// returned for other pointer to-string functions.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(std::nullptr_t) {
	return u"0";
}

///
/// Return the name of the type index.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(std::type_index value) {
	return toString16<AllocatorT>(value.name());
}

///
/// A toString16 function that creates the UTF-16 string by calling the function.
///
template <typename AllocatorT>
inline Balau::U16String<AllocatorT> toString16(const std::function<std::u16string ()> & f) {
	return f();
}

///
/// Convert the supplied std::pair value to a UTF-16 string.
///
template <typename AllocatorT, typename F, typename S>
inline Balau::U16String<AllocatorT> toString16(const std::pair<F, S> & p) {
	return std::u16string(u"{ ") + toString16<AllocatorT>(p.first) + u", " + toString16<AllocatorT>(p.second) + u" }";
}

///
/// Helper for container to UTF-16 string functions.
///
/// This helper function can be used for custom container types if required.
///
template <typename AllocatorT, typename ... T, template <typename ...> class C>
inline std::u16string toString16Helper(const C<T ...> & c)  {
	std::u16string builder = u"{";
	std::u16string prefix = u" ";
	bool empty = true;

	for (const auto & e : c) {
		builder += prefix + toString16<AllocatorT>(e);
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
#define BALAU_CONTAINER1_TO_STRING16_A(CONTAINER_TYPE)                           \
	template <typename AllocatorT, typename T>                                    \
	inline Balau::U16String<AllocatorT> toString16(const CONTAINER_TYPE<T> & c) { \
		return toString16Helper<AllocatorT>(c);                                   \
	}

///
/// Define a UTF-16 to-string function for a container that has two template parameters.
///
#define BALAU_CONTAINER2_TO_STRING16_A(CONTAINER_TYPE)                              \
	template <typename AllocatorT, typename T, typename U>                           \
	inline Balau::U16String<AllocatorT> toString16(const CONTAINER_TYPE<T, U> & c) { \
		return toString16Helper<AllocatorT>(c);                                      \
	}

///
/// Define a UTF-16 to-string function for a container that has three template parameters.
///
#define BALAU_CONTAINER3_TO_STRING16_A(CONTAINER_TYPE)                                 \
	template <typename AllocatorT, typename T, typename U, typename V>                  \
	inline Balau::U16String<AllocatorT> toString16(const CONTAINER_TYPE<T, U, V> & c) { \
		return toString16Helper<AllocatorT>(c);                                         \
	}

///
/// Define a UTF-16 to-string function for a container that has four template parameters.
///
#define BALAU_CONTAINER4_TO_STRING16_A(CONTAINER_TYPE)                                    \
	template <typename AllocatorT, typename T, typename U, typename V, typename W>         \
	inline Balau::U16String<AllocatorT> toString16(const CONTAINER_TYPE<T, U, V, W> & c) { \
		return toString16Helper<AllocatorT>(c);                                            \
	}

///
/// Define a UTF-16 to-string function for a container that has five template parameters.
///
#define BALAU_CONTAINER5_TO_STRING16_A(CONTAINER_TYPE)                                        \
	template <typename AllocatorT, typename T, typename U, typename V, typename W, typename X> \
	inline Balau::U16String<AllocatorT> toString16(const CONTAINER_TYPE<T, U, V, W, X> & c) {  \
		return toString16Helper<AllocatorT>(c);                                                \
	}

BALAU_CONTAINER2_TO_STRING16_A(std::deque)
BALAU_CONTAINER2_TO_STRING16_A(std::forward_list)
BALAU_CONTAINER2_TO_STRING16_A(std::list)
BALAU_CONTAINER2_TO_STRING16_A(std::map)
BALAU_CONTAINER3_TO_STRING16_A(std::multiset)
BALAU_CONTAINER4_TO_STRING16_A(std::multimap)
BALAU_CONTAINER3_TO_STRING16_A(std::priority_queue)
BALAU_CONTAINER2_TO_STRING16_A(std::queue)
BALAU_CONTAINER2_TO_STRING16_A(std::set)
BALAU_CONTAINER2_TO_STRING16_A(std::stack)
BALAU_CONTAINER5_TO_STRING16_A(std::unordered_map)
BALAU_CONTAINER5_TO_STRING16_A(std::unordered_multimap)
BALAU_CONTAINER4_TO_STRING16_A(std::unordered_multiset)
BALAU_CONTAINER3_TO_STRING16_A(std::unordered_set)
BALAU_CONTAINER2_TO_STRING16_A(std::vector)

///
/// Concatenates the to-string values of the input arguments.
///
/// Calls toString16 on each input argument and concatenates them together to
/// form a single UTF-16 string.
///
template <typename AllocatorT, typename P1, typename P2, typename ... P>
inline Balau::U16String<AllocatorT> toString16(const P1 & p1, const P2 & p2, const P & ... p) {
	return toString16<AllocatorT>(p1)
		+ toString16<AllocatorT>(p2)
		+ (Balau::U16String<AllocatorT>() + ... + toString16<AllocatorT>(p));
}

///////////////////// Universal UTF-32 to-string function /////////////////////

///
/// Convert the supplied UTF-8 string to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::string & value) {
	// TODO
	return Balau::U32String<AllocatorT>();
}

///
/// Convert the supplied UTF-8 string to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const Balau::U8String<AllocatorT> & value) {
	// TODO
	return Balau::U32String<AllocatorT>();
}

///
/// Convert the supplied UTF-8 string view to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::string_view & value) {
	return toString32<AllocatorT>(Balau::U8String<AllocatorT>(value));
}

///
/// Convert the supplied UTF-16 string to a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::u16string & value) {
	// TODO
	return Balau::U32String<AllocatorT>();
}

///
/// Returns the supplied value as is.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const Balau::U32String<AllocatorT> & value) {
	return value;
}

///
/// Convert the std::u32string to a Balau::U32String<AllocatorT>.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::u32string & value) {
	return Balau::U8String<AllocatorT>(value.data(), value.length());
}

///
/// Creates a UTF-32 string from the UTF-32 string view.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::u32string_view & value) {
	return Balau::U32String<AllocatorT>(value);
}

///
/// Returns the supplied ascii character as a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(char value) {
	Balau::U8String<AllocatorT> str;
	str += value;
	return toString32<AllocatorT>(str);
}

///
/// Convert the supplied UTF-16 character to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(char16_t & value) {
	Balau::U16String<AllocatorT> str;
	str += value;
	return toString32<AllocatorT>(str);
}

///
/// Convert the supplied code point to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(char32_t & value) {
	Balau::U32String<AllocatorT> str;
	str += value;
	return str;
}

///
/// Convert the supplied signed char to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(signed char value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied short to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(short value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied int to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(int value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied long to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(long value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied long long to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(long long value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned char to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(unsigned char value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned short to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(unsigned short value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned int to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(unsigned int value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned long to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(unsigned long value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied unsigned long long to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(unsigned long long value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied float to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(float value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied double to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(double value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied long double to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(long double value) {
	return toString32<AllocatorT>(toString<AllocatorT>(value));
}

///
/// Convert the supplied complex to a UTF-32 string.
///
template <typename T, typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::complex<T> & value) {
	return toString32<AllocatorT>(value.real(), " + j", value.imag());
}

///
/// Convert the supplied boolean to a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(bool value) {
	return value ? U"true" : U"false";
}

///
/// Returns the supplied const char * as a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const char * value) {
	return toString32<AllocatorT>(toString<AllocatorT>((value)));
}

///
/// Returns the supplied const char16_t * as a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const char16_t * value) {
	return toString32<AllocatorT>(toString16<AllocatorT>((value)));
}

///
/// Returns the supplied const char32_t * as a UTF-32 string.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const char32_t * value) {
	return Balau::U32String<AllocatorT>(value);
}

///
/// Convert the supplied pointer to a UTF-32 string.
///
/// The pointer is converted to an unsigned long long integer value and toString32
/// is called on the value.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const void * ptr) {
	return toString32<AllocatorT>((size_t) ptr);
}

///
/// Convert the supplied nullptr value to a UTF-32 string.
///
/// The returned value is zero in order to match the values
/// returned for other pointer to-string functions.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(std::nullptr_t) {
	return U"0";
}

///
/// Return the name of the type index.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(std::type_index value) {
	return toString32<AllocatorT>(value.name());
}

///
/// A toString32 function that creates the UTF-32 string by calling the function.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(const std::function<Balau::U32String<AllocatorT> ()> & f) {
	return f();
}

///
/// Convert the supplied std::pair value to a UTF-32 string.
///
template <typename AllocatorT, typename F, typename S>
inline Balau::U32String<AllocatorT> toString32(const std::pair<F, S> & p) {
	return Balau::U32String<AllocatorT>(U"{ ")
		+ toString32<AllocatorT>(p.first)
		+ U", "
		+ toString32<AllocatorT>(p.second)
		+ U" }";
}

///
/// Helper for container to UTF-8 string functions.
///
/// This helper function can be used for custom container types if required.
///
template <typename AllocatorT, typename ... T, template <typename ...> class C>
inline Balau::U32String<AllocatorT> toString32Helper(const C<T ...> & c)  {
	Balau::U32String<AllocatorT> builder = U"{";
	Balau::U32String<AllocatorT> prefix = U" ";
	bool empty = true;

	for (const auto & e : c) {
		builder += prefix + toString32<AllocatorT>(e);
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
#define BALAU_CONTAINER1_TO_STRING32_A(CONTAINER_TYPE)                           \
	template <typename AllocatorT, typename T>                                    \
	inline Balau::U32String<AllocatorT> toString32(const CONTAINER_TYPE<T> & c) { \
		return toString32Helper<AllocatorT>(c);                                   \
	}

///
/// Define a UTF-32 to-string function for a container that has two template parameters.
///
#define BALAU_CONTAINER2_TO_STRING32_A(CONTAINER_TYPE)                              \
	template <typename AllocatorT, typename T, typename U>                           \
	inline Balau::U32String<AllocatorT> toString32(const CONTAINER_TYPE<T, U> & c) { \
		return toString32Helper<AllocatorT>(c);                                      \
	}

///
/// Define a UTF-32 to-string function for a container that has three template parameters.
///
#define BALAU_CONTAINER3_TO_STRING32_A(CONTAINER_TYPE)                                 \
	template <typename AllocatorT, typename T, typename U, typename V>                  \
	inline Balau::U32String<AllocatorT> toString32(const CONTAINER_TYPE<T, U, V> & c) { \
		return toString32Helper<AllocatorT>(c);                                         \
	}

///
/// Define a UTF-32 to-string function for a container that has four template parameters.
///
#define BALAU_CONTAINER4_TO_STRING32_A(CONTAINER_TYPE)                                    \
	template <typename AllocatorT, typename T, typename U, typename V, typename W>         \
	inline Balau::U32String<AllocatorT> toString32(const CONTAINER_TYPE<T, U, V, W> & c) { \
		return toString32Helper<AllocatorT>(c);                                            \
	}

///
/// Define a UTF-32 to-string function for a container that has five template parameters.
///
#define BALAU_CONTAINER5_TO_STRING32_A(CONTAINER_TYPE)                                        \
	template <typename AllocatorT, typename T, typename U, typename V, typename W, typename X> \
	inline Balau::U32String<AllocatorT> toString32(const CONTAINER_TYPE<T, U, V, W, X> & c) {  \
		return toString32Helper<AllocatorT>(c);                                                \
	}

BALAU_CONTAINER2_TO_STRING32_A(std::deque)
BALAU_CONTAINER2_TO_STRING32_A(std::forward_list)
BALAU_CONTAINER2_TO_STRING32_A(std::list)
BALAU_CONTAINER2_TO_STRING32_A(std::map)
BALAU_CONTAINER3_TO_STRING32_A(std::multiset)
BALAU_CONTAINER4_TO_STRING32_A(std::multimap)
BALAU_CONTAINER3_TO_STRING32_A(std::priority_queue)
BALAU_CONTAINER2_TO_STRING32_A(std::queue)
BALAU_CONTAINER2_TO_STRING32_A(std::set)
BALAU_CONTAINER2_TO_STRING32_A(std::stack)
BALAU_CONTAINER5_TO_STRING32_A(std::unordered_map)
BALAU_CONTAINER5_TO_STRING32_A(std::unordered_multimap)
BALAU_CONTAINER4_TO_STRING32_A(std::unordered_multiset)
BALAU_CONTAINER3_TO_STRING32_A(std::unordered_set)
BALAU_CONTAINER2_TO_STRING32_A(std::vector)

///
/// Concatenates the to-string values of the input arguments.
///
/// Calls toString32 on each input argument and concatenates them together to
/// form a single UTF-32 string.
///
template <typename AllocatorT, typename P1, typename P2, typename ... P>
inline Balau::U32String<AllocatorT> toString32(const P1 & p1, const P2 & p2, const P & ... p) {
	return toString32<AllocatorT>(p1)
		+ toString32<AllocatorT>(p2)
		+ (Balau::U32String<AllocatorT>() + ... + toString32<AllocatorT>(p));
}

/////////////// Universal to-string function - templated version //////////////

///
/// Template class based to-string function for use in template classes.
///
template <typename CharT, typename AllocatorT = std::allocator<CharT>> struct ToString;

///
/// UTF-8 specialisation of ToString<CharT, AllocatorT>.
///
/// Convert the supplied object to a Balau::U8String<AllocatorT> by calling toString<AllocatorT>.
///
template <typename AllocatorT> struct ToString<char, AllocatorT> {
	template <typename T> Balau::U8String<AllocatorT> operator () (const T & object) const {
		return toString<AllocatorT>(object);
	}
};

///
/// UTF-16 specialisation of ToString<CharT, AllocatorT>.
///
/// Convert the supplied object to a Balau::U16String<AllocatorT> by calling toString16<AllocatorT>.
///
template <typename AllocatorT> struct ToString<char16_t, AllocatorT> {
	template <typename T> Balau::U16String<AllocatorT> operator () (const T & object) const {
		return toString16<AllocatorT>(object);
	}
};

///
/// UTF-32 specialisation of ToString<CharT, AllocatorT>.
///
/// Convert the supplied object to a Balau::U32String<AllocatorT> by calling toString32<AllocatorT>.
///
template <typename AllocatorT> struct ToString<char32_t, AllocatorT> {
	template <typename T> Balau::U32String<AllocatorT> operator () (const T & object) const {
		return toString32<AllocatorT>(object);
	}
};

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_A
