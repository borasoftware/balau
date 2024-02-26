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
/// @file PrettyPrint.hpp
///
/// Utilities for printing numeric values in different formats.
///

#ifndef COM_BORA_SOFTWARE_BALAU_UTIL_PRETTY_PRINT
#define COM_BORA_SOFTWARE_BALAU_UTIL_PRETTY_PRINT

#include <Balau/Exception/ContainerExceptions.hpp>
#include <Balau/Util/DateTime.hpp>

namespace Balau::Util {

///
/// Utilities for printing numeric values in different formats.
///
struct PrettyPrint final {
	///
	/// Print the value in fixed notation.
	///
	/// @tparam T the numeric type
	/// @param value the value to pretty print
	/// @param decimalPlaces the number of decimal places to print
	///
	template <typename T> static std::string fixed(const T & value, const size_t decimalPlaces) {
		std::ostringstream builder;
		builder.precision(decimalPlaces);
		builder << std::fixed << value;
		return builder.str();
	}

	///
	/// Print the value in scientific notation.
	///
	/// @tparam T the numeric type
	/// @param value the value to pretty print
	/// @param decimalPlaces the number of decimal places to print
	///
	template <typename T> static std::string scientific(const T & value, const size_t decimalPlaces) {
		std::ostringstream builder;
		builder.precision(decimalPlaces);
		builder << std::scientific << value;
		return builder.str();
	}

	///
	/// Print the value with a metric prefix.
	///
	/// @tparam T the numeric type - must be castable to a long double
	/// @param value the value to pretty print
	/// @param decimalPlaces the number of decimal places to print
	///
	template <typename T> static std::string metricPrefix(const T & value, size_t decimalPlaces) {
		auto prefix = (int) std::floor(std::log10(value) / std::log10(1000.0));

		if (prefix > 6) {
			prefix = 6;
		} else if (prefix < -6) {
			prefix = -6;
		}

		const char * prefixChar = prefixes()[prefix + 6];
		const long double multiplier = std::pow(1000.0, (long double) -prefix);
		return fixed((long double) value * multiplier, decimalPlaces) + prefixChar;
	}

	///
	/// Print the value with a binary prefix.
	///
	/// This is similar to a metric prefix, but uses 2^10 (1024) as the divisor.
	///
	/// @tparam T the numeric type: must be castable to a double
	/// @param value the value to pretty print
	/// @param decimalPlaces the number of decimal places to print
	///
	template <typename T> static std::string binaryPrefix(const T & value, const size_t decimalPlaces) {
		auto prefix = (int) std::floor(std::log10((long double) value) / std::log10((long double) 1024.0));

		if (prefix > 6) {
			prefix = 6;
		} else if (prefix < -6) {
			prefix = -6;
		}

		const char * prefixChar = prefixes()[prefix + 6];
		const long double multiplier = std::pow(1024.0, (long double) -prefix);
		return fixed((long double) value * multiplier, decimalPlaces) + prefixChar;
	}

	///
	/// Returns a string containing the supplied byte value in terms of B/KB/MB/GB etc.
	///
	static std::string byteValue(const size_t bytes, const size_t decimalPlaces = 0) {
		return binaryPrefix(bytes, decimalPlaces) + "B";
	}

	///
	/// Pretty print the duration.
	///
	/// @param d the duration
	/// @param decimalPlaces the number of decimal places to print
	///
	template <typename Rep, typename Period> static
	std::string duration(std::chrono::duration<Rep, Period> d, const size_t decimalPlaces = 0) {
		using Days = std::chrono::duration<int, std::ratio<86400>>;

		long double totalSeconds = (long double) d.count() / (long double) Period::den * (long double) Period::num;

		std::ostringstream builder;

		if (totalSeconds < 0.0) {
			builder << "-";
			totalSeconds = -totalSeconds;
		}

		const long double fraction = totalSeconds - (long double) (unsigned long long) totalSeconds;

		if (totalSeconds == 0.0) {
			return "0s";
		} else if (totalSeconds < 60.0) {
			builder << metricPrefix(totalSeconds, decimalPlaces) <<  "s";
			return builder.str();
		} else if (totalSeconds < 60.0 * 60.0) { // print minutes::seconds.fraction
			builder.fill('0');
			auto minutes = std::chrono::duration_cast<std::chrono::minutes>(d);
			d -= minutes;
			auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
			builder << std::setw(2) << minutes.count() << "m"
			        << std::setw(2) << seconds.count();
		} else if (totalSeconds < 60.0 * 60.0 * 24.0) { // print hours::minutes::seconds.fraction
			builder.fill('0');
			auto hours = std::chrono::duration_cast<std::chrono::hours>(d);
			d -= hours;
			auto minutes = std::chrono::duration_cast<std::chrono::minutes>(d);
			d -= minutes;
			auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
			builder << std::setw(2) <<   hours.count() << "h"
			        << std::setw(2) << minutes.count() << "m"
			        << std::setw(2) << seconds.count();
		} else { // print days::hours::minutes::seconds.fraction
			builder.fill('0');
			auto days = std::chrono::duration_cast<Days>(d);
			d -= days;
			auto hours = std::chrono::duration_cast<std::chrono::hours>(d);
			d -= hours;
			auto minutes = std::chrono::duration_cast<std::chrono::minutes>(d);
			d -= minutes;
			auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
			builder << std::setw(2) <<    days.count() << "d"
			        << std::setw(2) <<   hours.count() << "h"
			        << std::setw(2) << minutes.count() << "m"
			        << std::setw(2) << seconds.count();
		}

		if (decimalPlaces != 0 && fraction != 0) {
			builder << fixed(fraction, decimalPlaces).substr(1);
		}

		builder << "s";

		return builder.str();
	}

	///
	/// Print the value as bytes in hexadecimal.
	///
	/// @tparam T the numeric type
	/// @param value the value to print
	/// @param lineLength the maximum length length to print (0 == single line)
	/// @param groupSize the number of hex byte values to group together (0 == no groups)
	///
	template <typename T>
	static std::string printHexBytes(const T & value, size_t lineLength = 0, size_t groupSize = 0) {
		return printHexBytes((const char *) & value, sizeof(T), lineLength, groupSize);
	}

	///
	/// Print the bytes in hexadecimal.
	///
	/// @param ptr the pointer to the bytes to print
	/// @param byteCount the number of bytes
	/// @param lineLength the maximum length length to print (0 == single line)
	/// @param groupSize the number of hex byte values to group together (0 == no groups)
	///
	static std::string printHexBytes(const char * ptr, size_t byteCount, size_t lineLength = 0, size_t groupSize = 0) {
		std::ostringstream builder;
		const size_t groupsPerLine = calculateGroupsPerLine(lineLength, groupSize);

		if (groupsPerLine == 0) {
			// Return single line.

			for (size_t m = 0; m < byteCount; m++) {
				builder << std::right << std::setw(2) << std::hex << std::setfill('0') << (int) ptr[m];
			}

			return builder.str();
		}

		size_t index = 0;

		// groupsPerLine > 0

		while (index < byteCount) {
			for (size_t m = 0; m < groupsPerLine - 1; m++) {
				for (size_t n = 0; n < groupSize; n++) {
					if (index == byteCount) {
						return builder.str();
					}

					builder << std::right << std::setw(2) << std::hex << std::setfill('0') << (int) ptr[index++];
				}

				builder << " ";
			}

			for (size_t n = 0; n < groupSize; n++) {
				if (index == byteCount) {
					return builder.str();
				}

				builder << std::right << std::setw(2) << std::hex << std::setfill('0') << (int) ptr[index++];
			}

			builder << "\n";
		}

		return builder.str();
	}

	///////////////////////////////////////////////////////////////////////////

	PrettyPrint() = delete;
	PrettyPrint(const PrettyPrint &) = delete;
	PrettyPrint & operator = (const PrettyPrint &) = delete;

	////////////////////////// Private implementation /////////////////////////

	private: static size_t calculateGroupsPerLine(size_t lineLength, size_t groupSize) {
		const size_t groupCharCount = groupSize * 2;

		if (groupCharCount > lineLength) {
			return 0;
		}

		size_t groupCount = 1;

		while (true) {
			const size_t size = groupCharCount * (groupCount + 1) + (groupCount + 1) - 1;

			if (size > lineLength) {
				return groupCount;
			}

			++groupCount;
		}
	}

	private: static const char ** prefixes() {
		static const char * PREFIXES[] = {
			// atto    a    0.000000000000000001    10^−18
			"a" // ..., -18, -17, -16

			// femto    f    0.000000000000001    10^−15
			, "f" // -15, -14, -13

			// pico    p    0.000000000001    10^−12
			, "p" // -12, -11, -10

			// nano    n    0.000000001    10^−9
			, "n" // -9, -8, -7

			// micro    μ    0.000001    10^−6
			, "μ" // -6, -5, -4

			// milli    m    0.001    10^−3
			, "m" // -3, -2, -1

			// none    none    1    10^0
			, "" // 0, 1, 2

			// kilo    k    1000    10^3
			, "k" // 3, 4, 5

			// mega    M    1000000    10^6
			, "M" // 6, 7, 8

			// giga    G    1000000000    10^9
			, "G" // 9, 10, 11

			// tera    T    1000000000000    10^12
			, "T" // 12, 13, 14

			// peta    P    1000000000000000    10^15
			, "P" // 15, 16, 17

			// exa    E    1000000000000000000    10^18
			, "E" // 18, ...
		};

		return PREFIXES;
	}
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE_BALAU_UTIL_PRETTY_PRINT
