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
	public: enum class Result {
		Success, Failure, Ignored
	};

	public: long long duration;
	public: unsigned int groupIndex;
	public: unsigned int testIndex;
	public: Result result;
	public: std::string resultText;

	public: TestResult()
		: duration(-1)
		, groupIndex(0)
		, testIndex(0)
		, result(Result::Ignored) {}

	public: TestResult(long long duration_,
	                   unsigned int groupIndex_,
	                   unsigned int testIndex_,
	                   Result result_,
	                   std::string && resultText_)
		: duration(duration_)
		, groupIndex(groupIndex_)
		, testIndex(testIndex_)
		, result(result_)
		, resultText(std::move(resultText_)){}

	public: TestResult(unsigned int groupIndex_, unsigned int testIndex_)
		: duration(0)
		, groupIndex(groupIndex_)
		, testIndex(testIndex_)
		, result(Result::Ignored) {}

	public: TestResult(TestResult && rhs) noexcept
		: duration(rhs.duration)
		, groupIndex(rhs.groupIndex)
		, testIndex(rhs.testIndex)
		, result(rhs.result)
		, resultText(std::move(rhs.resultText)) {}

	public: TestResult & operator = (TestResult && rhs) noexcept {
		duration = rhs.duration;
		groupIndex = rhs.groupIndex;
		testIndex = rhs.testIndex;
		result = rhs.result;
		resultText = std::move(rhs.resultText);
		return *this;
	}

	public: void operator += (const TestResult & rhs) {
		duration = rhs.duration;
		groupIndex = rhs.groupIndex;
		testIndex = rhs.testIndex;
		result = rhs.result;
		resultText += rhs.resultText;
	}

	// Serialisation method for marshalling/unmarshalling.
	public: template <typename Archive> void serialize(Archive & archive, unsigned int ) {
		archive
			& BoostSerialization(duration)
			& BoostSerialization(groupIndex)
			& BoostSerialization(testIndex)
			& BoostSerialization(result)
			& BoostSerialization(resultText);
	}
}; // class TestResult

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT
