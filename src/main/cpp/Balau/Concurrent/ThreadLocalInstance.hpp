// @formatter:off
//
// Balau core C++ library
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
/// @file ThreadLocalInstance.hpp
///
/// A thread local instance that can be used as a class member instance variable.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONCURRENT__THREAD_LOCAL_INSTANCE
#define COM_BORA_SOFTWARE__BALAU_CONCURRENT__THREAD_LOCAL_INSTANCE

#ifdef __linux__
	#include <pthread.h>
#else
	#error "Platform not yet supported."
#endif

namespace Balau::Concurrent {

///
/// A thread local instance that can be used as a class member instance variable.
///
template <typename T, typename Enable = void> class ThreadLocalInstance;

template <typename T>
class ThreadLocalInstance<T, typename std::enable_if<std::is_default_constructible<T>::value>::type> {
	///
	/// Create a thread local storage object, specifying the object construction and destruction functions.
	///
	/// @param construct_ the function that will be called to construct a new thread local object
	/// @param destruct the function that will be called to destruct an existing thread local object
	///
	public: ThreadLocalInstance() noexcept {
		pthread_key_create(&key, (void (*) (void *)) destroyObject);
	}

	///
	/// Get the thread local instance, instantiating it with the default constructor if necessary.
	///
	public: T & operator () () {
		void * ptr = pthread_getspecific(key);

		if (!ptr) {
			pthread_setspecific(key, new T);
			ptr = pthread_getspecific(key);
		}

		return *reinterpret_cast<T *>(ptr);
	}

	///
	/// Get the thread local instance, instantiating it with the supplied function if necessary.
	///
	public: T & operator () (std::function<T * ()> instantiate) {
		void * ptr = pthread_getspecific(key);

		if (!ptr) {
			pthread_setspecific(key, instantiate());
			ptr = pthread_getspecific(key);
		}

		return *reinterpret_cast<T *>(ptr);
	}

	///////////////////////// Private implementation //////////////////////////

	private: static void destroyObject(void * object) {
		delete (T *) object;
	}

	private: pthread_key_t key {};
};

template <typename T>
class ThreadLocalInstance<T, std::negation<typename std::enable_if<std::is_default_constructible<T>::value>::type>> {
	///
	/// Create a thread local storage object, specifying the object construction and destruction functions.
	///
	/// @param construct_ the function that will be called to construct a new thread local object
	/// @param destruct the function that will be called to destruct an existing thread local object
	///
	public: ThreadLocalInstance() noexcept {
		pthread_key_create(&key, (void (*) (void *)) destroyObject);
	}

	///
	/// Get the thread local instance, instantiating it with the supplied function if necessary.
	///
	public: T & operator () (std::function<T * ()> instantiate) {
		void * ptr = pthread_getspecific(key);

		if (!ptr) {
			pthread_setspecific(key, instantiate());
			ptr = pthread_getspecific(key);
		}

		return *reinterpret_cast<T *>(ptr);
	}

	///////////////////////// Private implementation //////////////////////////

	private: static void destroyObject(void * object) {
		delete (T *) object;
	}

	private: pthread_key_t key {};
};

} // namespace Balau::Concurrent

#endif // COM_BORA_SOFTWARE__BALAU_CONCURRENT__THREAD_LOCAL_INSTANCE
