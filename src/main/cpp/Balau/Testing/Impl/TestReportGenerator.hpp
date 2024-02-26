#include <utility>

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

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_TEST_REPORT_GENERATOR
#define COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_TEST_REPORT_GENERATOR

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

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_TEST_REPORT_GENERATOR
