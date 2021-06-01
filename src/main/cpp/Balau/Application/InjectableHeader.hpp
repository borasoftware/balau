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
/// @file InjectableHeader.hpp
///
/// Injection macros used in injectable class headers.
///
/// For libraries, avoid using these header/body macros if possible. Use the
/// header only macros instead. Use of the header/body macros will require a
/// rebuild when switching to the fake macros, as part of the boilerplate code
/// will be in the body file.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_HEADER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_HEADER

#include <Balau/Application/Impl/InjectorHeaderMacros.hpp>

///
/// Annotate an injectable class header by specifying the class name .
///
#define BalauInjectHeader(...) BALAU_InjectHeader(__VA_ARGS__)

///
/// Annotate an injectable class header by specifying the class name.
///
/// This version includes the definition of an injectable constructor.
///
#define BalauInjectHeaderConstruct(...) BALAU_InjectHeaderConstruct(__VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_HEADER
