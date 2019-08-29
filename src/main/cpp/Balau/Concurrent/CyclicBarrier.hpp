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
/// @file CyclicBarrier.hpp
///
/// A synchronising barrier that can be configured for an arbitrary number of threads.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONCURRENT__CYCLIC_BARRIER
#define COM_BORA_SOFTWARE__BALAU_CONCURRENT__CYCLIC_BARRIER

#include <Balau/Concurrent/Semaphore.hpp>

namespace Balau {

class Logger;

namespace Concurrent {

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
	public: void countdown();

	///////////////////////// Private implementation //////////////////////////

	private: static Logger & LOG();

	private: std::mutex mutex;
	private: Semaphore turnstile1;
	private: Semaphore turnstile2;
	private: int number;
	private: int count;
};

} // namespace Concurrent

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_CONCURRENT__CYCLIC_BARRIER
