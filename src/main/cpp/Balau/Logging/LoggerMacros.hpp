
// @formatter:off
//
// CPPLF C++ library
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

#ifndef COM_BORA_SOFTWARE_BALAU_LOGGING_LOGGER_MACROS
#define COM_BORA_SOFTWARE_BALAU_LOGGING_LOGGER_MACROS

#include <Balau/Logging/Logger.hpp>

///// Logger call macros that do not have the Balau prefix in  //////
///// their name. Do not use in libraries (use CPPLF instead). //////

///
/// Log a trace message with the source code location of the logging statement.
///
#define LogTrace(...) BalauLogTrace(__VA_ARGS__)

///
/// Log a debug message with the source code location of the logging statement.
///
#define LogDebug(...) BalauLogDebug(__VA_ARGS__)

///
/// Log an info message with the source code location of the logging statement.
///
#define LogInfo(...)  BalauLogInfo(__VA_ARGS__)

///
/// Log a warn message with the source code location of the logging statement.
///
#define LogWarn(...)  BalauLogWarn(__VA_ARGS__)

///
/// Log an error message with the source code location of the logging statement.
///
#define LogError(...) BalauLogError(__VA_ARGS__)

///
/// Log a message at the specified level and with the source code location of the logging statement.
///
#define LogLog(...) BalauLogLog(__VA_ARGS__)

#endif // COM_BORA_SOFTWARE_BALAU_LOGGING_LOGGER_MACROS
