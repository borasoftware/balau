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

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_INJECTOR_HEADER_MACROS
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_INJECTOR_HEADER_MACROS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Application/Macros.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define BALAU_HIPF(F)    decltype(F) F ## _
#define BALAU_HIMF(F)    F(std::forward<decltype(F)>(F ## _))

namespace Balau {

class Injector;

} // namespace Balau

///////////////////////////////////// BALAU_InjectHeader macros /////////////////////////////////////

#define BALAU_InjectHeader(TYPE)                                                                    \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector & );                        \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector & );                         \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys()

/////////////////////////////  BALAU_InjectHeaderConstructNamed macros //////////////////////////////

#define BALAU_InjectHeaderConstruct_1(TYPE)                                                        \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE() {}

#define BALAU_InjectHeaderConstruct_2(TYPE, F0)                                                    \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: explicit TYPE(BALAU_HIPF(F0)) : BALAU_HIMF(F0) {}

#define BALAU_InjectHeaderConstruct_3(TYPE, F0, F1)                                                \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1))                                                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1) {}

#define BALAU_InjectHeaderConstruct_4(TYPE, F0, F1, F2)                                            \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2))                                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2) {}

#define BALAU_InjectHeaderConstruct_5(TYPE, F0, F1, F2, F3)                                        \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3))                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3) {}

#define BALAU_InjectHeaderConstruct_6(TYPE, F0, F1, F2, F3, F4)                                    \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3), BALAU_HIPF(F4))    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3), BALAU_HIMF(F4) {}

#define BALAU_InjectHeaderConstruct_7(TYPE, F0, F1, F2, F3, F4, F5)                                \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3),                    \
	             BALAU_HIPF(F4), BALAU_HIPF(F5))                                                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3)                            \
		, BALAU_HIMF(F4), BALAU_HIMF(F5) {}

#define BALAU_InjectHeaderConstruct_8(TYPE, F0, F1, F2, F3, F4, F5, F6)                            \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3),                    \
	             BALAU_HIPF(F4), BALAU_HIPF(F5), BALAU_HIPF(F6))                                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3)                            \
	 	, BALAU_HIMF(F4), BALAU_HIMF(F5), BALAU_HIMF(F6) {}

#define BALAU_InjectHeaderConstruct_9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7)                        \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3),                    \
	             BALAU_HIPF(F4), BALAU_HIPF(F5), BALAU_HIPF(F6), BALAU_HIPF(F7))                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3)                            \
		, BALAU_HIMF(F4), BALAU_HIMF(F5), BALAU_HIMF(F6), BALAU_HIMF(F7) {}

#define BALAU_InjectHeaderConstruct_10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8)                   \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3),                    \
	             BALAU_HIPF(F4), BALAU_HIPF(F5), BALAU_HIPF(F6), BALAU_HIPF(F7),                    \
	             BALAU_HIPF(F8))                                                                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3)                            \
		, BALAU_HIMF(F4), BALAU_HIMF(F5), BALAU_HIMF(F6), BALAU_HIMF(F7)                            \
		, BALAU_HIMF(F8) {}

#define BALAU_InjectHeaderConstruct_11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9)               \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3),                    \
	             BALAU_HIPF(F4), BALAU_HIPF(F5), BALAU_HIPF(F6), BALAU_HIPF(F7),                    \
	             BALAU_HIPF(F8), BALAU_HIPF(F9))                                                    \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3)                            \
		, BALAU_HIMF(F4), BALAU_HIMF(F5), BALAU_HIMF(F6), BALAU_HIMF(F7)                            \
		, BALAU_HIMF(F8), BALAU_HIMF(F9) {}

#define BALAU_InjectHeaderConstruct_12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10)          \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0), BALAU_HIPF(F1), BALAU_HIPF(F2), BALAU_HIPF(F3),                    \
	             BALAU_HIPF(F4), BALAU_HIPF(F5), BALAU_HIPF(F6), BALAU_HIPF(F7),                    \
	             BALAU_HIPF(F8), BALAU_HIPF(F9), BALAU_HIPF(F10))                                   \
		: BALAU_HIMF(F0), BALAU_HIMF(F1), BALAU_HIMF(F2), BALAU_HIMF(F3)                            \
		, BALAU_HIMF(F4), BALAU_HIMF(F5), BALAU_HIMF(F6), BALAU_HIMF(F7)                            \
		, BALAU_HIMF(F8), BALAU_HIMF(F9), BALAU_HIMF(F10) {}

#define BALAU_InjectHeaderConstruct_13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11)     \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0),  BALAU_HIPF(F1),  BALAU_HIPF(F2),  BALAU_HIPF(F3),                 \
	             BALAU_HIPF(F4),  BALAU_HIPF(F5),  BALAU_HIPF(F6),  BALAU_HIPF(F7),                 \
	             BALAU_HIPF(F8),  BALAU_HIPF(F9),  BALAU_HIPF(F10), BALAU_HIPF(F11))                \
		: BALAU_HIMF(F0),  BALAU_HIMF(F1),  BALAU_HIMF(F2),  BALAU_HIMF(F3)                         \
		, BALAU_HIMF(F4),  BALAU_HIMF(F5),  BALAU_HIMF(F6),  BALAU_HIMF(F7)                         \
		, BALAU_HIMF(F8),  BALAU_HIMF(F9),  BALAU_HIMF(F10), BALAU_HIMF(F11)                        \
	{}

#define BALAU_InjectHeaderConstruct_14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0),  BALAU_HIPF(F1),  BALAU_HIPF(F2),  BALAU_HIPF(F3),                 \
	             BALAU_HIPF(F4),  BALAU_HIPF(F5),  BALAU_HIPF(F6),  BALAU_HIPF(F7),                 \
	             BALAU_HIPF(F8),  BALAU_HIPF(F9),  BALAU_HIPF(F10), BALAU_HIPF(F11),                \
	             BALAU_HIPF(F12))                                                                   \
		: BALAU_HIMF(F0),  BALAU_HIMF(F1),  BALAU_HIMF(F2),  BALAU_HIMF(F3)                         \
		, BALAU_HIMF(F4),  BALAU_HIMF(F5),  BALAU_HIMF(F6),  BALAU_HIMF(F7)                         \
		, BALAU_HIMF(F8),  BALAU_HIMF(F9),  BALAU_HIMF(F10), BALAU_HIMF(F11)                        \
		, BALAU_HIMF(F12) {}

#define BALAU_InjectHeaderConstruct_15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0),  BALAU_HIPF(F1),  BALAU_HIPF(F2),  BALAU_HIPF(F3),                 \
	             BALAU_HIPF(F4),  BALAU_HIPF(F5),  BALAU_HIPF(F6),  BALAU_HIPF(F7),                 \
	             BALAU_HIPF(F8),  BALAU_HIPF(F9),  BALAU_HIPF(F10), BALAU_HIPF(F11),                \
	             BALAU_HIPF(F12), BALAU_HIPF(F13))                                                  \
		: BALAU_HIMF(F0),  BALAU_HIMF(F1),  BALAU_HIMF(F2),  BALAU_HIMF(F3)                         \
		, BALAU_HIMF(F4),  BALAU_HIMF(F5),  BALAU_HIMF(F6),  BALAU_HIMF(F7)                         \
		, BALAU_HIMF(F8),  BALAU_HIMF(F9),  BALAU_HIMF(F10), BALAU_HIMF(F11)                        \
		, BALAU_HIMF(F12), BALAU_HIMF(F13) {}

#define BALAU_InjectHeaderConstruct_16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0),  BALAU_HIPF(F1),  BALAU_HIPF(F2),  BALAU_HIPF(F3),                 \
	             BALAU_HIPF(F4),  BALAU_HIPF(F5),  BALAU_HIPF(F6),  BALAU_HIPF(F7),                 \
	             BALAU_HIPF(F8),  BALAU_HIPF(F9),  BALAU_HIPF(F10), BALAU_HIPF(F11),                \
	             BALAU_HIPF(F12), BALAU_HIPF(F13), BALAU_HIPF(F14))                                 \
		: BALAU_HIMF(F0),  BALAU_HIMF(F1),  BALAU_HIMF(F2),  BALAU_HIMF(F3)                         \
		, BALAU_HIMF(F4),  BALAU_HIMF(F5),  BALAU_HIMF(F6),  BALAU_HIMF(F7)                         \
		, BALAU_HIMF(F8),  BALAU_HIMF(F9),  BALAU_HIMF(F10), BALAU_HIMF(F11)                        \
		, BALAU_HIMF(F12), BALAU_HIMF(F13), BALAU_HIMF(F14) {}

#define BALAU_InjectHeaderConstruct_17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	BALAU_InjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(BALAU_HIPF(F0),  BALAU_HIPF(F1),  BALAU_HIPF(F2),  BALAU_HIPF(F3)                  \
	             BALAU_HIPF(F4),  BALAU_HIPF(F5),  BALAU_HIPF(F6),  BALAU_HIPF(F7)                  \
	             BALAU_HIPF(F8),  BALAU_HIPF(F9),  BALAU_HIPF(F10), BALAU_HIPF(F11)                 \
	             BALAU_HIPF(F12), BALAU_HIPF(F13), BALAU_HIPF(F14), BALAU_HIPF(F15))                \
		: BALAU_HIMF(F0),  BALAU_HIMF(F1),  BALAU_HIMF(F2),  BALAU_HIMF(F3)                         \
		, BALAU_HIMF(F4),  BALAU_HIMF(F5),  BALAU_HIMF(F6),  BALAU_HIMF(F7)                         \
		, BALAU_HIMF(F8),  BALAU_HIMF(F9),  BALAU_HIMF(F10), BALAU_HIMF(F11)                        \
		, BALAU_HIMF(F12), BALAU_HIMF(F13), BALAU_HIMF(F14), BALAU_HIMF(F15) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define BALAU_InjectHeaderConstruct_(N)          BALAU_InjectHeaderConstruct_##N
#define BALAU_InjectHeaderConstruct_Eval(N)      BALAU_InjectHeaderConstruct_(N)

#define BALAU_InjectHeaderConstruct(...)          BALAU_InjectHeaderConstruct_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_INJECTOR_HEADER_MACROS
