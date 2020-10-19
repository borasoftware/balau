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

///
/// @file Semaphore.hpp
///
/// Traditional semaphore synchronisation object.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONCURRENT__SEMAPHORE
#define COM_BORA_SOFTWARE__BALAU_CONCURRENT__SEMAPHORE

#include <condition_variable>

namespace Balau::Concurrent {

///
/// Traditional semaphore synchronisation object.
///
class Semaphore {
	///
	/// Create a semaphore object, setting an initial count if desired.
	///
	public: explicit Semaphore(unsigned int count_ = 0) : count(count_) {}

	///
	/// Increment the semaphore.
	///
	public: void increment() {
		std::lock_guard<std::mutex> lock(mutex);
		++count;
		conditionVariable.notify_one();
	}

	///
	/// Decrement the semaphore, blocking if the count is zero.
	///
	public: void decrement() {
		std::unique_lock<std::mutex> lock(mutex);

		while (count == 0) {
			conditionVariable.wait(lock);
		}

		--count;
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::mutex mutex;
	private: std::condition_variable conditionVariable;
	private: unsigned int count;
};

} // namespace Balau::Concurrent

#endif // COM_BORA_SOFTWARE__BALAU_CONCURRENT__SEMAPHORE
