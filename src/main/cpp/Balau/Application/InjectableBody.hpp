// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file Injectable.hpp
///
/// Injection macros used in injectable classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_BODY
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_BODY

#include <Balau/Application/Injector.hpp>
#include <Balau/Application/Impl/InjectorBodyMacros.hpp>

///
/// Annotate an injectable class body by specifying the class name and its direct or indirect dependency fields.
///
#define BalauInjectBody(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectBody)(__VA_ARGS__))

///
/// Annotate an injectable class body by specifying the class name and its named direct or indirect dependency fields.
///
#define BalauInjectBodyNamed(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectBodyNamed)(__VA_ARGS__))

///
/// Annotate an injectable class body by specifying the class name and the types of its dependencies.
///
#define BalauInjectBodyTypes(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectBodyTypes)(__VA_ARGS__))

///
/// Annotate an injectable class body by specifying the class name and the named types of its dependencies.
///
#define BalauInjectBodyNamedTypes(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectBodyNamedTypes)(__VA_ARGS__))

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_BODY
