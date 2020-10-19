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
