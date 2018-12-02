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
/// @file FileUtf8To32ReadResource.hpp
///
/// A read-only file based UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Utf8To32ReadResource.hpp>

#include <boost/filesystem.hpp>

namespace Balau::Resource {

class File;
class Uri;

///
/// A read-only file based UTF-8 resource which is read as UTF-32 characters.
///
class FileUtf8To32ReadResource : public Utf8To32ReadResource {
	///
	/// Create a new file UTF-8 to UTF-32 read resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit FileUtf8To32ReadResource(const File & file_);

	public: ~FileUtf8To32ReadResource() {
		close();
	}

	public: std::u32istream & readStream() override {
		return stream;
	}

	public: const Uri & uri() const override;

	///
	/// Get the file URI.
	///
	/// @return the file URI
	///
	public: const File & getFile() const;

	public: void close() override {
		stream.close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<File> file;
	private: std::u32ifstream stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE
