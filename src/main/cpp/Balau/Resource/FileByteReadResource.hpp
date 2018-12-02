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
/// @file FileByteReadResource.hpp
///
/// A read-only standard file on a file system which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_READ_RESOURCE

#include <Balau/Resource/ByteReadResource.hpp>

#include <boost/filesystem.hpp>

namespace Balau::Resource {

class File;
class Uri;

///
/// A read-only standard file on a file system which is read as bytes.
///
class FileByteReadResource : public ByteReadResource {
	///
	/// Create a new file byte read resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit FileByteReadResource(const File & file_);

	public: ~FileByteReadResource() {
		close();
	}

	public: std::istream & readStream() override {
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
	private: boost::filesystem::ifstream stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_READ_RESOURCE
