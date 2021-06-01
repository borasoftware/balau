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

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__FLATTENED_TEST_CASE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__FLATTENED_TEST_CASE

#include <memory>

namespace Balau::Testing::Impl {

class TestGroupBase;
class TestMethodBase;

// Test case, flattened to contain the group and an overall test index.
class FlattenedTestCase {
	public: const unsigned int testIndex;
	public: const unsigned int executionModels;
	public: std::string preText;
	public: std::string postText;
	public: std::string testName;
	public: TestGroupBase * group;
	public: std::shared_ptr<TestMethodBase> method;

	public: FlattenedTestCase(unsigned int testIndex_,
	                          unsigned int executionModels_,
	                          std::string preText_,
	                          std::string postText_,
	                          std::string testName_,
	                          TestGroupBase * group_,
	                          std::shared_ptr<TestMethodBase> && method_)
		: testIndex(testIndex_)
		, executionModels(executionModels_)
		, preText(std::move(preText_))
		, postText(std::move(postText_))
		, testName(std::move(testName_))
		, group(group_)
		, method(std::move(method_)) {}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__FLATTENED_TEST_CASE
