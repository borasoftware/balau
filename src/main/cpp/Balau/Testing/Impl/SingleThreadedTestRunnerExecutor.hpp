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
	                                         std::shared_ptr<Impl::TestReportGenerator> & reportGenerator_,
	                                         bool useNamespaces_,
	                                         GroupedTestCaseMap & testCasesByGroup,
	                                         const std::string & testList)
		: TestRunnerExecutor(
			  std::unique_ptr<TestResultQueue>(new SingleThreadedTestResultQueue)
			, writer_
			, reportGenerator_
			, useNamespaces_
			, testCasesByGroup
			, testList
			, false
		) {}

	public: void run() override {
		if (tests.empty()) {
			writer << "No tests to run.\n";
			return;
		}

		resultQueue = std::unique_ptr<TestResultQueue>(new SingleThreadedTestResultQueue());
		size_t committedRuns = 0;

		while (committedRuns < tests.size()) {
			runTest("", tests[committedRuns]);

			TestResult testResult = resultQueue->tryDequeue();

			if (testResult.duration != -1) {
				// A valid test case was dequeued.
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
