// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT

#include <Balau/Serialization/SerializationMacros.hpp>

#include <string>

namespace Balau::Testing::Impl {

//
// Test result message.
//
// Note that the encode and decode functions in the out of process queue must
// be updated if this class is changed.
//
class TestResult {
	public: long long duration;
	public: unsigned int groupIndex;
	public: unsigned int testIndex;
	public: bool success;
	public: std::string resultText;

	public: TestResult()
		: duration(-1)
		, groupIndex(0)
		, testIndex(0)
		, success(false) {}

	public: TestResult(long long duration_,
	                   unsigned int groupIndex_,
	                   unsigned int testIndex_,
	                   bool success_,
	                   std::string && resultText_)
		: duration(duration_)
		, groupIndex(groupIndex_)
		, testIndex(testIndex_)
		, success(success_)
		, resultText(std::move(resultText_)){}

	public: TestResult(TestResult && rhs) noexcept
		: duration(rhs.duration)
		, groupIndex(rhs.groupIndex)
		, testIndex(rhs.testIndex)
		, success(rhs.success)
		, resultText(std::move(rhs.resultText)) {}

	public: TestResult & operator = (TestResult && rhs) noexcept {
		duration = rhs.duration;
		groupIndex = rhs.groupIndex;
		testIndex = rhs.testIndex;
		success = rhs.success;
		resultText = std::move(rhs.resultText);
		return *this;
	}

	public: void operator += (const TestResult & rhs) {
		duration = rhs.duration;
		groupIndex = rhs.groupIndex;
		testIndex = rhs.testIndex;
		success = rhs.success;
		resultText += rhs.resultText;
	}

	// Serialisation method for marshalling/unmarshalling.
	public: template <typename Archive> void serialize(Archive & archive, unsigned int ) {
		archive
			& BoostSerialization(duration)
			& BoostSerialization(groupIndex)
			& BoostSerialization(testIndex)
			& BoostSerialization(success)
			& BoostSerialization(resultText);
	}
}; // class TestResult

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT
