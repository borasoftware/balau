// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_GROUP_BASE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_GROUP_BASE

#include <string>

namespace Balau::Testing {

class TestRunner;

namespace Impl {

class TestRunnerExecutor;
class WorkerThreadsTestRunnerExecutor;
class MultiProcessTestRunnerExecutor;
class WorkerProcessesTestRunnerExecutor;
class ProcessPerTestTestRunnerExecutor;

// Non-parameterised base class of test classes.
class TestGroupBase {
	friend class ::Balau::Testing::TestRunner;
	friend class TestRunnerExecutor;
	friend class WorkerThreadsTestRunnerExecutor;
	friend class MultiProcessTestRunnerExecutor;
	friend class WorkerProcessesTestRunnerExecutor;
	friend class ProcessPerTestTestRunnerExecutor;

	public: virtual ~TestGroupBase() = default;

	private: virtual void setup() = 0;

	private: virtual void teardown() = 0;

	// Get the test class' name during the test framework setup.
	private: virtual const std::string & getGroupName() const = 0;

	// Set by the test runner.
	private: unsigned int groupIndex = 0;
};

} // namespace Impl

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_GROUP_BASE
