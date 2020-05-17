// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_LIMITS
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_LIMITS

namespace Balau::Impl {

// Defines the implementation limits of the test runner.
class TestRunnerLimits final {
	public: static const unsigned int MaxConcurrency = 128;

	///////////////////////////////////////////////////////////////////////////

	public: TestRunnerLimits() = delete;
	public: TestRunnerLimits(const TestRunnerLimits &) = delete;
	public: TestRunnerLimits & operator = (const TestRunnerLimits &) = delete;
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_LIMITS
