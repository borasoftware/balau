// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file TestRunner.hpp
///
/// The main test runner class and test group base class.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER
#define COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Exception/SystemExceptions.hpp>
#include <Balau/Exception/TestExceptions.hpp>
#include <Balau/Dev/Assert.hpp>
#include <Balau/Resource/FileByteWriteResource.hpp>
#include <Balau/Testing/ExecutionModel.hpp>
#include <Balau/Testing/Impl/SingleThreadedTestRunnerExecutor.hpp>
#include <Balau/Testing/Impl/ProcessPerTestTestRunnerExecutor.hpp>
#include <Balau/Testing/Impl/TestGroupBase.hpp>
#include <Balau/Testing/Impl/TestMethodBase.hpp>
#include <Balau/Testing/Impl/WorkerProcessesTestRunnerExecutor.hpp>
#include <Balau/Testing/Impl/WorkerThreadsTestRunnerExecutor.hpp>
#include <Balau/Util/DateTime.hpp>

#include <termios.h>
#include <thread>

#include <unistd.h>
#include <boost/predef.h>

namespace Balau::Testing {

//////////////////////////// Test group base class ////////////////////////////

///
/// Templated base class of all test classes.
///
/// Test group classes should extend this class, specifying themselves as the
/// template parameter.
///
template <typename TestGroupT> class TestGroup : public Impl::TestGroupBase {
	private: using Method = void (TestGroupT::*)();

	// Container for test instance and test method.
	private: class TestMethod : public Impl::TestMethodBase {
		private: TestGroupT & testClassInstance;
		private: Method method;

		public: TestMethod(TestGroupT & testClassInstance_, Method method_)
			: testClassInstance(testClassInstance_), method(method_) {}

		private: void run() override {
			(testClassInstance.*method)();
		}
	};

	///
	/// Write additional logging to the test writers.
	///
	public: void log(const std::string & string);

	///
	/// Write additional logging to the test writers.
	///
	/// A line break is written after the string.
	///
	public: void logLine(const std::string & string = "");

	///
	/// Write additional logging to the test writers.
	///
	/// This version of log line accepts a variable number of references, and
	/// concatenates them together by calling toString.
	///
	/// A line break is written after the string.
	///
	public: template <typename S, typename ... SR> void logLine(const S & p, const SR & ... pRest);

	///
	/// Create a test group that runs on all execution models.
	///
	protected: explicit TestGroup(TestRunner & testRunner_);

	///
	/// Create a test group that runs only when the test run has the specified execution model(s).
	///
	/// The execution models can be ORed together if the test group should be
	/// run for multiple execution models.
	///
	protected: explicit TestGroup(TestRunner & testRunner_, int executionModels_);

	///
	/// Register a test case with the runner.
	///
	/// @param method the method of the test case to register
	/// @param testName the name of the test case to register
	///
	protected: void registerTest(Method method, const std::string & testName);

	///
	/// This method is called before each test is called.
	///
	/// This method should be overridden by test classes which require a setup method.
	///
	protected: void setup() override {
		// Default is NOP.
	}

	///
	/// This method is called after each test is called.
	///
	/// This method should be overridden by test classes which require a teardown method.
	///
	protected: void teardown() override {
		// Default is NOP.
	}

	////////////////////////// Private implementation /////////////////////////

	private: const std::string & getGroupName() const final {
		return testGroupName;
	}

	private: TestRunner & testRunner;
	private: int executionModels;
	private: const std::string testGroupName;
};

////////////////////////////// Test runner class //////////////////////////////

///
/// The main test runner class.
///
/// The runner runs the registered test group class' test cases and produces a
/// report at the end of execution.
///
/// If not specified, the default execution model used is WorkerProcesses and
/// the default concurrency level is one thread/process per available CPU core.
///
class TestRunner {
	///
	/// Create a test runner with a WorkerProcesses execution model and default concurrency.
	///
	/// The concurrency level will be equal to the number of cores, and the logging
	/// will output to stdout.
	///
	public: TestRunner()
		: writer(StdOutTestWriter())
		, concurrencyLevel(getConcurrencyLevel())
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, printNS(false) {}

	///
	/// Create a test runner with the specified execution model and default concurrency.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	///
	public: explicit TestRunner(ExecutionModel executionModel_)
		: writer(StdOutTestWriter())
		, concurrencyLevel(getConcurrencyLevel())
		, executionModel(checkOutOfProcessCapability(executionModel_))
		, printNS(false) {}

	///
	/// Create a test runner with a WorkerProcesses execution model and the specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param concurrencyLevel_ the number of processes to use to run the tests
	///
	public: explicit TestRunner(unsigned int concurrencyLevel_)
		: writer(StdOutTestWriter())
		, concurrencyLevel(getConcurrencyLevel(concurrencyLevel_))
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, printNS(false) {}

	///
	/// Create a test runner with a WorkerProcesses execution model and the specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param concurrencyLevel_ the number of processes to use to run the tests
	///
	public: explicit TestRunner(int concurrencyLevel_)
		: writer(StdOutTestWriter())
		, concurrencyLevel(getConcurrencyLevel((unsigned int) concurrencyLevel_))
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, printNS(false) {}

	///
	/// Create a test runner with the specified execution model and specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	///
	public: explicit TestRunner(ExecutionModel executionModel_, unsigned int concurrencyLevel_)
		: writer(StdOutTestWriter())
		, concurrencyLevel(getConcurrencyLevel(concurrencyLevel_))
		, executionModel(checkOutOfProcessCapability(executionModel_))
		, printNS(false) {}

	///
	/// Create a test runner with the specified execution model and specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	///
	public: explicit TestRunner(ExecutionModel executionModel_, int concurrencyLevel_)
		: writer(StdOutTestWriter())
		, concurrencyLevel(getConcurrencyLevel((unsigned int) concurrencyLevel_))
		, executionModel(checkOutOfProcessCapability(executionModel_))
		, printNS(false) {}

	///
	/// Create a test runner with a WorkerProcesses execution model, default concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the number of cores, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param writerItems the writer items
	///
	public: template <typename ... WriterItemT>
	explicit TestRunner(const WriterItemT & ... writerItems)
		: writer(writerItems ...)
		, concurrencyLevel(getConcurrencyLevel())
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, printNS(false) {}

	///
	/// Create a test runner with the specified execution model, default concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the number of cores, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param executionModel_ the execution model to use when running the tests
	/// @param writerItems the writer items
	///
	public: template <typename ... WriterItemT>
	explicit TestRunner(ExecutionModel executionModel_, const WriterItemT & ... writerItems)
		: writer(writerItems ...)
		, concurrencyLevel(getConcurrencyLevel())
		, executionModel(checkOutOfProcessCapability(executionModel_))
		, printNS(false) {}

	///
	/// Create a test runner with a WorkerProcesses execution model, the specified concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	/// @param writerItems the writer items
	///
	public: template <typename ... WriterItemT>
	explicit TestRunner(unsigned int concurrencyLevel_, const WriterItemT & ... writerItems)
		: writer(writerItems ...)
		, concurrencyLevel(getConcurrencyLevel(concurrencyLevel_))
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, printNS(false) {}

	///
	/// Create a test runner with the specified execution model, specified concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param executionModel_ the execution model to use when running the tests
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	/// @param writerItems the writer items
	///
	public: template <typename ... WriterItemT>
	explicit TestRunner(ExecutionModel executionModel_,
	                    unsigned int concurrencyLevel_,
	                    const WriterItemT & ... writerItems)
		: writer(writerItems ...)
		, concurrencyLevel(getConcurrencyLevel(concurrencyLevel_))
		, executionModel(checkOutOfProcessCapability(executionModel_))
		, printNS(false) {}

	///
	/// Register a test group class' test methods.
	///
	/// The test runner will instantiate the test class and the test class'
	/// constructor will call back the test runner to register the tests.
	///
	/// @tparam T the test group class
	///
	public: template <typename T> TestRunner & registerGroup() {
		std::shared_ptr<Impl::TestGroupBase> instance = std::shared_ptr<Impl::TestGroupBase>(new T(*this));
		instance->groupIndex = currentGroupIndex++;

		// Set the group pointer in each test case, ready for assignment later on.
		for (Impl::TestCase & testCase : testCasesByGroup.at(instance->getGroupName())) {
			testCase.group = instance;
		}

		return *this;
	}

	///
	/// Specify whether to print test namespaces in the reporting.
	///
	/// Call this if you wish test failure reports to include the test class' namespace prefix
	/// (useful if you have identically named test classes in different namespaces).
	///
	/// By default, namespaces are not printed.
	///
	/// @param value set to true to print namespaces
	///
	public: TestRunner & printNamespaces(bool value = false) {
		printNS = value;
		return *this;
	}

	///
	/// Perform the test run, specify whether to pause at the end.
	///
	/// The default is no pause.
	///
	/// @param pauseAtExit set to true to pause at exit
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: int run(bool pauseAtExit = false) {
		writer << "\n------------------------- STARTING TESTS -------------------------\n\n";

		auto startTime = System::SystemClock().nanotime();
		std::unique_ptr<Impl::TestRunnerExecutor> executor;

		switch (executionModel) {
			case SingleThreaded: {
				writer << "Run type = single process, single threaded\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::SingleThreadedTestRunnerExecutor(writer, printNS, testCasesByGroup)
				);

				break;
			}

			case WorkerThreads: {
				writer << "Run type = single process, multi-threaded "
				       << "(" << concurrencyLevel << " thread" << (concurrencyLevel > 1 ? "s" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::WorkerThreadsTestRunnerExecutor(writer, printNS, testCasesByGroup, concurrencyLevel)
				);

				break;
			}

			case WorkerProcesses: {
				writer << "Run type   = worker processes "
				       << "(" << concurrencyLevel << " worker process" << (concurrencyLevel > 1 ? "es" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::WorkerProcessesTestRunnerExecutor(writer, printNS, testCasesByGroup, concurrencyLevel)
				);

				break;
			}

			case ProcessPerTest: {
				writer << "Run type   = process per test "
				       << "(" << concurrencyLevel << " simultaneous process" << (concurrencyLevel > 1 ? "es" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::ProcessPerTestTestRunnerExecutor(writer, printNS, testCasesByGroup, concurrencyLevel)
				);

				break;
			}

			default: ThrowBalauException(Exception::BugException, "Unhandled ExecutionModel in switch statement.");
		}

		executor->run();

		const std::chrono::nanoseconds duration = System::SystemClock().nanotime() - startTime;
		const bool success = report(*executor, duration);

		if (executionModel == SingleThreaded || executionModel == WorkerThreads) {
			writer << ("\nTest application process with pid " + ::toString(getpid()) + " finished execution.\n");
		} else {
			writer << ("\nTest application parent process with pid " + ::toString(getpid()) + " finished execution.");
		}

		if (pauseAtExit) {
			writer << "Press a key to exit..\n";
			struct termios oldattr = {};
			tcgetattr(STDIN_FILENO, &oldattr);
			struct termios newattr = oldattr;
			newattr.c_lflag &= ~(ICANON | ECHO); // NOLINT
			tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
			getchar();
			tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
		}

		return success ? 0 : 1;
	}

	////////////////////////// Private implementation /////////////////////////

	private: unsigned int getConcurrencyLevel(const unsigned int requestedConcurrency = 0) {
		const unsigned int coreCount = std::thread::hardware_concurrency();
		const unsigned int maxConcurrency = Impl::TestRunnerLimits::MaxConcurrency;

		if (requestedConcurrency != 0) {
			if (requestedConcurrency > maxConcurrency) {
				writer << "Requested concurrency level exceeds the maximum supported concurrency level of "
				       << maxConcurrency << ". Setting concurrency level to " << maxConcurrency << "\n";
				return maxConcurrency;
			}

			return requestedConcurrency;
		} else if (coreCount == 0) {
			ThrowBalauException(
				  Exception::TestRunnerException
				, "Could not determine the core count of the machine "
				  "(std::thread::hardware_concurrency() returned 0). "
				  "Please specify the concurrency level manually via "
				  "the test runner constructor argument."
			);
		} else if (coreCount > maxConcurrency) {
			writer << "Core count exceeds the maximum supported concurrency level of "
			       << maxConcurrency << ". Setting concurrency level to " << maxConcurrency << "\n";

			return maxConcurrency;
		}

		return coreCount;
	}

	private: ExecutionModel checkOutOfProcessCapability(ExecutionModel desiredExecutionModel) {
		if (Concurrent::Fork::forkSupported()) {
			return desiredExecutionModel;
		}

		if (desiredExecutionModel != WorkerProcesses) {
			writer << "Warning: running WorkerThreads execution model (this platform does not support out of process test runs).\n";
		}

		return WorkerProcesses;
	}

	private: bool report(const Impl::TestRunnerExecutor & runner, std::chrono::nanoseconds duration) {
		const std::vector<Impl::TestResult> & testResults = runner.getTestResults();
		const std::vector<Impl::FlattenedTestCase> & tests = runner.getTests();

		const auto failureCount = (size_t) std::count_if(
			  testResults.begin()
			, testResults.end()
			, [] (const Impl::TestResult & message) { return !message.success; }
		);

		const size_t successCount = testResults.size() - failureCount;
		const std::string totalCoreDuration = Impl::TestRunnerExecutor::durationStr(runner.getTotalCoreTime());
		const std::string totalClockDuration = Impl::TestRunnerExecutor::durationStr(duration);
		const std::string testAverageDuration = Impl::TestRunnerExecutor::durationStr(runner.getTotalCoreTime() / runner.getTestResults().size());

		writer << "\n------------------------- COMPLETED TESTS ------------------------\n"
		       << "\nTotal duration   (test run clock time)    = " << totalCoreDuration
		       << "\nAverage duration (test run clock time)    = " << testAverageDuration
		       << "\nTotal duration   (application clock time) = " << totalClockDuration << "\n";

		if (failureCount == 0) {
			writer << "\n***** ALL TESTS PASSED - "
			       << successCount << " test" << (successCount != 1 ? "s" : "") << " executed *****\n";
		} else {
			writer << "\n***** THERE WERE TEST FAILURES. *****\n\n"
			       << "Total tests run: " << (successCount + failureCount) << "\n\n"
			       << "  " << Util::Strings::padLeft(::toString(successCount), 6)
			       << " test" << (successCount > 1 ? "s" : "") << " passed\n"
			       << "  " << Util::Strings::padLeft(::toString(failureCount), 6)
			       << " test" << (failureCount > 1 ? "s" : "") << " failed\n\n"
			       << "Failed tests:\n";

			std::for_each(
				  testResults.begin()
				, testResults.end()
				, [&tests, this] (const Impl::TestResult & message) {
					if (!message.success) {
						writer << "      " << tests[message.testIndex].testName << "\n";
					}
				}
			);

			writer << "\n";
		}

		return failureCount == 0;
	}

	private: void registerTest(const std::string & testGroupName,
	                           const std::shared_ptr<Impl::TestMethodBase> & method,
	                           const std::string & name) {
		testCasesByGroup.at(testGroupName).emplace_back(
			Impl::TestCase(method, testGroupName + "::" + name)
		);
	}

	private: void log(const std::string & string) {
		writer << string;
	}

	private: void logLine(const std::string & string) {
		writer << string << "\n";
	}

	private: void logLine() {
		writer << "\n";
	}

	template <typename T> friend class TestGroup;

	private: std::unordered_map<std::string, std::vector<Impl::TestCase>> testCasesByGroup;
	private: Impl::CompositeWriter writer;
	private: const unsigned int concurrencyLevel;
	private: ExecutionModel executionModel;
	private: bool printNS = false;
	private: unsigned int currentGroupIndex = 0;
};

////////////////////////// Test group implementation //////////////////////////

template <typename TestClassT>
inline void TestGroup<TestClassT>::log(const std::string & string) {
	return testRunner.log(string);
}

template <typename TestClassT>
inline void TestGroup<TestClassT>::logLine(const std::string & string) {
	return testRunner.logLine(string);
}

template <typename TestClassT>
template <typename S, typename ... SR>
inline void TestGroup<TestClassT>::logLine(const S & p, const SR & ... pRest) {
	using ::toString;

	logLine(toString(p, pRest ...));
}

template <typename TestClassT>
inline TestGroup<TestClassT>::TestGroup(TestRunner & testRunner_)
	: testRunner(testRunner_)
	, executionModels(SingleThreaded | WorkerThreads | WorkerProcesses | ProcessPerTest)
	, testGroupName(Impl::TestRunnerExecutor::extractTypeName(typeid(*this).name(), testRunner_.printNS)) {
	testRunner.testCasesByGroup.insert(
		std::pair<std::string, std::vector<Impl::TestCase>>(testGroupName, std::vector<Impl::TestCase>())
	);
}

template <typename TestClassT>
inline TestGroup<TestClassT>::TestGroup(TestRunner & testRunner_, int executionModels_)
	: testRunner(testRunner_)
	, executionModels(executionModels_)
	, testGroupName(Impl::TestRunnerExecutor::extractTypeName(typeid(*this).name(), testRunner_.printNS)) {
	testRunner.testCasesByGroup.insert(
		std::pair<std::string, std::vector<Impl::TestCase>>(testGroupName, std::vector<Impl::TestCase>())
	);

	if (!(executionModels & testRunner.executionModel)) {
		auto n = boost::core::demangle(typeid(this).name());
		logLine("Test group ", n, " disabled for this execution model.");
	}
}

template <typename TestClassT>
inline void TestGroup<TestClassT>::registerTest(Method method, const std::string & testName) {
	auto m = std::shared_ptr<Impl::TestMethodBase>(new TestMethod(*static_cast<TestClassT *>(this), method));

	if (executionModels & testRunner.executionModel) {
		testRunner.registerTest(testGroupName, m, testName);
	}
}

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER
