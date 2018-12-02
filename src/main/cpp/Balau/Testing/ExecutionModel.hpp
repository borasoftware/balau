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
/// @file ExecutionModel.hpp
///
/// Test runner execution model enum.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER_EXECUTION_MODEL
#define COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER_EXECUTION_MODEL

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Util/Strings.hpp>

namespace Balau::Testing {

///
/// The type of execution model to be used by the test runner.
///
/// This parameter determines whether the test runner runs the tests in threads,
/// worker processes, or a process per test. Each execution model has its own
/// advantages and disadvantages.
///
enum ExecutionModel {
	///
	/// Run all tests in a single thread.
	///
	/// For this execution model, any specified concurrency level is ignored.
	///
	/// If a test causes a segmentation fault, the entire test application will
	/// terminate.
	///
	SingleThreaded = 1

	///
	/// Run all tests in a single process.
	///
	/// A set of worker threads is created and each worker thread consumes test
	/// cases from a common list.
	///
	/// For this execution model, the concurrency level determines the number
	/// of worker threads to create.
	///
	/// If a test causes a segmentation fault, the entire test application will
	/// terminate.
	///
	, WorkerThreads = 2

	///
	/// Run tests in a set of worker processes.
	///
	/// The worker processes are created and each worker process consumes test
	/// cases from a common list.
	///
	/// For this execution model, the concurrency level determines the number
	/// of worker processes to maintain.
	///
	/// If a test causes a segmentation fault, the worker process will terminate and
	/// a new worker process will be spawned to take its place.
	///
	, WorkerProcesses = 4

	///
	/// Run each test in a separate worker process.
	///
	/// This execution model provides a completely fresh process for each test,
	/// resulting in complete isolation from other tests.
	///
	/// For this execution model, the concurrency level determines the number
	/// of simultaneous tests to run.
	///
	/// If a test causes a segmentation fault, the test's worker process will
	/// terminate. This will not affect the other tests.
	///
	, ProcessPerTest = 8
};

///
/// Print the execution model value as a UTF-8 string.
///
/// @return a UTF-8 string representing the execution model value
///
inline std::string toString(ExecutionModel model) {
	switch (model) {
		case SingleThreaded:  return "SingleThreaded";
		case WorkerThreads:   return "WorkerThreads";
		case WorkerProcesses: return "WorkerProcesses";
		case ProcessPerTest:  return "ProcessPerTest";
		default: ThrowBalauException(Exception::BugException, "Unknown execution model: " + ::toString(static_cast<int>(model)));
	}
}

///
/// Obtain the execution model from a string.
///
/// The string is parsed in order to obtain the execution model enum value.
/// The case is ignored.
///
inline ExecutionModel executionModelFromString(const std::string & model) {
	if (Util::Strings::equalsIgnoreCase(model, "SingleThreaded")) {
		return SingleThreaded;
	}

	if (Util::Strings::equalsIgnoreCase(model, "WorkerThreads")) {
		return WorkerThreads;
	}

	if (Util::Strings::equalsIgnoreCase(model, "WorkerProcesses")) {
		return WorkerProcesses;
	}

	if (Util::Strings::equalsIgnoreCase(model, "ProcessPerTest")) {
		return ProcessPerTest;
	}

	ThrowBalauException(Exception::BugException, "Unknown execution model: " + model);
}

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER_EXECUTION_MODEL
