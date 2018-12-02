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
/// @file BlockingQueue.hpp
///
/// Base interface for blocking queues.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__BLOCKING_QUEUE
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__BLOCKING_QUEUE

#include <Balau/Container/Queue.hpp>

#include <chrono>

namespace Balau::Container {

///
/// Base interface for blocking queues.
///
template <typename T> class BlockingQueue : public Queue<T> {
	///
	/// Enqueue an object, waiting for space to be available if the queue is full.
	///
	/// The supplied element is moved during enqueuing.
	///
	public: virtual void enqueue(T && element) = 0;

	///
	/// Dequeue an object, waiting for an object to become available if the queue is empty.
	///
	public: virtual T dequeue() = 0;

	///
	/// Try to dequeue an object.
	///
	/// if no dequeue was made, a default constructed object is returned.
	///
	public: virtual T tryDequeue() = 0;

	///
	/// Try to dequeue an object, waiting for the specified time if the queue is empty.
	///
	/// if no dequeue was made, a default constructed object is returned.
	///
	public: virtual T tryDequeue(std::chrono::milliseconds waitTime) = 0;

	///
	/// Returns true if the queue is full.
	///
	/// Given the concurrent nature of block queues, this operation is approximate.
	///
	public: virtual bool full() const  = 0;

	///
	/// Returns true if the queue is empty.
	///
	/// Given the concurrent nature of block queues, this operation is approximate.
	///
	public: virtual bool empty() const  = 0;
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__BLOCKING_QUEUE
