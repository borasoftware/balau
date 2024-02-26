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

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_WORKER_PROCESSES_TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_WORKER_PROCESSES_TEST_RUNNER_EXECUTOR

#include <Balau/Testing/Impl/MultiProcessTestRunnerExecutor.hpp>

namespace Balau::Testing::Impl {

// Test runner executor which implements the WorkerProcesses execution model.
class WorkerProcessesTestRunnerExecutor : public MultiProcessTestRunnerExecutor {
	public: WorkerProcessesTestRunnerExecutor(CompositeWriter & writer_,
	                                          std::shared_ptr<Impl::TestReportGenerator> & reportGenerator_,
	                                          bool useNamespaces_,
	                                          GroupedTestCaseMap & testCasesByGroup,
	                                          const std::string & testList,
	                                          unsigned int concurrencyLevel_)
		: MultiProcessTestRunnerExecutor(
			  writer_
			, reportGenerator_
			, useNamespaces_
			, testCasesByGroup
			, testList
			, concurrencyLevel_
		) {}

	public: void run() override {
		if (tests.empty()) {
			writer << "No tests to run.\n";
			return;
		}

		writer << ("Parent pid = " + ::toString(getpid()) + "\n");

		//
		// The child PID vector.
		//  - Elements that are zero represent a missing child process.
		//  - Elements that are negative represent a previous child process that has finished execution.
		//
		std::vector<int> childPids(concurrencyLevel, 0);
		std::unordered_map<int, size_t> pidIndexByPid;

		size_t committedRuns = 0;
		bool newChildProcessesRequired = true;
		bool firstCall = true;
		std::chrono::nanoseconds period = System::SystemClock().nanotime();

		do {
			// Create any missing child processes.
			if (newChildProcessesRequired) {
				firstCall = createChildProcesses(childPids, pidIndexByPid, false, firstCall) ? false : firstCall;
				newChildProcessesRequired = false;
			}

			TestResult testResult = resultQueue->tryDequeue();

			if (testResult.duration != -1) {
				// A valid test case was dequeued.
				processTestResultMessage(std::move(testResult));
				++committedRuns;
			}

			const std::chrono::nanoseconds elapsed = System::SystemClock().nanotime();
			const int64_t msToNs100 = 1000 * 1000 * 50; // 50ms

			// Periodically check for abnormally terminated child processes.
			if ((elapsed - period).count() > msToNs100) {
				period = elapsed;
				newChildProcessesRequired = checkForChildProcessTermination(childPids, pidIndexByPid);
			}
		} while (committedRuns < tests.size());

		checkForChildProcessTermination(childPids, pidIndexByPid);
	}

	public: ExecutionModel getExecutionModel() const override {
		return ExecutionModel::WorkerProcesses;
	}

	private: void reportChildProcessCompletion() const override {
		writer << ("\nTest application child process with pid " + ::toString(getpid()) + " finished execution.\n");
	}

	private: bool checkForChildProcessTermination(std::vector<int> & childPids,
	                                              std::unordered_map<int, size_t> & pidIndexByPid) {
		bool childProcessesRegenRequired = false;
		siginfo_t infop;
		int options = WEXITED | WNOHANG; // NOLINT

		for (int & pid : childPids) {
			if (pid <= 0) {
				continue;
			}

			memset(&infop, 0, sizeof(siginfo_t));

			if (waitid(P_PID, (unsigned int) pid, &infop, options) == -1) {
				writer << ("ERROR: waitid returned -1 for PID " + ::toString(pid) + ". Error = " + strerror(errno) + "\n");
			} else if (infop.si_pid) {
				switch (infop.si_code) {
					case CLD_EXITED: {
						if (infop.si_status != 0) {
							writer << (
								"Warning: child process with pid " + ::toString(pid) + " exited with exit status "
								+ ::toString(infop.si_status) + ".\n"
							);
						}

						pidIndexByPid.erase(pid);
						// Negative entry in pid vector indicates no regen.
						pid = -1;

						break;
					}

					case CLD_KILLED:
					case CLD_DUMPED: {
						writer << (
							"Child process with pid " + ::toString(pid) + " killed by signal: "
							+ ::toString(strsignal(infop.si_status)) + ".\n"
						);

						pidIndexByPid.erase(pid);
						// A new child process will be created for this slot.
						pid = 0;
						childProcessesRegenRequired = true;
						break;
					}

					case CLD_CONTINUED: {
						break;
					}

					default: {
						writer << (
							"ERROR: waitid returned unhandled value for si_code for PID "
							+ ::toString(pid) + ": " + ::toString(infop.si_code) + "\n"
						);
					}
				}
			}
		}

		return childProcessesRegenRequired;
	}
}; // class WorkerProcessesTestRunnerExecutor

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_WORKER_PROCESSES_TEST_RUNNER_EXECUTOR
