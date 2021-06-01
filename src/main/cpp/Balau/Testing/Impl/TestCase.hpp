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

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_CASE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_CASE

#include <memory>

namespace Balau::Testing::Impl {

class TestGroupBase;
class TestMethodBase;

// Test case contain the group instance, the test method, and the test name.
class TestCase {
	public: TestGroupBase * group;
	public: std::shared_ptr<TestMethodBase> method;
	public: std::string name;

	public: TestCase(TestGroupBase * group_, std::shared_ptr<TestMethodBase> method_, std::string && name_)
		: group(group_)
		, method(std::move(method_))
		, name(std::move(name_)) {}

	public: TestCase(TestCase && rhs) noexcept
		: group(rhs.group)
		, method(std::move(rhs.method))
		, name(std::move(rhs.name)) {}

	public: TestCase & operator = (TestCase && rhs) noexcept {
		group = rhs.group;
		method = std::move(rhs.method);
		name = std::move(rhs.name);
		return *this;
	}
}; // class TestCase

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_CASE
