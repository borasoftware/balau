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

#ifndef COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_INJECTOR_BODY_MACROS
#define COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_INJECTOR_BODY_MACROS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Application/Macros.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define BALAU_BIGF(F, N)  BALAU_Injector.getInstance<decltype(F)>(N)
#define BALAU_BIGT(T, N)  BALAU_Injector.getInstance<T>(N)
#define BALAU_BIKF(F, N) ::Balau::Impl::CreateBindingKey<decltype(F)>()(N)
#define BALAU_BIKT(T, N) ::Balau::Impl::CreateBindingKey<T>()(N)

////////////////////////////////////// BalauInjectBodyNamed macros //////////////////////////////////////

#define BALAU_InjectBodyNamed_1(TYPE)                                                                  \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector & ) {                                    \
		return new TYPE();                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector & ) {                                     \
		return TYPE();                                                                                  \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {};                                                                                      \
	}

#define BALAU_InjectBodyNamed_3(TYPE, F0, N0)                                                          \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(BALAU_BIGF(F0, N0));                                                            \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(BALAU_BIGF(F0, N0));                                                                \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return { BALAU_BIKF(F0, N0) };                                                                  \
	}

#define BALAU_InjectBodyNamed_5(TYPE, F0, N0, F1, N1)                                                  \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1));                                        \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1));                                            \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return { BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1) };                                              \
	}

#define BALAU_InjectBodyNamed_7(TYPE, F0, N0, F1, N1, F2, N2)                                          \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2));                    \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2));                        \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return { BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2) };                          \
	}

#define BALAU_InjectBodyNamed_9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                                  \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)              \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                         \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4)                                                                        \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)                 \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5)                                                    \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6)         \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6)                                \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6), BALAU_BIKF(F7, N7)            \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
			, BALAU_BIGF(F8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
			, BALAU_BIGF(F8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6), BALAU_BIKF(F7, N7)            \
			, BALAU_BIKF(F8, N8)                                                                        \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6), BALAU_BIKF(F7, N7)            \
			, BALAU_BIKF(F8, N8), BALAU_BIKF(F9, N9)                                                    \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9), BALAU_BIGF(F10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2), BALAU_BIGF(F3, N3)            \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6), BALAU_BIGF(F7, N7)            \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9), BALAU_BIGF(F10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2), BALAU_BIKF(F3, N3)            \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6), BALAU_BIKF(F7, N7)            \
			, BALAU_BIKF(F8, N8), BALAU_BIKF(F9, N9), BALAU_BIKF(F10, N10)                              \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)          \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)          \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9), BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)          \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)          \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9), BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2),   BALAU_BIKF(F3, N3)          \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6),   BALAU_BIKF(F7, N7)          \
			, BALAU_BIKF(F8, N8), BALAU_BIKF(F9, N9), BALAU_BIKF(F10, N10), BALAU_BIKF(F11, N11)        \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)          \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)          \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9), BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)        \
			, BALAU_BIGF(F12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0), BALAU_BIGF(F1, N1), BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)          \
			, BALAU_BIGF(F4, N4), BALAU_BIGF(F5, N5), BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)          \
			, BALAU_BIGF(F8, N8), BALAU_BIGF(F9, N9), BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)        \
			, BALAU_BIGF(F12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0), BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2),   BALAU_BIKF(F3, N3)          \
			, BALAU_BIKF(F4, N4), BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6),   BALAU_BIKF(F7, N7)          \
			, BALAU_BIKF(F8, N8), BALAU_BIKF(F9, N9), BALAU_BIKF(F10, N10), BALAU_BIKF(F11, N11)        \
			, BALAU_BIKF(F12, N12)                                                                      \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0),   BALAU_BIGF(F1, N1),   BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)      \
			, BALAU_BIGF(F4, N4),   BALAU_BIGF(F5, N5),   BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)      \
			, BALAU_BIGF(F8, N8),   BALAU_BIGF(F9, N9),   BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)    \
			, BALAU_BIGF(F12, N12), BALAU_BIGF(F13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0),   BALAU_BIGF(F1, N1),   BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)      \
			, BALAU_BIGF(F4, N4),   BALAU_BIGF(F5, N5),   BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)      \
			, BALAU_BIGF(F8, N8),   BALAU_BIGF(F9, N9),   BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)    \
			, BALAU_BIGF(F12, N12), BALAU_BIGF(F13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0),   BALAU_BIKF(F1, N1), BALAU_BIKF(F2, N2),   BALAU_BIKF(F3, N3)        \
			, BALAU_BIKF(F4, N4),   BALAU_BIKF(F5, N5), BALAU_BIKF(F6, N6),   BALAU_BIKF(F7, N7)        \
			, BALAU_BIKF(F8, N8),   BALAU_BIKF(F9, N9), BALAU_BIKF(F10, N10), BALAU_BIKF(F11, N11)      \
			, BALAU_BIKF(F12, N12), BALAU_BIKF(F13, N13)                                                \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0),   BALAU_BIGF(F1, N1),   BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)      \
			, BALAU_BIGF(F4, N4),   BALAU_BIGF(F5, N5),   BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)      \
			, BALAU_BIGF(F8, N8),   BALAU_BIGF(F9, N9),   BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)    \
			, BALAU_BIGF(F12, N12), BALAU_BIGF(F13, N13), BALAU_BIGF(F14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0),   BALAU_BIGF(F1, N1),   BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)      \
			, BALAU_BIGF(F4, N4),   BALAU_BIGF(F5, N5),   BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)      \
			, BALAU_BIGF(F8, N8),   BALAU_BIGF(F9, N9),   BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)    \
			, BALAU_BIGF(F12, N12), BALAU_BIGF(F13, N13), BALAU_BIGF(F14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0),   BALAU_BIKF(F1, N1),   BALAU_BIKF(F2, N2),   BALAU_BIKF(F3, N3)      \
			, BALAU_BIKF(F4, N4),   BALAU_BIKF(F5, N5),   BALAU_BIKF(F6, N6),   BALAU_BIKF(F7, N7)      \
			, BALAU_BIKF(F8, N8),   BALAU_BIKF(F9, N9),   BALAU_BIKF(F10, N10), BALAU_BIKF(F11, N11)    \
			, BALAU_BIKF(F12, N12), BALAU_BIKF(F13, N13), BALAU_BIKF(F14, N14)                          \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamed_33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGF(F0, N0),   BALAU_BIGF(F1, N1),   BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)      \
			, BALAU_BIGF(F4, N4),   BALAU_BIGF(F5, N5),   BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)      \
			, BALAU_BIGF(F8, N8),   BALAU_BIGF(F9, N9),   BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)    \
			, BALAU_BIGF(F12, N12), BALAU_BIGF(F13, N13), BALAU_BIGF(F14, N14), BALAU_BIGF(F15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGF(F0, N0),   BALAU_BIGF(F1, N1),   BALAU_BIGF(F2, N2),   BALAU_BIGF(F3, N3)      \
			, BALAU_BIGF(F4, N4),   BALAU_BIGF(F5, N5),   BALAU_BIGF(F6, N6),   BALAU_BIGF(F7, N7)      \
			, BALAU_BIGF(F8, N8),   BALAU_BIGF(F9, N9),   BALAU_BIGF(F10, N10), BALAU_BIGF(F11, N11)    \
			, BALAU_BIGF(F12, N12), BALAU_BIGF(F13, N13), BALAU_BIGF(F14, N14), BALAU_BIGF(F15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKF(F0, N0),   BALAU_BIKF(F1, N1),   BALAU_BIKF(F2, N2),   BALAU_BIKF(F3, N3)      \
			, BALAU_BIKF(F4, N4),   BALAU_BIKF(F5, N5),   BALAU_BIKF(F6, N6),   BALAU_BIKF(F7, N7)      \
			, BALAU_BIKF(F8, N8),   BALAU_BIKF(F9, N9),   BALAU_BIKF(F10, N10), BALAU_BIKF(F11, N11)    \
			, BALAU_BIKF(F12, N12), BALAU_BIKF(F13, N13), BALAU_BIKF(F14, N14), BALAU_BIKF(F15, N15)    \
		};                                                                                              \
	}

/////////////////////////////////// BalauInjectBodyNamedTypes macros ////////////////////////////////////

#define BALAU_InjectBodyNamedTypes_1(TYPE)                                                             \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE();                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE();                                                                                  \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {};                                                                                      \
	}

#define BALAU_InjectBodyNamedTypes_3(TYPE, T0, N0)                                                     \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(BALAU_BIGT(T0, N0));                                                            \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(BALAU_BIGT(T0, N0));                                                                \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return { BALAU_BIKT(T0, N0) };                                                                  \
	}

#define BALAU_InjectBodyNamedTypes_5(TYPE, T0, N0, T1, N1)                                             \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1));                                        \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1));                                            \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return { BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1) };                                              \
	}

#define BALAU_InjectBodyNamedTypes_7(TYPE, T0, N0, T1, N1, T2, N2)                                     \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2));                    \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2));                        \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return { BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2) };                          \
	}

#define BALAU_InjectBodyNamedTypes_9(TYPE, T0, N0, T1, N1, T2, N2, T3, N3)                             \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)              \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_11(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4)                    \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4)                                                                        \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_13(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5)            \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5)                                                    \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_15(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6)    \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6)                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6)                                \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_17(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6), BALAU_BIKT(T7, N7)            \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_19(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
			, BALAU_BIGT(T8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
			, BALAU_BIGT(T8, N8)                                                                        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6), BALAU_BIKT(T7, N7)            \
			, BALAU_BIKT(T8, N8)                                                                        \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_21(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
			, BALAU_BIGT(T8, N8), BALAU_BIGT(T9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
			, BALAU_BIGT(T8, N8), BALAU_BIGT(T9, N9)                                                    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6), BALAU_BIKT(T7, N7)            \
			, BALAU_BIKT(T8, N8), BALAU_BIKT(T9, N9)                                                    \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_23(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
			, BALAU_BIGT(T8, N8), BALAU_BIGT(T9, N9), BALAU_BIGT(T10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2), BALAU_BIGT(T3, N3)            \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6), BALAU_BIGT(T7, N7)            \
			, BALAU_BIGT(T8, N8), BALAU_BIGT(T9, N9), BALAU_BIGT(T10, N10)                              \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2), BALAU_BIKT(T3, N3)            \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6), BALAU_BIKT(T7, N7)            \
			, BALAU_BIKT(T8, N8), BALAU_BIKT(T9, N9), BALAU_BIKT(T10, N10)                              \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_25(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)          \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)          \
			, BALAU_BIGT(T8, N8), BALAU_BIGT(T9, N9), BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0), BALAU_BIGT(T1, N1), BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)          \
			, BALAU_BIGT(T4, N4), BALAU_BIGT(T5, N5), BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)          \
			, BALAU_BIGT(T8, N8), BALAU_BIGT(T9, N9), BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)        \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0), BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2),   BALAU_BIKT(T3, N3)          \
			, BALAU_BIKT(T4, N4), BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6),   BALAU_BIKT(T7, N7)          \
			, BALAU_BIKT(T8, N8), BALAU_BIKT(T9, N9), BALAU_BIKT(T10, N10), BALAU_BIKT(T11, N11)        \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_27(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12)                                                                      \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0),   BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2),   BALAU_BIKT(T3, N3)        \
			, BALAU_BIKT(T4, N4),   BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6),   BALAU_BIKT(T7, N7)        \
			, BALAU_BIKT(T8, N8),   BALAU_BIKT(T9, N9), BALAU_BIKT(T10, N10), BALAU_BIKT(T11, N11)      \
			, BALAU_BIKT(T12, N12)                                                                      \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_29(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12), BALAU_BIGT(T13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12), BALAU_BIGT(T13, N13)                                                \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0),   BALAU_BIKT(T1, N1), BALAU_BIKT(T2, N2),   BALAU_BIKT(T3, N3)        \
			, BALAU_BIKT(T4, N4),   BALAU_BIKT(T5, N5), BALAU_BIKT(T6, N6),   BALAU_BIKT(T7, N7)        \
			, BALAU_BIKT(T8, N8),   BALAU_BIKT(T9, N9), BALAU_BIKT(T10, N10), BALAU_BIKT(T11, N11)      \
			, BALAU_BIKT(T12, N12), BALAU_BIKT(T13, N13)                                                \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_31(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12), BALAU_BIGT(T13, N13), BALAU_BIGT(T14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12), BALAU_BIGT(T13, N13), BALAU_BIGT(T14, N14)                          \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0),   BALAU_BIKT(T1, N1),   BALAU_BIKT(T2, N2),   BALAU_BIKT(T3, N3)      \
			, BALAU_BIKT(T4, N4),   BALAU_BIKT(T5, N5),   BALAU_BIKT(T6, N6),   BALAU_BIKT(T7, N7)      \
			, BALAU_BIKT(T8, N8),   BALAU_BIKT(T9, N9),   BALAU_BIKT(T10, N10), BALAU_BIKT(T11, N11)    \
			, BALAU_BIKT(T12, N12), BALAU_BIKT(T13, N13), BALAU_BIKT(T14, N14)                          \
		};                                                                                              \
	}

#define BALAU_InjectBodyNamedTypes_33(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14, T15, N15) \
	TYPE * TYPE::BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {                     \
		return new TYPE(                                                                                \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12), BALAU_BIGT(T13, N13), BALAU_BIGT(T14, N14), BALAU_BIGT(T15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	TYPE TYPE::BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {                      \
		return TYPE(                                                                                    \
			  BALAU_BIGT(T0, N0),   BALAU_BIGT(T1, N1),   BALAU_BIGT(T2, N2),   BALAU_BIGT(T3, N3)      \
			, BALAU_BIGT(T4, N4),   BALAU_BIGT(T5, N5),   BALAU_BIGT(T6, N6),   BALAU_BIGT(T7, N7)      \
			, BALAU_BIGT(T8, N8),   BALAU_BIGT(T9, N9),   BALAU_BIGT(T10, N10), BALAU_BIGT(T11, N11)    \
			, BALAU_BIGT(T12, N12), BALAU_BIGT(T13, N13), BALAU_BIGT(T14, N14), BALAU_BIGT(T15, N15)    \
		);                                                                                              \
	}                                                                                                   \
	                                                                                                    \
	std::vector<::Balau::Impl::BindingKey> TYPE::BALAU_getDependencyKeys() {                            \
		return {                                                                                        \
			  BALAU_BIKT(T0, N0),   BALAU_BIKT(T1, N1),   BALAU_BIKT(T2, N2),   BALAU_BIKT(T3, N3)      \
			, BALAU_BIKT(T4, N4),   BALAU_BIKT(T5, N5),   BALAU_BIKT(T6, N6),   BALAU_BIKT(T7, N7)      \
			, BALAU_BIKT(T8, N8),   BALAU_BIKT(T9, N9),   BALAU_BIKT(T10, N10), BALAU_BIKT(T11, N11)    \
			, BALAU_BIKT(T12, N12), BALAU_BIKT(T13, N13), BALAU_BIKT(T14, N14), BALAU_BIKT(T15, N15)    \
		};                                                                                              \
	}

//////////////////////////////////////// BalauInjectBody macros /////////////////////////////////////////

#define BALAU_InjectBody_1(TYPE) \
	BALAU_InjectBodyNamed_1(TYPE)

#define BALAU_InjectBody_2(TYPE, F0) \
	BALAU_InjectBodyNamed_3(TYPE, F0, "")

#define BALAU_InjectBody_3(TYPE, F0, F1) \
	BALAU_InjectBodyNamed_5(TYPE, F0, "", F1, "")

#define BALAU_InjectBody_4(TYPE, F0, F1, F2) \
	BALAU_InjectBodyNamed_7(TYPE, F0, "", F1, "", F2, "")

#define BALAU_InjectBody_5(TYPE, F0, F1, F2, F3) \
	BALAU_InjectBodyNamed_9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define BALAU_InjectBody_6(TYPE, F0, F1, F2, F3, F4) \
	BALAU_InjectBodyNamed_11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define BALAU_InjectBody_7(TYPE, F0, F1, F2, F3, F4, F5) \
	BALAU_InjectBodyNamed_13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define BALAU_InjectBody_8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	BALAU_InjectBodyNamed_15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define BALAU_InjectBody_9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	BALAU_InjectBodyNamed_17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define BALAU_InjectBody_10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	BALAU_InjectBodyNamed_19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define BALAU_InjectBody_11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	BALAU_InjectBodyNamed_21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define BALAU_InjectBody_12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	BALAU_InjectBodyNamed_23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define BALAU_InjectBody_13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	BALAU_InjectBodyNamed_25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define BALAU_InjectBody_14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	BALAU_InjectBodyNamed_27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define BALAU_InjectBody_15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	BALAU_InjectBodyNamed_29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define BALAU_InjectBody_16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	BALAU_InjectBodyNamed_31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define BALAU_InjectBody_17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	BALAU_InjectBodyNamed_33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

////////////////////////////////////// BalauInjectBodyTypes macros //////////////////////////////////////

#define BALAU_InjectBodyTypes_1(TYPE) \
	BALAU_InjectBodyNamedTypes_1(TYPE)

#define BALAU_InjectBodyTypes_2(TYPE, F0) \
	BALAU_InjectBodyNamedTypes_3(TYPE, F0, "")

#define BALAU_InjectBodyTypes_3(TYPE, F0, F1) \
	BALAU_InjectBodyNamedTypes_5(TYPE, F0, "", F1, "")

#define BALAU_InjectBodyTypes_4(TYPE, F0, F1, F2) \
	BALAU_InjectBodyNamedTypes_7(TYPE, F0, "", F1, "", F2, "")

#define BALAU_InjectBodyTypes_5(TYPE, F0, F1, F2, F3) \
	BALAU_InjectBodyNamedTypes_9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define BALAU_InjectBodyTypes_6(TYPE, F0, F1, F2, F3, F4) \
	BALAU_InjectBodyNamedTypes_11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define BALAU_InjectBodyTypes_7(TYPE, F0, F1, F2, F3, F4, F5) \
	BALAU_InjectBodyNamedTypes_13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define BALAU_InjectBodyTypes_8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	BALAU_InjectBodyNamedTypes_15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define BALAU_InjectBodyTypes_9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	BALAU_InjectBodyNamedTypes_17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define BALAU_InjectBodyTypes_10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	BALAU_InjectBodyNamedTypes_19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define BALAU_InjectBodyTypes_11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	BALAU_InjectBodyNamedTypes_21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define BALAU_InjectBodyTypes_12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	BALAU_InjectBodyNamedTypes_23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define BALAU_InjectBodyTypes_13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	BALAU_InjectBodyNamedTypes_25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define BALAU_InjectBodyTypes_14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	BALAU_InjectBodyNamedTypes_27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define BALAU_InjectBodyTypes_15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	BALAU_InjectBodyNamedTypes_29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define BALAU_InjectBodyTypes_16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	BALAU_InjectBodyNamedTypes_31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define BALAU_InjectBodyTypes_17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	BALAU_InjectBodyNamedTypes_33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define BALAU_InjectBody_(N)                   BALAU_InjectBody_##N
#define BALAU_InjectBody_Eval(N)               BALAU_InjectBody_(N)

#define BALAU_InjectBodyNamed_(N)              BALAU_InjectBodyNamed_##N
#define BALAU_InjectBodyNamed_Eval(N)          BALAU_InjectBodyNamed_(N)

#define BALAU_InjectBodyTypes_(N)              BALAU_InjectBodyTypes_##N
#define BALAU_InjectBodyTypes_Eval(N)          BALAU_InjectBodyTypes_(N)

#define BALAU_InjectBodyNamedTypes_(N)         BALAU_InjectBodyNamedTypes_##N
#define BALAU_InjectBodyNamedTypes_Eval(N)     BALAU_InjectBodyNamedTypes_(N)

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define BALAU_InjectBody(...)                   BALAU_InjectBody_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectBodyNamed(...)              BALAU_InjectBodyNamed_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectBodyTypes(...)              BALAU_InjectBodyTypes_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectBodyNamedTypes(...)         BALAU_InjectBodyNamedTypes_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE_BALAU_APPLICATION_IMPL_INJECTOR_BODY_MACROS
