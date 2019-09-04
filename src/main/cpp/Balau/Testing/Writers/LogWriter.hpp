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
/// Test writer that writes to the specified %Balau logger.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__LOG_WRITER
#define COM_BORA_SOFTWARE__BALAU_TESTING__LOG_WRITER

#include <Balau/Testing/Writers/TestWriter.hpp>

namespace Balau {

class Logger;

namespace Testing {

///
/// A test writer that writes to the specified %Balau logger.
///
class LogWriter : public TestWriter {
	public: explicit LogWriter(const Logger & logger_)
		: logger(logger_) {}

	public: explicit LogWriter(const std::string & loggingNamespace = "testrunner");

	public: void writeString(const std::string & str) override;

	public: std::unique_ptr<TestWriter> clone() const override {
		return std::unique_ptr<TestWriter>(new LogWriter(logger));
	}

	////////////////////////// Private implementation /////////////////////////

	private: const Logger & logger;
};

} // namespace Testing

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__LOG_WRITER
