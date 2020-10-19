// @formatter:off
//
// Balau core C++ library
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
/// @file Injectable.hpp
///
/// Injection macros used in injectable classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE

#include <Balau/Application/Injector.hpp>
#include <Balau/Application/Impl/InjectorMacros.hpp>

///
/// Annotate an injectable class by specifying the class name and its direct or indirect dependency fields.
///
#define BalauInject(...) _BalauInject(__VA_ARGS__)

///
/// Annotate an injectable class by specifying the class name and its named direct or indirect dependency fields.
///
#define BalauInjectNamed(...) _BalauInjectNamed(__VA_ARGS__)

///
/// Annotate an injectable class by specifying the class name and its direct or indirect dependency fields.
///
/// This version includes the definition of an injectable constructor.
///
#define BalauInjectConstruct(...) _BalauInjectConstruct(__VA_ARGS__)

///
/// Annotate an injectable class by specifying the class name and its named direct or indirect dependency fields.
///
/// This version includes the definition of an injectable constructor.
///
#define BalauInjectConstructNamed(...) _BalauInjectConstructNamed(__VA_ARGS__)

///
/// Annotate an injectable class by specifying the class name and the types of its dependencies.
///
#define BalauInjectTypes(...) _BalauInjectTypes(__VA_ARGS__)

///
/// Annotate an injectable class by specifying the class name and the named types of its dependencies.
///
#define BalauInjectNamedTypes(...) _BalauInjectNamedTypes(__VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE
