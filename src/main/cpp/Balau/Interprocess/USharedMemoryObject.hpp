// @formatter:off
//
// Balau core C++ library
//
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
/// @file USharedMemoryObject.hpp
///
/// A shared memory object that uses the Boost interprocess library.
///

#ifndef COM_BORA_SOFTWARE__BALAU_INTERPROCESS__UNMANAGED_SHARED_MEMORY_OBJECT2
#define COM_BORA_SOFTWARE__BALAU_INTERPROCESS__UNMANAGED_SHARED_MEMORY_OBJECT2

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Interprocess/SharedMemoryUtils.hpp>
#include <Balau/Dev/Assert.hpp>
#include <Balau/Type/UUID.hpp>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

namespace Balau::Interprocess {

///
/// A shared memory object that uses the Boost interprocess library.
///
/// This version uses unmanaged shared memory which requires a call to remap
/// in the child process after forking.
///
/// Note that shared memory is typically allocated in 4k pages, and a whole page
/// will be allocated even if the object size is a single byte.
///
/// This template class encapsulates the calls to the Boost interprocess library
/// in order to create a shared memory object, and provides a simple API to
/// construct/open and use the object.
///
/// Note that when using this class, the type T must have a POD type structure.
/// If the type contains pointers, the objects pointed to will not share and
/// consequently your application will be defective.
///
template <typename T> class USharedMemoryObject {
	///
	/// Create a shared memory object of type T with the supplied input arguments.
	///
	/// An automatically generated name is used.
	///
	public: template <typename ... P> explicit USharedMemoryObject(const P & ... params)
		: USharedMemoryObject(CreateOnlySelector(), "SMO_" + UUID().asString(), params ...) {}

	///
	/// Create a shared memory object of type T with the supplied input arguments.
	///
	/// The specified name is used.
	///
	public: template <typename ... P>
	USharedMemoryObject(boost::interprocess::create_only_t, std::string name_, const P & ... params)
		: name(std::move(name_)) {
		boost::interprocess::shared_memory_object::remove(name.c_str());

		sharedMemoryObject = boost::interprocess::shared_memory_object(
			CreateOnlySelector(), name.c_str(), boost::interprocess::read_write
		);

		sharedMemoryObject.truncate(sizeof(T));
		mappedRegion = boost::interprocess::mapped_region(sharedMemoryObject, boost::interprocess::read_write);
		object = new (mappedRegion.get_address()) T(params ...);
	}

	///
	/// Create or open a shared memory object of type T with the supplied input arguments.
	///
	/// The specified name is used.
	///
	/// If the object already exists, it is opened and the supplied parameters are ignored.
	///
	public: template <typename ... P>
	USharedMemoryObject(boost::interprocess::open_or_create_t, std::string name_, const P & ... params)
		: name(std::move(name_)) {
		boost::interprocess::shared_memory_object::remove(name.c_str());

		boost::interprocess::shared_memory_object shm_obj(
			boost::interprocess::open_or_create, name.c_str(), boost::interprocess::read_write
		);

		boost::interprocess::offset_t size;

		if (shm_obj.get_size(size)) {
			shm_obj.truncate(sizeof(T));
			boost::interprocess::mapped_region region(shm_obj, boost::interprocess::read_write);
			object = new (region.get_address()) T(params ...);
		} else {
			remap();
		}
	}

	///
	/// Open a shared memory object of type T and with the specified name.
	///
	public: USharedMemoryObject(boost::interprocess::open_only_t, std::string name_)
		: name(std::move(name_)) {
		boost::interprocess::shared_memory_object sharedMemoryObject(
			boost::interprocess::open_only, name.c_str(), boost::interprocess::read_write
		);

		remap();
	}

	///
	/// Open read-only a shared memory object of type T and with the specified name.
	///
	public: USharedMemoryObject(boost::interprocess::open_read_only_t, std::string name_)
		: name(std::move(name_)) {
		boost::interprocess::shared_memory_object shm_obj(
			boost::interprocess::open_only, name.c_str(), boost::interprocess::read_only
		);

		remap();
	}

	///
	/// Create a shared memory object by moving from a previously created one.
	///
	public: USharedMemoryObject(USharedMemoryObject && rhs) noexcept
		: name(std::move(rhs.name))
		, sharedMemoryObject(std::move(rhs.sharedMemoryObject))
		, mappedRegion(std::move(rhs.mappedRegion))
		, object(rhs.object) {
		rhs.object = nullptr;
	}

	public: ~USharedMemoryObject() {
		boost::interprocess::shared_memory_object::remove(name.c_str());
	}

	///
	/// Assign a shared memory object by moving from a previously created one.
	///
	public: USharedMemoryObject & operator = (USharedMemoryObject && rhs) noexcept {
		name = std::move(rhs.name);
		sharedMemoryObject = std::move(rhs.sharedMemoryObject);
		mappedRegion = std::move(rhs.mappedRegion);
		object = rhs.object;
		rhs.object = nullptr;
		return *this;
	}

	public: void remap() {
		mappedRegion = boost::interprocess::mapped_region(sharedMemoryObject, boost::interprocess::read_write);
		object = (T *) mappedRegion.get_address();
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

	////////////////////////// Private implementation /////////////////////////

	private: std::string name;
	private: boost::interprocess::shared_memory_object sharedMemoryObject;
	private: boost::interprocess::mapped_region mappedRegion;
	protected: T * object = nullptr;
};

} // namespace Balau::Interprocess

#endif // COM_BORA_SOFTWARE__BALAU_INTERPROCESS__UNMANAGED_SHARED_MEMORY_OBJECT2
