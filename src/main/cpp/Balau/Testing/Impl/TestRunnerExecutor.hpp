// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_EXECUTOR

#include <Balau/System/SystemClock.hpp>
#include <Balau/System/Sleep.hpp>
#include <Balau/Testing/Impl/CompositeWriter.hpp>
#include <Balau/Testing/Impl/FlattenedTestCase.hpp>
#include <Balau/Testing/Impl/TestCase.hpp>
#include <Balau/Testing/Impl/TestGroupBase.hpp>
#include <Balau/Testing/Impl/TestMethodBase.hpp>
#include <Balau/Testing/Impl/TestResult.hpp>
#include <Balau/Testing/Impl/TestResultQueue.hpp>
#include <Balau/Type/UUID.hpp>
#include <Balau/Util/DateTime.hpp>
#include <Balau/Util/PrettyPrint.hpp>

namespace Balau::Testing::Impl {

// Common functionality for test runner executors.
class TestRunnerExecutor {
	protected: using GroupedTestCaseMap = std::unordered_map<std::string, std::vector<TestCase>>;

	private: std::unordered_map<std::string, size_t> groupSizeByName;
	private: std::unordered_map<std::string, size_t> testRunCountsByGroupName;

	protected: std::unique_ptr<TestResultQueue> resultQueue;
	protected: CompositeWriter & writer;
	protected: const size_t maxLineLength;
	protected: std::vector<FlattenedTestCase> tests;

	private: const bool printNamespaces;
	private: std::chrono::nanoseconds totalCoreTime;
	private: std::vector<TestResult> testResults;
	private: size_t pendingTestResultIndex;

	public: virtual ~TestRunnerExecutor() = default;

	// Runs the executor.
	public: virtual void run() = 0;

	// For final reporting.
	public: const std::vector<FlattenedTestCase> & getTests() const {
		return tests;
	}

	// For final reporting.
	public: const std::chrono::nanoseconds & getTotalCoreTime() const {
		return totalCoreTime;
	}

	// For final reporting.
	public: const std::vector<TestResult> & getTestResults() const {
		return testResults;
	}

	protected: TestRunnerExecutor(std::unique_ptr<TestResultQueue> resultQueue_,
	                              CompositeWriter & writer_,
	                              bool printNamespaces_,
	                              GroupedTestCaseMap & testCases,
	                              bool isMultiProcess)
		: resultQueue(std::move(resultQueue_))
		, writer(writer_)
		, maxLineLength(determineMaximumLogLineLength(testCases, isMultiProcess))
		, tests(addTests(testCases))
		, printNamespaces(printNamespaces_)
		, totalCoreTime(0)
		, testResults(tests.size())
		, pendingTestResultIndex(0) {}

	// Run a single test.
	// The buffer is used by out of process queue implementations. It is ignored otherwise.
	protected: void runTest(const std::string & pidStr, FlattenedTestCase & testToRun) {
		std::ostringstream output;
		const std::string text = pidStr + " - Running test " + testToRun.testName;
		output << std::left << std::setw((int) maxLineLength) << text;

		const std::chrono::nanoseconds start = System::SystemClock().nanotime();

		// Simulate slow tests with IO.
		//System::Sleep::milliSleep(100);

		bool success = runSetup(*testToRun.group, output);

		if (success) {
			success = runTheTest(testToRun, output);
		}

		success = runTeardown(*testToRun.group, output, success);

		if (success) {
			output << " - passed.";
		}

		const std::chrono::nanoseconds duration = System::SystemClock().nanotime() - start;
		output << " Duration = " << durationStr(duration) << "\n";
		std::string outputStr = output.str();
		const auto ns = (long long) duration.count();

		//writer << ("CHILD " + ::toString(getpid()) + " enqueue result for index " + ::toString(testToRun.testIndex) + "\n");

		resultQueue->enqueue(
			  TestResult(ns, testToRun.group->groupIndex, testToRun.testIndex, success, std::move(outputStr))
		);
	}

	private: bool runSetup(TestGroupBase & group, std::ostream & output) {
		try {
			group.setup();
			return true;
		} catch (const Exception::AssertionException & e) {
			output << " - FAILED!\n\n"
			       << "Exception thrown in setup method\n"
			       << "Assertion failed: " << e.message << "\n\n";
		} catch (const std::exception & e) {
			output << " - FAILED!\n\n"
			       << "Exception thrown in setup method\n"
			       << "Exception thrown: " << e.what() << "\n"
			       << "of type: " << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
		} catch (...) {
			// TODO this doesn't work
			const auto e = std::current_exception();

			output << " - FAILED!\n\n"
			       << "Exception thrown in setup method\n"
			       << "Unknown exception thrown of type: "
			       << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
		}

		return false;
	}

	private: bool runTheTest(FlattenedTestCase & testToRun, std::ostream & output) {
		try {
			testToRun.method->run();
			return true;
		} catch (const Exception::AssertionException & e) {
			output << " - FAILED!\n\n"
			       << "Assertion failed: " << e.message << "\n\n";
		} catch (const std::exception & e) {
			output << " - FAILED!\n\n"
			       << "Exception thrown: " << e.what() << "\n"
			       << "of type: " << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
		} catch (...) {
			// TODO this doesn't work
			const auto e = std::current_exception();

			output << " - FAILED!\n\n"
			       << "Unknown exception thrown of type: "
			       << extractTypeName(typeid(e).name(), printNamespaces) << "\n";
		}

		return false;
	}

	private: bool runTeardown(TestGroupBase & group, std::ostream & output, bool success) {
		try {
			group.teardown();
			return success;
		} catch (const Exception::AssertionException & e) {
			if (success) {
				output << " - FAILED!\n\n"
				       << "Exception thrown in teardown method\n"
				       << "Assertion failed: " << e.message << "\n\n";
			} else {
				output << "Exception thrown in teardown method\n"
				       << "Assertion failed: " << e.message << "\n\n";
			}
		} catch (const std::exception & e) {
			if (success) {
				output << " - FAILED!\n\n"
				       << "Exception thrown in teardown method\n"
				       << "Exception thrown: " << e.what() << "\n"
				       << "of type: " << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
			} else {
				output << "Exception thrown in teardown method\n\n"
				       << "Exception thrown: " << e.what() << "\n"
				       << "of type: " << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
			}
		} catch (...) {
			// TODO this doesn't work
			auto e = std::current_exception();

			if (success) {
				output << " - FAILED!\n\n"
				       << "Exception thrown in teardown method\n"
				       << "Unknown exception thrown of type: "
				       << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
			} else {
				output << "Exception thrown in teardown method\n\n"
				       << "Unknown exception thrown of type: "
				       << extractTypeName(typeid(e).name(), printNamespaces) << "\n\n";
			}
		}

		return false;
	}

	protected: void processTestResultMessage(TestResult && message) {
		TestResult & resultMessage = testResults[message.testIndex];
		resultMessage += message;
		printAvailableResults();
	}

	// Advances the committed result index if possible and prints any
	// available test results in chronological order.
	protected: void printAvailableResults() {
		// Loop from pendingTestResultIndex up to the first result which is not committed.
		for (size_t index = pendingTestResultIndex; index < testResults.size(); ++index) {
			TestResult & testResult = testResults[index];
			const FlattenedTestCase & test = tests[index];
			const std::string & groupName = test.group->getGroupName();

			if (testResult.duration != -1) {
				size_t & groupCount = testRunCountsByGroupName.at(groupName);
				++groupCount;

				writer << test.preText<< testResult.resultText << test.postText;

				if (groupCount == groupSizeByName[groupName]) {
					std::chrono::nanoseconds groupDuration(0);

					for (auto & thisTestResult : testResults) {
						if (thisTestResult.groupIndex == testResult.groupIndex) {
							groupDuration += std::chrono::nanoseconds(thisTestResult.duration);
						}

						if (thisTestResult.testIndex == testResult.testIndex) {
							// Last one in group.
							totalCoreTime += groupDuration;
							writer << " Group duration (core clock time) = " << durationStr(groupDuration) << "\n";
							break;
						}
					}
				}

				pendingTestResultIndex = index + 1;
			} else {
				break;
			}
		}
	}

	private: static size_t determineMaximumLogLineLength(GroupedTestCaseMap & testCases, bool isMultiProcess) {
		size_t maxLineLength = 0;

		for (auto & groupIterator : testCases) {
			auto & testGroup = groupIterator.second;

			for (auto & testCase : testGroup) {
				auto logLine = isMultiProcess
					? std::string(" - ----- - Running test ") + testCase.name
					: std::string(" - Running test ") + testCase.name;

				if (maxLineLength < logLine.length()) {
					maxLineLength = logLine.length();
				}
			}
		}

		return maxLineLength;
	}

	private: std::vector<FlattenedTestCase> addTests(GroupedTestCaseMap & testCases) {
		std::vector<FlattenedTestCase> tests;
		unsigned int testIndex = 0;

		for (auto & testCaseSetIterator : testCases) {
			const std::string groupName = testCaseSetIterator.first;
			auto & testCaseSet = testCaseSetIterator.second;

			if (!testCaseSet.empty()) {
				std::string preText = "\n\n++ Running test group " + groupName + "\n\n";

				groupSizeByName[groupName] = testCaseSetIterator.second.size();
				testRunCountsByGroupName[groupName] = 0;

				for (TestCase & testCase : testCaseSet) {
					tests.emplace_back(
						FlattenedTestCase(
							testIndex
							, preText
							, ""
							, testCase.name
							, testCase.group
							, std::move(testCase.method)
						)
					);

					++testIndex;
					preText = "";
				}

				tests.back().postText = "\n== " + testCaseSetIterator.first + " group completed.";
			}
		}

		return tests;
	}

	// Public for the TestRunner class.
	public: static std::string durationStr(const std::chrono::nanoseconds duration) {
		return duration < std::chrono::milliseconds(1)
			? Util::PrettyPrint::duration(duration, 0)
			: Util::PrettyPrint::duration(duration, 1);
	}

	// Public for the TestRunner class.
	public: static std::string extractTypeName(const char * mangledName, bool printNamespaces) {
		std::string fullName = extractTemplateParameter(boost::core::demangle(mangledName));
		return printNamespaces ? fullName : removeNamespaces(fullName);
	}

	private: static std::string extractTemplateParameter(const std::string & className) {
		const size_t startIndex = className.find('<');
		const size_t endIndex = className.find('>');
		return className.substr(startIndex + 1, endIndex - (startIndex + 1));
	}

	private: static std::string removeNamespaces(const std::string & className) {
		const size_t startIndex = className.find_last_of(':');
		return startIndex == std::string::npos ? className : className.substr(startIndex + 1);
	}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_EXECUTOR
