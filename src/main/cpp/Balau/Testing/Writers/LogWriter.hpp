// @formatter:off
//
// Balau core C++ library
//
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
/// @file TestWriter.hpp
///
/// Test writer that writes to the specified %Balau logger.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__LOG_WRITER
#define COM_BORA_SOFTWARE__BALAU_TESTING__LOG_WRITER

#include <Balau/Testing/Writers/TestWriter.hpp>
#include <Balau/Logging/Logger.hpp>

namespace Balau {

class Logger;

namespace Testing {

///
/// A test writer that writes to the specified %Balau logger.
///
class LogWriter : public TestWriter {
	public: explicit LogWriter(const Logger & logger_)
		: logger(logger_) {}

	public: explicit LogWriter(const std::string & loggingNamespace = "testrunner")
		: logger(Logger::getLogger(loggingNamespace)) {}

	public: void writeString(const std::string & str) override {
		logger.info(str.c_str());
	}

	public: std::unique_ptr<TestWriter> clone() const override {
		return std::unique_ptr<TestWriter>(new LogWriter(logger));
	}

	////////////////////////// Private implementation /////////////////////////

	private: const Logger & logger;
};

} // namespace Testing

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__LOG_WRITER
