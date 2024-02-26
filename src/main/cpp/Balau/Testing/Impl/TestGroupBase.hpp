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

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_TEST_GROUP_BASE
#define COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_TEST_GROUP_BASE

#include <Balau/Testing/Impl/TestRunnerBase.hpp>

namespace Balau::Testing {

class TestRunner;

namespace Impl {

class TestRunnerExecutor;
class WorkerThreadsTestRunnerExecutor;
class MultiProcessTestRunnerExecutor;
class WorkerProcessesTestRunnerExecutor;
class ProcessPerTestTestRunnerExecutor;

// Non-parameterised base class of test classes.
class TestGroupBase {
	friend class ::Balau::Testing::TestRunner;
	friend class TestRunnerExecutor;
	friend class WorkerThreadsTestRunnerExecutor;
	friend class MultiProcessTestRunnerExecutor;
	friend class WorkerProcessesTestRunnerExecutor;
	friend class ProcessPerTestTestRunnerExecutor;

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
};

} // namespace Impl

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_TEST_GROUP_BASE
