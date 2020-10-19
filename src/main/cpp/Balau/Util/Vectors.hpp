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
/// @file Vectors.hpp
///
/// Utilities for vectors.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__VECTORS
#define COM_BORA_SOFTWARE__BALAU_UTIL__VECTORS

#include <Balau/Type/ToString.hpp>

#include <cstring>
#include <memory>
#include <vector>

namespace Balau::Util {

///
/// Utilities for vectors.
///
struct Vectors final {
	///////////////// Multiple push back using a parameter pack ///////////////

	///
	/// Populate an existing vector via emplace back of multiple elements.
	///
	/// Given a variadic set of input arguments, call emplace_back on each element.
	///
	template <typename T, typename ... PT>
	static void pushBack(std::vector<T> & vector, T && first, PT && ... remaining) {
		vector.emplace_back(std::forward<T>(first));
		pushBack(vector, std::forward<PT>(remaining) ...);
	}

	///
	/// Populate an existing vector via emplace back of multiple elements (end case).
	///
	/// Given no variadic input arguments, do nothing.
	///
	template <typename T>
	static void pushBack(std::vector<T> & ) {}

	///
	/// Create and populate a vector via emplace back of multiple elements.
	///
	/// Given a variadic set of input arguments, call emplace_back on each element.
	///
	template <typename T, typename ... PT>
	static std::vector<T> pushBack(T && first, PT && ... remaining) {
		std::vector<T> vector;
		vector.emplace_back(std::forward<T>(first));
		pushBack(vector, std::forward<PT>(remaining) ...);
		return vector;
	}

	///
	/// Create and populate a vector via emplace back of multiple elements (empty case).
	///
	/// Given a variadic set of input arguments, call emplace_back on each element.
	///
	template <typename T> static std::vector<T> pushBack() {
		return std::vector<T>();
	}

	///////////////////// Push back or replace functions //////////////////////

	///
	/// Appends the supplied element to the vector or replaces an existing element if a matching one is found.
	///
	/// The supplied comparator is used to compare values (defaults to std::equal_to).
	///
	/// The supplied replace function is used to assign the supplied value to the
	/// existing one when a matching value is is found (defaults to operator =).
	///
	/// The data is copied or moved into the new or existing value depending on the typename.
	///
	template <typename T, typename T2, typename Compare = std::equal_to<T>, typename Replace = std::function<void (T &, T2 &&)>>
	static void pushBackOrReplace(std::vector<T> & dst, T2 && element, Compare compare = std::equal_to<T>(), Replace replace = DefaultReplace<T, T2>()) {
		auto existing = std::find_if(
			dst.begin(), dst.end(), [&element, &compare] (const auto & dstElement) { return compare(dstElement, element); }
		);

		if (existing != dst.end()) {
			replace(*existing, std::forward<T2>(element));
		} else {
			dst.push_back(std::forward<T2>(element));
		}
	}

	///////////////////// Miscellaneous utility functions /////////////////////

	///
	/// Appends the source vector to the destination vector.
	///
	template <typename T> static void append(std::vector<T> & dst, const std::vector<T> & src) {
		dst.insert(std::end(dst), std::begin(src), std::end(src));
	}

	///
	/// Moves the source vector into the destination vector.
	///
	template <typename T> static void move(std::vector<T> & dst, const std::vector<T> & src) {
		std::move(src.begin(), src.end(), std::back_inserter(dst));
	}

	///////////////////////////////////////////////////////////////////////////

	///
	/// Perform a map operator from one vector to another (transform the input elements to a vector of different elements).
	///
	template <typename D, typename S>
	static std::vector<D> map(const std::vector<S> & input, std::function<D (const S &)> f) {
		std::vector<D> ret;
		std::transform(input.begin(), input.end(), std::back_inserter(ret), f);
		return ret;
	}

	///////////////////////////////////////////////////////////////////////////

	///
	/// Convert the supplied UTF-8 string to a char vector.
	///
	static std::vector<char> toCharVector(const std::string & str) {
		std::vector<char> ret;
		ret.resize(str.length());
		std::memcpy(ret.data(), str.c_str(), str.length());
		return ret;
	}

	///
	/// Convert the supplied UTF-32 string to a char32_t vector.
	///
	static std::vector<char32_t> toCharVector(const std::u32string & str) {
		std::vector<char32_t> ret;
		ret.resize(str.length());
		std::memcpy(ret.data(), str.c_str(), str.length() * sizeof(char32_t));
		return ret;
	}

	///
	/// Convert the supplied vector to a vector of UTF-8 strings.
	///
	template <typename T> static std::vector<std::string> toStringVector(const std::vector<T> & vector) {
		std::vector<std::string> ret;
		ret.reserve(vector.size());

		using ::toString;

		for (size_t m = 0; m < vector.size(); m++) {
			ret.emplace_back(toString(vector[m]));
		}

		return ret;
	}

	///
	/// Convert the supplied vector to a vector of UTF-32 strings.
	///
	template <typename T> static std::vector<std::u32string> toString32Vector(const std::vector<T> & vector) {
		std::vector<std::u32string> ret;
		ret.reserve(vector.size());

		for (size_t m = 0; m < vector.size(); m++) {
			ret.emplace_back(toString32(vector[m]));
		}

		return ret;
	}

	///////////////////////////////////////////////////////////////////////////

	///
	/// Convert the characters in the supplied char vector to a UTF-8.
	///
	static std::string charsToString(const std::vector<char> & vector) {
		return std::string(vector.data(), vector.size());
	}

	///
	/// Convert the characters in the supplied char16_t vector to a UTF-16.
	///
	static std::u16string charsToString(const std::vector<char16_t> & vector) {
		return std::u16string(vector.data(), vector.size());
	}

	///
	/// Convert the characters in the supplied char32_t vector to a UTF-32.
	///
	static std::u32string charsToString(const std::vector<char32_t> & vector) {
		return std::u32string(vector.data(), vector.size());
	}

	///////////////////////////////////////////////////////////////////////////

	// The default copy/move replace function used in pushBackOrReplace.
	private: template <typename T, typename T2> struct DefaultReplace {
		void operator () (T & lhs, T2 && rhs) {
			lhs = std::forward<T2>(rhs);
		}
	};

	Vectors() = delete;
	Vectors(const Vectors &) = delete;
	Vectors & operator = (const Vectors &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__VECTORS
