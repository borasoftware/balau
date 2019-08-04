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
/// @file InjectHeader.hpp
///
/// InjectableHeader.hpp macros without Balau prefix.
///

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECT_HEADER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECT_HEADER

#include <Balau/Application/InjectableHeader.hpp>

///
/// Annotate an injectable class header by specifying the class name .
///
#define InjectHeader(...) BalauInjectHeader(__VA_ARGS__)

///
/// Annotate an injectable class header by specifying the class name.
///
/// This version includes the definition of an injectable constructor.
///
#define InjectHeaderConstruct(...) BalauInjectHeaderConstruct(__VA_ARGS__)

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECT_HEADER
