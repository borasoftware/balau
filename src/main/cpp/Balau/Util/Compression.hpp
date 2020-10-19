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
/// @file Compression.hpp
///
/// Compression utilities: gzip and zip reading/writing.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__COMPRESSION
#define COM_BORA_SOFTWARE__BALAU_UTIL__COMPRESSION

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Util/DateTime.hpp>
#include <Balau/Util/Files.hpp>

#include <algorithm>
#include <chrono>
#include <iosfwd>
#include <sstream>

#include <boost/iostreams/filtering_stream.hpp>

namespace Balau {

namespace Util {

///
/// %GZip compression utilities.
///
struct GZip final {
	///
	/// Gzip the specified input file to the specified output file.
	///
	/// @param input the file to gzip
	/// @param output the file to write the gzipped stream to
	///
	static void gzip(const Resource::File & input, const Resource::File & output) {
		std::ofstream out(output.getEntry().path(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;
		filter.push(boost::iostreams::gzip_compressor());
		filter.push(out);
		std::ifstream in(input.getEntry().path(), std::ios::binary);
		boost::iostreams::copy(in, filter);
	}

	///
	/// Gzip the supplied string to the specified output file.
	///
	/// @param input the string to gzip
	/// @param output the file to write the gzipped stream to
	///
	static void gzip(const std::string & input, const Resource::File & output) {
		std::ofstream out(output.getEntry().path(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;
		filter.push(boost::iostreams::gzip_compressor());
		filter.push(out);
		std::istringstream in(input, std::ios::binary);
		boost::iostreams::copy(in, filter);
	}

	///
	/// Gzip the supplied input stream to the specified output file.
	///
	/// @param input the input stream to gzip
	/// @param output the file to write the gzipped stream to
	///
	static void gzip(std::istream & input, const Resource::File & output) {
		std::ofstream out(output.getEntry().path(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;
		filter.push(boost::iostreams::gzip_compressor());
		filter.push(out);
		boost::iostreams::copy(input, filter);
	}

	///
	/// Gunzip the specified input file to the specified output file.
	///
	/// @param input the gzipped file
	/// @param output the file to write the uncompressed stream to
	///
	static void gunzip(const Resource::File & input, const Resource::File & output) {
		std::ifstream in(input.getEntry().path(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> filter;
		filter.push(boost::iostreams::gzip_decompressor());
		filter.push(in);
		std::ofstream out(output.getEntry().path(), std::ios::binary);
		boost::iostreams::copy(filter, out);
	}

	///
	/// Gunzip the specified file to a string.
	///
	/// Be sure that the data will fit in memory (e.g. check the file size before
	/// calling), or the application will cause a catastophic memory failure.
	///
	/// @param input the gzipped file
	/// @return a string containing the uncompressed data
	///
	static void gunzip(const Resource::File & input, std::string & output) {
		std::ifstream in(input.getEntry().path(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> filter;
		filter.push(boost::iostreams::gzip_decompressor());
		filter.push(in);
		std::ostringstream out(std::ios::binary);
		boost::iostreams::copy(filter, out);
		output = out.str();
	}

	///
	/// Get an input stream of the uncompressed contents of the specified gzipped input file.
	///
	/// @param input the gzipped file
	/// @return an input stream wrapper supplying a uncompressed stream
	///
	static void gunzip(const Resource::File & input, std::unique_ptr<std::istream> & stream) {
		std::ifstream inputStream(input.getEntry().path(), std::ios::binary);
		stream = std::make_unique<boost::iostreams::filtering_istream>();
		auto * filterStream = static_cast<boost::iostreams::filtering_istream *>(stream.get());
		filterStream->push(boost::iostreams::gzip_decompressor());
		filterStream->push(inputStream);
	}

	///////////////////////////////////////////////////////////////////////////

	GZip() = delete;
	GZip(const GZip &) = delete;
	GZip & operator = (const GZip &) = delete;
};

} // namespace Util

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__COMPRESSION
