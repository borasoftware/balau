// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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
/// @file MSharedMemoryObject.hpp
///
/// A shared memory object that uses the Boost interprocess library.
///

#ifndef COM_BORA_SOFTWARE__BALAU_INTERPROCESS__MANAGED_SHARED_MEMORY_OBJECT
#define COM_BORA_SOFTWARE__BALAU_INTERPROCESS__MANAGED_SHARED_MEMORY_OBJECT

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Interprocess/SharedMemoryUtils.hpp>
#include <Balau/Dev/Assert.hpp>
#include <Balau/Type/UUID.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>

namespace Balau::Interprocess {

///
/// A shared memory object that uses the Boost interprocess library.
///
/// This version uses managed shared memory which does not require a call to
/// remap in the child process after forking.
///
/// Note that shared memory is typically allocated in 4k pages, and a whole page
/// will be allocated even if the object size is a single byte.
///
/// Note that the management state requires 512 bytes of the 4k page,
/// so the maximum object size is 3.5KB before a second page is used.
///
/// This template class encapsulates the calls to the Boost interprocess library
/// in order to create a shared memory object, and provides a simple API to
/// construct/open and use the object.
///
/// Note that when using this class, the type T must have a POD type structure.
/// If the type contains pointers, the objects pointed to will not share and
/// consequently your application will be defective.
///
template <typename T> class MSharedMemoryObject {
	///
	/// Create a shared memory object of type T with the supplied input arguments
	///
	/// The name prefix is automatically generated.
	///
	public: template <typename ... P> explicit MSharedMemoryObject(const P & ... params)
		: MSharedMemoryObject(CreateOnly, "SMO_" + UUID().asString(), params ...) {}

	///
	/// Create a shared memory object of type T with the supplied input arguments.
	///
	/// The specified name prefix is used.
	///
	public: template <typename ... P>
	MSharedMemoryObject(CreateOnlySelector, const std::string & name, const P & ... params) {
		const std::string memoryName = name + "_memory";

		boost::interprocess::shared_memory_object::remove(memoryName.c_str());

		segment = boost::interprocess::managed_shared_memory(
			CreateOnly, memoryName.c_str(), metadataOverhead + sizeof(T)
		);

		object = segment.construct<T>((name + "_object").c_str())(params ...);
	}

	///
	/// Create or open a shared memory object of type T with the supplied input arguments.
	///
	/// The specified name prefix is used.
	///
	/// If the object already exists, it is opened and the supplied parameters are ignored.
	///
	public: template <typename ... P>
	MSharedMemoryObject(OpenOrCreateSelector, const std::string & name, const P & ... params) {
		const std::string memoryName = name + "_memory";

		boost::interprocess::shared_memory_object::remove(memoryName.c_str());

		segment = boost::interprocess::managed_shared_memory(
			boost::interprocess::open_or_create, memoryName.c_str(), metadataOverhead + sizeof(T)
		);

		object = segment.construct<T>((name + "_object").c_str())(params ...);
	}

	///
	/// Open a shared memory object of type T.
	///
	/// The specified name prefix is used.
	///
	/// @throw NotFoundException if no such shared memory object exists
	///
	public: MSharedMemoryObject(OpenOnlySelector, const std::string & name) {
		const std::string memoryName = name + "_memory";
		segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_only, memoryName.c_str());
		auto item = segment.find<T>((name + "_object").c_str());

		if (item.first == nullptr) {
			ThrowBalauException(
				Exception::NotFoundException, "The shared memory object with name prefix " + name + " was not found."
			);
		}

		object = item.first;
	}

	///
	/// Open read-only a shared memory object of type T.
	///
	/// The specified name prefix is used.
	///
	/// @throw NotFoundException if no such shared memory object exists
	///
	public: MSharedMemoryObject(OpenReadOnlySelector, const std::string & name) {
		const std::string memoryName = name + "_memory";
		segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_read_only, memoryName.c_str());
		auto item = segment.find<T>((name + "_object").c_str());

		if (item.first == nullptr) {
			ThrowBalauException(
				Exception::NotFoundException, "The shared memory object with name prefix " + name + " was not found."
			);
		}

		object = item.first;
	}

	///
	/// Destroy the shared memory object.
	///
	public: ~MSharedMemoryObject() {
		segment.destroy<T>((name + "_memory").c_str());
		boost::interprocess::shared_memory_object::remove((name + "_object").c_str());
	}

	///
	/// Create a shared memory object by moving from a previously created one.
	///
	public: MSharedMemoryObject(MSharedMemoryObject && rhs) noexcept
		: name(std::move(rhs.name))
		, segment(std::move(rhs.segment))
		, object(rhs.object) {
		rhs.object = nullptr;
	}

	///
	/// Assign a shared memory object by moving from a previously created one.
	///
	public: MSharedMemoryObject & operator = (MSharedMemoryObject && rhs) noexcept {
		name = std::move(rhs.name);
		segment = std::move(rhs.segment);
		object = rhs.object;
		rhs.object = nullptr;
		return *this;
	}

	///
	/// Get the shared memory object pointer.
	///
	public: T * operator -> () {
		Assert::assertion(object != nullptr, "Attempt to dereference null object in shared memory object.");
		return object;
	}

	///
	/// Get the shared memory object pointer.
	///
	public: const T * operator -> () const {
		Assert::assertion(object != nullptr, "Attempt to dereference null object in shared memory object.");
		return object;
	}

	///
	/// Get a reference to the shared memory object.
	///
	public: T & operator * () {
		Assert::assertion(object != nullptr, "Attempt to dereference null object in shared memory object.");
		return *object;
	}

	///
	/// Get a const reference to the shared memory object.
	///
	public: const T & operator * () const {
		Assert::assertion(object != nullptr, "Attempt to dereference null object in shared memory object.");
		return *object;
	}

	///////////////////////// Private implementation //////////////////////////

	// Reserved memory for metadata/index used in the allocated block.
	private: static const size_t metadataOverhead = 512;

	private: std::string name;
	private: boost::interprocess::managed_shared_memory segment;
	private: T * object = nullptr;
};

} // namespace Balau::Interprocess

#endif // COM_BORA_SOFTWARE__BALAU_INTERPROCESS__MANAGED_SHARED_MEMORY_OBJECT
