// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__MACROS_IMPL
#define COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__MACROS_IMPL

#define _BalauExpand(ARG)       ARG
#define _BalauStringExpand(ARG) #ARG

#define _BalauVariadicNArg_(...) _BalauExpand(_BalauVariadicNArgN(__VA_ARGS__))
#define _BalauVariadicNArgN(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, N, ...) N
#define _BalauVariadicRSeqN() 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#endif // COM_BORA_SOFTWARE__BALAU_UTIL_IMPL__MACROS_IMPL
