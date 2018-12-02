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
/// @file Queue.hpp
///
/// Base interface for queues.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__QUEUE
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__QUEUE

namespace Balau::Container {

///
/// Base interface for queues.
///
/// @tparam T the element type (must be move constructable and assignable)
///
template <typename T> class Queue {
	///
	/// Enqueue an object, moving the supplied element.
	///
	public: virtual void enqueue(T && element) = 0;

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

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__QUEUE
