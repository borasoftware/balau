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
	                                         bool useNamespaces_,
	                                         GroupedTestCaseMap & testCasesByGroup,
	                                         const std::string & testList)
		: TestRunnerExecutor(
			  std::unique_ptr<TestResultQueue>(new SingleThreadedTestResultQueue)
			, writer_
			, useNamespaces_
			, testCasesByGroup
			, testList
			, false
		) {}

	public: void run() override {
		resultQueue = std::unique_ptr<TestResultQueue>(new SingleThreadedTestResultQueue());
		size_t committedRuns = 0;

		while (committedRuns < tests.size()) {
			runTest("", tests[committedRuns]);

			TestResult testResult = resultQueue->tryDequeue();

			if (testResult.duration != -1 && testResult.result == TestResult::Result::Ignored) {
				// A valid test case was dequeued but was set to ignore.
				++committedRuns;
			} else if (testResult.duration != -1) {
				// A valid test case was dequeued and was run.
				processTestResultMessage(std::move(testResult));
				++committedRuns;
			}
		}
	}

	public: ExecutionModel getExecutionModel() const override {
		return ExecutionModel::SingleThreaded;
	}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__SINGLE_THREADED_TEST_RUNNER_EXECUTOR
