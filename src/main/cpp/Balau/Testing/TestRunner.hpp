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
/// The main test runner singleton class and test group base class.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER
#define COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER

#include <Balau/Application/CommandLine.hpp>
#include <Balau/Dev/Assert.hpp>
#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Exception/SystemExceptions.hpp>
#include <Balau/Exception/TestExceptions.hpp>
#include <Balau/Resource/FileByteWriteResource.hpp>
#include <Balau/System/ThreadName.hpp>
#include <Balau/Testing/ExecutionModel.hpp>
#include <Balau/Testing/Impl/SingleThreadedTestRunnerExecutor.hpp>
#include <Balau/Testing/Impl/ProcessPerTestTestRunnerExecutor.hpp>
#include <Balau/Testing/Impl/TestGroup.hpp>
#include <Balau/Testing/Impl/TestMethodBase.hpp>
#include <Balau/Testing/Impl/WorkerProcessesTestRunnerExecutor.hpp>
#include <Balau/Testing/Impl/WorkerThreadsTestRunnerExecutor.hpp>
#include <Balau/Testing/Reporters/SurefireTestReportGenerator.hpp>
#include <Balau/Testing/Writers/StdWriters.hpp>
#include <Balau/Util/DateTime.hpp>

#include <termios.h>
#include <thread>

#include <unistd.h>
#include <boost/predef.h>

namespace Balau::Testing {

enum class TestRunnerOption {
	  ExecutionModel
	, Namespaces
	, PauseAtExit
	, ConcurrencyLevel
	, ReportFolder
	, Help
};

inline std::string toString(const TestRunnerOption & option) {
	switch (option) {
		case TestRunnerOption::ExecutionModel:   return "ExecutionModel";
		case TestRunnerOption::Namespaces:       return "Namespaces";
		case TestRunnerOption::PauseAtExit:      return "PauseAtExit";
		case TestRunnerOption::ConcurrencyLevel: return "ConcurrencyLevel";
		case TestRunnerOption::ReportFolder:     return "ReportFolder";
		case TestRunnerOption::Help:             return "Help";

		default: {
			ThrowBalauException(
				  Exception::IllegalArgumentException
				, ::toString("Unknown test option supplied to toString function: ", Util::Enums::toUnderlying(option))
			);
		}
	}
}

///
/// The main test runner class.
///
/// The runner runs the registered test group class' test cases and produces a
/// report at the end of execution.
///
/// If not explicitly initialised, the default execution model used is
/// WorkerProcesses and the default concurrency level is one thread/process
/// per available CPU logical core.
///
class TestRunner : public Impl::TestRunnerBase {
	///
	/// Run the test runner after parsing the supplied input arguments.
	///
	/// If not specified, the default concurrency level will be equal to the number
	/// of logical cores if the execution model is WorkerThreads or WorkerProcesses,
	/// and the logging will output to stdout.
	///
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param ignoreFirst ignore the first command line argument (default == true)
	/// @param reportGenerator the report generator to use if reporting is enabled (default == Surefire report generator)
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(int argc,
	                       char * argv[],
	                       bool ignoreFirst = true,
	                       std::shared_ptr<Impl::TestReportGenerator> reportGenerator = std::shared_ptr<Impl::TestReportGenerator>(new Reporters::SurefireTestReportGenerator)) {
		CommandLine<TestRunnerOption> cl = getCommandLineParser();

		cl.parse(argc, argv, ignoreFirst);

		if (cl.hasOption(TestRunnerOption::Help)) {
			std::cout << cl.getHelpText(4, 80) << "\n";
			return 0;
		}

		if (reportGenerator && cl.hasOption(TestRunnerOption::ReportFolder)) {
			const std::string reportFolderString = cl.getOption(TestRunnerOption::ReportFolder);
			setReportOutputFolder(Resource::File(reportFolderString), reportGenerator);
		}

		const auto executionModel = parseExecutionModel(cl);
		auto & r = runner();

		r.testList = createTestList(cl);
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.reportGenerator = std::move(reportGenerator);
		r.concurrencyLevel = r.getConcurrencyLevel(cl);
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(cl.hasOption(TestRunnerOption::Namespaces));
		r.pauseAtExit = cl.hasOption(TestRunnerOption::PauseAtExit);
		return r.performTestRun();
	}

	///
	/// Run the test runner after parsing the supplied input arguments and using the supplied writers.
	///
	/// If not specified, the concurrency level will be equal to the number of
	/// logical cores if the execution model is WorkerThreads or WorkerProcesses.
	///
	/// The supplied report generator must have its output folder set.
	///
	/// @tparam WriterItemT the writer item types
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param ignoreFirst ignore the first command line argument
	/// @param reportGenerator the report generator to use
	/// @param writerItems the writer items to write the output to
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(int argc,
	               char * argv[],
	               bool ignoreFirst,
	               std::shared_ptr<Impl::TestReportGenerator> reportGenerator,
	               const WriterItemT & ... writerItems) {
		CommandLine<TestRunnerOption> cl = getCommandLineParser();

		cl.parse(argc, argv, ignoreFirst);

		if (cl.hasOption(TestRunnerOption::Help)) {
			std::cout << cl.getHelpText(4, 80) << "\n";
			return 0;
		}

		if (reportGenerator && cl.hasOption(TestRunnerOption::ReportFolder)) {
			const std::string reportFolderString = cl.getOption(TestRunnerOption::ReportFolder);
			setReportOutputFolder(Resource::File(reportFolderString), reportGenerator);
		}

		const auto executionModel = parseExecutionModel(cl);
		auto & r = runner();

		r.testList = createTestList(cl);
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.reportGenerator = std::move(reportGenerator);
		r.concurrencyLevel = r.getConcurrencyLevel(cl);
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(cl.hasOption(TestRunnerOption::Namespaces));
		r.pauseAtExit = cl.hasOption(TestRunnerOption::PauseAtExit);
		return r.performTestRun();
	}

	///
	/// Run all the tests in the test runner.
	///
	/// The concurrency level will be equal to the number of logical cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging will output to stdout.
	///
	/// Test output reports will be written if a report output folder is specified.
	///
	/// @param executionModel the execution model to run the tests
	/// @param reportOutputFolder the output folder in which the reports will be written (default == empty == no reports written)
	/// @param reportGenerator the report generator to use if reporting is enabled (default == Surefire report generator)
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(ExecutionModel executionModel,
	                       const Resource::File & reportOutputFolder = Resource::File(),
	                       std::shared_ptr<Impl::TestReportGenerator> reportGenerator = std::shared_ptr<Impl::TestReportGenerator>(new Reporters::SurefireTestReportGenerator)) {
		const bool empty = reportOutputFolder.toRawString().empty();

		if (reportGenerator && !empty) {
			setReportOutputFolder(reportOutputFolder, reportGenerator);
		}

		auto & r = runner();

		r.writer = Impl::CompositeWriter(StdOutTestWriter());

		if (!empty) {
			r.reportGenerator = std::move(reportGenerator);
		}

		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(false);
		r.pauseAtExit = false;
		return r.performTestRun();
	}

	///
	/// Run the test runner after parsing the supplied input arguments and using the supplied writers.
	///
	/// If not specified, the concurrency level will be equal to the number of
	/// logical cores if the execution model is WorkerThreads or WorkerProcesses.
	///
	/// Test output reports will only be written if a report output folder is specified.
	///
	/// @tparam WriterItemT the writer item types
	/// @param executionModel the execution model to run the tests
	/// @param reportOutputFolder the output folder in which the reports will be written (empty == no reports written)
	/// @param reportGenerator the report generator to use if reporting is enabled
	/// @param writerItems the writer items to write the output to
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(ExecutionModel executionModel,
	               const Resource::File & reportOutputFolder,
	               std::shared_ptr<Impl::TestReportGenerator> reportGenerator,
	               const WriterItemT & ... writerItems) {
		const bool empty = reportOutputFolder.toRawString().empty();

		if (reportGenerator && !empty) {
			setReportOutputFolder(reportOutputFolder, reportGenerator);
		}

		auto & r = runner();

		r.writer = Impl::CompositeWriter(writerItems ...);

		if (!empty) {
			r.reportGenerator = std::move(reportGenerator);
		}

		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(false);
		r.pauseAtExit = false;
		return r.performTestRun();
	}

	////////////////////////// Private implementation /////////////////////////

	//
	// bool pauseAtExit              = false
	// unsigned int concurrencyLevel = number of logical cores
	// bool generateReports          = false
	// bool reportGenerator          = SurefireTestReportGenerator
	// bool reportOutputDirectory    = no default
	//
	// const WriterItemT & ... writerItems
	// std::shared_ptr<Impl::TestReportGenerator> reportGenerator = SurefireTestReportGenerator
	//
	private: static CommandLine<TestRunnerOption> getCommandLineParser() {
		return CommandLine<TestRunnerOption>(CommandLineStyle::SwitchSpaceValue)
			.withOption(TestRunnerOption::ExecutionModel,   "e", "execution-model",  true,  "The execution model (default = SingleThreaded).")
			.withOption(TestRunnerOption::Namespaces,       "n", "namespaces",       false, "Use namespaces in test group names (default is not to use).")
			.withOption(TestRunnerOption::PauseAtExit,      "p", "pause",            false, "Pause at exit (default is not to pause).")
			.withOption(TestRunnerOption::ConcurrencyLevel, "c", "concurrency",      true,  "The number of threads or processes to use to run the tests (default = detect).")
			.withOption(TestRunnerOption::ReportFolder,     "r", "report-folder",    true,  "Generate test reports in the specified folder.")
			.withOption(TestRunnerOption::Help,             "h", "help",             false, "Displays this help message.")
			.withOptionalFinalValue();
	}

	private: static void setReportOutputFolder(const Resource::File & reportOutputFolder,
	                                           std::shared_ptr<Impl::TestReportGenerator> & reportGenerator) {
		if (!reportOutputFolder.toRawString().empty()) {
			auto path = Resource::File(boost::filesystem::system_complete(reportOutputFolder.getEntry()));

			if (path.isRegularFile()) {
				ThrowBalauException(
					  Exception::IllegalArgumentException
					, ::toString("The specified report output folder is an existing regular file: ", path.toRawString())
				);
			}

			if (!path.isRegularDirectory()) {
				try {
					path.createDirectories();
				} catch (const boost::filesystem::filesystem_error & e) {
					ThrowBalauException(
						  Exception::IllegalArgumentException
						, ::toString("The specified report output folder could not be created: ", path.toRawString())
					);
				}
			}

			reportGenerator->setOutputFolder(path);
		}
	}

	// The test runner singleton.
	private: static TestRunner & runner() {
		static TestRunner r;
		return r;
	}

	private: TestRunner()
		: writer(Impl::CompositeWriter(StdOutTestWriter()))
		, reportGenerator()
		, concurrencyLevel(getConcurrencyLevel())
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, useNamespaces(false)
		, pauseAtExit(false) {}

	// Called by the test group base class in order to get a unique index.
	private: unsigned int getGroupIndex() override {
		std::lock_guard<std::mutex> lock(runner().mutex);
		return runner().currentGroupIndex++;
	}

	// Parse the execution model string.
	private: static ExecutionModel parseExecutionModel(CommandLine<TestRunnerOption> & cl) {
		if (!cl.hasOption(TestRunnerOption::ExecutionModel)) {
			return ExecutionModel::SingleThreaded;
		}

		const auto m = Util::Strings::toLower(Util::Strings::trim(cl.getOption(TestRunnerOption::ExecutionModel)));
		Testing::ExecutionModel model;

		if (isExecutionModel(m)) {
			fromString(model, m);
			std::cout << "\nRunning tests for command line specified execution model " << toString(model) << "\n" << std::endl;
		} else {
			model = Testing::SingleThreaded;
			std::cout << "\nUnknown execution model specified. Using execution model SingleThreaded\n" << std::endl;
		}

		return model;
	}

	// Called from the run methods after configuring the test runner.
	private: int performTestRun() {
		System::ThreadName::setName("TestMain");

		writer << "\n------------------------- STARTING TESTS -------------------------\n\n";

		auto startTime = System::SystemClock().nanotime();
		std::unique_ptr<Impl::TestRunnerExecutor> executor;

		switch (executionModel) {
			case SingleThreaded: {
				writer << "Run type = single process, single threaded\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::SingleThreadedTestRunnerExecutor(
						writer, reportGenerator, useNamespaces, testCasesByGroup, testList
					)
				);

				break;
			}

			case WorkerThreads: {
				writer << "Run type = single process, multi-threaded "
				       << "(" << concurrencyLevel << " thread" << (concurrencyLevel > 1 ? "s" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::WorkerThreadsTestRunnerExecutor(
						writer, reportGenerator, useNamespaces, testCasesByGroup, testList, concurrencyLevel
					)
				);

				break;
			}

			case WorkerProcesses: {
				writer << "Run type = worker processes "
				       << "(" << concurrencyLevel << " worker process" << (concurrencyLevel > 1 ? "es" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::WorkerProcessesTestRunnerExecutor(
						writer, reportGenerator, useNamespaces, testCasesByGroup, testList, concurrencyLevel
					)
				);

				break;
			}

			case ProcessPerTest: {
				writer << "Run type = process per test "
				       << "(" << concurrencyLevel << " simultaneous process" << (concurrencyLevel > 1 ? "es" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::ProcessPerTestTestRunnerExecutor(
						writer, reportGenerator, useNamespaces, testCasesByGroup, testList, concurrencyLevel
					)
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

	// Concatenates final values into a single string, forming the test list.
	private: static std::string createTestList(CommandLine<TestRunnerOption> & cl) {
		const size_t count = cl.getFinalValueCount();

		if (count == 0) {
			return "";
		}

		std::string tl;
		std::string prefix;

		for (size_t m = 0; m < count; m++) {
			tl += prefix;
			tl += cl.getFinalValue(m);
			prefix = " ";
		}

		return tl;
	}

	// Determine the concurrency level, either from the supplied requested concurrency level or from the hardware capabilities.
	private: unsigned int getConcurrencyLevel(CommandLine<TestRunnerOption> & cl) {
		const size_t requestedConcurrency = cl.getOptionAsUnsignedIntOrDefault(TestRunnerOption::ConcurrencyLevel, 0);
		return getConcurrencyLevel(requestedConcurrency);
	}

	// Determine the concurrency level, either from the supplied requested concurrency level or from the hardware capabilities.
	private: unsigned int getConcurrencyLevel(size_t requestedConcurrency = 0) {
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
				  "a test runner constructor argument."
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
			, [] (const Impl::TestResult & message) { return message.result == Impl::TestResult::Result::Failure; }
		);

		const auto errorCount = (size_t) std::count_if(
			  testResults.begin()
			, testResults.end()
			, [] (const Impl::TestResult & message) { return message.result == Impl::TestResult::Result::Error; }
		);

		const auto ignoredCount = (size_t) std::count_if(
			  testResults.begin()
			, testResults.end()
			, [] (const Impl::TestResult & message) { return message.result == Impl::TestResult::Result::Ignored; }
		);

		const size_t successCount = testResults.size() - ignoredCount - failureCount - errorCount;
		const std::string totalCoreDuration = Impl::TestRunnerExecutor::durationStr(runner.getTotalCoreTime());
		const std::string totalClockDuration = Impl::TestRunnerExecutor::durationStr(duration);
		const std::string testAverageDuration = runner.getTestResults().empty() ? "n/a" : Impl::TestRunnerExecutor::durationStr(runner.getTotalCoreTime() / runner.getTestResults().size());

		writer << "\n------------------------- COMPLETED TESTS ------------------------\n"
		       << "\nTotal duration   (test run clock time)    = " << totalCoreDuration
		       << "\nAverage duration (test run clock time)    = " << testAverageDuration
		       << "\nTotal duration   (application clock time) = " << totalClockDuration << "\n";

		if (failureCount == 0 && errorCount == 0) {
			writer << "\nALL TESTS PASSED"
			       << "\n  tests passed:  " << Util::Strings::padLeft(::toString(successCount), 6)
			       << "\n  tests ignored: " << Util::Strings::padLeft(::toString(ignoredCount), 6)
			       << "\n";
		} else {
			writer << "\n***** THERE WERE TEST FAILURES. *****\n"
			       << "\nTotal tests run: " << (successCount + ignoredCount + failureCount + errorCount) << "\n"
			       << "\n  tests passed:  " << Util::Strings::padLeft(::toString(successCount), 6)
			       << "\n  tests ignored: " << Util::Strings::padLeft(::toString(ignoredCount), 6)
			       << "\n  tests failed:  " << Util::Strings::padLeft(::toString(failureCount), 6)
			       << "\n  tests errored: " << Util::Strings::padLeft(::toString(errorCount), 6)
			       << "\n\n"
			       << "Failed/errored tests:\n";

			std::for_each(
				  testResults.begin()
				, testResults.end()
				, [&tests, this] (const Impl::TestResult & message) {
					if (message.result == Impl::TestResult::Result::Failure || message.result == Impl::TestResult::Result::Error) {
						writer << "      " << tests[message.testIndex].testName << "\n";
					}
				}
			);

			writer << "\n";
		}

		return failureCount == 0 && errorCount == 0;
	}

	private: void registerTest(Impl::TestGroupBase * group,
	                           const std::string & testGroupName,
	                           const std::shared_ptr<Impl::TestMethodBase> & method,
	                           const std::string & name) {
		std::lock_guard<std::mutex> lock(mutex);
		auto & testCases = testCasesByGroup[testGroupName];
		testCases.emplace_back(Impl::TestCase(group, method, testGroupName + "::" + name));
	}

	private: void setUseNamespaces(bool useNamespaces_) {
		std::lock_guard<std::mutex> lock(mutex);

		useNamespaces = useNamespaces_;

		for (auto & testCaseGroup : testCasesByGroup) {
			auto & testCaseVector = testCaseGroup.second;

			if (!testCaseVector.empty()) {
				testCaseVector[0].group->setGroupName(
					Impl::TestRunnerExecutor::extractTypeName(
						testCaseVector[0].group->getGroupName().c_str(), useNamespaces
					)
				);
			}
		}
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

	private: std::mutex mutex;
	private: std::string testList;
	private: std::unordered_map<std::string, std::vector<Impl::TestCase>> testCasesByGroup;
	private: Impl::CompositeWriter writer;
	private: std::shared_ptr<Impl::TestReportGenerator> reportGenerator;
	private: unsigned int concurrencyLevel {};
	private: ExecutionModel executionModel;
	private: bool useNamespaces;
	private: bool pauseAtExit;
	private: unsigned int currentGroupIndex = 0;
};

////////////////////////// Test group implementation //////////////////////////

template <typename TestClassT>
inline void TestGroup<TestClassT>::ignore() {
	ignoreCurrent();
}

template <typename TestClassT>
inline void TestGroup<TestClassT>::log(const std::string & string) {
	return Testing::TestRunner::runner().log(string);
}

template <typename TestClassT>
template <typename S, typename ... SR>
inline void TestGroup<TestClassT>::log(const S & p, const SR & ... pRest) {
	using ::toString;
	log(toString(p, pRest ...));
}

template <typename TestClassT>
inline void TestGroup<TestClassT>::logLine(const std::string & string) {
	return Testing::TestRunner::runner().logLine(string);
}

template <typename TestClassT>
template <typename S, typename ... SR>
inline void TestGroup<TestClassT>::logLine(const S & p, const SR & ... pRest) {
	using ::toString;
	logLine(toString(p, pRest ...));
}

template <typename TestClassT>
inline TestGroup<TestClassT>::TestGroup()
	: TestGroupBase(TestRunner::runner())
	, executionModels(SingleThreaded | WorkerThreads | WorkerProcesses | ProcessPerTest)
	, testGroupName(Impl::TestRunnerExecutor::extractTypeName(typeid(*this).name(), false)) {
	(void) &instance; // force instantiation
}

template <typename TestClassT>
inline TestGroup<TestClassT>::TestGroup(unsigned int executionModels_)
	: TestGroupBase(TestRunner::runner())
	, executionModels(executionModels_)
	, testGroupName(Impl::TestRunnerExecutor::extractTypeName(typeid(*this).name(), false)) {
	(void) &instance; // force instantiation
}

template <typename TestClassT>
inline void TestGroup<TestClassT>::registerTest(Method method, const std::string & testName) {
	auto m = std::shared_ptr<Impl::TestMethodBase>(new TestMethod(*static_cast<TestClassT *>(this), method));
	auto & runner = Testing::TestRunner::runner();
	runner.registerTest(this, testGroupName, m, testName);
}

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER
