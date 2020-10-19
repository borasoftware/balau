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
#include "CyclicBarrier.hpp"
#include "../Logging/Logger.hpp"

namespace Balau::Concurrent {

Logger & CyclicBarrier::LOG() {
	static Logger & instance = Logger::getLogger("balau.concurrent");
	return instance;
}

void CyclicBarrier::countdown() {
	BalauLogTrace(LOG(), "A enter latch");

	{
		std::lock_guard<std::mutex> lock(mutex);
		count -= 1;
		BalauLogTrace(LOG(), "B counted down to {}", count);

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
		BalauLogTrace(LOG(), "H counted up to {}", count);

		if (count == number) {
			turnstile1.decrement();
			turnstile2.increment();
		}
	}

	turnstile2.decrement();
	turnstile2.increment();

	BalauLogTrace(LOG(), "M exit latch, count = {}", count);
}

} // namespace Balau::Concurrent

