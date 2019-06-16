// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__MULTI_PROCESS_TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__MULTI_PROCESS_TEST_RUNNER_EXECUTOR

#include <Balau/Concurrent/Fork.hpp>
#include <Balau/Exception/SystemExceptions.hpp>
#include <Balau/Interprocess/USharedMemoryObject.hpp>
#include <Balau/Interprocess/SharedMemoryQueue.hpp>
#include <Balau/Dev/Assert.hpp>
#include <Balau/System/Sleep.hpp>
#include <Balau/Testing/Impl/TestResultQueue.hpp>
#include <Balau/Testing/Impl/TestRunnerLimits.hpp>
#include <Balau/Testing/Impl/TestRunnerExecutor.hpp>
#include <Balau/Type/UUID.hpp>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <sys/wait.h>

namespace Balau::Testing::Impl {

// Abstract base class for test runner executors that run child processes.
class MultiProcessTestRunnerExecutor : public TestRunnerExecutor {
	private: class MultiProcessTestResultQueue : public TestResultQueue {
		private: static constexpr unsigned int messageSizeLimit = 2048;

		private: Interprocess::SharedMemoryQueue<TestResult> resultQueue;

		public: MultiProcessTestResultQueue() : resultQueue(queueSize, messageSizeLimit) {}

		public: void enqueue(TestResult message) override {
			resultQueue.enqueue(std::move(message));
		}

		public: TestResult tryDequeue() override {
			return resultQueue.tryDequeue(std::chrono::milliseconds(100));
		}
	};

	private: struct SharedState {
		// Shared atomic int, providing the next test case index to run.
		std::atomic_uint nextTestIndex { 0 };

		// Non shared ints, providing the current test case index that a child process is running.
		// These are checked when a child process starts up in order to determine if a child process seg-faulted.
		std::array<unsigned int, TestRunnerLimits::MaxConcurrency> currentTestIndices { 0 };
	};

	protected: const unsigned int concurrencyLevel;
	protected: const std::string sharedMemoryNamePrefix;
	private: Interprocess::MSharedMemoryObject<SharedState> sharedState;

	protected: MultiProcessTestRunnerExecutor(CompositeWriter & writer_,
	                                          std::shared_ptr<Impl::TestReportGenerator> & reportGenerator_,
	                                          bool useNamespaces_,
	                                          GroupedTestCaseMap & testCasesByGroup,
	                                          const std::string & testList,
	                                          unsigned int concurrencyLevel_)
		: TestRunnerExecutor(
			  std::unique_ptr<TestResultQueue>(new MultiProcessTestResultQueue)
			, writer_
			, reportGenerator_
			, useNamespaces_
			, testCasesByGroup
			, testList
			, true
		)
		, concurrencyLevel(concurrencyLevel_)
		, sharedMemoryNamePrefix(createSharedMemoryNamePrefix()) {}

	//
	// Create any missing child processes.
	// This method returns if the process is the parent.
	// If the process is the child, the child exits by calling exit(0) from within this method.
	//
	// @return true if the parent created any child processes
	//
	protected: bool createChildProcesses(std::vector<int> & childPids,
	                                     std::unordered_map<int, size_t> & pidIndexByPid,
	                                     bool singleShot,
	                                     bool firstCall = false) {
		bool childProcessesCreated = false;

		for (unsigned int childProcessIndex = 0; childProcessIndex < concurrencyLevel; ++childProcessIndex) {
			if (childPids[childProcessIndex] == 0) {
				const int pid = Concurrent::Fork::performFork(
					[&, this] () {
						// Child process.

						if (!singleShot) {
							if (firstCall) {
								writer << ("Child pid  = " + ::toString(getpid()) + "\n");
							} else {
								writer << ("Replacement child process created, pid  = " + ::toString(getpid()) + "\n");
							}
						}

						runChildLogic(childProcessIndex, singleShot);
						reportChildProcessCompletion();
						exit(0);
					}
				);

				// Parent process.
				childPids[childProcessIndex] = pid;
				pidIndexByPid[pid] = childProcessIndex;
				childProcessesCreated = true;
			}
		}

		return childProcessesCreated;
	}

	private: virtual void reportChildProcessCompletion() const = 0;

	private: void runChildLogic(unsigned int childProcessIndex, bool singleShot) {
		const std::string pidStr = getPidString();

		// Check for previous failed test due to segfault of previous child process.
		unsigned int testIndex = getCurrentTestIndex(childProcessIndex);

		if (testIndex != 0) {
			reportSegFaultedTest(pidStr, tests[testIndex]);
		}

		if (singleShot) {
			testIndex = claimNextTestIndex(childProcessIndex);

			if (testIndex < tests.size()) {
				runTest(pidStr, tests[testIndex]);
			}

			// Reset current test index (no segfault).
			resetCurrentTestIndex(childProcessIndex);
		} else {
			while (true) {
				testIndex = claimNextTestIndex(childProcessIndex);

				if (testIndex >= tests.size()) {
					break; // No more tests to run.
				}

				runTest(pidStr, tests[testIndex]);
			}
		}
	}

	protected: void reportSegFaultedTest(const std::string & pidStr, FlattenedTestCase & testToRun) {
		std::ostringstream output;

		output << std::left << std::setw((int) maxLineLength)
		       << (pidStr + " - Found test:  " + testToRun.testName) << "  - SEGMENTATION FAULT!\n";

		std::string outputText = output.str();

		resultQueue->enqueue(
			TestResult(
				  testToRun.testName
				, 0
				, testToRun.group->groupIndex
				, testToRun.testIndex
				, TestResult::Result::Error
				, std::move(outputText)
			)
		);
	}

	// The out of process message queue name is a uuid with a prefix.
	private: static std::string createSharedMemoryNamePrefix() {
		std::string name = std::string("TestRunner_") + UUID().asString();
		// Replace characters from the name that the message queue does not like.
		Util::Strings::replaceAll(name, "-", "_");
		return name;
	}

	private: static std::string getPidString() {
		std::ostringstream str;
		str << " -" << std::right << std::setw(6) << getpid();
		return str.str();
	}

	private: unsigned int getCurrentTestIndex(unsigned int childProcessIndex) {
		return sharedState->currentTestIndices[childProcessIndex];
	}

	private: void resetCurrentTestIndex(unsigned int childProcessIndex) {
		sharedState->currentTestIndices[childProcessIndex] = 0;
	}

	private: unsigned int claimNextTestIndex(unsigned int childProcessIndex) {
		unsigned int thisTestIndex = sharedState->nextTestIndex++;
		sharedState->currentTestIndices[childProcessIndex] = thisTestIndex;
		return thisTestIndex;
	}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__MULTI_PROCESS_TEST_RUNNER_EXECUTOR
