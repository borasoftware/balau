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
/// @file FileByteReadResource.hpp
///
/// A read-only standard file on a file system which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_READ_RESOURCE

#include <Balau/Resource/ByteReadResource.hpp>

#include <filesystem>
#include <fstream>

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

	public: FileByteReadResource(FileByteReadResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileByteReadResource() override {
		close();
	}

	public: std::istream & readStream() override {
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
	private: std::unique_ptr<std::ifstream> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_BYTE_READ_RESOURCE
