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
/// @file CommandLine.hpp
///
/// A compact command line argument parser.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__COMMAND_LINE
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__COMMAND_LINE

#include <Balau/Exception/CommandLineExceptions.hpp>
#include <Balau/Type/StdTypes.hpp>
#include <Balau/Util/Strings.hpp>

#include <cstdlib>
#include <iomanip>
#include <map>

namespace Balau {

///
/// The command line style that the application uses.
///
enum class CommandLineStyle {
	///
	/// Switches that have a leading dash '-' character.
	///
	/// Switches with values are separated from the value with whitespace.
	///
	SwitchSpaceValue,

	///
	/// Switches that do not require a leading dash.
	///
	/// Switches with values are separated from the value with an equals '=' character..
	///
	SwitchEqualsValue,

	///
	/// The parser will allow either switch-space-value or switch-equals-value.
	///
	/// If the first switch has a leading dash, the switch-space-value style will
	/// be used. Otherwise, the switch-equals-value format will be used.
	///
	Detect
};

///
/// A compact command line argument parser.
///
/// This class is currently not injectable. Use a provider instead, with the
/// command line configuration set via the fluent API.
///
/// @param <KeyT> the key type (typically an enum or std::string)
///
template <typename KeyT> class CommandLine {
	///
	/// Create a command line parser with the specified style.
	///
	/// @param style_ the command line style to use
	///
	public: explicit CommandLine(CommandLineStyle style_ = CommandLineStyle::Detect)
		: style(style_) {}

	///
	/// Create a command line parser by copying the supplied parser.
	///
	public: CommandLine(const CommandLine & ) = default;

	///
	/// Create a command line parser by moving the supplied parser.
	///
	public: CommandLine(CommandLine && ) noexcept = default;

	///
	/// Assign the current command line parser to a copy of the supplied parser.
	///
	public: CommandLine & operator = (const CommandLine & ) = default;

	///
	/// Assign the current command line parser by moving the contents of the supplied parser.
	///
	public: CommandLine & operator = (CommandLine && ) noexcept = default;

	///////////////////////// Registration of options /////////////////////////

	///
	/// Specify an option that has a short style switch and a long style switch.
	///
	/// Also specify whether the option has a value.
	///
	/// Do not specify a leading "-" or "--" for the short and long switches,
	/// as the parser will detect these itself according to the switch style
	/// specified in the constructor.
	///
	public: CommandLine & withOption(KeyT key,
	                                 const std::string & shortSwitch,
	                                 const std::string & longSwitch,
	                                 bool hasValue,
	                                 const std::string & documentation) {
		keysBySwitch[shortSwitch] = key;
		keysBySwitch[longSwitch] = key;
		optionsByKey[key] = Option(key, shortSwitch, longSwitch, hasValue, documentation);
		return *this;
	}

	///
	/// Specify an option that has a short style switch.
	///
	/// Also specify whether the option has a value.
	///
	/// Do not specify a leading "-" for the short switch, as the parser will detect
	/// these itself according to the switch style specified in the constructor.
	///
	public: CommandLine & withOption(KeyT key,
	                                 const std::string & shortSwitch,
	                                 bool hasValue,
	                                 const std::string & documentation) {
		keysBySwitch[shortSwitch] = key;
		optionsByKey[key] = Option(key, shortSwitch, "", hasValue, documentation);
		return *this;
	}

	///
	/// Specify the help command line short and long switches.
	///
	/// Do not specify a leading "-" or "--" for the short and long switches,
	/// as the parser will detect these itself according to the switch style
	/// specified in the constructor.
	///
	public: CommandLine & withHelpOption(KeyT helpKey,
	                                     const std::string & shortSwitch,
	                                     const std::string & longSwitch,
	                                     const std::string & helpDocumentation) {
		keysBySwitch[shortSwitch] = helpKey;
		keysBySwitch[longSwitch] = helpKey;
		optionsByKey[helpKey] = Option(helpKey, shortSwitch, longSwitch, false, helpDocumentation);
		return *this;
	}

	///
	/// Specify that the command line arguments include one (SEV style) or one or more (SSV style) final stand alone values.
	///
	public: CommandLine & withFinalValue() {
		commandLineHasFinalValue = HasFinalValue::Yes;
		return *this;
	}

	///
	/// Specify that the command line arguments include one (SEV style) or one or more (SSV style) final stand alone values.
	///
	public: CommandLine & withOptionalFinalValue() {
		commandLineHasFinalValue = HasFinalValue::Maybe;
		return *this;
	}

	////////////////////////// Parse input arguments //////////////////////////

	///
	/// Parse the input arguments.
	///
	/// Pass true for ignoreFirst if the first argument is the program name
	/// (normally the case).
	///
	/// If the parser is configured to detect the switch style, the first parsed
	/// program argument will be examined. If it starts with a leading '-', the
	/// switch style used will be switch-space-value, otherwise the switch style
	/// used will be switch-equals-value
	///
	public: void parse(int argc, char * argv[], bool ignoreFirst) {
		std::vector<std::string> args(static_cast<unsigned long>(argc));

		for (int m = 0; m < argc; m++) {
			args[m] = std::string(argv[m]);
		}

		parse(args, ignoreFirst);
	}

	///
	/// Parse the input arguments.
	///
	/// Pass true for ignoreFirst if the first argument is the program name
	/// (normally the case).
	///
	/// If the parser is configured to detect the switch style, the first parsed
	/// program argument will be examined. If it starts with a leading '-', the
	/// switch style used will be switch-space-value, otherwise the switch style
	/// used will be switch-equals-value
	///
	public: void parse(int argc, const char * argv[], bool ignoreFirst) {
		std::vector<std::string> args(static_cast<unsigned long>(argc));

		for (int m = 0; m < argc; m++) {
			args[m] = std::string(argv[m]);
		}

		parse(args, ignoreFirst);
	}

	///
	/// Parse the input arguments, which are supplied as a vector.
	///
	/// Pass true for ignoreFirst if the first argument is the program name.
	///
	/// If the parser is configured to detect the switch style, the first parsed
	/// program argument will be examined. If it starts with a leading '-', the
	/// switch style used will be switch-space-value, otherwise the switch style
	/// used will be switch-equals-value
	///
	/// @throw MissingFinalValueException if a final value is required and none was supplied
	/// @throw OptionNotFoundException if an option was not found
	/// @throw MissingOptionValueException if an option with a required value was specified and no value was provided
	///
	public: void parse(const std::vector<std::string> & args, bool ignoreFirst) {
		using ::toString;

		parsedValuesByKey.clear();

		if (args.empty() || (ignoreFirst && args.size() == 1)) {
			if (commandLineHasFinalValue == HasFinalValue::Yes) {
				ThrowBalauException(Exception::MissingFinalValueException, "");
			}

			return; // No arguments provided.. nothing to do.
		}

		const CommandLineStyle derivedStyle = style == CommandLineStyle::Detect
			? determineStyle(args[ignoreFirst ? 1 : 0], ignoreFirst ? args.size() - 1: args.size())
			: style;

		if (derivedStyle == CommandLineStyle::SwitchEqualsValue) {
			parseSwitchEqualsValue(derivedStyle, args, ignoreFirst);
		} else {
			parseSwitchSpaceValue(derivedStyle, args, ignoreFirst);
		}
	}

	///////////////////////// Query for value presence ////////////////////////

	///
	/// Does the command line option set have the specified option?
	///
	public: bool hasOption(KeyT key) const {
		return parsedValuesByKey.find(key) != parsedValuesByKey.end();
	}


	///
	/// Get the number of final values available.
	///
	/// This will be zero or 1 for SEV style command lines, or zero or more for SSV style command lines.
	///
	public: size_t getFinalValueCount() const {
		return finalValues.size();
	}

	////////////////////////////// String value ///////////////////////////////

	///
	/// Get the specified option value as a UTF-8 string.
	///
	/// @throw OptionNotFoundException if no such option was provided in the command line
	///
	public: std::string getOption(KeyT key) const {
		using ::toString;

		if (parsedValuesByKey.find(key) == parsedValuesByKey.end()) {
			ThrowBalauException(Exception::OptionNotFoundException, toString(key));
		}

		return parsedValuesByKey.at(key);
	}

	///
	/// Get the specified option value as a string, or the specified default if there is no such option.
	///
	public: std::string getOptionOrDefault(KeyT key, std::string defaultValue) const {
		if (parsedValuesByKey.find(key) == parsedValuesByKey.end()) {
			return defaultValue;
		}

		return parsedValuesByKey.at(key);
	}

	////////////////////////////// Integer value //////////////////////////////

	///
	/// Get the specified option value as a signed short value.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: short getOptionAsShort(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stoi(str); };
		return getOptionAs<short, int>(key, "a short integer", parse);
	}

	///
	/// Get the specified option value as an unsigned short value.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: unsigned short getOptionAsUnsignedShort(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stoull(str); };
		return getOptionAs<unsigned short, unsigned long long>(key, "an unsigned short integer", parse);
	}

	///
	/// Get the specified option value as a signed integer value.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: int getOptionAsInt(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stoi(str); };
		return getOptionAs<int, int>(key, "an integer", parse);
	}

	///
	/// Get the specified option value as an unsigned integer value.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: unsigned int getOptionAsUnsignedInt(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stoull(str); };
		return getOptionAs<unsigned int, unsigned long long>(key, "an unsigned integer", parse);
	}

	///
	/// Get the specified option value as a signed long long value.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: long long getOptionAsLong(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stoll(str); };
		return getOptionAs<long long, long long>(key, "a long integer", parse);
	}

	///
	/// Get the specified option value as an unsigned long long value.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: unsigned long long getOptionAsUnsignedLong(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stoull(str); };
		return getOptionAs<unsigned long long, unsigned long long>(key, "an unsigned long long integer", parse);
	}

	///////////////////////// Integer value or default ////////////////////////

	///
	/// Get the specified option value as a signed short, or the specified default if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: short getOptionAsShortOrDefault(KeyT key, short defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stoi(str); };
		return getOptionAs<short, int>(key, "a short integer", parse, true, defaultValue);
	}

	///
	/// Get the specified option value as an unsigned short, or the specified default if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: unsigned short getOptionAsUnsignedShortOrDefault(KeyT key, unsigned short defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stoull(str); };
		return getOptionAs<unsigned short, unsigned long long>(key, "an unsigned short integer", parse, true, defaultValue);
	}

	///
	/// Get the specified option value as a signed int, or the specified default if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: int getOptionAsIntOrDefault(KeyT key, int defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stoi(str); };
		return getOptionAs<int, int>(key, "an integer", parse, true, defaultValue);
	}

	///
	/// Get the specified option value as an unsigned int, or the specified default if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: unsigned int getOptionAsUnsignedIntOrDefault(KeyT key, unsigned int defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stoull(str); };
		return getOptionAs<unsigned int, unsigned long long>(key, "an unsigned integer", parse, true, defaultValue);
	}

	///
	/// Get the specified option value as a signed long long, or the specified default if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: long long getOptionAsLongOrDefault(KeyT key, long long defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stoll(str); };
		return getOptionAs<long long, long long>(key, "a long integer", parse, true, defaultValue);
	}

	///
	/// Get the specified option value as an unsigned long long, or the specified default if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: unsigned long long getOptionAsUnsignedLongOrDefault(KeyT key, unsigned long long defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stoull(str); };
		return getOptionAs<unsigned long long, unsigned long long>(key, "an unsigned long long integer", parse, true, defaultValue);
	}

	/////////////////////////// Floating point value //////////////////////////

	///
	/// Get the specified option value as a float.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: float getOptionAsFloat(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stof(str); };
		return getOptionAs<float, float>(key, "a single precision floating point value", parse);
	}

	///
	/// Get the specified option value as a double.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: double getOptionAsDouble(KeyT key) const {
		auto parse = [] (const std::string & str) { return std::stod(str); };
		return getOptionAs<double, double>(key, "a double precision floating point value", parse);
	}

	///////////////////// Floating point value or default /////////////////////

	///
	/// Get the specified option value as a float or return the supplied value if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: float getOptionAsFloatOrDefault(KeyT key, float defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stof(str); };
		return getOptionAs<float, float>(key, "a single precision floating point value", parse, true, defaultValue);
	}

	///
	/// Get the specified option value as a double or return the supplied value if there is no such option.
	///
	/// @throw OptionNotFoundException if the specified option was not found in the parsed command line
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: double getOptionAsDoubleOrDefault(KeyT key, double defaultValue) const {
		auto parse = [] (const std::string & str) { return std::stod(str); };
		return getOptionAs<double, double>(key, "a double precision floating point value", parse, true, defaultValue);
	}

	/////////////////////////////// Final values //////////////////////////////

	///
	/// Get the final value with the specified index as a string or throw if no final value is available.
	///
	/// @throw OptionNotFoundException if this parser does not have final values or the index is out of range
	///
	public: std::string getFinalValue(size_t index = 0) const {
		if (commandLineHasFinalValue == HasFinalValue::No) {
			ThrowBalauException(Exception::OptionNotFoundException, "This parser does not have final values.");
		}

		if (finalValues.size() <= index) {
			ThrowBalauException(
				  Exception::OptionNotFoundException
				, ::toString("This parser does not have a final value with index ", index)
			);
		}

		return finalValues[index];
	}

	///
	/// Get the final value as a float or throw if no final value is available.
	///
	/// @throw OptionNotFoundException if no final value exists or the parser does not have final values
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: float getFinalValueAsFloat(size_t index = 0) const {
		auto parse = [] (const std::string & str) { return std::stof(str); };
		return getFinalValueAs<float>("a single precision floating point value", parse, index);
	}

	///
	/// Get the final value as a double or throw if no final value is available.
	///
	/// @throw OptionNotFoundException if no final value exists or the parser does not have final values
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: double getFinalValueAsDouble(size_t index = 0) const {
		auto parse = [] (const std::string & str) { return std::stod(str); };
		return getFinalValueAs<double>("a double precision floating point value", parse, index);
	}

	///////////////////////// Final value or default //////////////////////////

	///
	/// Get the final value as a string or return the supplied default if no final value is available.
	///
	public: std::string getFinalValueOrDefault(const std::string & defaultValue, size_t index = 0) const {
		if (finalValues.size() <= index) {
			return defaultValue;
		}

		return finalValues[index];
	}

	///
	/// Get the final value as a float or return the supplied default if no final value is available.
	///
	/// @throw OptionNotFoundException if no final value exists or the parser does not have final values
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: float getFinalValueAsFloatOrDefault(float defaultValue, size_t index = 0) const {
		auto parse = [] (const std::string & str) { return std::stof(str); };
		return getFinalValueAs<float>("a single precision floating point value", parse, index, true, defaultValue);
	}

	///
	/// Get the final value as a double or return the supplied default if no final value is available.
	///
	/// @throw OptionNotFoundException if no final value exists or the parser does not have final values
	/// @throw OptionValueException if the value supplied on the command line was invalid
	///
	public: double getFinalValueAsDoubleOrDefault(double defaultValue, size_t index = 0) const {
		auto parse = [] (const std::string & str) { return std::stod(str); };
		return getFinalValueAs<double>("a double precision floating point value", parse, index, true, defaultValue);
	}

	//////////////////////////////// Help text ////////////////////////////////

	///
	/// Get the help text.
	///
	/// For parsers that are configured with CommandLineStyle::Detect, the help
	/// text will default to switch-space-value unless this is overridden here.
	///
	/// If both styles need to be communicated, this method can be called twice
	/// with different overrides.
	///
	/// Note that the implementation of this method does not current do word
	/// break analysis, thus it is currently up to the end developer to ensure
	/// words are not cut across lines (patches to implement word break analysis
	/// are welcome).
	///
	public: std::string getHelpText(size_t indent,
	                                size_t totalWidth,
	                                CommandLineStyle styleOverride = CommandLineStyle::Detect) const {
		const CommandLineStyle determinedStyle = styleOverride != CommandLineStyle::Detect
			? styleOverride
			: style != CommandLineStyle::Detect
			? style
			: CommandLineStyle::SwitchSpaceValue;

		size_t maxSwitchTextWidth = 0;

		for (const auto & pair : optionsByKey) {
			const std::string shortSw = determinedStyle == CommandLineStyle::SwitchSpaceValue
				? "-" + pair.second.shortSwitch
				: pair.second.shortSwitch;

			const std::string longSw = determinedStyle == CommandLineStyle::SwitchSpaceValue
				? (pair.second.longSwitch.empty() ? "" : "--" + pair.second.longSwitch)
				: pair.second.longSwitch;

			const std::string switches = shortSw + (longSw.empty() ? "" : ", " + longSw);

			if (maxSwitchTextWidth < switches.length()) {
				maxSwitchTextWidth = switches.length();
			}
		}

		// Apply a minimum text width if the specified total width is very small.
		if (totalWidth < maxSwitchTextWidth + 20) {
			totalWidth = maxSwitchTextWidth + 20;
		}

		std::string prefix(indent, ' ');
		std::string contPrefix(indent + 3 + maxSwitchTextWidth, ' ');
		std::ostringstream builder;

		for (const auto & pair : optionsByKey) {
			const std::string shortSw = determinedStyle == CommandLineStyle::SwitchSpaceValue
				? "-" + pair.second.shortSwitch
				: pair.second.shortSwitch;

			const std::string longSw = determinedStyle == CommandLineStyle::SwitchSpaceValue
				? (pair.second.longSwitch.empty() ? "" : "--" + pair.second.longSwitch)
				: pair.second.longSwitch;

			const auto & option = pair.second;

			const std::string switches = shortSw + (longSw.empty() ? "" : ", " + longSw);

			builder << prefix << std::left << std::setw((int) maxSwitchTextWidth) << switches << " - ";

			const std::string & docText = option.documentation;
			size_t charactersWritten = 0;
			bool first = true;

			while (charactersWritten < docText.length()) {
				const size_t charactersRemaining = docText.length() - charactersWritten;

				if (charactersRemaining <= totalWidth) {
					builder << (first ? "" : contPrefix) << docText.substr(charactersWritten) << "\n";
					charactersWritten = docText.length();
				} else {
					builder << (first ? "" : contPrefix) << docText.substr(charactersWritten, totalWidth) << "\n";
					charactersWritten += totalWidth;
				}

				first = false;
			}
		}

		return builder.str();
	}

	///////////////////////// Private implementation //////////////////////////

	private: CommandLineStyle determineStyle(const std::string & firstArgument, size_t argCount) const {
		if (style != CommandLineStyle::Detect) {
			return style;
		}

		if (commandLineHasFinalValue == HasFinalValue::Yes && argCount == 1) {
			// Doesn't matter.. only the final value argument is present.
			return CommandLineStyle::SwitchSpaceValue;
		}

		return Util::Strings::startsWith(firstArgument, "-")
			? CommandLineStyle::SwitchSpaceValue
			: CommandLineStyle::SwitchEqualsValue;
	}

	private: void parseSwitchEqualsValue(const CommandLineStyle & derivedStyle,
	                                     const std::vector<std::string> & args,
	                                     bool ignoreFirst) {
		using ::toString;

		std::vector<std::string> cleanedArgs;
		size_t m = ignoreFirst ? 1 : 0;

		while (m < args.size()) {
			const std::string & arg = args[m];

			if (Util::Strings::contains(arg, "=")) {
				const size_t first = arg.find('=');
				cleanedArgs.emplace_back(arg.substr(0, first));
				cleanedArgs.emplace_back(arg.substr(first + 1));
			} else {
				cleanedArgs.emplace_back(arg);
			}

			m++;
		}

		for (size_t m = 0; m < (commandLineHasFinalValue == HasFinalValue::Yes ? cleanedArgs.size() - 1 : cleanedArgs.size()); m++) {
			const std::string optionSwitch = toString(Util::Strings::trim(cleanedArgs[m]));
			auto iter = keysBySwitch.find(optionSwitch);

			if (iter == keysBySwitch.end()) {
				ThrowBalauException(Exception::OptionNotFoundException, optionSwitch);
			}

			const auto & key = iter->second;
			const auto & option = optionsByKey.at(key);

			if (option.hasValue) {
				if (commandLineHasFinalValue == HasFinalValue::Yes && cleanedArgs.size() - m < 2) {
					ThrowBalauException(Exception::MissingFinalValueException, "");
				} else if (cleanedArgs.size() - m < 1) {
					ThrowBalauException(Exception::MissingOptionValueException, optionSwitch);
				}

				m++;
				parsedValuesByKey[key] = toString(Util::Strings::trim(cleanedArgs[m]));
			} else {
				parsedValuesByKey[key] = "";
			}
		}

		if (commandLineHasFinalValue == HasFinalValue::Yes) {
			finalValues.push_back(toString(Util::Strings::trim(cleanedArgs.back())));
		}
	}

	private: void parseSwitchSpaceValue(const CommandLineStyle & derivedStyle,
	                                    const std::vector<std::string> & args,
	                                    bool ignoreFirst) {
		size_t m = ignoreFirst ? 1 : 0;

		for (; m < args.size(); m++) {
			using ::toString;
			std::string optionSwitch = toString(Util::Strings::trim(args[m]));

			// Full switches must start with "--", abbreviated switches must start with "-".
			// These are removed for the derivedSwitch.

			if (Util::Strings::startsWith(optionSwitch, "--")) {
				optionSwitch = optionSwitch.substr(2);
			} else if (Util::Strings::startsWith(optionSwitch, "-")) {
				optionSwitch = optionSwitch.substr(1);
			} else {
				if (commandLineHasFinalValue == HasFinalValue::No) {
					ThrowBalauException(Exception::IllegalArgumentException, "Illegal value found when parsing.");
				}

				// Not a switch.
				finalValues.push_back(std::move(optionSwitch));
				continue;
			}

			auto iter = keysBySwitch.find(optionSwitch);

			if (iter == keysBySwitch.end()) {
				ThrowBalauException(Exception::OptionNotFoundException, optionSwitch);
			}

			const auto & key = iter->second;
			const auto & option = optionsByKey.at(key);

			if (option.hasValue) {
				if (commandLineHasFinalValue == HasFinalValue::Yes && finalValues.empty() && m + 2 == args.size()) {
					ThrowBalauException(Exception::MissingFinalValueException, "");
				} else if (m + 1 == args.size()) {
					ThrowBalauException(Exception::MissingOptionValueException, optionSwitch);
				}

				m++;
				parsedValuesByKey[key] = toString(Util::Strings::trim(args[m]));
			} else {
				parsedValuesByKey[key] = "";
			}
		}
	}

	//
	// Option numeric conversion function.
	//  - T is the required type.
	//  - C is the conversion function type which is subsequently narrowed.
	//
	private: template<typename T, typename C> T getOptionAs(KeyT key,
	                                                        const std::string & typeAsString,
	                                                        C parse(const std::string &),
	                                                        bool orDefault = false,
	                                                        T defaultValue = T()) const {
		if (parsedValuesByKey.find(key) == parsedValuesByKey.end()) {
			if (orDefault) {
				return defaultValue;
			}

			using ::toString;

			ThrowBalauException(
				  Exception::OptionNotFoundException
				, std::string("No option with key ") + toString(key) + " was supplied on the command line."
			);
		}

		const std::string str = parsedValuesByKey.at(key);

		try {
			const C value = parse(str);
			return boost::numeric_cast<T>(value);
		} catch (const std::invalid_argument &) {
			using ::toString;

			ThrowBalauException(Exception::OptionValueException
				, std::string(
					"Option value of key '" + toString(key) + "' is not of type " + typeAsString + ": "
					+ parsedValuesByKey.at(key)
				)
			);

			// Prevent compiler warning.
			return defaultValue;
		} catch (...) {
			using ::toString;

			ThrowBalauException(
				  Exception::OptionValueException
				, std::string(
					"Option value of key '" + toString(key) + "' is not in the valid range for " + typeAsString + ": "
					+ parsedValuesByKey.at(key)
				)
			);

			// Prevent compiler warning.
			return defaultValue;
		}
	}

	//
	// Final value numeric conversion function.
	//  - T is the required type.
	//
	private: template<typename T> T getFinalValueAs(const std::string & typeAsString,
	                                                T parse(const std::string &),
	                                                size_t index,
	                                                bool orDefault = false,
	                                                T defaultValue = T()) const {
		if (commandLineHasFinalValue == HasFinalValue::No) {
			if (orDefault) {
				return defaultValue;
			}

			ThrowBalauException(Exception::OptionNotFoundException, "This parser does not have final values.");
		}

		if (finalValues.size() <= index) {
			if (orDefault) {
				return defaultValue;
			}

			ThrowBalauException(
				  Exception::OptionNotFoundException
				, ::toString("This parser does not have a final value with index ", index)
			);
		}

		try {
			return parse(finalValues[index]);
		} catch (const std::invalid_argument &) {
			ThrowBalauException(
				Exception::OptionValueException, std::string("Final value is not " + typeAsString + ": " + finalValues[index])
			);
		} catch (...) {
			ThrowBalauException(
				  Exception::OptionValueException
				, std::string("Final value is not in the valid range for " + typeAsString + ": " + finalValues[index])
			);
		}
	}

	private: struct Option {
		KeyT key;
		std::string shortSwitch;
		std::string longSwitch;
		bool hasValue;
		std::string documentation;

		Option() : hasValue(false) {}

		Option(KeyT key_,
		       std::string shortSwitch_,
		       std::string longSwitch_,
		       bool hasValue_,
		       std::string documentation_)
			: key(std::move(key_))
			, shortSwitch(std::move(shortSwitch_))
			, longSwitch(std::move(longSwitch_))
			, hasValue(hasValue_)
			, documentation(std::move(documentation_)) {}

		Option & operator = (const Option & copy) = default;
	};

	private: enum class HasFinalValue {
		No, Yes, Maybe
	};

	private: CommandLineStyle style;
	private: std::map<std::string, KeyT> keysBySwitch; // switch, key
	private: std::map<KeyT, Option> optionsByKey; // key, option info
	private: std::map<KeyT, std::string> parsedValuesByKey; // key, value
	private: HasFinalValue commandLineHasFinalValue = HasFinalValue::No;
	private: std::vector<std::string> finalValues;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__COMMAND_LINE
