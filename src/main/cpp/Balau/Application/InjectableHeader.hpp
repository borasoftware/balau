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
/// @file InjectableHeader.hpp
///
/// Injection macros used in injectable class headers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_HEADER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_HEADER

#include <Balau/Application/Impl/InjectorHeaderMacros.hpp>

///
/// Annotate an injectable class header by specifying the class name .
///
#define BalauInjectHeader(...) _BalauInjectHeader(__VA_ARGS__)

///
/// Annotate an injectable class header by specifying the class name.
///
/// This version includes the definition of an injectable constructor.
///
#define BalauInjectHeaderConstruct(...) _BalauInjectHeaderConstruct(__VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTABLE_HEADER
