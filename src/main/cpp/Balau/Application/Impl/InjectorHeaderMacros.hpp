// @formatter:off
//
// Balau core C++ library
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
#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_HEADER_MACROS
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_HEADER_MACROS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Util/Macros.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define _BalauHIPF(F)    decltype(F) F ## _
#define _BalauHIMF(F)    F(std::forward<decltype(F)>(F ## _))

namespace Balau {

class Injector;

} // namespace Balau

///////////////////////////////////// _BalauInjectHeader macros /////////////////////////////////////

#define _BalauInjectHeader(TYPE)                                                                    \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & );                       \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & );                        \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys()

/////////////////////////////  _BalauInjectHeaderConstructNamed macros //////////////////////////////

#define _BalauInjectHeaderConstruct__1(TYPE)                                                        \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE() {}

#define _BalauInjectHeaderConstruct__2(TYPE, F0)                                                    \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: explicit TYPE(_BalauHIPF(F0)) : _BalauHIMF(F0) {}

#define _BalauInjectHeaderConstruct__3(TYPE, F0, F1)                                                \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1))                                                    \
		: _BalauHIMF(F0), _BalauHIMF(F1) {}

#define _BalauInjectHeaderConstruct__4(TYPE, F0, F1, F2)                                            \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2))                                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2) {}

#define _BalauInjectHeaderConstruct__5(TYPE, F0, F1, F2, F3)                                        \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3))                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3) {}

#define _BalauInjectHeaderConstruct__6(TYPE, F0, F1, F2, F3, F4)                                    \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3), _BalauHIPF(F4))    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3), _BalauHIMF(F4) {}

#define _BalauInjectHeaderConstruct__7(TYPE, F0, F1, F2, F3, F4, F5)                                \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3),                    \
	             _BalauHIPF(F4), _BalauHIPF(F5))                                                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3)                            \
		, _BalauHIMF(F4), _BalauHIMF(F5) {}

#define _BalauInjectHeaderConstruct__8(TYPE, F0, F1, F2, F3, F4, F5, F6)                            \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3),                    \
	             _BalauHIPF(F4), _BalauHIPF(F5), _BalauHIPF(F6))                                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3)                            \
	 	, _BalauHIMF(F4), _BalauHIMF(F5), _BalauHIMF(F6) {}

#define _BalauInjectHeaderConstruct__9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7)                        \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3),                    \
	             _BalauHIPF(F4), _BalauHIPF(F5), _BalauHIPF(F6), _BalauHIPF(F7))                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3)                            \
		, _BalauHIMF(F4), _BalauHIMF(F5), _BalauHIMF(F6), _BalauHIMF(F7) {}

#define _BalauInjectHeaderConstruct__10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8)                   \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3),                    \
	             _BalauHIPF(F4), _BalauHIPF(F5), _BalauHIPF(F6), _BalauHIPF(F7),                    \
	             _BalauHIPF(F8))                                                                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3)                            \
		, _BalauHIMF(F4), _BalauHIMF(F5), _BalauHIMF(F6), _BalauHIMF(F7)                            \
		, _BalauHIMF(F8) {}

#define _BalauInjectHeaderConstruct__11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9)               \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3),                    \
	             _BalauHIPF(F4), _BalauHIPF(F5), _BalauHIPF(F6), _BalauHIPF(F7),                    \
	             _BalauHIPF(F8), _BalauHIPF(F9))                                                    \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3)                            \
		, _BalauHIMF(F4), _BalauHIMF(F5), _BalauHIMF(F6), _BalauHIMF(F7)                            \
		, _BalauHIMF(F8), _BalauHIMF(F9) {}

#define _BalauInjectHeaderConstruct__12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10)          \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0), _BalauHIPF(F1), _BalauHIPF(F2), _BalauHIPF(F3),                    \
	             _BalauHIPF(F4), _BalauHIPF(F5), _BalauHIPF(F6), _BalauHIPF(F7),                    \
	             _BalauHIPF(F8), _BalauHIPF(F9), _BalauHIPF(F10))                                   \
		: _BalauHIMF(F0), _BalauHIMF(F1), _BalauHIMF(F2), _BalauHIMF(F3)                            \
		, _BalauHIMF(F4), _BalauHIMF(F5), _BalauHIMF(F6), _BalauHIMF(F7)                            \
		, _BalauHIMF(F8), _BalauHIMF(F9), _BalauHIMF(F10) {}

#define _BalauInjectHeaderConstruct__13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11)     \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0),  _BalauHIPF(F1),  _BalauHIPF(F2),  _BalauHIPF(F3),                 \
	             _BalauHIPF(F4),  _BalauHIPF(F5),  _BalauHIPF(F6),  _BalauHIPF(F7),                 \
	             _BalauHIPF(F8),  _BalauHIPF(F9),  _BalauHIPF(F10), _BalauHIPF(F11))                \
		: _BalauHIMF(F0),  _BalauHIMF(F1),  _BalauHIMF(F2),  _BalauHIMF(F3)                         \
		, _BalauHIMF(F4),  _BalauHIMF(F5),  _BalauHIMF(F6),  _BalauHIMF(F7)                         \
		, _BalauHIMF(F8),  _BalauHIMF(F9),  _BalauHIMF(F10), _BalauHIMF(F11)                        \
	{}

#define _BalauInjectHeaderConstruct__14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0),  _BalauHIPF(F1),  _BalauHIPF(F2),  _BalauHIPF(F3),                 \
	             _BalauHIPF(F4),  _BalauHIPF(F5),  _BalauHIPF(F6),  _BalauHIPF(F7),                 \
	             _BalauHIPF(F8),  _BalauHIPF(F9),  _BalauHIPF(F10), _BalauHIPF(F11),                \
	             _BalauHIPF(F12))                                                                   \
		: _BalauHIMF(F0),  _BalauHIMF(F1),  _BalauHIMF(F2),  _BalauHIMF(F3)                         \
		, _BalauHIMF(F4),  _BalauHIMF(F5),  _BalauHIMF(F6),  _BalauHIMF(F7)                         \
		, _BalauHIMF(F8),  _BalauHIMF(F9),  _BalauHIMF(F10), _BalauHIMF(F11)                        \
		, _BalauHIMF(F12) {}

#define _BalauInjectHeaderConstruct__15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0),  _BalauHIPF(F1),  _BalauHIPF(F2),  _BalauHIPF(F3),                 \
	             _BalauHIPF(F4),  _BalauHIPF(F5),  _BalauHIPF(F6),  _BalauHIPF(F7),                 \
	             _BalauHIPF(F8),  _BalauHIPF(F9),  _BalauHIPF(F10), _BalauHIPF(F11),                \
	             _BalauHIPF(F12), _BalauHIPF(F13))                                                  \
		: _BalauHIMF(F0),  _BalauHIMF(F1),  _BalauHIMF(F2),  _BalauHIMF(F3)                         \
		, _BalauHIMF(F4),  _BalauHIMF(F5),  _BalauHIMF(F6),  _BalauHIMF(F7)                         \
		, _BalauHIMF(F8),  _BalauHIMF(F9),  _BalauHIMF(F10), _BalauHIMF(F11)                        \
		, _BalauHIMF(F12), _BalauHIMF(F13) {}

#define _BalauInjectHeaderConstruct__16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0),  _BalauHIPF(F1),  _BalauHIPF(F2),  _BalauHIPF(F3),                 \
	             _BalauHIPF(F4),  _BalauHIPF(F5),  _BalauHIPF(F6),  _BalauHIPF(F7),                 \
	             _BalauHIPF(F8),  _BalauHIPF(F9),  _BalauHIPF(F10), _BalauHIPF(F11),                \
	             _BalauHIPF(F12), _BalauHIPF(F13), _BalauHIPF(F14))                                 \
		: _BalauHIMF(F0),  _BalauHIMF(F1),  _BalauHIMF(F2),  _BalauHIMF(F3)                         \
		, _BalauHIMF(F4),  _BalauHIMF(F5),  _BalauHIMF(F6),  _BalauHIMF(F7)                         \
		, _BalauHIMF(F8),  _BalauHIMF(F9),  _BalauHIMF(F10), _BalauHIMF(F11)                        \
		, _BalauHIMF(F12), _BalauHIMF(F13), _BalauHIMF(F14) {}

#define _BalauInjectHeaderConstruct__17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	_BalauInjectHeader(TYPE);                                                                       \
	                                                                                                \
	public: TYPE(_BalauHIPF(F0),  _BalauHIPF(F1),  _BalauHIPF(F2),  _BalauHIPF(F3)                  \
	             _BalauHIPF(F4),  _BalauHIPF(F5),  _BalauHIPF(F6),  _BalauHIPF(F7)                  \
	             _BalauHIPF(F8),  _BalauHIPF(F9),  _BalauHIPF(F10), _BalauHIPF(F11)                 \
	             _BalauHIPF(F12), _BalauHIPF(F13), _BalauHIPF(F14), _BalauHIPF(F15))                \
		: _BalauHIMF(F0),  _BalauHIMF(F1),  _BalauHIMF(F2),  _BalauHIMF(F3)                         \
		, _BalauHIMF(F4),  _BalauHIMF(F5),  _BalauHIMF(F6),  _BalauHIMF(F7)                         \
		, _BalauHIMF(F8),  _BalauHIMF(F9),  _BalauHIMF(F10), _BalauHIMF(F11)                        \
		, _BalauHIMF(F12), _BalauHIMF(F13), _BalauHIMF(F14), _BalauHIMF(F15) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define _BalauInjectHeaderConstruct__(N)          _BalauInjectHeaderConstruct__##N
#define _BalauInjectHeaderConstruct__Eval(N)      _BalauInjectHeaderConstruct__(N)

#define _BalauInjectHeaderConstruct(...)          _BalauInjectHeaderConstruct__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_HEADER_MACROS
