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
/// @file FileByteWriteResource.hpp
///
/// A write only standard file on a file system which is written as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE

#include <Balau/Resource/ByteWriteResource.hpp>

#include <filesystem>
#include <fstream>

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

	public: FileByteWriteResource(FileByteWriteResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileByteWriteResource() override {
		close();
	}

	public: std::ostream & writeStream() override {
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
	private: std::unique_ptr<std::ofstream> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_WRITE_RESOURCE
