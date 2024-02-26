#include <utility>

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
/// @file SingleTimeExecutor.hpp
///
/// Executes the supplied code block a single time.
///

#ifndef COM_BORA_SOFTWARE_BALAU_CONCURRENT_SINGLE_TIME_EXECUTOR
#define COM_BORA_SOFTWARE_BALAU_CONCURRENT_SINGLE_TIME_EXECUTOR

#include <atomic>
#include <functional>
#include <mutex>

namespace Balau::Concurrent {

///
/// Executes the supplied code block a single time.
///
/// Optionally register a code block to run in the destructor, only if the
/// execution has happened.
///
class SingleTimeExecutor {
	///
	/// Create a single time executor with a code block.
	///
	public: explicit SingleTimeExecutor(std::function<void ()> code_)
		: code(std::move(code_))
		, destructorCode() {}

	///
	/// Create a single time executor with a code block and a destructor code block.
	///
	/// The executor will execute the first supplied code block a single time and
	/// will execute the second supplied code block if the first had already been
	/// executed at the time of destruction.
	///
	public: SingleTimeExecutor(std::function<void ()> code_, std::function<void ()> destructorCode_)
		: code(std::move(code_))
		, destructorCode(std::move(destructorCode_)) {}

	///
	/// Destroy the single time executor.
	///
	public: ~SingleTimeExecutor() {
		bool isExecuted = executed.load(std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_acquire);

		if (isExecuted) {
			std::lock_guard<std::mutex> lock(mutex);
			isExecuted = executed.load(std::memory_order_relaxed);

			if (isExecuted && destructorCode) {
					destructorCode();
			}
		}
	}

	///
	/// Execute the code a single time.
	///
	public: void operator () () {
		bool isExecuted = executed.load(std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_acquire);

		if (!isExecuted) {
			std::lock_guard<std::mutex> lock(mutex);
			isExecuted = executed.load(std::memory_order_relaxed);

			if (!isExecuted && code) {
				code();
				std::atomic_thread_fence(std::memory_order_release);
				executed.store(true, std::memory_order_relaxed);
			}
		}
	}

	///
	/// Returns true if the code has been executed.
	///
	/// If the operator () is being concurrently run in other threads whilst this
	/// method is called, this method will not necessarily reflect any objective
	/// reality to whether the code has been run at the time of the call. This
	/// method should thus only be used when there is no possibility of the
	/// operator () being run in other threads, otherwise it will return a
	/// meaningless result.
	///
	public: bool hasExecuted() const {
		return executed;
	}

	///
	/// Get the internal mutex in order to synchronise on external code.
	///
	/// Use with care.
	///
	public: std::mutex & getMutex() {
		return mutex;
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::function<void ()> code;
	private: std::function<void ()> destructorCode;
	private: std::atomic<bool> executed;
	private: std::mutex mutex;
};

} // namespace Balau::Concurrent

#endif // COM_BORA_SOFTWARE_BALAU_CONCURRENT_SINGLE_TIME_EXECUTOR
