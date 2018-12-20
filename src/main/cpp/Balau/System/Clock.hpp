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
