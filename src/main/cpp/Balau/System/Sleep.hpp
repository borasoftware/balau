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
/// @file Sleep.hpp
///
/// %Sleep utilities.
///

#ifndef COM_BORA_SOFTWARE__BALAU_SYSTEM__SLEEP
#define COM_BORA_SOFTWARE__BALAU_SYSTEM__SLEEP

#include <chrono>
#include <thread>

namespace Balau::System {

///
/// %Sleep utilities.
///
class Sleep final {
	///
	/// %Sleep for the indicated number of seconds.
	///
	public: static void sleep(size_t s) {
		std::this_thread::sleep_for(std::chrono::seconds(s));
	}

	///
	/// %Sleep for the indicated number of milli-seconds.
	///
	public: static void milliSleep(size_t ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	///
	/// %Sleep for the indicated number of micro-seconds.
	///
	public: static void microSleep(size_t us) {
		std::this_thread::sleep_for(std::chrono::microseconds(us));
	}

	///
	/// %Sleep for the indicated number of nano-seconds.
	///
	public: static void nanoSleep(size_t ns) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(ns));
	}

	///////////////////////////////////////////////////////////////////////////

	Sleep() = delete;
	Sleep(const Sleep &) = delete;
	Sleep & operator = (const Sleep &) = delete;
};

} // namespace Balau::System

#endif // COM_BORA_SOFTWARE__BALAU_SYSTEM__SLEEP
