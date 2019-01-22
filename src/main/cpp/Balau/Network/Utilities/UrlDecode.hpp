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
/// @file UrlDecode.hpp
///
/// Utility for splitting and decoding URL encoded data.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_DECODE
#define COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_DECODE

#include <Balau/Exception/NetworkExceptions.hpp>
#include <Balau/Util/Strings.hpp>

#include <map>

namespace Balau::Network {

///
/// Utility for splitting and decoding URL encoded data.
///
struct UrlDecode {
	///
	/// Split the URL encoded parameters then decode each one.
	///
	/// @param data the input string containing the URL encoded parameters
	/// @return a hash map of split parameters, represented by strings
	/// @throw NetworkException if the parameter list was invalid
	///
	static std::unordered_map<std::string_view, std::string_view> splitAndDecode(const std::string_view & data,
	                                                                             bool validateUtf8 = true,
	                                                                             bool throwOnError = false) {
		std::unordered_map<std::string_view, std::string_view> decodedParameters;

		const auto parameters = split(data);

		for (auto iter = parameters.begin(); iter != parameters.end(); ++iter) {
			auto key = iter->first;
			auto value = iter->second;

			auto decodedKey = decode(key, validateUtf8, throwOnError);
			auto decodedValue = decode(value, validateUtf8, throwOnError);

			decodedParameters[decodedKey] = decodedValue;
		}

		return decodedParameters;
	}

	///
	/// Split the URL encoded parameters into a map of string views.
	///
	/// The caller is responsible for maintaining the original string onto which the views are pointing.
	///
	/// @param data the input string view containing the URL encoded parameters
	/// @return a hash map of split parameters, represented by string views onto the input string
	/// @throw NetworkException if the parameter list was invalid
	///
	static std::unordered_map<std::string_view, std::string_view> split(const std::string_view data) {
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

	///
	/// Decode the percent encoded input string into a UTF-8 string, according to the following rules.
	///
	///  - If the character is a '+', decode it as a space.
	///
	///  - If the character is a '%', consume characters according to the decode algorithm given below.
	///
	///  - Otherwise, output the character as is.
	///
	/// The decode algorithm is as follows.
	///
	///  - Decode all successive sequences of %XX, where each X is a hexadecimal ascii character in the
	///    ranges '0' to '9', 'a' to 'f', and  'A' to 'F', into bytes corresponding to the value obtained
	///    by converting the hexadecimal XX into an unsigned integer.
	///
	///  - Optionally, verify that the resulting byte sequence is valid UTF-8.
	///
	/// @param input the percent encoded input string
	/// @param validateUtf8 if true, the decoded percent encoded data will only be added if it is valid UTF-8
	/// @param throwOnError if true, errors will cause an exception to be thrown instead of being silently ignored
	/// @throw NetworkException if throwOnError was set to true and the input was invalid
	///
	static std::string decode(const std::string_view input, bool validateUtf8 = true, bool throwOnError = false) {
		std::string output;
		size_t index = 0;

		while (index < input.length()) {
			switch (input[index]) {
				case '+': {
					output += ' ';
					++index;
					break;
				}

				case '%': {
					std::string thisOutput;

					while (input[index] == '%') {
						++index;

						if (index == input.length()) {
							// Illegal input.. % character on its own at the end.
							if (throwOnError) {
								ThrowBalauException(Exception::NetworkException, "Invalid percent encoded string");
							}

							continue;
						}

						const size_t x = index;
						++index;

						if (index == input.length()) {
							// Illegal input.. % character with single following character at the end.
							if (throwOnError) {
								ThrowBalauException(Exception::NetworkException, "Invalid percent encoded string");
							}

							continue;
						}

						// Try to convert the hex character pair into an unsigned byte.
						std::string_view v = input.substr(x, 2);
						unsigned char b;
						fromString(b, v, 16);
						thisOutput += (char) b;
						++index;
					}

					bool valid = true;

					if (validateUtf8) {
						int offset = 0;

						while (offset != (int) thisOutput.length()) {
							Character::getNextUtf8Safe(thisOutput, offset);

							if (offset < 0) {
								// Not valid UTF-8.
								if (throwOnError) {
									ThrowBalauException(Exception::NetworkException, "Invalid percent encoded string");
								}

								valid = false;
								break;
							}
						}
					}

					if (valid) {
						output += thisOutput;
					}

					break;
				}

				default: {
					output += input[index];
					++index;
					break;
				}
			}
		}

		return output;
	}

	////////////////////////// Private implementation /////////////////////////

	UrlDecode() = delete;
	UrlDecode(const UrlDecode &) = delete;
	UrlDecode & operator = (const UrlDecode &) = delete;
};

} // namespace Balau::Network

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__URL_DECODE
