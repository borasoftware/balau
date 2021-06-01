// @formatter:off
//
// Balau core C++ library
//
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
/// @file Streams.hpp
///
/// Utilities for streams.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__STREAMS
#define COM_BORA_SOFTWARE__BALAU_UTIL__STREAMS

#include <Balau/Type/StdTypes.hpp>

#include <iostream>
#include <sstream>

/////////////////////////////////// Reading ///////////////////////////////////

///
/// Read all the text into a UTF-8 string from the supplied UTF-8 input stream.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(std::istream & inputStream) {
	std::istreambuf_iterator<char> eof;
	return Balau::U8String<AllocatorT>(std::istreambuf_iterator<char>(inputStream), eof);
}

///
/// Read all the text into a UTF-8 string from the supplied UTF-8 input stream.
///
inline std::string toString(std::istream & inputStream) {
	std::istreambuf_iterator<char> eof;
	return std::string(std::istreambuf_iterator<char>(inputStream), eof);
}

///
/// Read all the text into a UTF-32 string from the supplied UTF-32 input stream.
///
template <typename AllocatorT>
inline Balau::U32String<AllocatorT> toString32(std::u32istream & inputStream) {
	std::istreambuf_iterator<char32_t> eof;
	return Balau::U32String<AllocatorT>(std::istreambuf_iterator<char32_t>(inputStream), eof);
}

///
/// Read all the text into a UTF-32 string from the supplied UTF-32 input stream.
///
inline std::u32string toString32(std::u32istream & inputStream) {
	std::istreambuf_iterator<char32_t> eof;
	return std::u32string(std::istreambuf_iterator<char32_t>(inputStream), eof);
}

namespace Balau::Util {

///
/// Utilities for streams.
///
class Streams final {
	///
	/// Read all lines of text from the supplied input stream into a vector.
	///
	public: static std::vector<std::string> readLinesToVector(std::istream & inputStream) {
		std::vector<std::string> lines;
		std::string s;

		while (std::getline(inputStream, s)) {
			lines.push_back(s);
		}

		return lines;
	}

	///
	/// Consume all the data from the supplied input stream into the supplied
	/// output stream.
	///
	public: static void consume(std::ostream & dst, std::istream & src) {
		dst << src.rdbuf();
	}

	///////////////////////////////////////////////////////////////////////////

	Streams() = delete;
	Streams(const Streams &) = delete;
	Streams & operator = (const Streams &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__STREAMS
