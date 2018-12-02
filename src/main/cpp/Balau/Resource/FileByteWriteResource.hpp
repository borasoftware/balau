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
/// @file FileByteWriteResource.hpp
///
/// A write only standard file on a file system which is written as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE

#include <Balau/Resource/ByteWriteResource.hpp>

#include <boost/filesystem.hpp>

namespace Balau::Resource {

class File;
class Uri;

///
/// A write only standard file on a file system which is written as bytes.
///
class FileByteWriteResource : public ByteWriteResource {
	///
	/// Create a new file byte write resource from the supplied file URI.
	///
	public: explicit FileByteWriteResource(const File & file_);

	public: ~FileByteWriteResource() {
		close();
	}

	public: std::ostream & writeStream() override {
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
	private: boost::filesystem::ofstream stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE
