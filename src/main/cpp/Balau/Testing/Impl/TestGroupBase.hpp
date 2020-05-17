// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_GROUP_BASE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_GROUP_BASE

#include <Balau/Testing/Impl/TestRunnerBase.hpp>

namespace Balau::Testing {

class TestRunner;

} // Balau::Testing

namespace Balau::Impl {

class TestRunnerExecutor;
class WorkerThreadsTestRunnerExecutor;

#if BOOST_OS_UNIX

class MultiProcessTestRunnerExecutor;
class WorkerProcessesTestRunnerExecutor;
class ProcessPerTestTestRunnerExecutor;

#endif

// Non-parameterised base class of test classes.
class TestGroupBase {
	friend class ::Balau::Testing::TestRunner;
	friend class TestRunnerExecutor;
	friend class WorkerThreadsTestRunnerExecutor;

	#if BOOST_OS_UNIX

	friend class MultiProcessTestRunnerExecutor;
	friend class WorkerProcessesTestRunnerExecutor;
	friend class ProcessPerTestTestRunnerExecutor;

	#endif

	protected: TestGroupBase(TestRunnerBase & runner) : groupIndex(runner.getGroupIndex()) {}

	public: static bool currentIsIgnored() {
		return getCurrentIsIgnored();
	}

	public: static void resetIgnoreCurrent() {
		getCurrentIsIgnored() = false;
	}

	public: static void ignoreCurrent() {
		getCurrentIsIgnored() = true;
	}

	public: virtual const std::string & getGroupName() const = 0;

	public: virtual ~TestGroupBase() = default;

	private: virtual void setup() = 0;

	private: virtual void teardown() = 0;

	private: static bool & getCurrentIsIgnored() {
		thread_local bool currentIgnored = false;
		return currentIgnored;
	}

	// Set the test class' name following a useNamespace update.
	private: virtual void setGroupName(std::string && name) = 0;

	private: virtual unsigned int getExecutionModels() const = 0;

	private: const unsigned int groupIndex;
	private: static thread_local bool currentIgnored;
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_GROUP_BASE
