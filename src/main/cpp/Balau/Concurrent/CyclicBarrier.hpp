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

///
/// @file CyclicBarrier.hpp
///
/// A synchronising barrier that can be configured for an arbitrary number of threads.
///

#ifndef COM_BORA_SOFTWARE_BALAU_CONCURRENT_CYCLIC_BARRIER
#define COM_BORA_SOFTWARE_BALAU_CONCURRENT_CYCLIC_BARRIER

#include <Balau/Concurrent/Semaphore.hpp>

namespace Balau::Concurrent {

///
/// A synchronising barrier that can be configured for an arbitrary number of threads.
///
/// The barrier automatically resets after releasing the threads.
///
/// The barrier can be reconfigured for a different number of threads. It is the
/// responsibility of the using code to ensure that the barrier is not being used
/// when reconfigured.
///
class CyclicBarrier {
	///
	/// Create a cyclic barrier with the specified count.
	///
	public: explicit CyclicBarrier(unsigned int count_)
		: turnstile1(0)
		, turnstile2(1)
		, number(count_),
		  count(count_) {}

	///
	/// Reconfigure the latch to count the specified number of times.
	///
	/// It is the responsibility of the caller to ensure that the countdown
	/// latch is not being used at the time of the reconfiguration.
	///
	public: void reconfigure(unsigned int count_) {
		std::lock_guard<std::mutex> lock(mutex);
		number = count_;
		count = count_;
	}

	///
	/// Count down the barrier, blocking if the count has not reached 0.
	///
	public: void countdown() {
		{
			std::lock_guard<std::mutex> lock(mutex);
			count -= 1;

			if (count == 0) {
				turnstile2.decrement();
				turnstile1.increment();
			}
		}

		turnstile1.decrement();
		turnstile1.increment();

		{
			std::lock_guard<std::mutex> lock(mutex);
			count += 1;

			if (count == number) {
				turnstile1.decrement();
				turnstile2.increment();
			}
		}

		turnstile2.decrement();
		turnstile2.increment();
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::mutex mutex;
	private: Semaphore turnstile1;
	private: Semaphore turnstile2;
	private: int number;
	private: int count;
};

} // namespace Balau::Concurrent

#endif // COM_BORA_SOFTWARE_BALAU_CONCURRENT_CYCLIC_BARRIER
