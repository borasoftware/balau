// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>

#include <Balau/Container/ArrayBlockingQueue.hpp>
#include <Balau/Container/SynchronizedQueue.hpp>
#include <Balau/System/Sleep.hpp>
#include <Balau/Type/OnScopeExit.hpp>

#include <atomic>
#include <thread>

namespace Balau::Container {

struct Element {
	size_t value;

	Element() : value(0) {}

	explicit Element(size_t theValue) : value(theValue) {}

	Element(Element && rhs) noexcept : value(rhs.value) {
		rhs.value = 0;
	}

	Element & operator = (Element && rhs) noexcept {
		value = rhs.value;
		rhs.value = 0;
		return *this;
	}
};

inline bool operator == (const Element & lhs, const Element & rhs) {
	return lhs.value == rhs.value;
}

// Simple thread for posting elements.
struct TestExecutor {
	ArrayBlockingQueue<Element> & queue;
	SynchronizedQueue<Element> toBeQueued;
	std::atomic_bool shouldFinish;
	std::thread thread;

	explicit TestExecutor(ArrayBlockingQueue<Element> & queue_)
		: queue(queue_)
		, shouldFinish(false)
		, thread([this] () { execute(); }) {}

	void finish() {
		shouldFinish = true;
		thread.join();
	}

	void enqueueInThread(Element element) {
		toBeQueued.enqueue(Element(std::move(element)));
	}

	void execute() {
		while (!shouldFinish) {
			while (!toBeQueued.empty()) {
				auto element = toBeQueued.dequeue();
				queue.enqueue(std::move(element));
			}

			System::Sleep::microSleep(10); // backoff
		}
	}
};

struct ArrayBlockingQueueTest : public Testing::TestGroup<ArrayBlockingQueueTest> {
	ArrayBlockingQueueTest() {
		registerTest(&ArrayBlockingQueueTest::fullQueue,          "fullQueue");
		registerTest(&ArrayBlockingQueueTest::fullQueueCallbacks, "fullQueueCallbacks");
	}

	void fullQueue() {
		constexpr size_t QueueSize = 2;
		ArrayBlockingQueue<Element> queue(QueueSize);
		TestExecutor executor(queue);
		OnScopeExit finishExecution([&executor] () { executor.finish(); });

		for (size_t m = 1; m <= 10; m++) {
			const size_t first  = m * 3;
			const size_t second = first + 1;
			const size_t third  = first + 2;

			executor.queue.enqueue(Element(first));
			executor.queue.enqueue(Element(second));
			executor.enqueueInThread(Element(third));

			AssertThat(executor.queue.empty(), is(false));
			AssertThat(executor.queue.full(), is(true));

			AssertThat(executor.queue.dequeue().value, is(first));
			AssertThat(executor.queue.dequeue().value, is(second));
			AssertThat(executor.queue.dequeue().value, is(third));

			AssertThat(executor.queue.empty(), is(true));
			AssertThat(executor.queue.full(), is(false));
		}
	}

	void fullQueueCallbacks() {
		constexpr size_t QueueSize = 2;
		constexpr size_t IterationCount = 10;
		constexpr size_t FullEnqueueCount = 100;
		std::atomic_uint fullCount { 0 };
		std::atomic_uint spaceNowAvailableCount { 0 };

		ArrayBlockingQueue<Element> queue(
			QueueSize
			, [&fullCount] (const Element &) {
				++fullCount;
			}
			, [&spaceNowAvailableCount] (const Element &) {
				++spaceNowAvailableCount;
			}
		);

		TestExecutor executor(queue);
		OnScopeExit finishExecution([&executor] () { executor.finish(); });

		for (size_t m = 1; m <= IterationCount; m++) {
			const size_t first  = m * 3;
			const size_t second = first + 1;

			queue.enqueue(Element(first));
			queue.enqueue(Element(second));

			AssertThat(queue.empty(), is(false));
			AssertThat(queue.full(), is(true));

			for (size_t n = 0; n < FullEnqueueCount; n++) {
				executor.enqueueInThread(Element(second + 1 + n));
			}

			System::Sleep::microSleep(10); // backoff

			AssertThat(queue.dequeue().value, is(first));
			AssertThat(queue.dequeue().value, is(second));

			size_t n = 0;
			size_t p = 0;
			bool success;

			while (n < FullEnqueueCount && p < FullEnqueueCount * 100) {
				auto element = queue.tryDequeue(std::chrono::milliseconds(10), success);

				if (success) {
					AssertThat(element.value, is(second + 1 + n));
					++n;
				}

				++p;
			}

			AssertThat(n, isGreaterThan(0U));

			queue.tryDequeue(success);

			AssertThat(success, is(false));

			AssertThat(queue.empty(), is(true));
			AssertThat(queue.full(), is(false));

			AssertThat(fullCount.load(), isGreaterThan(0U));
			AssertThat(spaceNowAvailableCount.load(), isGreaterThan(0U));

			AssertThat(fullCount.load(), is(spaceNowAvailableCount.load()));
		}
	}
};

} // namespace Balau::Container
