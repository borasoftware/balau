// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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
	public: std::string preText;
	public: std::string postText;
	public: std::string testName;
	public: std::shared_ptr<TestGroupBase> group;
	public: std::shared_ptr<TestMethodBase> method;

	public: FlattenedTestCase(unsigned int testIndex_,
	                          std::string preText_,
	                          std::string postText_,
	                          std::string testName_,
	                          std::shared_ptr<TestGroupBase> group_,
	                          std::shared_ptr<TestMethodBase> && method_)
		: testIndex(testIndex_)
		, preText(std::move(preText_))
		, postText(std::move(postText_))
		, testName(std::move(testName_))
		, group(std::move(group_))
		, method(std::move(method_)) {}
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__FLATTENED_TEST_CASE
