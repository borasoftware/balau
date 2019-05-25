// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ArrayBlockingQueueTest.hpp"

#include <Balau/Container/ArrayBlockingQueue.hpp>
#include <Balau/Container/SynchronizedQueue.hpp>
#include <Balau/System/Sleep.hpp>

#include <atomic>
#include <thread>

namespace Balau {

using Testing::is;

namespace Container {

const size_t QueueSize = 2;

class Element {
	public: size_t value;

	public: Element() : value(0) {}

	public: explicit Element(size_t theValue) : value(theValue) {}

	public: Element(Element && rhs) noexcept : value(rhs.value) {
		rhs.value = 0;
	}

	public: Element & operator = (Element && rhs) noexcept {
		value = rhs.value;
		rhs.value = 0;
		return *this;
	}
};

inline bool operator == (const Element & lhs, const Element & rhs) {
	return lhs.value == rhs.value;
}

// Simple thread for posting elements.
class TestExecutor {
	private: ArrayBlockingQueue<Element> & queue;
	private: SynchronizedQueue<Element> testQueue;
	private: std::atomic_bool shouldFinish;
	private: std::thread thread;
	private: std::mutex mutex;

	public: explicit TestExecutor(ArrayBlockingQueue<Element> & queue_)
		: queue(queue_)
		, shouldFinish(false)
		, thread(TestExecutor::launch, this) {}

	public: void finish() {
		shouldFinish = true;
		thread.join();
	}

	public: void enqueueInThread(Element && element) {
		testQueue.enqueue(std::move(element));
	}

	public: void execute() {
		while (!shouldFinish) {
			while (!testQueue.empty()) {
				Element element = testQueue.dequeue();
				queue.enqueue(std::move(element));
			}

			System::Sleep::microSleep(10); // backoff
		}
	}

	private: static void launch(TestExecutor * self) {
		self->execute();
	}
};

void ArrayBlockingQueueTest::fullQueue() {
	ArrayBlockingQueue<Element> queue(QueueSize);
	TestExecutor executor(queue);

	for (size_t m = 1; m <= 10; m++) {
		const size_t first  = m * 3;
		const size_t second = first + 1;
		const size_t third  = first + 2;

		queue.enqueue(Element(first));
		queue.enqueue(Element(second));
		executor.enqueueInThread(Element(third));

		AssertThat(queue.empty(), is(false));
		AssertThat(queue.full(), is(true));

		AssertThat(queue.dequeue().value, is(first));
		AssertThat(queue.dequeue().value, is(second));
		AssertThat(queue.dequeue().value, is(third));

		AssertThat(queue.empty(), is(true));
		AssertThat(queue.full(), is(false));
	}

	executor.finish();
}

} // namespace Container

} // namespace Balau
