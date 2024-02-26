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
/// @file InjectBody.hpp
///
/// InjectableBody.hpp macros without Balau prefix.
///

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_INJECT_BODY
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_INJECT_BODY

#include <Balau/Application/InjectableBody.hpp>

///
/// Annotate an injectable class body by specifying the class name and its direct or indirect dependency fields.
///
#define InjectBody(...) BalauInjectBody(__VA_ARGS__)

///
/// Annotate an injectable class body by specifying the class name and its named direct or indirect dependency fields.
///
#define InjectBodyNamed(...) BalauInjectBodyNamed(__VA_ARGS__)

///
/// Annotate an injectable class body by specifying the class name and the types of its dependencies.
///
#define InjectBodyTypes(...) BalauInjectBodyTypes(__VA_ARGS__)

///
/// Annotate an injectable class body by specifying the class name and the named types of its dependencies.
///
#define InjectBodyNamedTypes(...) BalauInjectBodyNamedTypes(__VA_ARGS__)

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_INJECT_BODY
