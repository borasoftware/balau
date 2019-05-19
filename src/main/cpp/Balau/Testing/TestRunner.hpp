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

#include <Balau/Dev/Assert.hpp>
#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Exception/SystemExceptions.hpp>
#include <Balau/Exception/TestExceptions.hpp>
#include <Balau/Resource/FileByteWriteResource.hpp>
#include <Balau/System/ThreadName.hpp>
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
	/// Signal that the currently executing test case has been ignored by the test itself.
	///
	public: void ignore();

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
	protected: TestGroup();

	///
	/// Create a test group that runs only when the test run has the specified execution model(s).
	///
	/// The execution models can be ORed together if the test group should be
	/// run for multiple execution models.
	///
	protected: explicit TestGroup(unsigned int executionModels_);

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

	private: const std::string & getFullGroupName() const final {
		return fullTestGroupName;
	}

	private: const std::string & getGroupName() const final {
		return testGroupName;
	}

	private: void setGroupName(std::string && name) final {
		testGroupName = std::move(name);
	}

	private: unsigned int getExecutionModels() const final {
		return executionModels;
	}

	//
	// The single instance of the implementing test group class.
	// This is forcibly instantiated.
	//
	private: static TestGroupT instance;

	private: unsigned int executionModels;
	private: std::string fullTestGroupName;
	private: std::string testGroupName;
};

template <typename TestGroupT> TestGroupT TestGroup<TestGroupT>::instance;

////////////////////////////// Test runner class //////////////////////////////

///
/// The main test runner class.
///
/// The runner runs the registered test group class' test cases and produces a
/// report at the end of execution.
///
/// If not explicitly initialised, the default execution model used is WorkerProcesses
/// and the default concurrency level is one thread/process per available CPU core.
///
class TestRunner {
	///
	/// Run the test runner with the execution model specified in the first argv element, default concurrency and the test list specified in subsequent argv elements.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param argvStart the command line argument index of the start of the test list
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(int argc,
	                       char * argv[],
	                       int argvStart = 1,
	                       bool useNamespaces = false,
	                       bool pauseAtExit = false) {
		const auto executionModel = argc > argvStart ? parseExecutionModel(argv[argvStart]) : ExecutionModel::SingleThreaded;
		auto & r = runner();
		r.testList = createTestList(argc, argv, argvStart + 1);
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run the test runner with the execution model specified in the first argv element, the specified concurrency level and the test list specified in subsequent argv elements.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param argvStart the command line argument index of the start of the test list
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(int argc,
	                       char * argv[],
	                       unsigned int concurrencyLevel_,
	                       int argvStart = 1,
	                       bool useNamespaces = false,
	                       bool pauseAtExit = false) {
		const auto executionModel = argc > argvStart ? parseExecutionModel(argv[argvStart]) : ExecutionModel::SingleThreaded;
		auto & r = runner();
		r.testList = createTestList(argc, argv, argvStart + 1);
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = concurrencyLevel_;
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run the test runner with the specified execution model, default concurrency and the test list specified in argv.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param argvStart the command line argument index of the start of the test list
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(ExecutionModel executionModel_,
	                       int argc,
	                       char * argv[],
	                       int argvStart = 1,
	                       bool useNamespaces = false,
	                       bool pauseAtExit = false) {
		auto & r = runner();
		r.testList = createTestList(argc, argv, argvStart);
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with the specified execution model and default concurrency.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(ExecutionModel executionModel_, bool useNamespaces = false, bool pauseAtExit = false) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with a WorkerProcesses execution model and the specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param concurrencyLevel_ the number of processes to use to run the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(unsigned int concurrencyLevel_, bool useNamespaces = false, bool pauseAtExit = false) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel(concurrencyLevel_);
		r.executionModel = r.checkOutOfProcessCapability(WorkerProcesses);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with a WorkerProcesses execution model and the specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param concurrencyLevel_ the number of processes to use to run the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(int concurrencyLevel_, bool useNamespaces = false, bool pauseAtExit = false) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel((unsigned int) concurrencyLevel_);
		r.executionModel = r.checkOutOfProcessCapability(WorkerProcesses);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with the specified execution model and specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(ExecutionModel executionModel_,
	                       unsigned int concurrencyLevel_,
	                       bool useNamespaces = false,
	                       bool pauseAtExit = false) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel(concurrencyLevel_);
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with the specified execution model and specified concurrency.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: static int run(ExecutionModel executionModel_,
	                       int concurrencyLevel_,
	                       bool useNamespaces = false,
	                       bool pauseAtExit = false) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(StdOutTestWriter());
		r.concurrencyLevel = r.getConcurrencyLevel((unsigned int) concurrencyLevel_);
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run the test runner with the execution model specified in the first argv element, default concurrency, the specified writers, and the test list specified in subsequent argv elements.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param argvStart the command line argument index of the start of the test list
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(int argc,
	               char * argv[],
	               int argvStart,
	               bool useNamespaces,
	               bool pauseAtExit,
	               const WriterItemT & ... writerItems) {
		const bool firstArgIsExecutionModel = argc > argvStart ? isExecutionModel(argv[argvStart]) : false;
		Testing::ExecutionModel executionModel = Testing::SingleThreaded;

		if (firstArgIsExecutionModel) {
			fromString(executionModel, argv[argvStart]);
			std::cout << "\nRunning tests for command line specified execution model "
			          << toString(executionModel) << "\n" << std::endl;
		} else {
			std::cout << "\nRunning tests for predefined execution model SingleThreaded\n" << std::endl;
		}

		auto & r = runner();
		r.testList = createTestList(argc, argv, argvStart + (firstArgIsExecutionModel ? 1 : 0));
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run the test runner with the specified execution model, default concurrency, the specified writers, and the test list specified in argv.
	///
	/// The concurrency level will be equal to the number of cores if the
	/// execution model is WorkerThreads or WorkerProcesses, and the logging
	/// will output to stdout.
	///
	/// @param executionModel_ the execution model to use when running the tests
	/// @param argc the number of arguments in argv
	/// @param argv the command line arguments
	/// @param argvStart the command line argument index of the start of the test list
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(ExecutionModel executionModel_,
	               int argc,
	               char * argv[],
	               int argvStart,
	               bool useNamespaces,
	               bool pauseAtExit,
	               const WriterItemT & ... writerItems) {
		auto & r = runner();
		r.testList = createTestList(argc, argv, argvStart);
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with a WorkerProcesses execution model, default concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the number of cores, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @param writerItems the writer items
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(bool useNamespaces, bool pauseAtExit, const WriterItemT & ... writerItems) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(WorkerProcesses);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with the specified execution model, default concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the number of cores, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param executionModel_ the execution model to use when running the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @param writerItems the writer items
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(ExecutionModel executionModel_,
	               bool useNamespaces,
	               bool pauseAtExit,
	               const WriterItemT & ... writerItems) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.concurrencyLevel = r.getConcurrencyLevel();
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with a WorkerProcesses execution model, the specified concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @param writerItems the writer items
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(unsigned int concurrencyLevel_,
	               bool useNamespaces,
	               bool pauseAtExit,
	               const WriterItemT & ... writerItems) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.concurrencyLevel = r.getConcurrencyLevel(concurrencyLevel_);
		r.executionModel = r.checkOutOfProcessCapability(WorkerProcesses);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	///
	/// Run all tests registered with the test runner, with the specified execution model, specified concurrency, and the specified writers.
	///
	/// The concurrency level will be equal to the specified value, and the logging
	/// will output to the supplied writer items.
	///
	/// @tparam WriterItemT the writer item types
	/// @param executionModel_ the execution model to use when running the tests
	/// @param concurrencyLevel_ the number of threads or processes to use to run the tests
	/// @param useNamespaces specify whether to use or ignore test class namespaces
	/// @param pauseAtExit set this to true in order to pause for a key entry at the end of the test run
	/// @param writerItems the writer items
	/// @return 0 if all tests passed, 1 otherwise
	///
	public: template <typename ... WriterItemT>
	static int run(ExecutionModel executionModel_,
	               unsigned int concurrencyLevel_,
	               bool useNamespaces,
	               bool pauseAtExit,
	               const WriterItemT & ... writerItems) {
		auto & r = runner();
		r.writer = Impl::CompositeWriter(writerItems ...);
		r.concurrencyLevel = r.getConcurrencyLevel(concurrencyLevel_);
		r.executionModel = r.checkOutOfProcessCapability(executionModel_);
		r.setUseNamespaces(useNamespaces);
		r.pauseAtExit = pauseAtExit;
		return r.performTestRun();
	}

	////////////////////////// Private implementation /////////////////////////

	// The test runner singleton.
	private: static TestRunner & runner() {
		static TestRunner r;
		return r;
	}

	private: TestRunner()
		: writer(Impl::CompositeWriter(StdOutTestWriter()))
		, concurrencyLevel(getConcurrencyLevel())
		, executionModel(checkOutOfProcessCapability(WorkerProcesses))
		, useNamespaces(false)
		, pauseAtExit(false) {}

	friend Impl::TestGroupBase::TestGroupBase();

	// Called by the test group base class in order to get a unique index.
	private: static unsigned int getGroupIndex() {
		std::lock_guard<std::mutex> lock(runner().mutex);
		return runner().currentGroupIndex++;
	}

	// Parse the execution model string.
	private: static ExecutionModel parseExecutionModel(std::string_view s) {
		Testing::ExecutionModel model;
		const auto m = Util::Strings::toLower(s);

		if (isExecutionModel(m)) {
			fromString(model, m);
			std::cout << "\nRunning tests for command line specified execution model " << toString(model) << "\n" << std::endl;
		} else {
			model = Testing::SingleThreaded;
			std::cout << "\nRunning tests for predefined execution model SingleThreaded\n" << std::endl;
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
					new Impl::SingleThreadedTestRunnerExecutor(writer, useNamespaces, testCasesByGroup, testList)
				);

				break;
			}

			case WorkerThreads: {
				writer << "Run type = single process, multi-threaded "
				       << "(" << concurrencyLevel << " thread" << (concurrencyLevel > 1 ? "s" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::WorkerThreadsTestRunnerExecutor(writer, useNamespaces, testCasesByGroup, testList, concurrencyLevel)
				);

				break;
			}

			case WorkerProcesses: {
				writer << "Run type = worker processes "
				       << "(" << concurrencyLevel << " worker process" << (concurrencyLevel > 1 ? "es" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::WorkerProcessesTestRunnerExecutor(writer, useNamespaces, testCasesByGroup, testList, concurrencyLevel)
				);

				break;
			}

			case ProcessPerTest: {
				writer << "Run type = process per test "
				       << "(" << concurrencyLevel << " simultaneous process" << (concurrencyLevel > 1 ? "es" : "") << ")\n";

				executor = std::unique_ptr<Impl::TestRunnerExecutor>(
					new Impl::ProcessPerTestTestRunnerExecutor(writer, useNamespaces, testCasesByGroup, testList, concurrencyLevel)
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

	// Concatenates remaining arguments into a single string, forming the test list.
	private: static std::string createTestList(int argc, char * argv[], int argvStart) {
		std::string tl;

		// Empty list?
		if (argc <= argvStart) {
			return tl;
		}

		tl = argv[argvStart];

		for (int i = argvStart + 1; i < argc; ++i) {
			tl += " ";
			tl += argv[i];
		}

		return tl;
	}

	// Determine the concurrency level, either from the supplied requested concurrency level or from the hardware capabilities.
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

		const auto ignoredCount = (size_t) std::count_if(
			  testResults.begin()
			, testResults.end()
			, [] (const Impl::TestResult & message) { return message.result == Impl::TestResult::Result::Ignored; }
		);

		const size_t successCount = testResults.size() - ignoredCount - failureCount;
		const std::string totalCoreDuration = Impl::TestRunnerExecutor::durationStr(runner.getTotalCoreTime());
		const std::string totalClockDuration = Impl::TestRunnerExecutor::durationStr(duration);
		const std::string testAverageDuration = runner.getTestResults().empty() ? "n/a" : Impl::TestRunnerExecutor::durationStr(runner.getTotalCoreTime() / runner.getTestResults().size());

		writer << "\n------------------------- COMPLETED TESTS ------------------------\n"
		       << "\nTotal duration   (test run clock time)    = " << totalCoreDuration
		       << "\nAverage duration (test run clock time)    = " << testAverageDuration
		       << "\nTotal duration   (application clock time) = " << totalClockDuration << "\n";

		if (failureCount == 0) {
			writer << "\nALL TESTS PASSED"
			       << "\n  tests passed:  " << Util::Strings::padLeft(::toString(successCount), 6)
			       << "\n  tests ignored: " << Util::Strings::padLeft(::toString(ignoredCount), 6)
			       << "\n";
		} else {
			writer << "\n***** THERE WERE TEST FAILURES. *****\n"
			       << "\nTotal tests run: " << (successCount + failureCount + ignoredCount) << "\n\n"
			       << "\n  tests passed:  " << Util::Strings::padLeft(::toString(successCount), 6)
			       << "\n  tests ignored: " << Util::Strings::padLeft(::toString(ignoredCount), 6)
			       << "\n  tests failed:  " << Util::Strings::padLeft(::toString(failureCount), 6)
			       << "\n\n"
			       << "Failed tests:\n";

			std::for_each(
				  testResults.begin()
				, testResults.end()
				, [&tests, this] (const Impl::TestResult & message) {
					if (message.result == Impl::TestResult::Result::Failure) {
						writer << "      " << tests[message.testIndex].testName << "\n";
					}
				}
			);

			writer << "\n";
		}

		return failureCount == 0;
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
						testCaseVector[0].group->getFullGroupName().c_str(), useNamespaces
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
	private: unsigned int concurrencyLevel;
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
	: executionModels(SingleThreaded | WorkerThreads | WorkerProcesses | ProcessPerTest)
	, fullTestGroupName(Impl::TestRunnerExecutor::extractTypeName(typeid(*this).name(), false))
	, testGroupName(fullTestGroupName) {
	(void) &instance; // force instantiation
}

template <typename TestClassT>
inline TestGroup<TestClassT>::TestGroup(unsigned int executionModels_)
	: executionModels(executionModels_)
	, fullTestGroupName(Impl::TestRunnerExecutor::extractTypeName(typeid(*this).name(), false))
	, testGroupName(fullTestGroupName) {
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
