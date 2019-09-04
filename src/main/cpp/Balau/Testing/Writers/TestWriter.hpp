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
/// @file TestWriter.hpp
///
/// Base class of writer classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__TEST_WRITER
#define COM_BORA_SOFTWARE__BALAU_TESTING__TEST_WRITER

#include <Balau/Resource/FileByteWriteResource.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Resource/Uri.hpp>
#include <Balau/Testing/Assertions.hpp>
#include <Balau/Util/Vectors.hpp>

namespace Balau::Testing {

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

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__TEST_WRITER
