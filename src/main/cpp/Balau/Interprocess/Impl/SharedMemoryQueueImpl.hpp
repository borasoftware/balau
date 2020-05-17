// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_INTERPROCESS_IMPL__SHARED_MEMORY_QUEUE_IMPL
#define COM_BORA_SOFTWARE__BALAU_INTERPROCESS_IMPL__SHARED_MEMORY_QUEUE_IMPL

#include <vector>

namespace Balau::Interprocess {

template <typename T> class SharedMemoryQueue;

} // namespace Balau::Interprocess

namespace Balau::Impl {

//
// Header of all shared memory queue buffers.
//
// This structure is never instantiated. Instead, the buffer is cast to a pointer
// to this structure and the header data is then read/written.
//
struct QueueHeader {
	//
	// The sequence number identifies multiple chunks of a multi-chunk message.
	// The sequence number will wrap after 4e9 enqueues.
	//
	unsigned int sequenceNumber;

	//
	// The chunk count is the number of chunks in a multi-chunk message.
	//
	unsigned int chunkCount;

	//
	// The chunk number is either:
	//
	//  - the zero indexed nth chunk index for chunks; or
	//
	//  - chunkCount + the number of chunks already added to the marshal buffer (which was
	//    previously placed onto the pending queue because no more chunks were available).
	//
	unsigned int chunkNumber;

	//
	// The total number of bytes in the message, not including the header(s).
	//
	unsigned int totalBytes;
};

static_assert(sizeof(QueueHeader) == 16, "QueueHeader size is not 16 bytes");

// Thread-local buffers used for marshalling and unmarshalling when a queue
// has been configured to use thread-local storage.
class SharedMemoryQueueTLS {
	std::vector<char> marshalBuffer;
	std::vector<char> queueBuffer;

	template <typename T> friend class ::Balau::Interprocess::SharedMemoryQueue;

	static thread_local SharedMemoryQueueTLS storage;
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_INTERPROCESS_IMPL__SHARED_MEMORY_QUEUE_IMPL
