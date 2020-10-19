// @formatter:off
//
// Balau core C++ library
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
#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__TEST_GROUP
#define COM_BORA_SOFTWARE__BALAU_TESTING__TEST_GROUP

#include <Balau/Testing/Impl/TestGroupBase.hpp>

namespace Balau::Testing {

///////////////////////// Main test group base class //////////////////////////

///
/// Templated base class of all test classes.
///
/// Test group classes should extend this class, specifying themselves as the
/// template parameter.
///
template <typename TestGroupT> class TestGroup : public Impl::TestGroupBase {
	protected: using TestGroupType = TestGroupT;
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
	/// This version of the log method accepts a variable number of references, and
	/// concatenates them together by calling toString.
	///
	public: template <typename S, typename ... SR> void log(const S & p, const SR & ... pRest);

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

	public: const std::string & getGroupName() const final {
		return testGroupName;
	}

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
	private: std::string testGroupName;
};

template <typename TestGroupT> TestGroupT TestGroup<TestGroupT>::instance;

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__TEST_GROUP
