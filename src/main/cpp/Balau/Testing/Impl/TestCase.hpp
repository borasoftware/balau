// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_CASE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_CASE

#include <memory>

namespace Balau::Testing::Impl {

class TestGroupBase;
class TestMethodBase;

// Test case contain the group instance, the test method, and the test name.
class TestCase {
	public: std::shared_ptr<TestGroupBase> group;
	public: std::shared_ptr<TestMethodBase> method;
	public: std::string name;

	public: TestCase(std::shared_ptr<TestMethodBase> method_, std::string && name_)
		: method(std::move(method_))
		, name(std::move(name_)) {}

	public: TestCase(TestCase && rhs) noexcept
		: group(std::move(rhs.group))
		, method(std::move(rhs.method))
		, name(std::move(rhs.name)) {}

	public: TestCase & operator = (TestCase && rhs) noexcept {
		group = std::move(rhs.group);
		method = std::move(rhs.method);
		name = std::move(rhs.name);
		return *this;
	}
}; // class TestCase

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_CASE
