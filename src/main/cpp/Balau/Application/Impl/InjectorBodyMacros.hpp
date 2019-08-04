// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_BODY_MACROS
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_BODY_MACROS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Util/Macros.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define _BalauBIGF(F, N) _BalauInjector.getInstance<decltype(F)>(N)
#define _BalauBIGT(T, N) _BalauInjector.getInstance<T>(N)
#define _BalauBIKF(F, N) ::Balau::Impl::CreateBindingKey<decltype(F)>()(N)
#define _BalauBIKT(T, N) ::Balau::Impl::CreateBindingKey<T>()(N)

////////////////////////////////////// BalauInjectBodyNamed macros //////////////////////////////////////

#define _BalauInjectBodyNamed__1(TYPE)                                                                  \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & ) {                                   \
		return new TYPE();                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & ) {                                    \
		return TYPE();                                                                                  \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {};                                                                                      \
	}

#define _BalauInjectBodyNamed__3(TYPE, F0, N0)                                                          \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(_BalauBIGF(F0, N0));                                                            \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(_BalauBIGF(F0, N0));                                                                \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return { _BalauBIKF(F0, N0) };                                                                  \
	}

#define _BalauInjectBodyNamed__5(TYPE, F0, N0, F1, N1)                                                  \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(_BalauBIGF(F0, N0), _BalauBIGF(F1, N1));                                        \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(_BalauBIGF(F0, N0), _BalauBIGF(F1, N1));                                            \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return { _BalauBIKF(F0, N0), _BalauBIKF(F1, N1) };                                              \
	}

#define _BalauInjectBodyNamed__7(TYPE, F0, N0, F1, N1, F2, N2)                                          \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(_BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2));                    \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(_BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2));                        \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return { _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2) };                          \
	}

#define _BalauInjectBodyNamed__9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                                  \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			_BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			_BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			_BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)              \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                         \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4)                                                                        \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)                 \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5)                                                    \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6)         \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6)                                \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6), _BalauBIKF(F7, N7)            \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
			, _BalauBIGF(F8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
			, _BalauBIGF(F8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6), _BalauBIKF(F7, N7)            \
			, _BalauBIKF(F8, N8)                                                                        \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6), _BalauBIKF(F7, N7)            \
			, _BalauBIKF(F8, N8), _BalauBIKF(F9, N9)                                                    \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9), _BalauBIGF(F10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2), _BalauBIGF(F3, N3)            \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6), _BalauBIGF(F7, N7)            \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9), _BalauBIGF(F10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2), _BalauBIKF(F3, N3)            \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6), _BalauBIKF(F7, N7)            \
			, _BalauBIKF(F8, N8), _BalauBIKF(F9, N9), _BalauBIKF(F10, N10)                              \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)          \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)          \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9), _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)          \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)          \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9), _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2),   _BalauBIKF(F3, N3)          \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6),   _BalauBIKF(F7, N7)          \
			, _BalauBIKF(F8, N8), _BalauBIKF(F9, N9), _BalauBIKF(F10, N10), _BalauBIKF(F11, N11)        \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)          \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)          \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9), _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)        \
			, _BalauBIGF(F12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0), _BalauBIGF(F1, N1), _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)          \
			, _BalauBIGF(F4, N4), _BalauBIGF(F5, N5), _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)          \
			, _BalauBIGF(F8, N8), _BalauBIGF(F9, N9), _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)        \
			, _BalauBIGF(F12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0), _BalauBIKF(F1, N1), _BalauBIKF(F2, N2),   _BalauBIKF(F3, N3)          \
			, _BalauBIKF(F4, N4), _BalauBIKF(F5, N5), _BalauBIKF(F6, N6),   _BalauBIKF(F7, N7)          \
			, _BalauBIKF(F8, N8), _BalauBIKF(F9, N9), _BalauBIKF(F10, N10), _BalauBIKF(F11, N11)        \
			, _BalauBIKF(F12, N12)                                                                      \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0),   _BalauBIGF(F1, N1),   _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)      \
			, _BalauBIGF(F4, N4),   _BalauBIGF(F5, N5),   _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)      \
			, _BalauBIGF(F8, N8),   _BalauBIGF(F9, N9),   _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)    \
			, _BalauBIGF(F12, N12), _BalauBIGF(F13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0),   _BalauBIGF(F1, N1),   _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)      \
			, _BalauBIGF(F4, N4),   _BalauBIGF(F5, N5),   _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)      \
			, _BalauBIGF(F8, N8),   _BalauBIGF(F9, N9),   _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)    \
			, _BalauBIGF(F12, N12), _BalauBIGF(F13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0),   _BalauBIKF(F1, N1), _BalauBIKF(F2, N2),   _BalauBIKF(F3, N3)        \
			, _BalauBIKF(F4, N4),   _BalauBIKF(F5, N5), _BalauBIKF(F6, N6),   _BalauBIKF(F7, N7)        \
			, _BalauBIKF(F8, N8),   _BalauBIKF(F9, N9), _BalauBIKF(F10, N10), _BalauBIKF(F11, N11)      \
			, _BalauBIKF(F12, N12), _BalauBIKF(F13, N13)                                                \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0),   _BalauBIGF(F1, N1),   _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)      \
			, _BalauBIGF(F4, N4),   _BalauBIGF(F5, N5),   _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)      \
			, _BalauBIGF(F8, N8),   _BalauBIGF(F9, N9),   _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)    \
			, _BalauBIGF(F12, N12), _BalauBIGF(F13, N13), _BalauBIGF(F14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0),   _BalauBIGF(F1, N1),   _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)      \
			, _BalauBIGF(F4, N4),   _BalauBIGF(F5, N5),   _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)      \
			, _BalauBIGF(F8, N8),   _BalauBIGF(F9, N9),   _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)    \
			, _BalauBIGF(F12, N12), _BalauBIGF(F13, N13), _BalauBIGF(F14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0),   _BalauBIKF(F1, N1),   _BalauBIKF(F2, N2),   _BalauBIKF(F3, N3)      \
			, _BalauBIKF(F4, N4),   _BalauBIKF(F5, N5),   _BalauBIKF(F6, N6),   _BalauBIKF(F7, N7)      \
			, _BalauBIKF(F8, N8),   _BalauBIKF(F9, N9),   _BalauBIKF(F10, N10), _BalauBIKF(F11, N11)    \
			, _BalauBIKF(F12, N12), _BalauBIKF(F13, N13), _BalauBIKF(F14, N14)                          \
		};                                                                                              \
	}

#define _BalauInjectBodyNamed__33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGF(F0, N0),   _BalauBIGF(F1, N1),   _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)      \
			, _BalauBIGF(F4, N4),   _BalauBIGF(F5, N5),   _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)      \
			, _BalauBIGF(F8, N8),   _BalauBIGF(F9, N9),   _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)    \
			, _BalauBIGF(F12, N12), _BalauBIGF(F13, N13), _BalauBIGF(F14, N14), _BalauBIGF(F15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGF(F0, N0),   _BalauBIGF(F1, N1),   _BalauBIGF(F2, N2),   _BalauBIGF(F3, N3)      \
			, _BalauBIGF(F4, N4),   _BalauBIGF(F5, N5),   _BalauBIGF(F6, N6),   _BalauBIGF(F7, N7)      \
			, _BalauBIGF(F8, N8),   _BalauBIGF(F9, N9),   _BalauBIGF(F10, N10), _BalauBIGF(F11, N11)    \
			, _BalauBIGF(F12, N12), _BalauBIGF(F13, N13), _BalauBIGF(F14, N14), _BalauBIGF(F15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKF(F0, N0),   _BalauBIKF(F1, N1),   _BalauBIKF(F2, N2),   _BalauBIKF(F3, N3)      \
			, _BalauBIKF(F4, N4),   _BalauBIKF(F5, N5),   _BalauBIKF(F6, N6),   _BalauBIKF(F7, N7)      \
			, _BalauBIKF(F8, N8),   _BalauBIKF(F9, N9),   _BalauBIKF(F10, N10), _BalauBIKF(F11, N11)    \
			, _BalauBIKF(F12, N12), _BalauBIKF(F13, N13), _BalauBIKF(F14, N14), _BalauBIKF(F15, N15)    \
		};                                                                                              \
	}

/////////////////////////////////// BalauInjectBodyNamedTypes macros ////////////////////////////////////

#define _BalauInjectBodyNamedTypes__1(TYPE)                                                             \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE();                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE();                                                                                  \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {};                                                                                      \
	}

#define _BalauInjectBodyNamedTypes__3(TYPE, T0, N0)                                                     \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(_BalauBIGT(T0, N0));                                                            \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(_BalauBIGT(T0, N0));                                                                \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return { _BalauBIKT(T0, N0) };                                                                  \
	}

#define _BalauInjectBodyNamedTypes__5(TYPE, T0, N0, T1, N1)                                             \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(_BalauBIGT(T0, N0), _BalauBIGT(T1, N1));                                        \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(_BalauBIGT(T0, N0), _BalauBIGT(T1, N1));                                            \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return { _BalauBIKT(T0, N0), _BalauBIKT(T1, N1) };                                              \
	}

#define _BalauInjectBodyNamedTypes__7(TYPE, T0, N0, T1, N1, T2, N2)                                     \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(_BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2));                    \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(_BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2));                        \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return { _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2) };                          \
	}

#define _BalauInjectBodyNamedTypes__9(TYPE, T0, N0, T1, N1, T2, N2, T3, N3)                             \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			_BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			_BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			_BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)              \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__11(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4)                    \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4)                                                                        \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__13(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5)            \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5)                                                    \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__15(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6)    \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5), _BalauBIKT(T6, N6)                                \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__17(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5), _BalauBIKT(T6, N6), _BalauBIKT(T7, N7)            \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__19(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
			, _BalauBIGT(T8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
			, _BalauBIGT(T8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5), _BalauBIKT(T6, N6), _BalauBIKT(T7, N7)            \
			, _BalauBIKT(T8, N8)                                                                        \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__21(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
			, _BalauBIGT(T8, N8), _BalauBIGT(T9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
			, _BalauBIGT(T8, N8), _BalauBIGT(T9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5), _BalauBIKT(T6, N6), _BalauBIKT(T7, N7)            \
			, _BalauBIKT(T8, N8), _BalauBIKT(T9, N9)                                                    \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__23(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
			, _BalauBIGT(T8, N8), _BalauBIGT(T9, N9), _BalauBIGT(T10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2), _BalauBIGT(T3, N3)            \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6), _BalauBIGT(T7, N7)            \
			, _BalauBIGT(T8, N8), _BalauBIGT(T9, N9), _BalauBIGT(T10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2), _BalauBIKT(T3, N3)            \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5), _BalauBIKT(T6, N6), _BalauBIKT(T7, N7)            \
			, _BalauBIKT(T8, N8), _BalauBIKT(T9, N9), _BalauBIKT(T10, N10)                              \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__25(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)          \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)          \
			, _BalauBIGT(T8, N8), _BalauBIGT(T9, N9), _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0), _BalauBIGT(T1, N1), _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)          \
			, _BalauBIGT(T4, N4), _BalauBIGT(T5, N5), _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)          \
			, _BalauBIGT(T8, N8), _BalauBIGT(T9, N9), _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0), _BalauBIKT(T1, N1), _BalauBIKT(T2, N2),   _BalauBIKT(T3, N3)          \
			, _BalauBIKT(T4, N4), _BalauBIKT(T5, N5), _BalauBIKT(T6, N6),   _BalauBIKT(T7, N7)          \
			, _BalauBIKT(T8, N8), _BalauBIKT(T9, N9), _BalauBIKT(T10, N10), _BalauBIKT(T11, N11)        \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__27(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0),   _BalauBIKT(T1, N1), _BalauBIKT(T2, N2),   _BalauBIKT(T3, N3)        \
			, _BalauBIKT(T4, N4),   _BalauBIKT(T5, N5), _BalauBIKT(T6, N6),   _BalauBIKT(T7, N7)        \
			, _BalauBIKT(T8, N8),   _BalauBIKT(T9, N9), _BalauBIKT(T10, N10), _BalauBIKT(T11, N11)      \
			, _BalauBIKT(T12, N12)                                                                      \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__29(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12), _BalauBIGT(T13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12), _BalauBIGT(T13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0),   _BalauBIKT(T1, N1), _BalauBIKT(T2, N2),   _BalauBIKT(T3, N3)        \
			, _BalauBIKT(T4, N4),   _BalauBIKT(T5, N5), _BalauBIKT(T6, N6),   _BalauBIKT(T7, N7)        \
			, _BalauBIKT(T8, N8),   _BalauBIKT(T9, N9), _BalauBIKT(T10, N10), _BalauBIKT(T11, N11)      \
			, _BalauBIKT(T12, N12), _BalauBIKT(T13, N13)                                                \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__31(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12), _BalauBIGT(T13, N13), _BalauBIGT(T14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12), _BalauBIGT(T13, N13), _BalauBIGT(T14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0),   _BalauBIKT(T1, N1),   _BalauBIKT(T2, N2),   _BalauBIKT(T3, N3)      \
			, _BalauBIKT(T4, N4),   _BalauBIKT(T5, N5),   _BalauBIKT(T6, N6),   _BalauBIKT(T7, N7)      \
			, _BalauBIKT(T8, N8),   _BalauBIKT(T9, N9),   _BalauBIKT(T10, N10), _BalauBIKT(T11, N11)    \
			, _BalauBIKT(T12, N12), _BalauBIKT(T13, N13), _BalauBIKT(T14, N14)                          \
		};                                                                                              \
	}

#define _BalauInjectBodyNamedTypes__33(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14, T15, N15) \
	TYPE * TYPE::_Balau_newHeapInstance(const ::Balau::Injector & _BalauInjector) {                     \
		return new TYPE(                                                                                \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12), _BalauBIGT(T13, N13), _BalauBIGT(T14, N14), _BalauBIGT(T15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::_Balau_newStackInstance(const ::Balau::Injector & _BalauInjector) {                      \
		return TYPE(                                                                                    \
			  _BalauBIGT(T0, N0),   _BalauBIGT(T1, N1),   _BalauBIGT(T2, N2),   _BalauBIGT(T3, N3)      \
			, _BalauBIGT(T4, N4),   _BalauBIGT(T5, N5),   _BalauBIGT(T6, N6),   _BalauBIGT(T7, N7)      \
			, _BalauBIGT(T8, N8),   _BalauBIGT(T9, N9),   _BalauBIGT(T10, N10), _BalauBIGT(T11, N11)    \
			, _BalauBIGT(T12, N12), _BalauBIGT(T13, N13), _BalauBIGT(T14, N14), _BalauBIGT(T15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::_Balau_getDependencyKeys() {                           \
		return {                                                                                        \
			  _BalauBIKT(T0, N0),   _BalauBIKT(T1, N1),   _BalauBIKT(T2, N2),   _BalauBIKT(T3, N3)      \
			, _BalauBIKT(T4, N4),   _BalauBIKT(T5, N5),   _BalauBIKT(T6, N6),   _BalauBIKT(T7, N7)      \
			, _BalauBIKT(T8, N8),   _BalauBIKT(T9, N9),   _BalauBIKT(T10, N10), _BalauBIKT(T11, N11)    \
			, _BalauBIKT(T12, N12), _BalauBIKT(T13, N13), _BalauBIKT(T14, N14), _BalauBIKT(T15, N15)    \
		};                                                                                              \
	}

//////////////////////////////////////// BalauInjectBody macros /////////////////////////////////////////

#define _BalauInjectBody__1(TYPE) \
	_BalauInjectBodyNamed__1(TYPE)

#define _BalauInjectBody__2(TYPE, F0) \
	_BalauInjectBodyNamed__3(TYPE, F0, "")

#define _BalauInjectBody__3(TYPE, F0, F1) \
	_BalauInjectBodyNamed__5(TYPE, F0, "", F1, "")

#define _BalauInjectBody__4(TYPE, F0, F1, F2) \
	_BalauInjectBodyNamed__7(TYPE, F0, "", F1, "", F2, "")

#define _BalauInjectBody__5(TYPE, F0, F1, F2, F3) \
	_BalauInjectBodyNamed__9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define _BalauInjectBody__6(TYPE, F0, F1, F2, F3, F4) \
	_BalauInjectBodyNamed__11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define _BalauInjectBody__7(TYPE, F0, F1, F2, F3, F4, F5) \
	_BalauInjectBodyNamed__13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define _BalauInjectBody__8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	_BalauInjectBodyNamed__15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define _BalauInjectBody__9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	_BalauInjectBodyNamed__17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define _BalauInjectBody__10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	_BalauInjectBodyNamed__19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define _BalauInjectBody__11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	_BalauInjectBodyNamed__21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define _BalauInjectBody__12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	_BalauInjectBodyNamed__23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define _BalauInjectBody__13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	_BalauInjectBodyNamed__25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define _BalauInjectBody__14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	_BalauInjectBodyNamed__27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define _BalauInjectBody__15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	_BalauInjectBodyNamed__29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define _BalauInjectBody__16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	_BalauInjectBodyNamed__31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define _BalauInjectBody__17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	_BalauInjectBodyNamed__33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

////////////////////////////////////// BalauInjectBodyTypes macros //////////////////////////////////////

#define _BalauInjectBodyTypes__1(TYPE) \
	_BalauInjectBodyNamedTypes__1(TYPE)

#define _BalauInjectBodyTypes__2(TYPE, F0) \
	_BalauInjectBodyNamedTypes__3(TYPE, F0, "")

#define _BalauInjectBodyTypes__3(TYPE, F0, F1) \
	_BalauInjectBodyNamedTypes__5(TYPE, F0, "", F1, "")

#define _BalauInjectBodyTypes__4(TYPE, F0, F1, F2) \
	_BalauInjectBodyNamedTypes__7(TYPE, F0, "", F1, "", F2, "")

#define _BalauInjectBodyTypes__5(TYPE, F0, F1, F2, F3) \
	_BalauInjectBodyNamedTypes__9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define _BalauInjectBodyTypes__6(TYPE, F0, F1, F2, F3, F4) \
	_BalauInjectBodyNamedTypes__11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define _BalauInjectBodyTypes__7(TYPE, F0, F1, F2, F3, F4, F5) \
	_BalauInjectBodyNamedTypes__13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define _BalauInjectBodyTypes__8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	_BalauInjectBodyNamedTypes__15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define _BalauInjectBodyTypes__9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	_BalauInjectBodyNamedTypes__17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define _BalauInjectBodyTypes__10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	_BalauInjectBodyNamedTypes__19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define _BalauInjectBodyTypes__11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	_BalauInjectBodyNamedTypes__21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define _BalauInjectBodyTypes__12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	_BalauInjectBodyNamedTypes__23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define _BalauInjectBodyTypes__13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	_BalauInjectBodyNamedTypes__25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define _BalauInjectBodyTypes__14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	_BalauInjectBodyNamedTypes__27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define _BalauInjectBodyTypes__15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	_BalauInjectBodyNamedTypes__29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define _BalauInjectBodyTypes__16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	_BalauInjectBodyNamedTypes__31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define _BalauInjectBodyTypes__17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	_BalauInjectBodyNamedTypes__33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define _BalauInjectBody__(N)                   _BalauInjectBody__##N
#define _BalauInjectBody__Eval(N)               _BalauInjectBody__(N)

#define _BalauInjectBodyNamed__(N)              _BalauInjectBodyNamed__##N
#define _BalauInjectBodyNamed__Eval(N)          _BalauInjectBodyNamed__(N)

#define _BalauInjectBodyTypes__(N)              _BalauInjectBodyTypes__##N
#define _BalauInjectBodyTypes__Eval(N)          _BalauInjectBodyTypes__(N)

#define _BalauInjectBodyNamedTypes__(N)         _BalauInjectBodyNamedTypes__##N
#define _BalauInjectBodyNamedTypes__Eval(N)     _BalauInjectBodyNamedTypes__(N)

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define _BalauInjectBody(...)                   _BalauInjectBody__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectBodyNamed(...)              _BalauInjectBodyNamed__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectBodyTypes(...)              _BalauInjectBodyTypes__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define _BalauInjectBodyNamedTypes(...)         _BalauInjectBodyNamedTypes__Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_BODY_MACROS
