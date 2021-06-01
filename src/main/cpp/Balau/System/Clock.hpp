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
/// @file Clock.hpp
///
/// Base interface of clocks.
///

#ifndef COM_BORA_SOFTWARE__BALAU_SYSTEM__CLOCK
#define COM_BORA_SOFTWARE__BALAU_SYSTEM__CLOCK

#include <Balau/ThirdParty/Date/date.hpp>

namespace std { // NOLINT

namespace chrono {

// Missing typedefs.
typedef duration<int64_t, centi> centiseconds;
typedef duration<int64_t, deci>  deciseconds;

}

}

namespace Balau::System {

///
/// Base interface of clocks.
///
/// This interface is both a convenient API for clock functions, but is more
/// importantly a way to inject a test clock that can be manipulated from within
/// test methods in order to simulate changes in time.
///
/// The clock API uses both std::chrono and the HH date library.
///
/// @todo Add more clock functionality.
///
class Clock {
	public: using TimePoint = std::chrono::system_clock::time_point;

	///
	/// Get the current time point.
	///
	public: virtual std::chrono::system_clock::time_point now() const = 0;

	///
	/// Get the current date.
	///
	public: virtual Date::year_month_day today() const = 0;

	///
	/// Get the current time in nanoseconds since the unix epoch.
	///
	public: virtual std::chrono::nanoseconds nanotime() const = 0;

	///
	/// Get the current time in microseconds since the unix epoch.
	///
	public: virtual std::chrono::microseconds microtime() const = 0;

	///
	/// Get the current time in milliseconds since the unix epoch.
	///
	public: virtual std::chrono::milliseconds millitime() const = 0;

	///
	/// Get the current time in centiseconds since the unix epoch.
	///
	public: virtual std::chrono::centiseconds centitime() const = 0;

	///
	/// Get the current time in deciseconds since the unix epoch.
	///
	public: virtual std::chrono::deciseconds decitime() const = 0;

	public: virtual ~Clock() = default;
};

} // namespace Balau::System

#endif // COM_BORA_SOFTWARE__BALAU_SYSTEM__CLOCK
