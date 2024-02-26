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
/// @file CommandLineHolder.hpp
///
/// Integration with the injector of command line argument processing.
///

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_COMMAND_LINE_HOLDER
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_COMMAND_LINE_HOLDER

#include <Balau/Application/CommandLine.hpp>
#include <Balau/Application/Injectable.hpp>

namespace Balau {

///
/// Integration with the injector of command line argument processing.
///
/// If the static initialiseCommandLineArguments function is called before the
/// injector is created, then a singleton of this class will call the parse
/// method in the command line singleton. Then the command line may be consumed
/// by any other injectable, by passing in the command line holder singleton
/// and getting the command line singleton.
///
/// Note that the command line should not be consumed by injecting the command
/// line singleton directly if the command line is processed in the injectable's
/// constructor, as this may result in the command line not being parsed when
/// the injectable's constructor is run.
///
template <typename KeyT> class CommandLineHolder {
	///
	/// Called before the injector is created.
	///
	/// Initialises the command line arguments, ready for processing when the injector is created.
	///
	/// @param argc the argc from the main function
	/// @param argv the argv from the main function
	/// @param ignoreFirst_ set to true if the first argument should be ignored (i.e. it is the application name)
	///
	public: static void initialiseCommandLineArguments(int argc, const char * argv[], bool ignoreFirst_) {
		args.clear();

		for (int m = 0; m < argc; m++) {
			args.emplace_back(argv[m]);
		}

		ignoreFirst = ignoreFirst_;
	}

	public: explicit CommandLineHolder(std::shared_ptr<CommandLine<KeyT>> commandLine_);

	///
	/// Get the command line instance.
	///
	public: CommandLine<KeyT> & getCommandLine() noexcept {
		return *commandLine;
	}

	private: static std::vector<std::string> args;
	private: static bool ignoreFirst;

	private: std::shared_ptr<CommandLine<KeyT>> commandLine;

	BalauInject(CommandLineHolder<KeyT>, commandLine);
};

template <typename KeyT>
std::vector<std::string> CommandLineHolder<KeyT>::args;

template <typename KeyT>
bool CommandLineHolder<KeyT>::ignoreFirst;

template <typename KeyT>
CommandLineHolder<KeyT>::CommandLineHolder(std::shared_ptr<CommandLine<KeyT>> commandLine_)
	: commandLine(std::move(commandLine_)) {
	commandLine->parse(args, ignoreFirst);
}

} // namespace Balau

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_COMMAND_LINE_HOLDER
