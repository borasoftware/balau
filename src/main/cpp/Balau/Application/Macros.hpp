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
/// @file Macros.hpp
///
/// Utilities for macros.
///

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_MACROS
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_MACROS

#include <Balau/Application/Impl/MacrosImpl.hpp>

///
/// Expand the string within another macro.
///
#define BalauString(TOKEN) BALAU_StringExpand(TOKEN)

///
/// Compute the number of variadic macro arguments.
///
#define BalauVariadicNArg(...)  BALAU_Expand(BALAU_VariadicNArg_(__VA_ARGS__, BALAU_VariadicRSeqN()))

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_MACROS
