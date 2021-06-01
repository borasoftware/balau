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
/// @file Files.hpp
///
/// Utilities for files.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__FILES
#define COM_BORA_SOFTWARE__BALAU_UTIL__FILES

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <sstream>

namespace std { // NOLINT

template <> struct hash<boost::filesystem::directory_entry> {
	size_t operator () (const boost::filesystem::directory_entry & value) const noexcept {
		return hash<boost::filesystem::path::string_type>()(value.path().native());
	}
};

template <> struct equal_to<boost::filesystem::directory_entry> {
	bool operator () (const boost::filesystem::directory_entry & x,
	                  const boost::filesystem::directory_entry & y) const {
		return x == y;
	}
};

} // namespace std

namespace boost::filesystem {

///
/// Print the file system path as a UTF-8 string.
///
/// @return a UTF-8 string representing the file system path
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const boost::filesystem::path & path) {
	return toString<AllocatorT>(path.string());
}

///
/// Print the file system path as a UTF-8 string.
///
/// @return a UTF-8 string representing the file system path
///
inline std::string toString(const boost::filesystem::path & path) {
	return path.string();
}

} // namespace boost::filesystem

namespace Balau::Util {

///
/// File utilities.
///
struct Files final {
	///////////////////////////////// Copying /////////////////////////////////

	///
	/// Copy the contents of the source file into the destination file.
	///
	static void copy(const Resource::File & src, const Resource::File & dst) {
		boost::filesystem::copy(src.getEntry(), dst.getEntry());
	}

	///////////////////////////////// Reading /////////////////////////////////

	///
	/// Read all lines of text of the specified file into a string vector.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	///
	static std::vector<std::string> toLines(const Resource::File & file) {
		if (!file.exists()) {
			ThrowBalauException(Exception::FileNotFoundException, file);
		}

		boost::filesystem::ifstream inputStream(file.getEntry());
		std::vector<std::string> lines;
		std::string s;

		while (std::getline(inputStream, s)){
			lines.push_back(s);
		}

		return lines;
	}

	///
	/// Read the specified file into a string.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	///
	static std::string readToString(const Resource::File & file) {
		if (!file.exists()) {
			ThrowBalauException(Exception::FileNotFoundException, file);
		}

		boost::filesystem::ifstream inputStream(file.getEntry());
		std::istreambuf_iterator<char> eos;
		return std::string(std::istreambuf_iterator<char>(inputStream), eos);
	}

	///
	/// Read the specified file into a character vector.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	///
	static std::vector<char> readToVector(const Resource::File & file) {
		if (!file.exists()) {
			ThrowBalauException(Exception::FileNotFoundException, file);
		}

		boost::filesystem::ifstream inputStream(file.getEntry());
		std::istreambuf_iterator<char> eos;
		return std::vector<char>(std::istreambuf_iterator<char>(inputStream), eos);
	}

	///////////////////////////////////////////////////////////////////////////

	Files() = delete;
	Files(const Files &) = delete;
	Files & operator = (const Files &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__FILES
