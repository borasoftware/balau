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

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_MACROS_IMPL
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_MACROS_IMPL

#define BALAU_Expand(ARG)       ARG
#define BALAU_StringExpand(ARG) #ARG

#define BALAU_VariadicNArg_(...) BALAU_Expand(BALAU_VariadicNArgN(__VA_ARGS__))
#define BALAU_VariadicNArgN(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16, P17, P18, P19, P20, P21, P22, P23, P24, P25, P26, P27, P28, P29, P30, P31, P32, P33, N, ...) N
#define BALAU_VariadicRSeqN() 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_MACROS_IMPL
