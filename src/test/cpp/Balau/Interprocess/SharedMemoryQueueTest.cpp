// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "SharedMemoryQueueTest.hpp"

#include "SharedMemoryQueueTest.hpp"

#include "Balau/Concurrent/Semaphore.hpp"
#include "Balau/Interprocess/USharedMemoryObject.hpp"
#include "Balau/Interprocess/SharedMemoryQueue.hpp"
#include <Balau/System/SystemClock.hpp>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

using namespace Balau::Concurrent;

namespace Balau {

using Testing::is;

namespace Interprocess {

// Shared between the child processes in the tests.
// TODO convert to use boost interprocess basic_string when the ManagedSharedMemory class is written.
template <int childProcessCount> struct SharedTestState {
	std::array<std::array<char, 1024>, childProcessCount> reports { std::array<char, 1024> { 0 } };
	std::atomic_uint dequeueCounter { 0 };
	std::atomic_uint enqueueCounter { 0 };
	boost::interprocess::interprocess_semaphore sync { 0 };
};

////////////// Common tests with different queue configurations ///////////////

void runSPST(SharedMemoryQueue<SMT> & queue) {
	AssertThat(queue.tryDequeue(), is(SMT { 0.0, 0 }));

	queue.enqueue({ 42.0, 34 });

	AssertThat(queue.dequeue(), is(SMT { 42.0, 34 }));
}

void runSISO(SharedMemoryQueue<SMT> & queue) {
	const size_t messageCount = 1000;

	// Single process.
	AssertThat(queue.tryDequeue(), is(SMT { 0.0, 0 }));
	queue.enqueue({ 42.0, 34 });
	AssertThat(queue.dequeue(), is(SMT { 42.0, 34 }));

	const int pid = Fork::performFork(
		[&] () {
			for (size_t messagesDequeued= 0; messagesDequeued < messageCount; ++messagesDequeued) {
				if (queue.dequeue() != SMT { 42.0, 34 }) {
					// TODO add fail message in shared state
					raise(SIGSEGV);
				}
			}

			return 0;
		}
		, true
	);

	for (size_t messagesEnqueued = 0; messagesEnqueued < messageCount; ++messagesEnqueued) {
		queue.enqueue({ 42.0, 34 });
	}

	Fork::TerminationReport report = Fork::waitOnProcess(pid);

	AssertThat("Child process did not exit correctly.", report.code, is((int) CLD_EXITED));
}

void runMISO(SharedMemoryQueueTest & self, SharedMemoryQueue<SMT> & queue) {
	const size_t messageCount = 1000;
	const size_t childProcessCount = 4;
	const SMT noDequeue = { 0.0, 0 };
	const SMT expected = { 42.0, 34 };
	std::vector<int> pids;

	USharedMemoryObject<SharedTestState<childProcessCount>> sharedTestState;

	for (size_t childProcessIndex = 0; childProcessIndex < childProcessCount; childProcessIndex++) {
		pids.push_back(
			Fork::performFork(
				[&] () {
					sharedTestState.remap();

					while (true) {
						//std::cerr << ::toString(getpid(), " - getting enqueue counter\n");
						unsigned int count = sharedTestState->enqueueCounter++;
						//std::cerr << ::toString(getpid(), " - enqueue counter got: ", count, "\n");

						if (count < messageCount) {
							queue.enqueue({ 42.0, 34 });
						} else {
							break;
						}
					}

					return 0;
				}
				, true
			)
		);
	}

	const auto timeout = std::chrono::nanoseconds(1000000000LL * 30LL); // 30 seconds (allow for Valgrind delays)
	const auto startTime = System::SystemClock().nanotime();
	unsigned int messagesCounted = 0;
	std::string failureMessage;
	unsigned int failedMessages = 0;

	while (messagesCounted != messageCount) {
		SMT actual = queue.tryDequeue(std::chrono::milliseconds(10));

		if (actual == noDequeue) {
			// NOP
		} else if (actual != expected) {
			failureMessage += "Unexpected value for dequeued object: " + toString(actual) + "\n";
			++failedMessages;

			if (failedMessages > 10) {
				break;
			}
		} else {
			++messagesCounted;
		}

		if (System::SystemClock().nanotime() - startTime > timeout) {
			failureMessage += "Timed out before dequeueing all objects ("
			                  + ::toString(messagesCounted) + " messages dequeued).\n";
			break;
		}
	}

	std::vector<Fork::TerminationReport> terminationReports;
	terminationReports.reserve(pids.size());

	if (failureMessage.empty()) {
		for (int pid : pids) {
			terminationReports.emplace_back(Fork::waitOnProcess(pid));
		}
	} else {
		for (int pid : pids) {
			terminationReports.emplace_back(Fork::terminateProcess(pid));
		}
	}

	bool failed = false;

	for (size_t m = 0; m < childProcessCount; m++) {
		if (terminationReports[m].code != CLD_EXITED) {
			self.logLine("Pid ", pids[m], " did not exit correctly: ", terminationReports[m].code);
			failed = true;
		}
	}

	if (!failureMessage.empty()) {
		AssertFail(failureMessage);
	}

	if (failed) {
		AssertFail("One or more child processes failed.");
	}
}

///////////////////////////////////////////////////////////////////////////////

void SharedMemoryQueueTest::singleBufferSPST() {
	SharedMemoryQueue<SMT> queue(100);
	runSPST(queue);
}

void SharedMemoryQueueTest::singleBufferSISO() {
	SharedMemoryQueue<SMT> queue(100);
	runSISO(queue);
}

void SharedMemoryQueueTest::singleBufferSIMO() {
	const size_t messageCount = 1000;
	const size_t childProcessCount = 4;
	const SMT noDequeue = { 0.0, 0 };
	const SMT expected = { 42.0, 34 };
	std::vector<int> pids;

	USharedMemoryObject<SharedTestState<childProcessCount>> sharedTestState;
	SharedMemoryQueue<SMT> queue(100);

	for (size_t childProcessIndex = 0; childProcessIndex < childProcessCount; childProcessIndex++) {
		pids.push_back(
			Fork::performFork(
				[&] () {
					const auto timeout = std::chrono::nanoseconds(1000000000LL * 30LL); // 30 seconds (allow for Valgrind delays)
					const auto startTime = System::SystemClock().nanotime();

					while (true) {
						SMT object = queue.tryDequeue(std::chrono::milliseconds(10));

						if (object == noDequeue) {
							if (System::SystemClock().nanotime() - startTime > timeout) {
								const std::string message = ::toString("CHILD ", getpid(), " timed out.\n");
								std::memcpy(sharedTestState->reports[childProcessIndex].data(), message.c_str(), message.length());
								raise(SIGSEGV);
							}
						} else if (object != expected) {
							const std::string message = ::toString("CHILD ", getpid(), " object not equal to expected.\n");
							std::memcpy(sharedTestState->reports[childProcessIndex].data(), message.c_str(), message.length());
							raise(SIGSEGV);
						} else {
							++sharedTestState->dequeueCounter;
						}

						if (sharedTestState->dequeueCounter == messageCount) {
							break;
						}
					}

					return 0;
				}
				, true
			)
		);
	}

	for (size_t m = 0; m < messageCount; m++) {
		queue.enqueue({ 42.0, 34 });
	}

	std::vector<Fork::TerminationReport> terminationReports;
	terminationReports.reserve(pids.size());

	for (int pid : pids) {
		terminationReports.emplace_back(Fork::waitOnProcess(pid));
	}

	bool failed = false;

	for (size_t m = 0; m < childProcessCount; m++) {
		const char * reportText = sharedTestState->reports[m].data();

		if (reportText[0] != 0) {
			logLine("Child process with pid ", pids[m], " failed: ", reportText[0]);
			failed = true;
		}

		if (terminationReports[m].code != CLD_EXITED) {
			logLine("Pid ", pids[m], " did not exit correctly: ", terminationReports[m].code);
			failed = true;
		}
	}

	if (failed) {
		AssertFail("One or more child processes failed.");
	}
}

void SharedMemoryQueueTest::singleBufferMISO() {
	SharedMemoryQueue<SMT> queue(100);
	runMISO(*this, queue);
}

void SharedMemoryQueueTest::singleBufferMIMO() {
	const SMT noDequeue = { 0.0, 0 };
	const SMT expected = { 42.0, 34 };
	const unsigned int messageCount = 1000;

	const unsigned int dequeueingChildProcessCount = 4;
	const unsigned int enqueueingChildProcessCount = 4;
	std::vector<int> dequeueingPids;
	std::vector<int> enqueueingPids;

	USharedMemoryObject<SharedTestState<dequeueingChildProcessCount>> sharedTestState;
	SharedMemoryQueue<SMT> queue(messageCount);

	for (size_t childProcessIndex = 0; childProcessIndex < dequeueingChildProcessCount; childProcessIndex++) {
		dequeueingPids.push_back(
			Fork::performFork(
				[&] () {
					sharedTestState->sync.wait();

					const auto timeout = std::chrono::nanoseconds(1000000000LL * 30LL); // 30 seconds (allow for Valgrind delays)
					const auto startTime = System::SystemClock().nanotime();

					while (true) {
						SMT object = queue.tryDequeue(std::chrono::milliseconds(10));

						if (object == noDequeue) {
							if (System::SystemClock().nanotime() - startTime > timeout) {
								const std::string message = ::toString("CHILD ", getpid(), " timed out.\n");
								std::memcpy(sharedTestState->reports[childProcessIndex].data(), message.c_str(), message.length());
								raise(SIGSEGV);
							}
						} else if (object != expected) {
							const std::string message = ::toString("CHILD ", getpid(), " object not equal to expected.\n");
							std::memcpy(sharedTestState->reports[childProcessIndex].data(), message.c_str(), message.length());
							raise(SIGSEGV);
						} else {
							++sharedTestState->dequeueCounter;
						}

						if (sharedTestState->dequeueCounter == messageCount) {
							break;
						}
					}

					return 0;
				}
				, true
			)
		);
	}

	for (size_t childProcessIndex = 0; childProcessIndex < enqueueingChildProcessCount; childProcessIndex++) {
		enqueueingPids.push_back(
			Fork::performFork(
				[&] () {
					sharedTestState->sync.wait();

					while (true) {
						unsigned int c = sharedTestState->enqueueCounter++;

						if (c < messageCount) {
							queue.enqueue(SMT { 42.0, 34});
						} else {
							break;
						}
					}

					return 0;
				}
				, true
			)
		);
	}

	for (size_t m = 0; m < dequeueingChildProcessCount + enqueueingChildProcessCount; m++) {
		sharedTestState->sync.post();
	}

	std::vector<Fork::TerminationReport> dequeueingTerminationReports;
	dequeueingTerminationReports.reserve(dequeueingChildProcessCount);

	for (int pid : dequeueingPids) {
		dequeueingTerminationReports.emplace_back(Fork::waitOnProcess(pid));
	}

	std::vector<Fork::TerminationReport> enqueueingTerminationReports;
	enqueueingTerminationReports.reserve(enqueueingChildProcessCount);

	for (int pid : enqueueingPids) {
		enqueueingTerminationReports.emplace_back(Fork::waitOnProcess(pid));
	}

	bool failed = false;

	for (size_t m = 0; m < dequeueingChildProcessCount; m++) {
		const char * reportText = sharedTestState->reports[m].data();

		if (reportText[0] != 0) {
			logLine("Dequeueing pid ", dequeueingPids[m], " failed: ", reportText[0]);
			failed = true;
		}

		if (dequeueingTerminationReports[m].code != CLD_EXITED) {
			logLine(
				  "Dequeueing pid "
				, dequeueingPids[m]
				, " did not exit correctly: "
				, dequeueingTerminationReports[m].code
			);

			failed = true;
		}
	}

	for (size_t m = 0; m < enqueueingChildProcessCount; m++) {
		const char * reportText = sharedTestState->reports[m].data();

		if (reportText[0] != 0) {
			logLine("Enqueueing pid ", enqueueingPids[m], " failed: ", reportText[0]);
			failed = true;
		}

		if (enqueueingTerminationReports[m].code != CLD_EXITED) {
			logLine(
				  "Enqueueing pid "
				, enqueueingPids[m]
				, " did not exit correctly: "
				, enqueueingTerminationReports[m].code
			);

			failed = true;
		}
	}

	if (failed) {
		AssertFail("One or more child processes failed.");
	}
}

void SharedMemoryQueueTest::multipleBufferSPST() {
	SharedMemoryQueue<SMT> queue(100, (unsigned int) (sizeof(SMT) / 4));
	runSPST(queue);
}


void SharedMemoryQueueTest::multipleBufferSISO() {
	SharedMemoryQueue<SMT> queue(100, (unsigned int) (sizeof(SMT) / 4));
	runSISO(queue);
}

void SharedMemoryQueueTest::multipleBufferMISO() {
	SharedMemoryQueue<SMT> queue(100, (unsigned int) (sizeof(SMT) / 4));
	runMISO(*this, queue);
}

void SharedMemoryQueueTest::multipleBufferPending() {
	//
	// Testing of the pending buffer with a re-queued multi-chunk marshal buffer.
	// This is achieved by:
	//  - setting up the underlying shared memory queue directly;
	//  - calling tryDequeue();
	//  - appending the last chunk to to underlying shared memory queue directly;
	//  - calling dequeue().
	//
	// The steps are performed in a single thread.
	// In order to manipulate the underlying shared memory queue directly, this
	// test class is a friend of the SharedMemoryQueue class.
	//
	// The code is adapted from the enqueue method of the implementation.
	//

	SharedMemoryQueue<SMT> queue(100, (unsigned int) (sizeof(SMT) / 4));

	std::vector<std::vector<char>> chunkBuffers;

	const Impl::QueueHeader messageHeader { queue.queueState->sequenceNumber++, 1, 0, 0 };
	std::vector<char> marshalBuffer;
	marshalBuffer.clear();
	queue.marshal(marshalBuffer, SMT { 42.0, 34 }, messageHeader);
	const unsigned int totalBytes = (unsigned int) marshalBuffer.size() - SharedMemoryQueue<SMT>::headerSize;
	auto * marshalHeader = (Impl::QueueHeader *) marshalBuffer.data();
	marshalHeader->totalBytes = totalBytes;
	const unsigned int chunkCount = queue.calculateChunkCount(totalBytes);
	size_t dataStart = SharedMemoryQueue<SMT>::headerSize;

	// Build the chunks.
	for (unsigned int m = 0; m < chunkCount; m++) {
		size_t chunkStart = dataStart - SharedMemoryQueue<SMT>::headerSize;
		auto & chunkHeader = * (Impl::QueueHeader *) (marshalBuffer.data() + chunkStart);
		chunkHeader.sequenceNumber = messageHeader.sequenceNumber;
		chunkHeader.chunkCount = chunkCount;
		chunkHeader.chunkNumber = m;
		chunkHeader.totalBytes = totalBytes;

		const size_t chunkBytes = m < chunkCount - 1
			? queue.chunkSize
			: marshalBuffer.size() - chunkStart;

		chunkBuffers.emplace_back(marshalBuffer.data() + chunkStart, marshalBuffer.data() + chunkStart + chunkBytes);
		dataStart += queue.chunkSize - SharedMemoryQueue<SMT>::headerSize;
	}

	// Send all but the last chunk
	for (unsigned int m = 0; m < chunkBuffers.size() - 1; m++) {
		queue.queue.send(chunkBuffers[m].data(), chunkBuffers[m].size(), 0);
	}

	// Send another object via the API.
	queue.enqueue(SMT { 123.4, 12 });

	// Try to dequeue the incomplete object. This should result in the incomplete
	// object's marshal buffer being re-queued on the internal process local queue.
	SMT object = queue.tryDequeue();

	AssertThat(object, is(SMT {}));

	// Send the last chunk.
	queue.queue.send(chunkBuffers[chunkBuffers.size() - 1].data(), chunkBuffers[chunkBuffers.size() - 1].size(), 0);

	// Try to dequeue again. This should result in the object being returned.
	SMT object2 = queue.tryDequeue();

	AssertThat(object2, is(SMT { 42.0, 34 }));

	// Dequeue again. This should result in second enqueued object being returned.
	SMT object3 = queue.tryDequeue();

	AssertThat(object3, is(SMT { 123.4, 12 }));
}

} // namespace Interprocess

} // namespace Balau

#pragma clang diagnostic pop
