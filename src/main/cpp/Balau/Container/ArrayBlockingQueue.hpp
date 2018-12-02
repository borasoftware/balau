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
/// @file ArrayBlockingQueue.hpp
///
/// A blocking queue that uses wait/notify and an array to hold the elements.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__ARRAY_BLOCKING_QUEUE
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__ARRAY_BLOCKING_QUEUE

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
	///
	/// Create an array blocking queue with the specified capacity.
	///
	public: explicit ArrayBlockingQueue(unsigned int capacity)
		: elements(capacity)
		, count(0)
		, head(0)
		, tail(0) {}

	public: void enqueue(T && element) override {
		std::unique_lock<std::mutex> lock(mutex);

		while (full()) {
			enqueueCondition.wait(lock);
		}

		elements[head] = std::move(element);
		increment(head);
		++count;

		// When two threads are dequeueing, the second thread could
		// miss the notify call if it is outside of the lock.
		dequeueCondition.notify_one();
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

	public: T tryDequeue(std::chrono::milliseconds waitTime) override {
		std::unique_lock<std::mutex> lock(mutex);

		while (empty()) {
			dequeueCondition.wait_for(lock, waitTime);
		}

		if (empty()) {
			return T();
		}

		T element = std::move(elements[tail]);
		increment(tail);
		--count;

		// When two threads are enqueueing, the second thread could
		// miss the notify call if it is outside of the lock.
		enqueueCondition.notify_one();
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
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__ARRAY_BLOCKING_QUEUE
