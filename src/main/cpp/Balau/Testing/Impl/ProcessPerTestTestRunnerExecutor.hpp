// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__PROCESS_PER_TEST_TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__PROCESS_PER_TEST_TEST_RUNNER_EXECUTOR

#include <Balau/Testing/Impl/MultiProcessTestRunnerExecutor.hpp>

namespace Balau::Testing::Impl {

// Test runner executor which implements the ProcessPerTest execution model.
class ProcessPerTestTestRunnerExecutor : public MultiProcessTestRunnerExecutor {
	public: ProcessPerTestTestRunnerExecutor(CompositeWriter & writer_,
	                                         bool useNamespaces_,
	                                         GroupedTestCaseMap & testCasesByGroup,
	                                         const std::string & testList,
	                                         unsigned int concurrencyLevel_)
		: MultiProcessTestRunnerExecutor(
			  writer_
			, useNamespaces_
			, testCasesByGroup
			, testList
			, concurrencyLevel_
		) {}

	public: void run() override {
		writer << ("Parent pid = " + ::toString(getpid()) + "\n");

		//
		// The child PID vector.
		//  - Elements that are zero represent a missing child process.
		//  - Elements that are negative represent a previous child process that has finished execution.
		//
		std::vector<int> childPids(concurrencyLevel, 0);
		std::unordered_map<int, size_t> pidIndexByPid;

		size_t committedRuns = 0;

		do {
			createChildProcesses(childPids, pidIndexByPid, true);

			TestResult testResult = resultQueue->tryDequeue();

			if (testResult.duration != -1) {
				// A valid test case was dequeued.
				processTestResultMessage(std::move(testResult));
				++committedRuns;
			}

			checkForChildProcessTermination(childPids, pidIndexByPid);
		} while (committedRuns < tests.size());

		checkForChildProcessTermination(childPids, pidIndexByPid);
	}

	public: ExecutionModel getExecutionModel() const override {
		return ExecutionModel::ProcessPerTest;
	}

	private: void reportChildProcessCompletion() const override {
		// NOP
	}

	private: void checkForChildProcessTermination(std::vector<int> & childPids,
	                                              std::unordered_map<int, size_t> & pidIndexByPid) {
		int status;

		for (int & pid : childPids) {
			if (pid <= 0) {
				continue;
			}

			int result = waitpid(pid, &status, WNOHANG);

			if (result == -1) {
				writer << (
					"ERROR: waitid returned -1 for PID " + ::toString(pid)
					+ ". Error = " + strerror(errno) + "\n"
				);

				// TODO determine if this is the best course of action.
				// Permanently remove child process slot from run.
				pidIndexByPid.erase(pid);
				pid = -1;
			} else if (result == pid) {
				pidIndexByPid.erase(pid);
				pid = 0;
			}
		}
	}
}; // class ProcessPerTestTestRunnerExecutor

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__PROCESS_PER_TEST_TEST_RUNNER_EXECUTOR
