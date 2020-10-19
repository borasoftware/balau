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
#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_MACROS
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_MACROS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Util/Macros.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define _BalauIGF(F, N) _BalauInjector.getInstance<decltype(F)>(N)
#define _BalauIGT(T, N) _BalauInjector.getInstance<T>(N)
#define _BalauIPF(F)    decltype(F) F ## _
#define _BalauIMF(F)    F(std::forward<decltype(F)>(F ## _))
#define _BalauIKF(F, N) ::Balau::Impl::CreateBindingKey<decltype(F)>()(N)
#define _BalauIKT(T, N) ::Balau::Impl::CreateBindingKey<T>()(N)

////////////////////////////////////// BalauInjectNamed macros //////////////////////////////////////

#define _BalauInjectNamed__1(TYPE)                                                                  \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & ) {                      \
		return new TYPE();                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & ) {                       \
		return TYPE();                                                                              \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {};                                                                                  \
	}

#define _BalauInjectNamed__3(TYPE, F0, N0)                                                          \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(_BalauIGF(F0, N0));                                                         \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(_BalauIGF(F0, N0));                                                             \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return { _BalauIKF(F0, N0) };                                                               \
	}

#define _BalauInjectNamed__5(TYPE, F0, N0, F1, N1)                                                  \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(_BalauIGF(F0, N0), _BalauIGF(F1, N1));                                      \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(_BalauIGF(F0, N0), _BalauIGF(F1, N1));                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return { _BalauIKF(F0, N0), _BalauIKF(F1, N1) };                                            \
	}

#define _BalauInjectNamed__7(TYPE, F0, N0, F1, N1, F2, N2)                                          \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(_BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2));                   \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(_BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2));                       \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return { _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2) };                         \
	}

#define _BalauInjectNamed__9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                                  \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			_BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			_BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			_BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)              \
		};                                                                                          \
	}

#define _BalauInjectNamed__11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                         \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4)                                                                     \
		};                                                                                          \
	}

#define _BalauInjectNamed__13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)                 \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5)                                                  \
		};                                                                                          \
	}

#define _BalauInjectNamed__15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6)         \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6)                               \
		};                                                                                          \
	}

#define _BalauInjectNamed__17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6), _BalauIKF(F7, N7)            \
		};                                                                                          \
	}

#define _BalauInjectNamed__19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
			, _BalauIGF(F8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
			, _BalauIGF(F8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6), _BalauIKF(F7, N7)            \
			, _BalauIKF(F8, N8)                                                                     \
		};                                                                                          \
	}

#define _BalauInjectNamed__21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6), _BalauIKF(F7, N7)            \
			, _BalauIKF(F8, N8), _BalauIKF(F9, N9)                                                  \
		};                                                                                          \
	}

#define _BalauInjectNamed__23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9), _BalauIGF(F10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2), _BalauIGF(F3, N3)            \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6), _BalauIGF(F7, N7)            \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9), _BalauIGF(F10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2), _BalauIKF(F3, N3)            \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6), _BalauIKF(F7, N7)            \
			, _BalauIKF(F8, N8), _BalauIKF(F9, N9), _BalauIKF(F10, N10)                             \
		};                                                                                          \
	}

#define _BalauInjectNamed__25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2),   _BalauIGF(F3, N3)          \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6),   _BalauIGF(F7, N7)          \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9), _BalauIGF(F10, N10), _BalauIGF(F11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2),   _BalauIGF(F3, N3)          \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6),   _BalauIGF(F7, N7)          \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9), _BalauIGF(F10, N10), _BalauIGF(F11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2),   _BalauIKF(F3, N3)          \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6),   _BalauIKF(F7, N7)          \
			, _BalauIKF(F8, N8), _BalauIKF(F9, N9), _BalauIKF(F10, N10), _BalauIKF(F11, N11)        \
		};                                                                                          \
	}

#define _BalauInjectNamed__27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2),   _BalauIGF(F3, N3)          \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6),   _BalauIGF(F7, N7)          \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9), _BalauIGF(F10, N10), _BalauIGF(F11, N11)        \
			, _BalauIGF(F12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0), _BalauIGF(F1, N1), _BalauIGF(F2, N2),   _BalauIGF(F3, N3)          \
			, _BalauIGF(F4, N4), _BalauIGF(F5, N5), _BalauIGF(F6, N6),   _BalauIGF(F7, N7)          \
			, _BalauIGF(F8, N8), _BalauIGF(F9, N9), _BalauIGF(F10, N10), _BalauIGF(F11, N11)        \
			, _BalauIGF(F12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0), _BalauIKF(F1, N1), _BalauIKF(F2, N2),   _BalauIKF(F3, N3)          \
			, _BalauIKF(F4, N4), _BalauIKF(F5, N5), _BalauIKF(F6, N6),   _BalauIKF(F7, N7)          \
			, _BalauIKF(F8, N8), _BalauIKF(F9, N9), _BalauIKF(F10, N10), _BalauIKF(F11, N11)        \
			, _BalauIKF(F12, N12)                                                                   \
		};                                                                                          \
	}

#define _BalauInjectNamed__29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0),   _BalauIGF(F1, N1),   _BalauIGF(F2, N2),   _BalauIGF(F3, N3)      \
			, _BalauIGF(F4, N4),   _BalauIGF(F5, N5),   _BalauIGF(F6, N6),   _BalauIGF(F7, N7)      \
			, _BalauIGF(F8, N8),   _BalauIGF(F9, N9),   _BalauIGF(F10, N10), _BalauIGF(F11, N11)    \
			, _BalauIGF(F12, N12), _BalauIGF(F13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0),   _BalauIGF(F1, N1),   _BalauIGF(F2, N2),   _BalauIGF(F3, N3)      \
			, _BalauIGF(F4, N4),   _BalauIGF(F5, N5),   _BalauIGF(F6, N6),   _BalauIGF(F7, N7)      \
			, _BalauIGF(F8, N8),   _BalauIGF(F9, N9),   _BalauIGF(F10, N10), _BalauIGF(F11, N11)    \
			, _BalauIGF(F12, N12), _BalauIGF(F13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0),   _BalauIKF(F1, N1), _BalauIKF(F2, N2),   _BalauIKF(F3, N3)        \
			, _BalauIKF(F4, N4),   _BalauIKF(F5, N5), _BalauIKF(F6, N6),   _BalauIKF(F7, N7)        \
			, _BalauIKF(F8, N8),   _BalauIKF(F9, N9), _BalauIKF(F10, N10), _BalauIKF(F11, N11)      \
			, _BalauIKF(F12, N12), _BalauIKF(F13, N13)                                              \
		};                                                                                          \
	}

#define _BalauInjectNamed__31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0),   _BalauIGF(F1, N1),   _BalauIGF(F2, N2),   _BalauIGF(F3, N3)      \
			, _BalauIGF(F4, N4),   _BalauIGF(F5, N5),   _BalauIGF(F6, N6),   _BalauIGF(F7, N7)      \
			, _BalauIGF(F8, N8),   _BalauIGF(F9, N9),   _BalauIGF(F10, N10), _BalauIGF(F11, N11)    \
			, _BalauIGF(F12, N12), _BalauIGF(F13, N13), _BalauIGF(F14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0),   _BalauIGF(F1, N1),   _BalauIGF(F2, N2),   _BalauIGF(F3, N3)      \
			, _BalauIGF(F4, N4),   _BalauIGF(F5, N5),   _BalauIGF(F6, N6),   _BalauIGF(F7, N7)      \
			, _BalauIGF(F8, N8),   _BalauIGF(F9, N9),   _BalauIGF(F10, N10), _BalauIGF(F11, N11)    \
			, _BalauIGF(F12, N12), _BalauIGF(F13, N13), _BalauIGF(F14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0),   _BalauIKF(F1, N1),   _BalauIKF(F2, N2),   _BalauIKF(F3, N3)      \
			, _BalauIKF(F4, N4),   _BalauIKF(F5, N5),   _BalauIKF(F6, N6),   _BalauIKF(F7, N7)      \
			, _BalauIKF(F8, N8),   _BalauIKF(F9, N9),   _BalauIKF(F10, N10), _BalauIKF(F11, N11)    \
			, _BalauIKF(F12, N12), _BalauIKF(F13, N13), _BalauIKF(F14, N14)                         \
		};                                                                                          \
	}

#define _BalauInjectNamed__33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGF(F0, N0),   _BalauIGF(F1, N1),   _BalauIGF(F2, N2),   _BalauIGF(F3, N3)      \
			, _BalauIGF(F4, N4),   _BalauIGF(F5, N5),   _BalauIGF(F6, N6),   _BalauIGF(F7, N7)      \
			, _BalauIGF(F8, N8),   _BalauIGF(F9, N9),   _BalauIGF(F10, N10), _BalauIGF(F11, N11)    \
			, _BalauIGF(F12, N12), _BalauIGF(F13, N13), _BalauIGF(F14, N14), _BalauIGF(F15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGF(F0, N0),   _BalauIGF(F1, N1),   _BalauIGF(F2, N2),   _BalauIGF(F3, N3)      \
			, _BalauIGF(F4, N4),   _BalauIGF(F5, N5),   _BalauIGF(F6, N6),   _BalauIGF(F7, N7)      \
			, _BalauIGF(F8, N8),   _BalauIGF(F9, N9),   _BalauIGF(F10, N10), _BalauIGF(F11, N11)    \
			, _BalauIGF(F12, N12), _BalauIGF(F13, N13), _BalauIGF(F14, N14), _BalauIGF(F15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKF(F0, N0),   _BalauIKF(F1, N1),   _BalauIKF(F2, N2),   _BalauIKF(F3, N3)      \
			, _BalauIKF(F4, N4),   _BalauIKF(F5, N5),   _BalauIKF(F6, N6),   _BalauIKF(F7, N7)      \
			, _BalauIKF(F8, N8),   _BalauIKF(F9, N9),   _BalauIKF(F10, N10), _BalauIKF(F11, N11)    \
			, _BalauIKF(F12, N12), _BalauIKF(F13, N13), _BalauIKF(F14, N14), _BalauIKF(F15, N15)    \
		};                                                                                          \
	}

///////////////////////////////// _BalauInjectConstructNamed macros /////////////////////////////////

#define _BalauInjectConstructNamed__1(TYPE)                                                         \
	_BalauInjectNamed__1(TYPE)                                                                      \
	                                                                                                \
	public: TYPE() {}

#define _BalauInjectConstructNamed__3(TYPE, F0, N0)                                                 \
	_BalauInjectNamed__3(TYPE, F0, N0)                                                              \
	                                                                                                \
	public: explicit TYPE(_BalauIPF(F0)) : _BalauIMF(F0) {}

#define _BalauInjectConstructNamed__5(TYPE, F0, N0, F1, N1)                                         \
	_BalauInjectNamed__5(TYPE, F0, N0, F1, N1)                                                      \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1))                                                      \
		: _BalauIMF(F0), _BalauIMF(F1) {}

#define _BalauInjectConstructNamed__7(TYPE, F0, N0, F1, N1, F2, N2)                                 \
	_BalauInjectNamed__7(TYPE, F0, N0, F1, N1, F2, N2)                                              \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2))                                       \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2) {}

#define _BalauInjectConstructNamed__9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                         \
	_BalauInjectNamed__9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                                      \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3))                        \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3) {}

#define _BalauInjectConstructNamed__11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                \
	_BalauInjectNamed__11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                             \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3), _BalauIPF(F4))         \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3), _BalauIMF(F4) {}

#define _BalauInjectConstructNamed__13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)        \
	_BalauInjectNamed__13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)                     \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3),                        \
	             _BalauIPF(F4), _BalauIPF(F5))                                                      \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3)                                \
		, _BalauIMF(F4), _BalauIMF(F5) {}

#define _BalauInjectConstructNamed__15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6) \
	_BalauInjectNamed__15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6)             \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3),                        \
	             _BalauIPF(F4), _BalauIPF(F5), _BalauIPF(F6))                                       \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3)                                \
	 	, _BalauIMF(F4), _BalauIMF(F5), _BalauIMF(F6) {}

#define _BalauInjectConstructNamed__17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7) \
	_BalauInjectNamed__17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7)     \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3),                        \
	             _BalauIPF(F4), _BalauIPF(F5), _BalauIPF(F6), _BalauIPF(F7))                        \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3)                                \
		, _BalauIMF(F4), _BalauIMF(F5), _BalauIMF(F6), _BalauIMF(F7) {}

#define _BalauInjectConstructNamed__19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	_BalauInjectNamed__19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3),                        \
	             _BalauIPF(F4), _BalauIPF(F5), _BalauIPF(F6), _BalauIPF(F7),                        \
	             _BalauIPF(F8))                                                                     \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3)                                \
		, _BalauIMF(F4), _BalauIMF(F5), _BalauIMF(F6), _BalauIMF(F7)                                \
		, _BalauIMF(F8) {}

#define _BalauInjectConstructNamed__21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	_BalauInjectNamed__21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3),                        \
	             _BalauIPF(F4), _BalauIPF(F5), _BalauIPF(F6), _BalauIPF(F7),                        \
	             _BalauIPF(F8), _BalauIPF(F9))                                                      \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3)                                \
		, _BalauIMF(F4), _BalauIMF(F5), _BalauIMF(F6), _BalauIMF(F7)                                \
		, _BalauIMF(F8), _BalauIMF(F9) {}

#define _BalauInjectConstructNamed__23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	_BalauInjectNamed__23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0), _BalauIPF(F1), _BalauIPF(F2), _BalauIPF(F3),                        \
	             _BalauIPF(F4), _BalauIPF(F5), _BalauIPF(F6), _BalauIPF(F7),                        \
	             _BalauIPF(F8), _BalauIPF(F9), _BalauIPF(F10))                                      \
		: _BalauIMF(F0), _BalauIMF(F1), _BalauIMF(F2), _BalauIMF(F3)                                \
		, _BalauIMF(F4), _BalauIMF(F5), _BalauIMF(F6), _BalauIMF(F7)                                \
		, _BalauIMF(F8), _BalauIMF(F9), _BalauIMF(F10) {}

#define _BalauInjectConstructNamed__25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	_BalauInjectNamed__25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0),  _BalauIPF(F1),  _BalauIPF(F2),  _BalauIPF(F3),                     \
	             _BalauIPF(F4),  _BalauIPF(F5),  _BalauIPF(F6),  _BalauIPF(F7),                     \
	             _BalauIPF(F8),  _BalauIPF(F9),  _BalauIPF(F10), _BalauIPF(F11))                    \
		: _BalauIMF(F0),  _BalauIMF(F1),  _BalauIMF(F2),  _BalauIMF(F3)                             \
		, _BalauIMF(F4),  _BalauIMF(F5),  _BalauIMF(F6),  _BalauIMF(F7)                             \
		, _BalauIMF(F8),  _BalauIMF(F9),  _BalauIMF(F10), _BalauIMF(F11)                            \
	{}

#define _BalauInjectConstructNamed__27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	_BalauInjectNamed__27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0),  _BalauIPF(F1),  _BalauIPF(F2),  _BalauIPF(F3),                     \
	             _BalauIPF(F4),  _BalauIPF(F5),  _BalauIPF(F6),  _BalauIPF(F7),                     \
	             _BalauIPF(F8),  _BalauIPF(F9),  _BalauIPF(F10), _BalauIPF(F11),                    \
	             _BalauIPF(F12))                                                                    \
		: _BalauIMF(F0),  _BalauIMF(F1),  _BalauIMF(F2),  _BalauIMF(F3)                             \
		, _BalauIMF(F4),  _BalauIMF(F5),  _BalauIMF(F6),  _BalauIMF(F7)                             \
		, _BalauIMF(F8),  _BalauIMF(F9),  _BalauIMF(F10), _BalauIMF(F11)                            \
		, _BalauIMF(F12) {}

#define _BalauInjectConstructNamed__29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	_BalauInjectNamed__29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0),  _BalauIPF(F1),  _BalauIPF(F2),  _BalauIPF(F3),                     \
	             _BalauIPF(F4),  _BalauIPF(F5),  _BalauIPF(F6),  _BalauIPF(F7),                     \
	             _BalauIPF(F8),  _BalauIPF(F9),  _BalauIPF(F10), _BalauIPF(F11),                    \
	             _BalauIPF(F12), _BalauIPF(F13))                                                    \
		: _BalauIMF(F0),  _BalauIMF(F1),  _BalauIMF(F2),  _BalauIMF(F3)                             \
		, _BalauIMF(F4),  _BalauIMF(F5),  _BalauIMF(F6),  _BalauIMF(F7)                             \
		, _BalauIMF(F8),  _BalauIMF(F9),  _BalauIMF(F10), _BalauIMF(F11)                            \
		, _BalauIMF(F12), _BalauIMF(F13) {}

#define _BalauInjectConstructNamed__31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	_BalauInjectNamed__31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0),  _BalauIPF(F1),  _BalauIPF(F2),  _BalauIPF(F3),                     \
	             _BalauIPF(F4),  _BalauIPF(F5),  _BalauIPF(F6),  _BalauIPF(F7),                     \
	             _BalauIPF(F8),  _BalauIPF(F9),  _BalauIPF(F10), _BalauIPF(F11),                    \
	             _BalauIPF(F12), _BalauIPF(F13), _BalauIPF(F14))                                    \
		: _BalauIMF(F0),  _BalauIMF(F1),  _BalauIMF(F2),  _BalauIMF(F3)                             \
		, _BalauIMF(F4),  _BalauIMF(F5),  _BalauIMF(F6),  _BalauIMF(F7)                             \
		, _BalauIMF(F8),  _BalauIMF(F9),  _BalauIMF(F10), _BalauIMF(F11)                            \
		, _BalauIMF(F12), _BalauIMF(F13), _BalauIMF(F14) {}

#define _BalauInjectConstructNamed__33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	_BalauInjectNamed__33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	                                                                                                \
	public: TYPE(_BalauIPF(F0),  _BalauIPF(F1),  _BalauIPF(F2),  _BalauIPF(F3)                      \
	             _BalauIPF(F4),  _BalauIPF(F5),  _BalauIPF(F6),  _BalauIPF(F7)                      \
	             _BalauIPF(F8),  _BalauIPF(F9),  _BalauIPF(F10), _BalauIPF(F11)                     \
	             _BalauIPF(F12), _BalauIPF(F13), _BalauIPF(F14), _BalauIPF(F15))                    \
		: _BalauIMF(F0),  _BalauIMF(F1),  _BalauIMF(F2),  _BalauIMF(F3)                             \
		, _BalauIMF(F4),  _BalauIMF(F5),  _BalauIMF(F6),  _BalauIMF(F7)                             \
		, _BalauIMF(F8),  _BalauIMF(F9),  _BalauIMF(F10), _BalauIMF(F11)                            \
		, _BalauIMF(F12), _BalauIMF(F13), _BalauIMF(F14), _BalauIMF(F15) {}

/////////////////////////////////// BalauInjectNamedTypes macros ////////////////////////////////////

#define _BalauInjectNamedTypes__1(TYPE)                                                             \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE();                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE();                                                                              \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {};                                                                                  \
	}

#define _BalauInjectNamedTypes__3(TYPE, T0, N0)                                                     \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(_BalauIGT(T0, N0));                                                         \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(_BalauIGT(T0, N0));                                                             \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return { _BalauIKT(T0, N0) };                                                               \
	}

#define _BalauInjectNamedTypes__5(TYPE, T0, N0, T1, N1)                                             \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(_BalauIGT(T0, N0), _BalauIGT(T1, N1));                                      \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(_BalauIGT(T0, N0), _BalauIGT(T1, N1));                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return { _BalauIKT(T0, N0), _BalauIKT(T1, N1) };                                            \
	}

#define _BalauInjectNamedTypes__7(TYPE, T0, N0, T1, N1, T2, N2)                                     \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(_BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2));                   \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(_BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2));                       \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return { _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2) };                         \
	}

#define _BalauInjectNamedTypes__9(TYPE, T0, N0, T1, N1, T2, N2, T3, N3)                             \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			_BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			_BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			_BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)              \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__11(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4)                    \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4)                                                                     \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__13(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5)            \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5)                                                  \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__15(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6)    \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5), _BalauIKT(T6, N6)                               \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__17(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5), _BalauIKT(T6, N6), _BalauIKT(T7, N7)            \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__19(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
			, _BalauIGT(T8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
			, _BalauIGT(T8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5), _BalauIKT(T6, N6), _BalauIKT(T7, N7)            \
			, _BalauIKT(T8, N8)                                                                     \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__21(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
			, _BalauIGT(T8, N8), _BalauIGT(T9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
			, _BalauIGT(T8, N8), _BalauIGT(T9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5), _BalauIKT(T6, N6), _BalauIKT(T7, N7)            \
			, _BalauIKT(T8, N8), _BalauIKT(T9, N9)                                                  \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__23(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
			, _BalauIGT(T8, N8), _BalauIGT(T9, N9), _BalauIGT(T10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2), _BalauIGT(T3, N3)            \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6), _BalauIGT(T7, N7)            \
			, _BalauIGT(T8, N8), _BalauIGT(T9, N9), _BalauIGT(T10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2), _BalauIKT(T3, N3)            \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5), _BalauIKT(T6, N6), _BalauIKT(T7, N7)            \
			, _BalauIKT(T8, N8), _BalauIKT(T9, N9), _BalauIKT(T10, N10)                             \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__25(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2),   _BalauIGT(T3, N3)          \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6),   _BalauIGT(T7, N7)          \
			, _BalauIGT(T8, N8), _BalauIGT(T9, N9), _BalauIGT(T10, N10), _BalauIGT(T11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0), _BalauIGT(T1, N1), _BalauIGT(T2, N2),   _BalauIGT(T3, N3)          \
			, _BalauIGT(T4, N4), _BalauIGT(T5, N5), _BalauIGT(T6, N6),   _BalauIGT(T7, N7)          \
			, _BalauIGT(T8, N8), _BalauIGT(T9, N9), _BalauIGT(T10, N10), _BalauIGT(T11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0), _BalauIKT(T1, N1), _BalauIKT(T2, N2),   _BalauIKT(T3, N3)          \
			, _BalauIKT(T4, N4), _BalauIKT(T5, N5), _BalauIKT(T6, N6),   _BalauIKT(T7, N7)          \
			, _BalauIKT(T8, N8), _BalauIKT(T9, N9), _BalauIKT(T10, N10), _BalauIKT(T11, N11)        \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__27(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0),   _BalauIKT(T1, N1), _BalauIKT(T2, N2),   _BalauIKT(T3, N3)        \
			, _BalauIKT(T4, N4),   _BalauIKT(T5, N5), _BalauIKT(T6, N6),   _BalauIKT(T7, N7)        \
			, _BalauIKT(T8, N8),   _BalauIKT(T9, N9), _BalauIKT(T10, N10), _BalauIKT(T11, N11)      \
			, _BalauIKT(T12, N12)                                                                   \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__29(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12), _BalauIGT(T13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12), _BalauIGT(T13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0),   _BalauIKT(T1, N1), _BalauIKT(T2, N2),   _BalauIKT(T3, N3)        \
			, _BalauIKT(T4, N4),   _BalauIKT(T5, N5), _BalauIKT(T6, N6),   _BalauIKT(T7, N7)        \
			, _BalauIKT(T8, N8),   _BalauIKT(T9, N9), _BalauIKT(T10, N10), _BalauIKT(T11, N11)      \
			, _BalauIKT(T12, N12), _BalauIKT(T13, N13)                                              \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__31(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12), _BalauIGT(T13, N13), _BalauIGT(T14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12), _BalauIGT(T13, N13), _BalauIGT(T14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0),   _BalauIKT(T1, N1),   _BalauIKT(T2, N2),   _BalauIKT(T3, N3)      \
			, _BalauIKT(T4, N4),   _BalauIKT(T5, N5),   _BalauIKT(T6, N6),   _BalauIKT(T7, N7)      \
			, _BalauIKT(T8, N8),   _BalauIKT(T9, N9),   _BalauIKT(T10, N10), _BalauIKT(T11, N11)    \
			, _BalauIKT(T12, N12), _BalauIKT(T13, N13), _BalauIKT(T14, N14)                         \
		};                                                                                          \
	}

#define _BalauInjectNamedTypes__33(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14, T15, N15) \
	public: static TYPE * _Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {        \
		return new TYPE(                                                                            \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12), _BalauIGT(T13, N13), _BalauIGT(T14, N14), _BalauIGT(T15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE _Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {         \
		return TYPE(                                                                                \
			  _BalauIGT(T0, N0),   _BalauIGT(T1, N1),   _BalauIGT(T2, N2),   _BalauIGT(T3, N3)      \
			, _BalauIGT(T4, N4),   _BalauIGT(T5, N5),   _BalauIGT(T6, N6),   _BalauIGT(T7, N7)      \
			, _BalauIGT(T8, N8),   _BalauIGT(T9, N9),   _BalauIGT(T10, N10), _BalauIGT(T11, N11)    \
			, _BalauIGT(T12, N12), _BalauIGT(T13, N13), _BalauIGT(T14, N14), _BalauIGT(T15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> _Balau_getDependencyKeys() {              \
		return {                                                                                    \
			  _BalauIKT(T0, N0),   _BalauIKT(T1, N1),   _BalauIKT(T2, N2),   _BalauIKT(T3, N3)      \
			, _BalauIKT(T4, N4),   _BalauIKT(T5, N5),   _BalauIKT(T6, N6),   _BalauIKT(T7, N7)      \
			, _BalauIKT(T8, N8),   _BalauIKT(T9, N9),   _BalauIKT(T10, N10), _BalauIKT(T11, N11)    \
			, _BalauIKT(T12, N12), _BalauIKT(T13, N13), _BalauIKT(T14, N14), _BalauIKT(T15, N15)    \
		};                                                                                          \
	}

//////////////////////////////////////// BalauInject macros /////////////////////////////////////////

#define _BalauInject__1(TYPE) \
	_BalauInjectNamed__1(TYPE)

#define _BalauInject__2(TYPE, F0) \
	_BalauInjectNamed__3(TYPE, F0, "")

#define _BalauInject__3(TYPE, F0, F1) \
	_BalauInjectNamed__5(TYPE, F0, "", F1, "")

#define _BalauInject__4(TYPE, F0, F1, F2) \
	_BalauInjectNamed__7(TYPE, F0, "", F1, "", F2, "")

#define _BalauInject__5(TYPE, F0, F1, F2, F3) \
	_BalauInjectNamed__9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define _BalauInject__6(TYPE, F0, F1, F2, F3, F4) \
	_BalauInjectNamed__11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define _BalauInject__7(TYPE, F0, F1, F2, F3, F4, F5) \
	_BalauInjectNamed__13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define _BalauInject__8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	_BalauInjectNamed__15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define _BalauInject__9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	_BalauInjectNamed__17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define _BalauInject__10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	_BalauInjectNamed__19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define _BalauInject__11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	_BalauInjectNamed__21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define _BalauInject__12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	_BalauInjectNamed__23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define _BalauInject__13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	_BalauInjectNamed__25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define _BalauInject__14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	_BalauInjectNamed__27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define _BalauInject__15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	_BalauInjectNamed__29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define _BalauInject__16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	_BalauInjectNamed__31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define _BalauInject__17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	_BalauInjectNamed__33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

//////////////////////////////////// BalauInjectConstruct macros ////////////////////////////////////

#define _BalauInjectConstruct__1(TYPE) \
	_BalauInjectConstructNamed__1(TYPE)

#define _BalauInjectConstruct__2(TYPE, F0) \
	_BalauInjectConstructNamed__3(TYPE, F0, "")

#define _BalauInjectConstruct__3(TYPE, F0, F1) \
	_BalauInjectConstructNamed__5(TYPE, F0, "", F1, "")

#define _BalauInjectConstruct__4(TYPE, F0, F1, F2) \
	_BalauInjectConstructNamed__7(TYPE, F0, "", F1, "", F2, "")

#define _BalauInjectConstruct__5(TYPE, F0, F1, F2, F3) \
	_BalauInjectConstructNamed__9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define _BalauInjectConstruct__6(TYPE, F0, F1, F2, F3, F4) \
	_BalauInjectConstructNamed__11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define _BalauInjectConstruct__7(TYPE, F0, F1, F2, F3, F4, F5) \
	_BalauInjectConstructNamed__13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define _BalauInjectConstruct__8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	_BalauInjectConstructNamed__15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define _BalauInjectConstruct__9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	_BalauInjectConstructNamed__17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define _BalauInjectConstruct__10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	_BalauInjectConstructNamed__19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define _BalauInjectConstruct__11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	_BalauInjectConstructNamed__21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define _BalauInjectConstruct__12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	_BalauInjectConstructNamed__23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define _BalauInjectConstruct__13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	_BalauInjectConstructNamed__25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define _BalauInjectConstruct__14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	_BalauInjectConstructNamed__27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define _BalauInjectConstruct__15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	_BalauInjectConstructNamed__29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define _BalauInjectConstruct__16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	_BalauInjectConstructNamed__31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define _BalauInjectConstruct__17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	_BalauInjectConstructNamed__33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

////////////////////////////////////// BalauInjectTypes macros //////////////////////////////////////

#define _BalauInjectTypes__1(TYPE) \
	_BalauInjectNamedTypes__1(TYPE)

#define _BalauInjectTypes__2(TYPE, F0) \
	_BalauInjectNamedTypes__3(TYPE, F0, "")

#define _BalauInjectTypes__3(TYPE, F0, F1) \
	_BalauInjectNamedTypes__5(TYPE, F0, "", F1, "")

#define _BalauInjectTypes__4(TYPE, F0, F1, F2) \
	_BalauInjectNamedTypes__7(TYPE, F0, "", F1, "", F2, "")

#define _BalauInjectTypes__5(TYPE, F0, F1, F2, F3) \
	_BalauInjectNamedTypes__9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define _BalauInjectTypes__6(TYPE, F0, F1, F2, F3, F4) \
	_BalauInjectNamedTypes__11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define _BalauInjectTypes__7(TYPE, F0, F1, F2, F3, F4, F5) \
	_BalauInjectNamedTypes__13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define _BalauInjectTypes__8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	_BalauInjectNamedTypes__15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define _BalauInjectTypes__9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	_BalauInjectNamedTypes__17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define _BalauInjectTypes__10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	_BalauInjectNamedTypes__19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define _BalauInjectTypes__11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	_BalauInjectNamedTypes__21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define _BalauInjectTypes__12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	_BalauInjectNamedTypes__23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define _BalauInjectTypes__13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	_BalauInjectNamedTypes__25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define _BalauInjectTypes__14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	_BalauInjectNamedTypes__27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define _BalauInjectTypes__15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	_BalauInjectNamedTypes__29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define _BalauInjectTypes__16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	_BalauInjectNamedTypes__31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define _BalauInjectTypes__17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	_BalauInjectNamedTypes__33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define _BalauInject__(N)                   _BalauInject__##N
#define _BalauInject__Eval(N)               _BalauInject__(N)

#define _BalauInjectNamed__(N)              _BalauInjectNamed__##N
#define _BalauInjectNamed__Eval(N)          _BalauInjectNamed__(N)

#define _BalauInjectConstruct__(N)          _BalauInjectConstruct__##N
#define _BalauInjectConstruct__Eval(N)      _BalauInjectConstruct__(N)

#define _BalauInjectConstructNamed__(N)     _BalauInjectConstructNamed__##N
#define _BalauInjectConstructNamed__Eval(N) _BalauInjectConstructNamed__(N)

#define _BalauInjectTypes__(N)              _BalauInjectTypes__##N
#define _BalauInjectTypes__Eval(N)          _BalauInjectTypes__(N)

#define _BalauInjectNamedTypes__(N)         _BalauInjectNamedTypes__##N
#define _BalauInjectNamedTypes__Eval(N)     _BalauInjectNamedTypes__(N)

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define _BalauInject(...)               _BalauInject__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectNamed(...)          _BalauInjectNamed__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectConstruct(...)      _BalauInjectConstruct__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectConstructNamed(...) _BalauInjectConstructNamed__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectTypes(...)          _BalauInjectTypes__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectNamedTypes(...)     _BalauInjectNamedTypes__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_MACROS
