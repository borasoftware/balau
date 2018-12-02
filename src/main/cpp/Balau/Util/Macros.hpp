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
/// @file Macros.hpp
///
/// Utilities for macros.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__MACROS
#define COM_BORA_SOFTWARE__BALAU_UTIL__MACROS

#include <Balau/Util/Impl/MacrosImpl.hpp>

///
/// Expand the string within another macro.
///
#define BalauString(TOKEN) _BalauStringExpand(TOKEN)

///
/// Compute the number of variadic macro arguments.
///
#define BalauVariadicNArg(...)  _BalauExpand(_BalauVariadicNArg_(__VA_ARGS__, _BalauVariadicRSeqN()))

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__MACROS
