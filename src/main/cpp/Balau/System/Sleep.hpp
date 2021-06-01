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
