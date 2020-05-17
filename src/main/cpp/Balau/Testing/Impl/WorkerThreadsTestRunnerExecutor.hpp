// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__WORKER_THREADS_TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__WORKER_THREADS_TEST_RUNNER_EXECUTOR

#include <Balau/Container/ArrayBlockingQueue.hpp>
#include <Balau/Testing/Impl/TestRunnerExecutor.hpp>

namespace Balau::Impl {

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
	                                        std::shared_ptr<TestReportGenerator> & reportGenerator_,
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

	public: Testing::ExecutionModel getExecutionModel() const override {
		return Testing::ExecutionModel::WorkerThreads;
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

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__WORKER_THREADS_TEST_RUNNER_EXECUTOR
