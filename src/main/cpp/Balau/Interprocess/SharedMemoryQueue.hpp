// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file SharedMemoryQueue.hpp
///
/// A blocking, shared memory queue that uses the Boost interprocess library.
///

#ifndef COM_BORA_SOFTWARE__BALAU_INTERPROCESS__SHARED_MEMORY_QUEUE
#define COM_BORA_SOFTWARE__BALAU_INTERPROCESS__SHARED_MEMORY_QUEUE

#include <Balau/Container/BlockingQueue.hpp>
#include <Balau/Exception/ContainerExceptions.hpp>
#include <Balau/Interprocess/MSharedMemoryObject.hpp>
#include <Balau/Interprocess/SharedMemoryUtils.hpp>
#include <Balau/Interprocess/Impl/SharedMemoryQueueImpl.hpp>
#include <Balau/Type/UUID.hpp>
#include <Balau/Util/Vectors.hpp>
#include <Balau/Util/PrettyPrint.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>

#include <deque>

// Ignore false positives for constructor field initialisation.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

namespace Balau::Interprocess {

///
/// A blocking, shared memory queue that uses the Boost interprocess library.
///
/// This class encapsulates calls to the Boost interprocess library in order to
/// create a shared memory queue, and implements the Balau::Container::BlockingQueue
/// API.
///
/// The implementation uses the Boost serialization library for marshalling and
/// unmarshalling of objects. In order to use the queue, the object type T must
/// provide Boost serialize or save/load methods.
///
/// This queue implementation has the following concurrency guarantees.
///
/// The queue can be used for concurrent enqueues and concurrent dequeues across
/// processes/threads if the maximum enqueued serialised object size + header size
/// is guaranteed to be smaller than the shared memory queue buffer size.
///
/// If the above guarantee cannot be met, the queue can be used for concurrent
/// enqueues across processes/threads, but only synchronised dequeues in a single
/// process. This is due to the dequeueing of partial objects occurring in one
/// process, rendering the continuation of the dequeueing of that object impossible
/// in other processes.
///
/// If this limitation is breached, the application(s) will be defective due to:
///  - dequeues of partial objects occurring across different processes/threads;
///  - unprotected mutation of the std::dequeue used to hold pending dequeue buffers.
///
/// Note that the second of the above points necessitates a single threaded or
/// mutex protected dequeueing in the dequeueing process when oversize objects
/// are being enqueued.
///
/// @tparam T the type of enqueued/dequeued objects
///
template <typename T> class SharedMemoryQueue : public Container::BlockingQueue<T> {
	private: using CharVector = std::vector<char>;

	public: static const unsigned int headerSize = sizeof(Impl::QueueHeader);
	public: static const unsigned int minimumChunkSize = 2 * headerSize;

	///
	/// Create a shared memory queue of type T and with the specified capacity.
	///
	/// The name is automatically generated.
	///
	public: explicit SharedMemoryQueue(unsigned int capacity, bool throwOnOversize_ = false)
		: SharedMemoryQueue(capacity, calculateDefaultBufferSize(), "SMQ_" + UUID().asString(), throwOnOversize_) {}

	///
	/// Create a shared memory queue of type T, with the specified capacity, and with the specified buffer size
	///
	/// The name is automatically generated.
	///
	public: SharedMemoryQueue(unsigned int capacity, unsigned int bufferSize_, bool throwOnOversize_ = false)
		: SharedMemoryQueue(capacity, bufferSize_, "SMO_" + UUID().asString(), throwOnOversize_) {}

	///
	/// Create a shared memory queue of type T and with the specified capacity,
	///
	/// The specified name is used.
	///
	public: SharedMemoryQueue(unsigned int capacity, std::string name_, bool throwOnOversize_ = false)
		: SharedMemoryQueue(capacity, calculateDefaultBufferSize(), std::move(name_), throwOnOversize_) {}

	///
	/// Create a shared memory queue of type T, with the specified capacity and buffer size.
	///
	/// The specified name is used.
	///
	/// @throw SizeException if the buffer size is below the minimum required buffer size
	///
	public: SharedMemoryQueue(unsigned int capacity,
	                          unsigned int bufferSize_,
	                          std::string name_,
	                          bool throwOnOversize_ = false)
		: name(std::move(name_))
		, queue(createQueue(capacity, bufferSize_))
		, chunkSize(bufferSize_)
		, queueState(CreateOnly, name + "_queueState")
		, throwOnOversize(throwOnOversize_) {}

	///
	/// Open or create a shared memory queue of type T and with the specified capacity.
	///
	/// The specified name is used.
	///
	/// In an existing queue exists, the capacity is ignored.
	///
	public: SharedMemoryQueue(OpenOrCreateSelector,
	                          unsigned int capacity,
	                          std::string name_,
	                          bool throwOnOversize_ = false)
		: SharedMemoryQueue(
			  OpenOrCreate
			, capacity
			, calculateDefaultBufferSize()
			, true
			, std::move(name_)
			, throwOnOversize_
		) {}

	///
	/// Open or create a shared memory queue of type T, with the specified capacity and buffer size.
	///
	/// The specified name is used.
	///
	/// In an existing queue exists, the capacity and buffer size are ignored.
	///
	/// @throw SizeException if the buffer size is below the minimum required buffer size
	///
	public: SharedMemoryQueue(OpenOrCreateSelector,
	                          unsigned int capacity,
	                          unsigned int bufferSize_,
	                          std::string name_,
	                          bool throwOnOversize_ = false)
		: name(std::move(name_))
		, queue(openOrCreateQueue(capacity, bufferSize_))
		, chunkSize(bufferSize_)
		, queueState(OpenOrCreate, name + "_queueState")
		, throwOnOversize(throwOnOversize_) {}

	///
	/// Open an existing shared memory queue with the specified name.
	///
	public: explicit SharedMemoryQueue(std::string name_, bool throwOnOversize_ = false)
		: name(std::move(name_))
		, queue(openQueue())
		, chunkSize((unsigned int) queue.get_max_msg_size())
		, queueState(OpenOnly, name + "_queueState")
		, throwOnOversize(throwOnOversize_) {}

	///
	/// Enqueue an object with a priority of zero.
	///
	/// If the queue is full, this call will block until there is a slot available.
	///
	public: void enqueue(T && object) override {
		enqueue(object, 0);
	}

	///
	/// Enqueue an object with the specified priority.
	///
	/// If the queue is full, this call will block until there is a slot available.
	///
	/// @throw SizeException if enqueueing of oversize objects was set to forbidden and the object is oversize
	///
	public: void enqueue(const T & object, unsigned int priority) {
		const Impl::QueueHeader messageHeader { queueState->sequenceNumber++, 1, 0, 0 };
		CharVector & marshalBuffer = Impl::SharedMemoryQueueTLS::storage.marshalBuffer;
		marshalBuffer.clear();

		marshal(marshalBuffer, object, messageHeader);

		const unsigned int totalBytes = (unsigned int) marshalBuffer.size() - headerSize;
		auto * marshalHeader = (Impl::QueueHeader *) marshalBuffer.data();
		marshalHeader->totalBytes = totalBytes;

		if (marshalBuffer.size() <= chunkSize) {
			// The message fits in a single buffer.
			queue.send(marshalBuffer.data(), marshalBuffer.size(), priority);
		} else {
			// The object is split across multiple buffers.
			// This code avoids copying by writing each header into the marshal buffer,
			// in the discarded area just before the next data to be sent.

			if (throwOnOversize) {
				ThrowBalauException(
					  Exception::SizeException
					, ::toString(
						  "The serialized message is too large to fit into a single message ("
						, marshalBuffer.size()
						, "/"
						, chunkSize
						, ")."
					)
				);
			}

			const unsigned int chunkCount = calculateChunkCount(totalBytes);
			size_t dataStart = headerSize;

			for (unsigned int m = 0; m < chunkCount; m++) {
				size_t chunkStart = dataStart - headerSize;
				auto & chunkHeader = * (Impl::QueueHeader *) (marshalBuffer.data() + chunkStart);
				chunkHeader.sequenceNumber = messageHeader.sequenceNumber;
				chunkHeader.chunkCount = chunkCount;
				chunkHeader.chunkNumber = m;
				chunkHeader.totalBytes = totalBytes;

				// Account for a potentially partial buffer for the last queue buffer.
				const size_t chunkBytes = m < chunkCount - 1
					? chunkSize
					: marshalBuffer.size() - chunkStart;

				queue.send(marshalBuffer.data() + chunkStart, chunkBytes, priority);

				// The start of the data is moved forward the size of the buffer less the header size.
				dataStart += chunkSize - headerSize;
			}
		}
	}

	///
	/// Dequeue an object.
	///
	/// When the queue is being used with oversize objects (which require multiple
	/// queue receives, this method may only be used via synchronised calls.
	///
	public: T dequeue() override {
		CharVector & queueBuffer = Impl::SharedMemoryQueueTLS::storage.queueBuffer;
		unsigned long receivedSize;
		unsigned int priority;

		dequeueNextBuffer(queueBuffer, receivedSize, priority, 0);

		const auto * queueHeader = (const Impl::QueueHeader *) queueBuffer.data();

		if (queueHeader->chunkCount == 1) {
			// The entire object fits in a single buffer.
			return unmarshal(queueBuffer);
		} else if (queueHeader->chunkNumber < queueHeader->chunkCount) {
			// The object is split across multiple buffers, potentially interleaved with other buffers.
			return performMultiChunkDequeue(
				queueBuffer, receivedSize, priority
				, [this] (CharVector &,
					      CharVector & queueBuffer,
					      unsigned long & thisReceivedSize,
					      unsigned int & thisPriority,
					      size_t rejected) {
					dequeueNextBuffer(queueBuffer, thisReceivedSize, thisPriority, rejected);
					return true;
				}
			);
		} else {
			// This is a partially dequeued multi-chunk dequeue which was previously placed on the pending queue.
			return completeMultiChunkDequeue(
				queueBuffer, receivedSize, priority
				, [this] (CharVector &,
					      CharVector & queueBuffer,
					      unsigned long & thisReceivedSize,
					      unsigned int & thisPriority,
					      size_t rejected) {
					dequeueNextBuffer(queueBuffer, thisReceivedSize, thisPriority, rejected);
					return true;
				}
			);
		}
	}

	///
	/// Try to dequeue an object.
	///
	/// When the queue is being used with oversize objects (which require multiple
	/// queue receives, this method may only be used via synchronised calls.
	///
	public: T tryDequeue() override {
		return tryDequeue(std::chrono::milliseconds(0));
	}

	///
	/// Try to dequeue an object, waiting the specified time before giving up.
	///
	/// When the queue is being used with oversize objects (which require multiple
	/// queue receives, this method may only be used via synchronised calls.
	///
	public: T tryDequeue(std::chrono::milliseconds waitTime) override {
		CharVector & queueBuffer = Impl::SharedMemoryQueueTLS::storage.queueBuffer;

		unsigned long receivedSize;
		unsigned int priority;

		if (!tryDequeueNextBuffer(queueBuffer, receivedSize, priority, 0, waitTime)) {
			return T();
		}

		const auto * queueHeader = (const Impl::QueueHeader *) queueBuffer.data();

		if (queueHeader->chunkCount == 1) {
			// The entire object fits in a single buffer.
			return unmarshal(queueBuffer);
		} else if (queueHeader->chunkNumber < queueHeader->chunkCount) {
			// The object is split across multiple buffers, potentially interleaved with other buffers.
			return performMultiChunkDequeue(
				queueBuffer, receivedSize, priority
				, [waitTime, this] (CharVector & marshalBuffer,
					                CharVector & queueBuffer,
					                unsigned long & thisReceivedSize,
					                unsigned int & thisPriority,
					                size_t rejected) {
					if (!tryDequeueNextBuffer(queueBuffer, thisReceivedSize, thisPriority, rejected, waitTime)) {
						pendingBuffers.emplace_front(
							PendingBuffer(std::move(marshalBuffer), marshalBuffer.size(), thisPriority)
						);

						marshalBuffer = CharVector();
						return false;
					}

					return true;
				}
			);
		} else {
			// This is a partially dequeued multi-chunk dequeue which was previously placed on the pending queue.
			return completeMultiChunkDequeue(
				queueBuffer, receivedSize, priority
				, [waitTime, this] (CharVector & marshalBuffer,
					                CharVector & queueBuffer,
					                unsigned long & thisReceivedSize,
					                unsigned int & thisPriority,
					                size_t rejected) {
					if (!tryDequeueNextBuffer(queueBuffer, thisReceivedSize, thisPriority, rejected, waitTime)) {
						pendingBuffers.emplace_front(
							PendingBuffer(std::move(queueBuffer), queueBuffer.size(), thisPriority)
						);
						marshalBuffer = CharVector();
						return false;
					}

					return true;
				}
			);
		}
	}

	public: bool full() const override {
		return queue.get_max_msg() - queue.get_num_msg() == 0;
	}

	public: bool empty() const override {
		return queue.get_num_msg() == 0;
	}

	///
	/// Get the name of the queue.
	///
	/// This can be passed to other processes in order to open the queue.
	///
	public: std::string getName() const {
		return name;
	}

	////////////////////////// Private implementation /////////////////////////

	//
	// Shared between the dequeue and tryDequeue methods.
	// The object is split across multiple buffers, potentially interleaved with other buffers.
	// The supplied queue buffer is the first received chunk.
	//
	private: template <typename DequeueFunctionT>
	T performMultiChunkDequeue(CharVector & queueBuffer,
	                           unsigned long & receivedSize,
	                           unsigned int & priority,
	                           DequeueFunctionT dequeueFunction) {
		const auto * queueHeader = (const Impl::QueueHeader *) queueBuffer.data();
		const unsigned int totalBytes = queueHeader->totalBytes;

		CharVector & marshalBuffer = Impl::SharedMemoryQueueTLS::storage.marshalBuffer;
		marshalBuffer.clear();
		marshalBuffer.resize(headerSize + totalBytes);

		// Set up the header in case the marshal buffer ends up on the pending queue (tryDequeue only).
		memcpy(marshalBuffer.data(), queueBuffer.data(), headerSize);
		auto * marshalHeader = (Impl::QueueHeader *) marshalBuffer.data();
		marshalHeader->chunkNumber = marshalHeader->chunkCount;

		size_t rejectedPendingBuffers = 0;
		const unsigned long sequenceNumber = queueHeader->sequenceNumber;
		unsigned int partsLeft = queueHeader->chunkCount;

		while (true) {
			if (queueHeader->sequenceNumber != sequenceNumber) { // Never true on the first iteration.
				++rejectedPendingBuffers;
				pendingBuffers.emplace_back(PendingBuffer(std::move(queueBuffer), receivedSize, priority));
				queueBuffer = CharVector(chunkSize);
			} else {
				// This can occur out of order (if the chunks arrive out of order).
				const size_t copyStart = headerSize + queueHeader->chunkNumber * (chunkSize - headerSize);
				const size_t byteCount = queueBuffer.size() - headerSize;
				memcpy(marshalBuffer.data() + copyStart, queueBuffer.data() + headerSize, byteCount);
				++marshalHeader->chunkNumber;
				--partsLeft;
			}

			if (partsLeft == 0) {
				return unmarshal(marshalBuffer);
			}

			if (!dequeueFunction(marshalBuffer, queueBuffer, receivedSize, priority, rejectedPendingBuffers)) {
				// Only occurs for tryDequeue.
				return T();
			}

			queueHeader = (const Impl::QueueHeader *) queueBuffer.data();
		}
	}

	//
	// Shared between the dequeue and tryDequeue methods.
	// The object is split across multiple buffers, potentially interleaved with other buffers.
	// The supplied marshal buffer is part filled with previously dequeue chunk(s)
	// and already the correct length.
	//
	private:
	template <typename DequeueFunctionT>
	T completeMultiChunkDequeue(CharVector & marshalBuffer,
	                            unsigned long & receivedSize,
	                            unsigned int & priority,
	                            DequeueFunctionT dequeueFunction) {
		auto * marshalHeader = (Impl::QueueHeader *) marshalBuffer.data();
		const unsigned int totalBytes = marshalHeader->totalBytes;

		Assert::assertion(
			  receivedSize == totalBytes + headerSize
			, "Received size of partially dequeued buffer is not the same as the size of the buffer."
		);

		size_t rejectedPendingBuffers = 0;
		const unsigned long sequenceNumber = marshalHeader->sequenceNumber;
		unsigned int partsLeft = 2 * marshalHeader->chunkCount - marshalHeader->chunkNumber;
		CharVector queueBuffer {};

		while (true) {
			if (!dequeueFunction(marshalBuffer, queueBuffer, receivedSize, priority, rejectedPendingBuffers)) {
				// Only occurs for tryDequeue.
				return T();
			}

			auto * queueHeader = (const Impl::QueueHeader *) queueBuffer.data();

			if (sequenceNumber != queueHeader->sequenceNumber) {
				++rejectedPendingBuffers;
				pendingBuffers.emplace_back(PendingBuffer(std::move(queueBuffer), receivedSize, priority));
				queueBuffer = CharVector(chunkSize);
			} else {
				// This can occur out of order (if the chunks arrive out of order).
				const size_t copyStart = headerSize + (chunkSize - headerSize) * queueHeader->chunkNumber;
				const size_t byteCount = queueBuffer.size() - headerSize;
				memcpy(marshalBuffer.data() + copyStart, queueBuffer.data() + headerSize, byteCount);
				++marshalHeader->chunkNumber;
				--partsLeft;
			}

			if (partsLeft == 0) {
				return unmarshal(marshalBuffer);
			}
		}
	}

	//
	// Dequeue next buffer form the pending list if the pending list size is greater
	// than rejectedPendingBuffers, otherwise dequeue from the shared memory queue.
	//
	private: void dequeueNextBuffer(CharVector & buffer,
	                                unsigned long & receivedSize,
	                                unsigned int & priority,
	                                size_t rejectedPendingBuffers) {
		if (rejectedPendingBuffers < pendingBuffers.size()) {
			auto pendingBuffer = pendingBuffers.front();
			buffer = std::move(pendingBuffer.buffer);
			pendingBuffers.pop_front();
			receivedSize = pendingBuffer.receivedSize;
			priority = pendingBuffer.priority;
		} else {
			buffer.resize(chunkSize);
			queue.receive(buffer.data(), buffer.size(), receivedSize, priority);
			buffer.resize(receivedSize);
		}
	}

	//
	// Dequeue next buffer form the pending list if the pending list size is greater
	// than rejectedPendingBuffers, otherwise dequeue from the shared memory queue.
	//
	private: bool tryDequeueNextBuffer(CharVector & buffer,
	                                   unsigned long & receivedSize,
	                                   unsigned int & priority,
	                                   size_t rejectedPendingBuffers,
	                                   std::chrono::milliseconds waitTime) {
		if (rejectedPendingBuffers < pendingBuffers.size()) {
			auto pendingBuffer = pendingBuffers.front();
			buffer = std::move(pendingBuffer.buffer);
			pendingBuffers.pop_front();
			receivedSize = pendingBuffer.receivedSize;
			priority = pendingBuffer.priority;
			return true;
		} else {
			boost::posix_time::ptime timeout(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration w = boost::posix_time::milliseconds(waitTime.count());
			timeout += w;

			buffer.resize(chunkSize);

			if (queue.timed_receive(buffer.data(), buffer.size(), receivedSize, priority, timeout)) {
				buffer.resize(receivedSize);
				return true;
			} else {
				return false;
			}
		}
	}

	private: using SinkDevice   = boost::iostreams::back_insert_device<CharVector>;
	private: using SinkBuffer   = boost::iostreams::stream_buffer<SinkDevice>;
	private: using SourceDevice = boost::iostreams::basic_array_source<char>;
	private: using SourceBuffer = boost::iostreams::stream_buffer<SourceDevice>;

	// Marshal the object to bytes.
	// Assume single queue buffer (add the header in order to avoid later copying).
	private: void marshal(CharVector & buffer, const T & object, const Impl::QueueHeader & header) const {
		buffer.resize(headerSize);
		const char * headerBytes = (const char *) &header;
		memcpy(buffer.data(), headerBytes, headerSize);
		SinkBuffer oStreamBuffer = SinkBuffer(SinkDevice(buffer));
		boost::archive::binary_oarchive archive(oStreamBuffer);
		archive << BoostSerialization(object);
	}

	// Unmarshal the bytes to an object.
	// Skip the initial header bytes before unmarshalling.
	private: T unmarshal(const CharVector & buffer) {
		// Uncomment the following line for debug output.
		//std::cerr << toString(getpid(), " -\n", Util::PrettyPrint::printHexBytes(buffer.data(), buffer.size(), 90, 2), "\n");
		SourceBuffer iStreamBuffer(SourceDevice(buffer.data() + headerSize, buffer.size()));
		boost::archive::binary_iarchive archive(iStreamBuffer);
		T object;
		archive >> BoostSerialization(object);
		return object;
	}

	// The number of buffers required for a message of length totalBytes.
	private: unsigned int calculateChunkCount(unsigned int totalBytes) const {
		const unsigned int netBufferSize = chunkSize - headerSize;
		return totalBytes / netBufferSize + (totalBytes % netBufferSize != 0);
	}

	private: unsigned int calculateDefaultBufferSize() const {
		// Try marshalling a default constructed object to get an indication
		// of the serialised size. Multiply this size by the default buffer
		// size multiplier to get the buffer size.
		Impl::QueueHeader header {};
		T object {};
		CharVector buffer;
		marshal(buffer, object, header);
		return (unsigned int) buffer.size() + minimumChunkSize;
	}

	private: boost::interprocess::message_queue createQueue(unsigned int queueSize, unsigned int bufferSize) {
		if (bufferSize < minimumChunkSize) {
			ThrowBalauException(
				  Exception::SizeException
				, "The supplied shared memory queue buffer is less than the minimum legal size of "
				  + ::toString(minimumChunkSize) + "."
			);
		}

		boost::interprocess::shared_memory_object::remove(name.c_str());

		return boost::interprocess::message_queue(
			CreateOnly, name.c_str(), queueSize, bufferSize
		);
	}

	private: boost::interprocess::message_queue openOrCreateQueue(unsigned int queueSize, unsigned int bufferSize) {
		if (bufferSize < minimumChunkSize) {
			ThrowBalauException(
				  Exception::SizeException
				,  "The supplied shared memory queue buffer is less than the minimum legal size of "
				  + ::toString(minimumChunkSize) + "."
			);
		}

		return boost::interprocess::message_queue(OpenOrCreate, name.c_str(), queueSize, bufferSize);
	}

	private: boost::interprocess::message_queue openQueue() {
		return boost::interprocess::message_queue(OpenOnly, name.c_str());
	}

	friend struct SharedMemoryQueueTest;

	// Shared state across processes using the queue.
	private: struct QueueState {
		std::atomic_uint sequenceNumber;

		explicit QueueState() : sequenceNumber(0) {}
	};

	// Pending buffers are used to support single process dequeueing of oversize objects.
	private: struct PendingBuffer {
		CharVector buffer;
		unsigned long receivedSize;
		unsigned int priority;

		PendingBuffer(CharVector && buffer_, unsigned long receivedSize_, unsigned int priority_)
			: buffer(std::move(buffer_))
			  , receivedSize(receivedSize_)
			  , priority(priority_) {}
	};

	private: const std::string name;
	private: boost::interprocess::message_queue queue;
	private: const unsigned int chunkSize {};
	private: MSharedMemoryObject<QueueState> queueState;
	private: std::deque<PendingBuffer> pendingBuffers;
	private: const bool throwOnOversize;
};

} // namespace Balau::Interprocess

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_INTERPROCESS__SHARED_MEMORY_QUEUE
