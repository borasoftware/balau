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
/// @file Inject.hpp
///
/// Injectable.hpp macros without Balau prefix.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECT
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECT

#include <Balau/Application/Injectable.hpp>

///
/// Annotate an injectable class by specifying the class name and its direct or indirect dependency fields.
///
#define Inject(...) BalauVaArgsExpand(BalauVaArgsExpand(BalauInject)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and its named direct or indirect dependency fields.
///
#define InjectNamed(...) BalauVaArgsExpand(BalauVaArgsExpand(BalauInjectNamed)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and its direct or indirect dependency fields.
///
/// This version includes the definition of an injectable constructor.
///
#define InjectConstruct(...) BalauVaArgsExpand(BalauVaArgsExpand(BalauInjectConstruct)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and its named direct or indirect dependency fields.
///
/// This version includes the definition of an injectable constructor.
///
#define InjectConstructNamed(...) BalauVaArgsExpand(BalauVaArgsExpand(BalauInjectConstructNamed)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and the types of its dependencies.
///
#define InjectTypes(...) BalauVaArgsExpand(BalauVaArgsExpand(BalauInjectTypes)(__VA_ARGS__))

///
/// Annotate an injectable class by specifying the class name and the named types of its dependencies.
///
#define InjectNamedTypes(...) BalauVaArgsExpand(BalauVaArgsExpand(BalauInjectNamedTypes)(__VA_ARGS__))

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECT
