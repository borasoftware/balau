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
/// @file FileUtf32To8WriteResource.hpp
///
/// A write-only file based UTF-8 resource which is written as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF32_TO_8_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF32_TO_8_WRITE_RESOURCE

#include <Balau/Resource/Utf32To8WriteResource.hpp>

#include <boost/filesystem.hpp>

namespace Balau::Resource {

class File;
class Uri;

///
/// A write-only UTF-8 resource in a standard file on a file system, which is written with UTF-32 characters.
///
class FileUtf32To8WriteResource : public Utf32To8WriteResource {
	///
	/// Create a new file UTF-32 to UTF-8 write resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit FileUtf32To8WriteResource(const File & file_);

	public: FileUtf32To8WriteResource(FileUtf32To8WriteResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileUtf32To8WriteResource() override {
		close();
	}

	public: std::u32ostream & writeStream() override {
		return *stream;
	}

	public: const Uri & uri() const override;

	///
	/// Get the file URI.
	///
	/// @return the file URI
	///
	public: const File & getFile() const;

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<File> file;
	private: std::unique_ptr<std::u32ofstream> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF32_TO_8_WRITE_RESOURCE
