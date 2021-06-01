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
/// @tparam T the type of objects placed into the queue
///
template <typename T> class BlockingQueue : public Queue<T> {
	///
	/// Enqueue an object, waiting for space to be available if the queue is full.
	///
	/// @param object the object to move into the queue
	///
	public: virtual void enqueue(T object) = 0;

	///
	/// Enqueue an object if the queue is not full.
	///
	/// @param object the object to move into the queue
	/// @return true if the object was enqueued, false otherwise
	///
	public: virtual bool tryEnqueue(T object) = 0;

	///
	/// Enqueue an object, waiting a limited amount of time for space to be available if the queue is full.
	///
	/// @param object the object to move into the queue
	/// @param waitTime the number of milliseconds to wait if the queue is full
	/// @return true if the object was enqueued, false otherwise
	///
	public: virtual bool tryEnqueue(T object, std::chrono::milliseconds waitTime) = 0;

	///
	/// Dequeue an object, waiting for an object to become available if the queue is empty.
	///
	/// @return the dequeued object
	///
	public: virtual T dequeue() = 0;

	///
	/// Try to dequeue an object.
	///
	/// if no dequeue was made, a default constructed object is returned.
	///
	/// @return the dequeued object or a default constructed object if no object was dequeued
	///
	public: virtual T tryDequeue() = 0;

	///
	/// Try to dequeue an object.
	///
	/// if no dequeue was made, a default constructed object is returned.
	///
	/// @param success a reference to a boolean that is set to true on success and false otherwise
	/// @return the dequeued object or a default constructed object if no object was dequeued
	///
	public: virtual T tryDequeue(bool & success) = 0;

	///
	/// Try to dequeue an object, waiting for the specified time if the queue is empty.
	///
	/// if no dequeue was made, a default constructed object is returned.
	///
	/// @param waitTime the time in milliseconds to wait for an object to become available
	/// @return the dequeued object or a default constructed object if no object was dequeued
	///
	public: virtual T tryDequeue(std::chrono::milliseconds waitTime) = 0;

	///
	/// Try to dequeue an object, waiting for the specified time if the queue is empty.
	///
	/// if no dequeue was made, a default constructed object is returned.
	///
	/// @param waitTime the time in milliseconds to wait for an object to become available
	/// @param success a reference to a boolean that is set to true on success and false otherwise
	/// @return the dequeued object or a default constructed object if no object was dequeued
	///
	public: virtual T tryDequeue(std::chrono::milliseconds waitTime, bool & success) = 0;

	///
	/// Returns true if the queue is full.
	///
	/// Given the concurrent nature of block queues, this operation is approximate.
	///
	/// @return true if the queue was full at the point of time it was checked
	///
	public: virtual bool full() const  = 0;

	///
	/// Returns true if the queue is empty.
	///
	/// Given the concurrent nature of block queues, this operation is approximate.
	///
	/// @return true if the queue was empty at the point of time it was checked
	///
	public: virtual bool empty() const  = 0;
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__BLOCKING_QUEUE
