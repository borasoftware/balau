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
#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_REPORTERS__SUREFIRE_TEST_REPORT_GENERATOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_REPORTERS__SUREFIRE_TEST_REPORT_GENERATOR

#include <Balau/Testing/Impl/TestReportGenerator.hpp>

namespace Balau::Testing::Reporters {

///
/// Test group report generator that generates XML reports with the Maven Surefire plugin schema.
///
class SurefireTestReportGenerator : public Impl::TestReportGenerator {
	public: SurefireTestReportGenerator() = default;

	public: explicit SurefireTestReportGenerator(Resource::File outputFolder_)
		: Impl::TestReportGenerator(std::move(outputFolder_)) {}

	public: void generate(const Impl::TestGroupBase & group,
	                      const std::chrono::nanoseconds & groupDuration,
	                      const std::vector<const Impl::TestResult *> & groupTestResults) override;
};

} // namespace Balau::Testing::Reporters

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_REPORTERS__SUREFIRE_TEST_REPORT_GENERATOR
