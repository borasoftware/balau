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
/// @file Writers.hpp
///
/// Pre-defined test writer classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__WRITERS
#define COM_BORA_SOFTWARE__BALAU_TESTING__WRITERS

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <Balau/Resource/FileByteWriteResource.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Resource/Uri.hpp>
#include <Balau/Testing/Assertions.hpp>
#include <Balau/Util/Vectors.hpp>

#include <mutex>

namespace Balau {

class Logger;

namespace Testing {

///
/// Base class of test writers.
///
class TestWriter {
	///
	/// Destroy a test writer instance.
	///
	public: virtual ~TestWriter() = default;

	///
	/// Convert the supplied value to a UTF-8 string and write the string to the writer's output stream(s).
	///
	/// @param value the object to write
	///
	public: template<typename T> void write(T value) {
		using ::toString;

		writeString(toString(value));
	}

	///
	/// Write the supplied UTF-8 string to the writer's output stream(s).
	///
	/// @param value the string to write
	///
	public: virtual void writeString(const std::string & value) = 0;

	///
	/// Clone the writer instance.
	///
	/// @return a clone of the writer
	///
	public: virtual std::unique_ptr<TestWriter> clone() const = 0;

	protected: TestWriter() = default;
};

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

///
/// A test writer that writes to the specified %Balau logger.
///
class LoggerTestWriter : public TestWriter {
	public: explicit LoggerTestWriter(const Logger & logger_)
		: logger(logger_) {}

	public: explicit LoggerTestWriter(const std::string & loggingNamespace = "testrunner");

	public: void writeString(const std::string & str) override;

	public: std::unique_ptr<TestWriter> clone() const override {
		return std::unique_ptr<TestWriter>(new LoggerTestWriter(logger));
	}

	////////////////////////// Private implementation /////////////////////////

	private: const Logger & logger;
};

} // namespace Testing

} // namespace Balau

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__WRITERS
