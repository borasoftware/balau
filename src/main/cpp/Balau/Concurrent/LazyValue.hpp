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
/// @file LazyValue.hpp
///
/// Lazy setting of a value.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONCURRENT__LAZY_VALUE
#define COM_BORA_SOFTWARE__BALAU_CONCURRENT__LAZY_VALUE

#include <atomic>
#include <functional>
#include <mutex>

namespace Balau::Concurrent {

///
/// Member variable style lazy setting of a value.
///
/// This class can be used instead of static local variable style singleton
/// initialisation when the variable needs to be a instance member of a class.
/// If this is not the case, use static local variable style initialisation
/// instead.
///
/// @tparam T the variable type (must be default and copy/move constructable)
///
template <typename T> class LazyValue {
	///
	/// Create a lazy value.
	///
	/// @param destruct_ function called in LazyValue's destructor if the construct function was previously called
	///
	public: explicit LazyValue(std::function<void (T &)> destruct_ = std::function<void (T &)>())
		: destruct(destruct_)
		, constructed(false) {}

	///
	/// Destroy the lazy value instance.
	///
	public: ~LazyValue() {
		std::lock_guard<std::mutex> lock(mutex);

		if (constructed && destruct) {
			destruct(value);
		}
	}

	///
	/// Get the value, constructing it by calling the supplied function if necessary.
	///
	/// @return the value
	///
	public: T & operator () (std::function<T ()> construct) {
		if (!constructed.load(std::memory_order_acquire)) {
			std::lock_guard<std::mutex> lock(mutex);

			if (!constructed.load(std::memory_order_relaxed)) {
				value = construct();
				constructed.store(true, std::memory_order_release);
			}
		}

		return value;
	}

	///
	/// Returns true if the value has been constructed.
	///
	/// If the operator () is being concurrently run in other threads whilst this
	/// method is called, this method will not necessarily reflect any objective
	/// reality to whether the value has been constructed yet. This method should
	/// thus only be used when there is no possibility of the operator () being
	/// run in other threads, otherwise it will return a meaningless result.
	///
	/// @return true if the value has been constructed
	///
	public: bool isConstructed() const {
		return constructed;
	}

	///
	/// Get the internal mutex in order to synchronise on external code.
	///
	/// Use with great care.
	///
	/// @return the internal mutex
	///
	public: std::mutex & getMutex() {
		return mutex;
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::function<void (T &)> destruct;
	private: std::atomic_bool constructed;
	private: std::mutex mutex;
	private: T value;
};

} // namespace Balau::Concurrent

#endif // COM_BORA_SOFTWARE__BALAU_CONCURRENT__LAZY_VALUE
