// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__SINGLE_THREADED_TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__SINGLE_THREADED_TEST_RUNNER_EXECUTOR

#include <Balau/Testing/Impl/TestRunnerExecutor.hpp>

namespace Balau::Testing::Impl {

// Test runner executor which implements the SingleThreaded execution model.
class SingleThreadedTestRunnerExecutor : public TestRunnerExecutor {
	private: class SingleThreadedTestResultQueue : public TestResultQueue {
		private: TestResult result;

		public: void enqueue(TestResult message) override {
			result = std::move(message);
		}

		public: TestResult tryDequeue() override {
			return std::move(result);
		}
	};

	public: SingleThreadedTestRunnerExecutor(CompositeWriter & writer_,
	                                         bool printNamespaces_,
	                                         GroupedTestCaseMap & testCasesByGroup)
		: TestRunnerExecutor(
			  std::unique_ptr<TestResultQueue>(new SingleThreadedTestResultQueue)
			, writer_
			, printNamespaces_
			, testCasesByGroup
			, false
		) {}

	public: void run() override {
		resultQueue = std::unique_ptr<TestResultQueue>(new SingleThreadedTestResultQueue());
		size_t testIndex = 0;

		while (testIndex < tests.size()) {
			runTest("", tests[testIndex]);

			TestResult testResult = resultQueue->tryDequeue();

			if (testResult.duration != -1) {
				processTestResultMessage(std::move(testResult));
			}

			++testIndex;
		}
	}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__SINGLE_THREADED_TEST_RUNNER_EXECUTOR
