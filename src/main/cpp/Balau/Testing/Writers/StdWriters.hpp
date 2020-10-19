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
/// @file StdWriters.hpp
///
/// Pre-defined test writer classes for stdout, stderr, and files.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__STD_WRITERS
#define COM_BORA_SOFTWARE__BALAU_TESTING__STD_WRITERS

#include <Balau/Resource/FileByteWriteResource.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Resource/Uri.hpp>
#include <Balau/Testing/Assertions.hpp>
#include <Balau/Testing/Writers/TestWriter.hpp>
#include <Balau/Util/Vectors.hpp>

#include <mutex>

namespace Balau::Testing {

///
/// A test writer that writes to stdout.
///
class StdOutTestWriter : public TestWriter {
	public: void writeString(const std::string & str) override {
		std::cout << str;
	}

	public: std::unique_ptr<TestWriter> clone() const override {
		return std::unique_ptr<TestWriter>(new StdOutTestWriter());
	}
};

///
/// A test writer that writes to the specified file.
///
class FileTestWriter : public TestWriter {
	public: explicit FileTestWriter(const Resource::File & file) : writeResource(file) {}

	public: void writeString(const std::string & str) override {
		std::lock_guard<std::mutex> lock(mutex);
		writeResource.writeStream() << str;
	}

	public: std::unique_ptr<TestWriter> clone() const override {
		return std::unique_ptr<TestWriter>(new FileTestWriter(writeResource.getFile()));
	}

	////////////////////////// Private implementation /////////////////////////

	private: Resource::FileByteWriteResource writeResource;
	private: std::mutex mutex;
};

///
/// A test writer that writes to the supplied output stream.
///
class OStreamTestWriter : public TestWriter {
	public: explicit OStreamTestWriter(std::ostream & stream_) : stream(stream_) {}

	public: void writeString(const std::string & str) override {
		stream << str;
	}

	public: std::unique_ptr<TestWriter> clone() const override {
		return std::unique_ptr<TestWriter>(new OStreamTestWriter(stream));
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::ostream & stream;
};

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__STD_WRITERS
