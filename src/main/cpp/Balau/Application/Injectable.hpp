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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE

#include <Balau/Application/Injector.hpp>
#include <Balau/Application/Impl/InjectorMacros.hpp>

///
/// Annotate an injectable class by specifying the class name and its direct or indirect dependency fields.
///
#define BalauInject(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInject)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and its named direct or indirect dependency fields.
///
#define BalauInjectNamed(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectNamed)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and its direct or indirect dependency fields.
///
/// This version includes the definition of an injectable constructor.
///
#define BalauInjectConstruct(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectConstruct)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and its named direct or indirect dependency fields.
///
/// This version includes the definition of an injectable constructor.
///
#define BalauInjectConstructNamed(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectConstructNamed)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and the types of its dependencies.
///
#define BalauInjectTypes(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectTypes)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and the named types of its dependencies.
///
#define BalauInjectNamedTypes(...) BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectNamedTypes)(__VA_ARGS__))

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE
