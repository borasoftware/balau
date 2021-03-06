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

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_EXECUTOR
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_EXECUTOR

#include <Balau/System/SystemClock.hpp>
#include <Balau/System/Sleep.hpp>
#include <Balau/System/ThreadName.hpp>
#include <Balau/Testing/ExecutionModel.hpp>
#include <Balau/Testing/Impl/CompositeWriter.hpp>
#include <Balau/Testing/Impl/FlattenedTestCase.hpp>
#include <Balau/Testing/Impl/TestCase.hpp>
#include <Balau/Testing/Impl/TestGroupBase.hpp>
#include <Balau/Testing/Impl/TestMethodBase.hpp>
#include <Balau/Testing/Impl/TestReportGenerator.hpp>
#include <Balau/Testing/Impl/TestResultQueue.hpp>
#include <Balau/Type/UUID.hpp>
#include <Balau/Util/DateTime.hpp>
#include <Balau/Util/PrettyPrint.hpp>

namespace Balau::Testing {

class TestRunner;
template <typename T> class TestGroup;

namespace Impl {

// Common functionality for test runner executors.
class TestRunnerExecutor {
	protected: using GroupedTestCaseMap = std::unordered_map<std::string, std::vector<TestCase>>;

	private: std::unordered_map<std::string, size_t> groupSizeByName;
	private: std::unordered_map<std::string, size_t> testRunCountsByGroupName;

	protected: std::unique_ptr<TestResultQueue> resultQueue;
	protected: CompositeWriter & writer;
	private: std::shared_ptr<Impl::TestReportGenerator> & reportGenerator;
	protected: const size_t maxLineLength;
	protected: std::vector<FlattenedTestCase> tests;

	private: const bool useNamespaces;
	private: std::chrono::nanoseconds totalCoreTime;
	private: std::vector<TestResult> testResults;
	private: size_t pendingTestResultIndex;

	public: virtual ~TestRunnerExecutor() = default;

	// Runs the executor.
	public: virtual void run() = 0;

	public: virtual ExecutionModel getExecutionModel() const = 0;

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
	                              std::shared_ptr<Impl::TestReportGenerator> & reportGenerator_,
	                              bool useNamespaces_,
	                              GroupedTestCaseMap & testCasesByGroup,
	                              const std::string & testList,
	                              bool isMultiProcess)
		: resultQueue(std::move(resultQueue_))
		, writer(writer_)
		, reportGenerator(reportGenerator_)
		, maxLineLength(determineMaximumLogLineLength(testCasesByGroup, testList, isMultiProcess))
		, tests(addTests(testCasesByGroup, testList))
		, useNamespaces(useNamespaces_)
		, totalCoreTime(0)
		, testResults(tests.size())
		, pendingTestResultIndex(0) {}

	// Run a single test.
	protected: void runTest(const std::string & pidStr, FlattenedTestCase & testToRun) {
		if (!(testToRun.executionModels & getExecutionModel())) {
			// Ignore test.
			resultQueue->enqueue(TestResult(testToRun.testName, testToRun.group->groupIndex, testToRun.testIndex));
			return;
		}

		// Set the main execution thread to the test name.
		std::string previousThreadName = System::ThreadName::getName();
		System::ThreadName::setName(testToRun.testName);

		std::ostringstream output;
		const std::string text = pidStr + " - Test " + testToRun.testName;
		output << std::left << std::setw((int) maxLineLength) << text;
		const std::chrono::nanoseconds start = System::SystemClock().nanotime();

		auto status = runSetup(*testToRun.group, output);

		if (status == TestResult::Result::Success) {
			status = runTheTest(testToRun, output);
		}

		status = runTeardown(*testToRun.group, output, status);

		if (status == TestResult::Result::Success) {
			output << " - passed.";
		} else if (status == TestResult::Result::Ignored) {
			output << " - ignored.";
		}

		const std::chrono::nanoseconds duration = System::SystemClock().nanotime() - start;
		output << " Duration = " << durationStr(duration) << "\n";
		std::string outputStr = output.str();
		const auto ns = (long long) duration.count();

		//writer << ("CHILD " + ::toString(getpid()) + " enqueue result for index " + ::toString(testToRun.testIndex) + "\n");

		resultQueue->enqueue(
			TestResult(
				  testToRun.testName
				, ns
				, testToRun.group->groupIndex
				, testToRun.testIndex
				, status
				, std::move(outputStr)
			)
		);

		System::ThreadName::setName(previousThreadName);
	}

	private: TestResult::Result runSetup(TestGroupBase & group, std::ostream & output) {
		TestResult::Result result;

		try {
			group.setup();
			return TestResult::Result::Success;
		} catch (const Exception::AssertionException & e) {
			output << " - FAILED!\n\n"
			       << "Exception thrown in setup method\n"
			       << "Assertion failed: " << e.message << "\n\n";

			result = TestResult::Result::Failure;
		} catch (const std::exception & e) {
			output << " - FAILED!\n\n"
			       << "Exception thrown in setup method\n"
			       << "Exception thrown: " << e.what() << "\n"
			       << "of type: " << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";

			result = TestResult::Result::Error;
		} catch (...) {
			// TODO this doesn't work
			const auto e = std::current_exception();

			output << " - FAILED!\n\n"
			       << "Exception thrown in setup method\n"
			       << "Unknown exception thrown of type: "
			       << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";

			result = TestResult::Result::Error;
		}

		return result;
	}

	private: TestResult::Result runTheTest(FlattenedTestCase & testToRun, std::ostream & output) {
		TestResult::Result result;

		try {
			Balau::Testing::Impl::TestGroupBase::resetIgnoreCurrent();
			testToRun.method->run();
			return Balau::Testing::Impl::TestGroupBase::currentIsIgnored() ? TestResult::Result::Ignored : TestResult::Result::Success;
		} catch (const Exception::AssertionException & e) {
			output << " - FAILED!\n\n"
			       << "Assertion failed: " << e.message << "\n\n";

			result = TestResult::Result::Failure;
		} catch (const std::exception & e) {
			output << " - FAILED!\n\n"
			       << "Exception thrown: " << e.what() << "\n"
			       << "of type: " << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";

			result = TestResult::Result::Error;
		} catch (...) {
			// TODO this doesn't work
			const auto e = std::current_exception();

			output << " - FAILED!\n\n"
			       << "Unknown exception thrown of type: "
			       << extractTypeName(typeid(e).name(), useNamespaces) << "\n";

			result = TestResult::Result::Error;
		}

		return result;
	}

	private: TestResult::Result runTeardown(TestGroupBase & group, std::ostream & output, TestResult::Result status) {
		TestResult::Result result;

		try {
			group.teardown();
			return status;
		} catch (const Exception::AssertionException & e) {
			if (status != TestResult::Result::Failure) {
				output << " - FAILED!\n\n"
				       << "Exception thrown in teardown method\n"
				       << "Assertion failed: " << e.message << "\n\n";
			} else {
				output << "Exception thrown in teardown method\n"
				       << "Assertion failed: " << e.message << "\n\n";
			}

			result = status == TestResult::Result::Error ? TestResult::Result::Error : TestResult::Result::Failure;
		} catch (const std::exception & e) {
			if (status != TestResult::Result::Failure) {
				output << " - FAILED!\n\n"
				       << "Exception thrown in teardown method\n"
				       << "Exception thrown: " << e.what() << "\n"
				       << "of type: " << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";
			} else {
				output << "Exception thrown in teardown method\n\n"
				       << "Exception thrown: " << e.what() << "\n"
				       << "of type: " << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";
			}

			result = TestResult::Result::Error;
		} catch (...) {
			// TODO this doesn't work
			auto e = std::current_exception();

			if (status != TestResult::Result::Failure) {
				output << " - FAILED!\n\n"
				       << "Exception thrown in teardown method\n"
				       << "Unknown exception thrown of type: "
				       << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";
			} else {
				output << "Exception thrown in teardown method\n\n"
				       << "Unknown exception thrown of type: "
				       << extractTypeName(typeid(e).name(), useNamespaces) << "\n\n";
			}

			result = TestResult::Result::Error;
		}

		return result;
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
			const std::string & testGroupName = test.group->getGroupName();

			if (testResult.duration != -1) {
				size_t & groupCount = testRunCountsByGroupName.at(testGroupName);
				++groupCount;

				writer << test.preText<< testResult.resultText << test.postText;

				if (groupCount == groupSizeByName[testGroupName]) {
					std::chrono::nanoseconds groupDuration(0);

					std::vector<const TestResult *> groupTestResults;
					groupTestResults.reserve(groupCount);

					for (auto & thisTestResult : testResults) {
						if (thisTestResult.groupIndex == testResult.groupIndex) {
							groupTestResults.push_back(&thisTestResult);
							groupDuration += std::chrono::nanoseconds(thisTestResult.duration);
						}

						if (thisTestResult.testIndex == testResult.testIndex) {
							// Last one in group.
							totalCoreTime += groupDuration;
							writer << " Group duration (core clock time) = " << durationStr(groupDuration) << "\n";

							// Call the report generator for this completed test group.
							reportGenerator->generate(*test.group, groupDuration, groupTestResults);

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

	//
	// Supported globs:
	//  - * translates to .*
	//  - ? translates to .
	//
	private: static std::string createRegexString(const std::string & testName) {
		static const std::regex questionRegex("\\?");
		static const std::regex asterixRegex("\\*");
		return Util::Strings::replaceAll(Util::Strings::replaceAll(testName, questionRegex, "."), asterixRegex, ".*");
	}

	private: static std::vector<std::regex> parseTestList(const std::string & testList) {
		static const std::regex testListSeparator(" *, *| +");
		auto testNames = Util::Strings::toStringContainer(Util::Strings::split(testList, testListSeparator));
		std::vector<std::regex> testNameRegexes;

		for (const auto & testName : testNames) {
			const auto regexStr = createRegexString(testName);
			testNameRegexes.emplace_back(regexStr);
		}

		return testNameRegexes;
	}

	private: static bool testIsRunnable(const std::vector<std::regex> & regexes, const std::string & testName) {
		for (const auto & regex : regexes) {
			if (Util::Strings::matches(testName, regex)) {
				return true;
			}
		}

		return false;
	}

	private: static size_t determineMaximumLogLineLength(GroupedTestCaseMap & testCasesByGroup,
	                                                     const std::string & testList,
	                                                     bool isMultiProcess) {
		size_t maxLineLength = 0;
		const auto regexes = parseTestList(testList);

		for (auto & groupIterator : testCasesByGroup) {
			auto & testGroup = groupIterator.second;


			for (auto & testCase : testGroup) {
				if (!regexes.empty() && !testIsRunnable(regexes, testCase.name)) {
					continue;
				}

				auto logLine = isMultiProcess
					? std::string(" - ----- - Test ") + testCase.name
					: std::string(" - Test ") + testCase.name;

				if (maxLineLength < logLine.length()) {
					maxLineLength = logLine.length();
				}
			}
		}

		return maxLineLength;
	}

	private: std::vector<FlattenedTestCase> addTests(GroupedTestCaseMap & testCasesByGroup, const std::string & testList) {
		std::vector<FlattenedTestCase> addedTests;
		unsigned int testIndex = 0;
		const auto regexes = parseTestList(testList);

		for (auto & testCaseSetIterator : testCasesByGroup) {
			const std::string testGroupName = testCaseSetIterator.first;
			auto & testCaseSet = testCaseSetIterator.second;

			bool hasTestsToRun = false;

			if (regexes.empty()) {
				hasTestsToRun = true;
			} else {
				for (TestCase & testCase : testCaseSet) {
					if (testIsRunnable(regexes, testCase.name)) {
						hasTestsToRun = true;
						break;
					}
				}
			}

			if (!hasTestsToRun) {
				continue;
			}

			std::string preText = "\n\n++ Running test group " + testGroupName + "\n\n";

			testRunCountsByGroupName[testGroupName] = 0;
			size_t testCasesAdded = 0;

			for (TestCase & testCase : testCaseSet) {
				if (!regexes.empty() && !testIsRunnable(regexes, testCase.name)) {
					continue;
				}

				addedTests.emplace_back(
					FlattenedTestCase(
						  testIndex
						, testCase.group->getExecutionModels()
						, preText
						, ""
						, testCase.name
						, testCase.group
						, std::move(testCase.method)
					)
				);

				++testCasesAdded;
				++testIndex;
				preText = "";
			}

			groupSizeByName[testGroupName] = testCasesAdded;

			if (testCasesAdded) {
				addedTests.back().postText = "\n== " + testCaseSetIterator.first + " group completed.\n";
			}
		}

		return addedTests;
	}

	friend class ::Balau::Testing::TestRunner;
	template <typename T> friend class ::Balau::Testing::TestGroup;

	private: static std::string durationStr(const std::chrono::nanoseconds duration) {
		return duration < std::chrono::milliseconds(1)
			? Util::PrettyPrint::duration(duration, 0)
			: Util::PrettyPrint::duration(duration, 1);
	}

	private: static std::string extractTypeName(const char * mangledName, bool useNamespaces) {
		std::string fullName = extractTemplateParameter(boost::core::demangle(mangledName));
		return useNamespaces ? fullName : removeNamespaces(fullName);
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

} // namespace Impl

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RUNNER_EXECUTOR
