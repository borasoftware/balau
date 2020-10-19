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
/// @file FileUtf8To32ReadResource.hpp
///
/// A read-only file based UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Utf8To32ReadResource.hpp>

#include <filesystem>
#include <fstream>

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

	public: FileUtf8To32ReadResource(FileUtf8To32ReadResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileUtf8To32ReadResource() override {
		close();
	}

	public: std::u32istream & readStream() override {
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
	private: std::unique_ptr<std::u32ifstream> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE_UTF8_TO_32_READ_RESOURCE
