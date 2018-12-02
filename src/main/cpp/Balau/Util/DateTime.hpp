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
/// @file DateTime.hpp
///
/// Date and time utilities.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__DATE_TIME
#define COM_BORA_SOFTWARE__BALAU_UTIL__DATE_TIME

#include <Balau/Type/ToString.hpp>
#include <Balau/System/Clock.hpp>
#include <Balau/ThirdParty/Date/date.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

namespace Balau::Util {

///
/// Date and time utilities simplified.
///
struct DateTime final {
	///
	/// Format the time point as a string with the specified format.
	///
	/// Refer to the formatting documentation at https://howardhinnant.github.io/date/date.html
	/// for details on the format placeholders.
	///
	/// Example format: "%Y-%m-%d %H:%M:%S"
	///
	template <typename Clock, typename Dur>
	static std::string toString(const char * format, const std::chrono::time_point<Clock, Dur> & tp) {
		std::ostringstream str;
		Date::to_stream(str, format, tp);
		return str.str();
	}

	///
	/// Format the time point as a string with the specified format into the supplied output stream.
	///
	/// Refer to the formatting documentation at https://howardhinnant.github.io/date/date.html
	/// for details on the format placeholders.
	///
	/// Example format: "%Y-%m-%d %H:%M:%S"
	///
	template <typename Clock, typename Dur>
	static void toString(std::ostream & stream, const char * format, const std::chrono::time_point<Clock, Dur> & tp) {
		Date::to_stream(stream, format, tp);
	}

	///
	/// Format the duration as a string with the specified format.
	///
	/// Refer to the formatting documentation at https://howardhinnant.github.io/date/date.html
	/// for details on the format placeholders.
	///
	/// Example format: "%Y-%m-%d %H:%M:%S"
	///
	template <typename Rep, typename Period>
	static std::string toString(const char * format, const std::chrono::duration<Rep, Period> & d) {
		std::ostringstream str;

		// TODO??
		// The HH date library appears to do an abs(d), so
		Date::to_stream(str, format, d);
		return str.str();
	}

	///
	/// Format the duration as a string with the specified format into the supplied output stream.
	///
	/// Refer to the formatting documentation at https://howardhinnant.github.io/date/date.html
	/// for details on the format placeholders.
	///
	/// Example format: "%Y-%m-%d %H:%M:%S"
	///
	template <typename Rep, typename Period>
	static void toString(std::ostream & stream, const char * format, const std::chrono::duration<Rep, Period> & d) {
		Date::to_stream(stream, format, d);
	}

	///
	/// Create a duration from the supplied string which is in the specified format.
	///
	/// Refer to the formatting documentation at https://howardhinnant.github.io/date/date.html
	/// for details on the format placeholders.
	///
	/// Example format: "%Y-%m-%d %H:%M:%S"
	///
	template <typename Rep, typename Period>
	static std::chrono::duration<Rep, Period> toDuration(const char * format, const std::string & s) {
		std::istringstream str(s);
		std::chrono::duration<Rep, Period> ret {};
		Date::from_stream(str, format, ret);
		return ret;
	}

	///////////////////////////////////////////////////////////////////////////

	DateTime() = delete;
	DateTime(const DateTime & copy) = delete;
	DateTime & operator = (const DateTime & copy) = delete;
};

} // namespace Balau::Util

////////////////////// String functions for chrono types //////////////////////

///
/// ToString method for duration.
///
template<typename Rep, typename Period>
inline std::string toString(const std::chrono::duration<Rep, Period> & d) {
	using namespace Balau::Date;
	std::ostringstream s;
	s << d;
	return s.str();
}

///
/// ToString method for time point.
///
template<typename Clock, typename Dur>
inline std::string toString(const std::chrono::time_point<Clock, Dur> & tp) {
	using namespace Balau::Date;
	std::ostringstream s;
	s << tp;
	return s.str();
}

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__DATE_TIME
