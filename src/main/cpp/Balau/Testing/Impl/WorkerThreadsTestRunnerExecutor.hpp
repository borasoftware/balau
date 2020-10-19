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
#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__WORKER_THREADS_TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__WORKER_THREADS_TEST_RUNNER_EXECUTOR

#include <Balau/Container/ArrayBlockingQueue.hpp>
#include <Balau/Testing/Impl/TestRunnerExecutor.hpp>

namespace Balau::Testing::Impl {

// Test runner executor which implements the WorkerThreads execution model.
class WorkerThreadsTestRunnerExecutor : public TestRunnerExecutor {
	private: class WorkerThreadsTestResultQueue : public TestResultQueue {
		private: Container::ArrayBlockingQueue<TestResult> resultQueue;

		public: WorkerThreadsTestResultQueue() : resultQueue(queueSize) {}

		public: void enqueue(TestResult message) override {
			resultQueue.enqueue(std::move(message));
		}

		public: TestResult tryDequeue() override {
			return resultQueue.dequeue();
		}
	};

	private: unsigned int concurrencyLevel;
	private: std::atomic_uint nextTestIndex;

	public: WorkerThreadsTestRunnerExecutor(CompositeWriter & writer_,
	                                        std::shared_ptr<Impl::TestReportGenerator> & reportGenerator_,
	                                        bool useNamespaces_,
	                                        GroupedTestCaseMap & testCasesByGroup,
	                                        const std::string & testList,
	                                        unsigned int concurrencyLevel_)
		: TestRunnerExecutor(
			  std::unique_ptr<TestResultQueue>(new (WorkerThreadsTestResultQueue))
			, writer_
			, reportGenerator_
			, useNamespaces_
			, testCasesByGroup
			, testList
			, false
		)
		, concurrencyLevel(concurrencyLevel_)
		, nextTestIndex(0) {}

	public: void run() override {
		if (tests.empty()) {
			writer << "No tests to run.\n";
			return;
		}

		std::vector<std::thread> threads;

		for (size_t m = 0; m < concurrencyLevel; m++) {
			threads.emplace_back(std::thread(childThreadLogic, this));
		}

		size_t committedRuns = 0;

		do {
			TestResult testResult = resultQueue->tryDequeue();

			if (testResult.duration != -1) {
				// A valid test case was dequeued.
				processTestResultMessage(std::move(testResult));
				++committedRuns;
			}
		} while (committedRuns < tests.size());

		for (size_t m = 0; m < concurrencyLevel; m++) {
			threads[m].join();
		}
	}

	public: ExecutionModel getExecutionModel() const override {
		return ExecutionModel::WorkerThreads;
	}

	private: static void childThreadLogic(WorkerThreadsTestRunnerExecutor * self) {
		while (true) {
			unsigned int testIndex = self->nextTestIndex++;

			if (testIndex >= self->tests.size()) {
				break;
			}

			self->runTest("", self->tests[testIndex]);
		}
	}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__WORKER_THREADS_TEST_RUNNER_EXECUTOR
