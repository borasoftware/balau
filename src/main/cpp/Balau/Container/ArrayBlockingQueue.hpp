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
/// @file ArrayBlockingQueue.hpp
///
/// A blocking queue that uses wait/notify and an array to hold the elements.
///

#ifndef COM_BORA_SOFTWARE_BALAU_CONTAINER_ARRAY_BLOCKING_QUEUE
#define COM_BORA_SOFTWARE_BALAU_CONTAINER_ARRAY_BLOCKING_QUEUE

#include <Balau/Container/BlockingQueue.hpp>
#include <Balau/Type/StdTypes.hpp>

#include <condition_variable>

namespace Balau::Container {

///
/// A blocking queue that uses wait/notify and an array to hold the elements.
///
/// This queue is concurrent but not lock free.
///
/// @tparam T the element type (must be default constructable in
///            addition being move constructable and assignable)
///
template <typename T> class ArrayBlockingQueue : public BlockingQueue<T> {
	public: using Callback = std::function<void (const T &)>;

	///
	/// Create an array blocking queue with the specified capacity.
	///
	public: explicit ArrayBlockingQueue(unsigned int capacity)
		: elements(capacity)
		, count(0)
		, head(0)
		, tail(0)
		, defaultQueueIsFull([] (const T &) {})
		, defaultSpaceNowAvailable([] (const T &) {}) {}

	///
	/// Create an array blocking queue with the specified capacity and default callbacks.
	///
	public: explicit ArrayBlockingQueue(unsigned int capacity, Callback defaultQueueIsFull_, Callback defaultSpaceNowAvailable_)
		: elements(capacity)
		, count(0)
		, head(0)
		, tail(0)
		, defaultQueueIsFull(defaultQueueIsFull_)
		, defaultSpaceNowAvailable(defaultSpaceNowAvailable_) {}

	public: void enqueue(T object) override {
		enqueue(std::move(object), defaultQueueIsFull, defaultSpaceNowAvailable);
	}

	///
	/// Enqueue an object, waiting for space to be available if the queue is full.
	///
	/// The supplied full queue callbacks will be used instead of the default ones.
	///
	/// @param object the object to move into the queue
	/// @param queueIsFull the callback that is called if the queue is full
	/// @param spaceNowAvailable the callback that is called if the queue was full and subsequently became no longer full
	///
	public: void enqueue(T object, const Callback & queueIsFull, const Callback & spaceNowAvailable) {
		std::unique_lock<std::mutex> lock(mutex);

		if (full()) {
			queueIsFull(object);

			while (full()) {
				enqueueCondition.wait(lock);
			}

			spaceNowAvailable(object);
		}

		elements[head] = std::move(object);
		increment(head);
		++count;

		// When two threads are dequeueing, the second thread could
		// miss the notify call if it is outside of the lock.
		dequeueCondition.notify_one();
	}

	public: bool tryEnqueue(T object) override {
		return tryEnqueue(
			std::move(object), std::chrono::milliseconds(0), defaultQueueIsFull, defaultSpaceNowAvailable
		);
	}

	public: bool tryEnqueue(T object, std::chrono::milliseconds waitTime) override {
		return tryEnqueue(std::move(object), waitTime, defaultQueueIsFull, defaultSpaceNowAvailable);
	}

	///
	/// Enqueue an object, waiting a limited amount of time for space to be available if the queue is full.
	///
	/// The supplied full queue callbacks will be used instead of the default ones.
	///
	/// @param object the object to move into the queue
	/// @param waitTime the number of milliseconds to wait if the queue is full
	/// @param queueIsFull the callback that is called if the queue is full
	/// @param spaceNowAvailable the callback that is called if the queue was full and subsequently became no longer full
	/// @return true if the object was enqueued, false otherwise
	///
	public: bool tryEnqueue(T object,
	                        std::chrono::milliseconds waitTime,
	                        const Callback & queueIsFull,
	                        const Callback & spaceNowAvailable) {
		std::unique_lock<std::mutex> lock(mutex);

		if (full()) {
			queueIsFull(object);

			auto startTime = std::chrono::system_clock::now();
			auto waitTimeRemaining = std::chrono::duration_cast<std::chrono::nanoseconds>(waitTime);

			while (full() && waitTimeRemaining > std::chrono::nanoseconds(0)) {
				enqueueCondition.wait_for(lock, waitTimeRemaining);
				waitTimeRemaining = waitTime - (std::chrono::system_clock::now() - startTime);
			}

			if (full()) {
				return false;
			}

			spaceNowAvailable(object);
		}

		elements[head] = std::move(object);
		increment(head);
		++count;

		// When two threads are dequeueing, the second thread could
		// miss the notify call if it is outside of the lock.
		dequeueCondition.notify_one();

		return true;
	}

	public: T dequeue() override {
		std::unique_lock<std::mutex> lock(mutex);

		while (empty()) {
			dequeueCondition.wait(lock);
		}

		T element = std::move(elements[tail]);
		increment(tail);
		--count;

		// When two threads are enqueueing, the second thread could
		// miss the notify call if it is outside of the lock.
		enqueueCondition.notify_one();
		return element;
	}

	public: T tryDequeue() override {
		return tryDequeue(std::chrono::milliseconds(0));
	}

	public: T tryDequeue(bool & success) override {
		return tryDequeue(std::chrono::milliseconds(0), success);
	}

	public: T tryDequeue(std::chrono::milliseconds waitTime) override {
		bool success;
		return tryDequeue(waitTime, success);
	}

	public: T tryDequeue(std::chrono::milliseconds waitTime, bool & success) override {
		std::unique_lock<std::mutex> lock(mutex);

		if (empty()) {
			auto startTime = std::chrono::system_clock::now();
			auto waitTimeRemaining = std::chrono::duration_cast<std::chrono::nanoseconds>(waitTime);

			while (empty() && waitTimeRemaining > std::chrono::nanoseconds(0)) {
				dequeueCondition.wait_for(lock, waitTimeRemaining);
				waitTimeRemaining = waitTime - (std::chrono::system_clock::now() - startTime);
			}
		}

		if (empty()) {
			success = false;
			return T();
		}

		T element = std::move(elements[tail]);
		increment(tail);
		--count;

		// When two threads are enqueueing, the second thread could
		// miss the notify call if it is outside of the lock.
		enqueueCondition.notify_one();

		success = true;
		return element;
	}

	public: bool full() const override {
		return count == elements.size();
	}

	public: bool empty() const override {
		return count == 0;
	}

	////////////////////////// Private implementation /////////////////////////

	private: void increment(size_t & ptr) {
		++ptr;
		ptr = ptr - (ptr == elements.size()) * elements.size();
	}

	private: std::vector<T> elements;
	private: size_t count;
	private: size_t head;
	private: size_t tail;
	private: std::mutex mutex;
	private: std::condition_variable enqueueCondition;
	private: std::condition_variable dequeueCondition;
	private: const Callback defaultQueueIsFull;
	private: const Callback defaultSpaceNowAvailable;
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE_BALAU_CONTAINER_ARRAY_BLOCKING_QUEUE
