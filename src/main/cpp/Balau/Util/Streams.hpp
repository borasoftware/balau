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
inline std::string toString(std::istream & inputStream) {
	std::istreambuf_iterator<char> eof;
	return std::string(std::istreambuf_iterator<char>(inputStream), eof);
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
