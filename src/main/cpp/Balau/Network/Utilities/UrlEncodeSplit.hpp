// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file UrlEncodeSplit.hpp
///
/// Utility for splitting URL encoded data.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_ENCODE_SPLIT
#define COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_ENCODE_SPLIT

#include <Balau/Exception/NetworkExceptions.hpp>
#include <Balau/Util/Strings.hpp>

#include <map>

namespace Balau::Network {

///
/// Utility for splitting URL encoded data.
///
struct UrlEncodeSplit {
	///
	/// Split the URL encoded parameters into a map of string views.
	///
	/// The caller is responsible for maintaining the original string onto which the views are pointing.
	///
	///
	/// @throw NetworkException if the parameter list was invalid
	///
	static std::unordered_map<std::string_view, std::string_view> split(const std::string & data) {
		std::unordered_map<std::string_view, std::string_view> keysAndValues;
		std::vector<std::string_view> parameters = Util::Strings::split(data, "&");

		for (auto parameter : parameters) {
			std::vector<std::string_view> keyAndValue = Util::Strings::split(parameter, "=");

			if (keyAndValue.size() == 1) {
				// No value supplied.
				keysAndValues.emplace(keyAndValue[0], "");
			} else if (keyAndValue.size() != 2) {
				ThrowBalauException(Exception::NetworkException, "Invalid parameter list");
			} else {
				keysAndValues.emplace(keyAndValue[0], keyAndValue[1]);
			}
		}

		return keysAndValues;
	}

	////////////////////////// Private implementation /////////////////////////

	UrlEncodeSplit() = delete;
	UrlEncodeSplit(const UrlEncodeSplit &) = delete;
	UrlEncodeSplit & operator = (const UrlEncodeSplit &) = delete;
};

} // namespace Balau::Network

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_ENCODE_SPLIT
