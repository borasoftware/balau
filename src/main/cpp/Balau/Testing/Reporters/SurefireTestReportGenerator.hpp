// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_REPORTERS__SUREFIRE_TEST_REPORT_GENERATOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_REPORTERS__SUREFIRE_TEST_REPORT_GENERATOR

#include <Balau/Testing/Impl/TestReportGenerator.hpp>

namespace Balau::Testing::Reporters {

///
/// Test group report generator that generates XML reports with the Maven Surefire plugin schema.
///
class SurefireTestReportGenerator : public ::Balau::Impl::TestReportGenerator {
	public: SurefireTestReportGenerator() = default;

	public: explicit SurefireTestReportGenerator(Resource::File outputFolder_)
		: ::Balau::Impl::TestReportGenerator(std::move(outputFolder_)) {}

	public: void generate(const ::Balau::Impl::TestGroupBase & group,
	                      const std::chrono::nanoseconds & groupDuration,
	                      const std::vector<const ::Balau::Impl::TestResult *> & groupTestResults) override;
};

} // namespace Balau::Testing::Reporters

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_REPORTERS__SUREFIRE_TEST_REPORT_GENERATOR
