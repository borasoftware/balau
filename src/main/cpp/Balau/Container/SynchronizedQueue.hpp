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
/// @file SynchronizedQueue.hpp
///
/// A queue that uses a mutex to synchronise enqueue and dequeue calls.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__SYNCHRONISED_QUEUE
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__SYNCHRONISED_QUEUE

#include <Balau/Exception/ContainerExceptions.hpp>
#include <Balau/Container/Queue.hpp>
#include <Balau/Type/StdTypes.hpp>

#include <list>
#include <mutex>

namespace Balau::Container {

///
/// A queue that uses a mutex to synchronise enqueue and dequeue calls.
///
/// The underlying container is an std::list.
///
/// @tparam T the element type (must be default constructable in
///            addition being move constructable and assignable)
///
template <typename T> class SynchronizedQueue : public Queue<T> {
	public: void enqueue(T element) override {
		std::lock_guard<std::mutex> lock(mutex);
		elements.push_back(std::move(element));
	}

	///
	/// Dequeue an object.
	///
	/// @throw EmptyException if no elements are available for dequeueing
	///
	public: T dequeue() override {
		std::lock_guard<std::mutex> lock(mutex);

		if (elements.empty()) {
			ThrowBalauException(Exception::EmptyException, "Synchronised queue is empty.");
		}

		T element = std::move(elements.front());
		elements.pop_front();
		return element;
	}

	public: bool empty() const override {
		std::lock_guard<std::mutex> lock(mutex);
		return elements.size() == 0;
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::list<T> elements;
	private: mutable std::mutex mutex;
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__SYNCHRONISED_QUEUE
