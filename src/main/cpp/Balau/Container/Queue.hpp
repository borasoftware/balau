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
/// @file Queue.hpp
///
/// Base interface for queues.
///

#ifndef COM_BORA_SOFTWARE_BALAU_CONTAINER_QUEUE
#define COM_BORA_SOFTWARE_BALAU_CONTAINER_QUEUE

namespace Balau::Container {

///
/// Base interface for queues.
///
/// @tparam T the object type
///
template <typename T> class Queue {
	///
	/// Enqueue an object.
	///
	public: virtual void enqueue(T object) = 0;

	///
	/// Dequeue an object.
	///
	/// Depending on the implementation, this operation will either block or throw
	/// EmptyException when no elements are available for dequeueing.
	///
	public: virtual T dequeue() = 0;

	///
	/// Returns true if the queue is empty.
	///
	public: virtual bool empty() const = 0;
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE_BALAU_CONTAINER_QUEUE
