// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CyclicBarrier.hpp"
#include "../Logging/Logger.hpp"

namespace Balau::Concurrent {

Logger & CyclicBarrier::LOG = Logger::getLogger("balau.concurrent");

void CyclicBarrier::countdown() {
	BalauLogTrace(LOG, "A enter latch");

	{
		std::lock_guard<std::mutex> lock(mutex);
		count -= 1;
		BalauLogTrace(LOG, "B counted down to {}", count);

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
		BalauLogTrace(LOG, "H counted up to {}", count);

		if (count == number) {
			turnstile1.decrement();
			turnstile2.increment();
		}
	}

	turnstile2.decrement();
	turnstile2.increment();

	BalauLogTrace(LOG, "M exit latch, count = {}", count);
}

} // namespace Balau::Concurrent

