// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT_QUEUE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT_QUEUE

#include <Balau/Testing/Impl/TestResult.hpp>

#include <vector>

namespace Balau::Testing::Impl {

// Interface base class of test result message queue implementations.
class TestResultQueue {
	public: static constexpr size_t queueSize = 128;

	public: virtual ~TestResultQueue() = default;

	// Enqueue a full message.
	public: virtual void enqueue(TestResult message) = 0;

	// Dequeue a test result. If no test result is available,
	// a dummy object will be returned.
	public: virtual TestResult tryDequeue() = 0;
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_RESULT_QUEUE
