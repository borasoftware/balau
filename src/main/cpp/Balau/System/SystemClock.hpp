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
/// @file SystemClock.hpp
///
/// The default implementation of the clock API.
///

#ifndef COM_BORA_SOFTWARE__BALAU_SYSTEM__SYSTEM_CLOCK
#define COM_BORA_SOFTWARE__BALAU_SYSTEM__SYSTEM_CLOCK

#include <Balau/Application/Injectable.hpp>
#include <Balau/System/Clock.hpp>

namespace Balau::System {

///
/// The default implementation of the clock API.
///
/// This is the implementation that should be used in production.
///
class SystemClock : public Clock {
	BalauInject(SystemClock)

	public: std::chrono::system_clock::time_point now() const override {
		return std::chrono::system_clock::now();
	}

	public: date::year_month_day today() const override {
		return date::year_month_day {
			date::floor<date::days>(std::chrono::system_clock::now())
		};
	}

	public: std::chrono::nanoseconds nanotime() const override {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		);
	}

	public: std::chrono::milliseconds millitime() const override {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		);
	}

	public: std::chrono::centiseconds centitime() const override {
		return std::chrono::duration_cast<std::chrono::centiseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		);
	}

	public: std::chrono::deciseconds decitime() const override {
		return std::chrono::duration_cast<std::chrono::deciseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		);
	}
};

} // namespace Balau::System

#endif // COM_BORA_SOFTWARE__BALAU_SYSTEM__SYSTEM_CLOCK
