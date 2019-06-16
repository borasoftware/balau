#include <utility>

// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_REPORT_GENERATOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_REPORT_GENERATOR

#include <Balau/Resource/File.hpp>

namespace Balau::Testing::Impl {

class TestGroupBase;
class TestResult;

// Abstract base class for test group report generators.
class TestReportGenerator {
	public: void setOutputFolder(Resource::File outputFolder_) {
		outputFolder = std::move(outputFolder_);
	}

	public: virtual void generate(const TestGroupBase & group,
	                              const std::chrono::nanoseconds & groupDuration,
	                              const std::vector<const TestResult *> & groupTestResults) = 0;

	protected: TestReportGenerator() : outputFolder() {}

	protected: explicit TestReportGenerator(Resource::File outputFolder_) : outputFolder(std::move(outputFolder_)) {}

	public: virtual ~TestReportGenerator() = default;

	protected: Resource::File outputFolder;
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_REPORT_GENERATOR
