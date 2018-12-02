// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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
