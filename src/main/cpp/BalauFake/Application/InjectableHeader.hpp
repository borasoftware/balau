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
/// Empty injection macros to be used when injectable classes are not required.
///
/// For libraries, avoid these macros if possible. Using the fake header/body macros
/// will require a rebuild, as part of the boilerplate code will be in the body file.
///
/// If these macros are still required, avoid the BalauInjectConstruct* macros in libraries,
/// as these will change semantics when switching to the fake macros. For this reason,
/// those macros are not included here.
///

#ifndef COM_BORA_SOFTWARE_BALAU_FAKE_APPLICATION_INJECTABLE_HEADER
#define COM_BORA_SOFTWARE_BALAU_FAKE_APPLICATION_INJECTABLE_HEADER

///
/// Annotate an injectable class header by specifying the class name .
///
#define BalauInjectHeader(...)

#endif // COM_BORA_SOFTWARE_BALAU_FAKE_APPLICATION_INJECTABLE_HEADER
