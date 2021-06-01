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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_MACROS
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_MACROS

#include <Balau/Application/Impl/BindingKey.hpp>
#include <Balau/Application/Macros.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define BALAU_IGF(F, N)  BALAU_Injector.getInstance<decltype(F)>(N)
#define BALAU_IGT(T, N)  BALAU_Injector.getInstance<T>(N)
#define BALAU_IPF(F)    decltype(F) F ## _
#define BALAU_IMF(F)    F(std::forward<decltype(F)>(F ## _))
#define BALAU_IKF(F, N) ::Balau::Impl::CreateBindingKey<decltype(F)>()(N)
#define BALAU_IKT(T, N) ::Balau::Impl::CreateBindingKey<T>()(N)

////////////////////////////////////// BalauInjectNamed macros //////////////////////////////////////

#define BALAU_InjectNamed_1(TYPE)                                                                   \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector & ) {                       \
		return new TYPE();                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector & ) {                        \
		return TYPE();                                                                              \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {};                                                                                  \
	}

#define BALAU_InjectNamed_3(TYPE, F0, N0)                                                           \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(BALAU_IGF(F0, N0));                                                         \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(BALAU_IGF(F0, N0));                                                             \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return { BALAU_IKF(F0, N0) };                                                               \
	}

#define BALAU_InjectNamed_5(TYPE, F0, N0, F1, N1)                                                   \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(BALAU_IGF(F0, N0), BALAU_IGF(F1, N1));                                      \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(BALAU_IGF(F0, N0), BALAU_IGF(F1, N1));                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return { BALAU_IKF(F0, N0), BALAU_IKF(F1, N1) };                                            \
	}

#define BALAU_InjectNamed_7(TYPE, F0, N0, F1, N1, F2, N2)                                           \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2));                   \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2));                       \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return { BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2) };                         \
	}

#define BALAU_InjectNamed_9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                                   \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)              \
		};                                                                                          \
	}

#define BALAU_InjectNamed_11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                          \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4)                                                                     \
		};                                                                                          \
	}

#define BALAU_InjectNamed_13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)                  \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5)                                                  \
		};                                                                                          \
	}

#define BALAU_InjectNamed_15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6)          \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6)                               \
		};                                                                                          \
	}

#define BALAU_InjectNamed_17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7)  \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6), BALAU_IKF(F7, N7)            \
		};                                                                                          \
	}

#define BALAU_InjectNamed_19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
			, BALAU_IGF(F8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
			, BALAU_IGF(F8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6), BALAU_IKF(F7, N7)            \
			, BALAU_IKF(F8, N8)                                                                     \
		};                                                                                          \
	}

#define BALAU_InjectNamed_21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6), BALAU_IKF(F7, N7)            \
			, BALAU_IKF(F8, N8), BALAU_IKF(F9, N9)                                                  \
		};                                                                                          \
	}

#define BALAU_InjectNamed_23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9), BALAU_IGF(F10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2), BALAU_IGF(F3, N3)            \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6), BALAU_IGF(F7, N7)            \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9), BALAU_IGF(F10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2), BALAU_IKF(F3, N3)            \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6), BALAU_IKF(F7, N7)            \
			, BALAU_IKF(F8, N8), BALAU_IKF(F9, N9), BALAU_IKF(F10, N10)                             \
		};                                                                                          \
	}

#define BALAU_InjectNamed_25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)          \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)          \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9), BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)          \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)          \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9), BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2),   BALAU_IKF(F3, N3)          \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6),   BALAU_IKF(F7, N7)          \
			, BALAU_IKF(F8, N8), BALAU_IKF(F9, N9), BALAU_IKF(F10, N10), BALAU_IKF(F11, N11)        \
		};                                                                                          \
	}

#define BALAU_InjectNamed_27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)          \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)          \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9), BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)        \
			, BALAU_IGF(F12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0), BALAU_IGF(F1, N1), BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)          \
			, BALAU_IGF(F4, N4), BALAU_IGF(F5, N5), BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)          \
			, BALAU_IGF(F8, N8), BALAU_IGF(F9, N9), BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)        \
			, BALAU_IGF(F12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0), BALAU_IKF(F1, N1), BALAU_IKF(F2, N2),   BALAU_IKF(F3, N3)          \
			, BALAU_IKF(F4, N4), BALAU_IKF(F5, N5), BALAU_IKF(F6, N6),   BALAU_IKF(F7, N7)          \
			, BALAU_IKF(F8, N8), BALAU_IKF(F9, N9), BALAU_IKF(F10, N10), BALAU_IKF(F11, N11)        \
			, BALAU_IKF(F12, N12)                                                                   \
		};                                                                                          \
	}

#define BALAU_InjectNamed_29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0),   BALAU_IGF(F1, N1),   BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)      \
			, BALAU_IGF(F4, N4),   BALAU_IGF(F5, N5),   BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)      \
			, BALAU_IGF(F8, N8),   BALAU_IGF(F9, N9),   BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)    \
			, BALAU_IGF(F12, N12), BALAU_IGF(F13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0),   BALAU_IGF(F1, N1),   BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)      \
			, BALAU_IGF(F4, N4),   BALAU_IGF(F5, N5),   BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)      \
			, BALAU_IGF(F8, N8),   BALAU_IGF(F9, N9),   BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)    \
			, BALAU_IGF(F12, N12), BALAU_IGF(F13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0),   BALAU_IKF(F1, N1), BALAU_IKF(F2, N2),   BALAU_IKF(F3, N3)        \
			, BALAU_IKF(F4, N4),   BALAU_IKF(F5, N5), BALAU_IKF(F6, N6),   BALAU_IKF(F7, N7)        \
			, BALAU_IKF(F8, N8),   BALAU_IKF(F9, N9), BALAU_IKF(F10, N10), BALAU_IKF(F11, N11)      \
			, BALAU_IKF(F12, N12), BALAU_IKF(F13, N13)                                              \
		};                                                                                          \
	}

#define BALAU_InjectNamed_31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0),   BALAU_IGF(F1, N1),   BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)      \
			, BALAU_IGF(F4, N4),   BALAU_IGF(F5, N5),   BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)      \
			, BALAU_IGF(F8, N8),   BALAU_IGF(F9, N9),   BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)    \
			, BALAU_IGF(F12, N12), BALAU_IGF(F13, N13), BALAU_IGF(F14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0),   BALAU_IGF(F1, N1),   BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)      \
			, BALAU_IGF(F4, N4),   BALAU_IGF(F5, N5),   BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)      \
			, BALAU_IGF(F8, N8),   BALAU_IGF(F9, N9),   BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)    \
			, BALAU_IGF(F12, N12), BALAU_IGF(F13, N13), BALAU_IGF(F14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0),   BALAU_IKF(F1, N1),   BALAU_IKF(F2, N2),   BALAU_IKF(F3, N3)      \
			, BALAU_IKF(F4, N4),   BALAU_IKF(F5, N5),   BALAU_IKF(F6, N6),   BALAU_IKF(F7, N7)      \
			, BALAU_IKF(F8, N8),   BALAU_IKF(F9, N9),   BALAU_IKF(F10, N10), BALAU_IKF(F11, N11)    \
			, BALAU_IKF(F12, N12), BALAU_IKF(F13, N13), BALAU_IKF(F14, N14)                         \
		};                                                                                          \
	}

#define BALAU_InjectNamed_33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGF(F0, N0),   BALAU_IGF(F1, N1),   BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)      \
			, BALAU_IGF(F4, N4),   BALAU_IGF(F5, N5),   BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)      \
			, BALAU_IGF(F8, N8),   BALAU_IGF(F9, N9),   BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)    \
			, BALAU_IGF(F12, N12), BALAU_IGF(F13, N13), BALAU_IGF(F14, N14), BALAU_IGF(F15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGF(F0, N0),   BALAU_IGF(F1, N1),   BALAU_IGF(F2, N2),   BALAU_IGF(F3, N3)      \
			, BALAU_IGF(F4, N4),   BALAU_IGF(F5, N5),   BALAU_IGF(F6, N6),   BALAU_IGF(F7, N7)      \
			, BALAU_IGF(F8, N8),   BALAU_IGF(F9, N9),   BALAU_IGF(F10, N10), BALAU_IGF(F11, N11)    \
			, BALAU_IGF(F12, N12), BALAU_IGF(F13, N13), BALAU_IGF(F14, N14), BALAU_IGF(F15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKF(F0, N0),   BALAU_IKF(F1, N1),   BALAU_IKF(F2, N2),   BALAU_IKF(F3, N3)      \
			, BALAU_IKF(F4, N4),   BALAU_IKF(F5, N5),   BALAU_IKF(F6, N6),   BALAU_IKF(F7, N7)      \
			, BALAU_IKF(F8, N8),   BALAU_IKF(F9, N9),   BALAU_IKF(F10, N10), BALAU_IKF(F11, N11)    \
			, BALAU_IKF(F12, N12), BALAU_IKF(F13, N13), BALAU_IKF(F14, N14), BALAU_IKF(F15, N15)    \
		};                                                                                          \
	}

///////////////////////////////// BALAU_InjectConstructNamed macros /////////////////////////////////

#define BALAU_InjectConstructNamed_1(TYPE)                                                          \
	BALAU_InjectNamed_1(TYPE)                                                                       \
	                                                                                                \
	public: TYPE() {}

#define BALAU_InjectConstructNamed_3(TYPE, F0, N0)                                                  \
	BALAU_InjectNamed_3(TYPE, F0, N0)                                                               \
	                                                                                                \
	public: explicit TYPE(BALAU_IPF(F0)) : BALAU_IMF(F0) {}

#define BALAU_InjectConstructNamed_5(TYPE, F0, N0, F1, N1)                                          \
	BALAU_InjectNamed_5(TYPE, F0, N0, F1, N1)                                                       \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1))                                                      \
		: BALAU_IMF(F0), BALAU_IMF(F1) {}

#define BALAU_InjectConstructNamed_7(TYPE, F0, N0, F1, N1, F2, N2)                                  \
	BALAU_InjectNamed_7(TYPE, F0, N0, F1, N1, F2, N2)                                               \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2))                                       \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2) {}

#define BALAU_InjectConstructNamed_9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                          \
	BALAU_InjectNamed_9(TYPE, F0, N0, F1, N1, F2, N2, F3, N3)                                       \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3))                        \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3) {}

#define BALAU_InjectConstructNamed_11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                 \
	BALAU_InjectNamed_11(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4)                              \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3), BALAU_IPF(F4))         \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3), BALAU_IMF(F4) {}

#define BALAU_InjectConstructNamed_13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)         \
	BALAU_InjectNamed_13(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5)                      \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3),                        \
	             BALAU_IPF(F4), BALAU_IPF(F5))                                                      \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3)                                \
		, BALAU_IMF(F4), BALAU_IMF(F5) {}

#define BALAU_InjectConstructNamed_15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6) \
	BALAU_InjectNamed_15(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6)              \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3),                        \
	             BALAU_IPF(F4), BALAU_IPF(F5), BALAU_IPF(F6))                                       \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3)                                \
	 	, BALAU_IMF(F4), BALAU_IMF(F5), BALAU_IMF(F6) {}

#define BALAU_InjectConstructNamed_17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7) \
	BALAU_InjectNamed_17(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7)      \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3),                        \
	             BALAU_IPF(F4), BALAU_IPF(F5), BALAU_IPF(F6), BALAU_IPF(F7))                        \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3)                                \
		, BALAU_IMF(F4), BALAU_IMF(F5), BALAU_IMF(F6), BALAU_IMF(F7) {}

#define BALAU_InjectConstructNamed_19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	BALAU_InjectNamed_19(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3),                        \
	             BALAU_IPF(F4), BALAU_IPF(F5), BALAU_IPF(F6), BALAU_IPF(F7),                        \
	             BALAU_IPF(F8))                                                                     \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3)                                \
		, BALAU_IMF(F4), BALAU_IMF(F5), BALAU_IMF(F6), BALAU_IMF(F7)                                \
		, BALAU_IMF(F8) {}

#define BALAU_InjectConstructNamed_21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	BALAU_InjectNamed_21(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3),                        \
	             BALAU_IPF(F4), BALAU_IPF(F5), BALAU_IPF(F6), BALAU_IPF(F7),                        \
	             BALAU_IPF(F8), BALAU_IPF(F9))                                                      \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3)                                \
		, BALAU_IMF(F4), BALAU_IMF(F5), BALAU_IMF(F6), BALAU_IMF(F7)                                \
		, BALAU_IMF(F8), BALAU_IMF(F9) {}

#define BALAU_InjectConstructNamed_23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	BALAU_InjectNamed_23(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0), BALAU_IPF(F1), BALAU_IPF(F2), BALAU_IPF(F3),                        \
	             BALAU_IPF(F4), BALAU_IPF(F5), BALAU_IPF(F6), BALAU_IPF(F7),                        \
	             BALAU_IPF(F8), BALAU_IPF(F9), BALAU_IPF(F10))                                      \
		: BALAU_IMF(F0), BALAU_IMF(F1), BALAU_IMF(F2), BALAU_IMF(F3)                                \
		, BALAU_IMF(F4), BALAU_IMF(F5), BALAU_IMF(F6), BALAU_IMF(F7)                                \
		, BALAU_IMF(F8), BALAU_IMF(F9), BALAU_IMF(F10) {}

#define BALAU_InjectConstructNamed_25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	BALAU_InjectNamed_25(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0),  BALAU_IPF(F1),  BALAU_IPF(F2),  BALAU_IPF(F3),                     \
	             BALAU_IPF(F4),  BALAU_IPF(F5),  BALAU_IPF(F6),  BALAU_IPF(F7),                     \
	             BALAU_IPF(F8),  BALAU_IPF(F9),  BALAU_IPF(F10), BALAU_IPF(F11))                    \
		: BALAU_IMF(F0),  BALAU_IMF(F1),  BALAU_IMF(F2),  BALAU_IMF(F3)                             \
		, BALAU_IMF(F4),  BALAU_IMF(F5),  BALAU_IMF(F6),  BALAU_IMF(F7)                             \
		, BALAU_IMF(F8),  BALAU_IMF(F9),  BALAU_IMF(F10), BALAU_IMF(F11)                            \
	{}

#define BALAU_InjectConstructNamed_27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	BALAU_InjectNamed_27(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0),  BALAU_IPF(F1),  BALAU_IPF(F2),  BALAU_IPF(F3),                     \
	             BALAU_IPF(F4),  BALAU_IPF(F5),  BALAU_IPF(F6),  BALAU_IPF(F7),                     \
	             BALAU_IPF(F8),  BALAU_IPF(F9),  BALAU_IPF(F10), BALAU_IPF(F11),                    \
	             BALAU_IPF(F12))                                                                    \
		: BALAU_IMF(F0),  BALAU_IMF(F1),  BALAU_IMF(F2),  BALAU_IMF(F3)                             \
		, BALAU_IMF(F4),  BALAU_IMF(F5),  BALAU_IMF(F6),  BALAU_IMF(F7)                             \
		, BALAU_IMF(F8),  BALAU_IMF(F9),  BALAU_IMF(F10), BALAU_IMF(F11)                            \
		, BALAU_IMF(F12) {}

#define BALAU_InjectConstructNamed_29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	BALAU_InjectNamed_29(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0),  BALAU_IPF(F1),  BALAU_IPF(F2),  BALAU_IPF(F3),                     \
	             BALAU_IPF(F4),  BALAU_IPF(F5),  BALAU_IPF(F6),  BALAU_IPF(F7),                     \
	             BALAU_IPF(F8),  BALAU_IPF(F9),  BALAU_IPF(F10), BALAU_IPF(F11),                    \
	             BALAU_IPF(F12), BALAU_IPF(F13))                                                    \
		: BALAU_IMF(F0),  BALAU_IMF(F1),  BALAU_IMF(F2),  BALAU_IMF(F3)                             \
		, BALAU_IMF(F4),  BALAU_IMF(F5),  BALAU_IMF(F6),  BALAU_IMF(F7)                             \
		, BALAU_IMF(F8),  BALAU_IMF(F9),  BALAU_IMF(F10), BALAU_IMF(F11)                            \
		, BALAU_IMF(F12), BALAU_IMF(F13) {}

#define BALAU_InjectConstructNamed_31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	BALAU_InjectNamed_31(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0),  BALAU_IPF(F1),  BALAU_IPF(F2),  BALAU_IPF(F3),                     \
	             BALAU_IPF(F4),  BALAU_IPF(F5),  BALAU_IPF(F6),  BALAU_IPF(F7),                     \
	             BALAU_IPF(F8),  BALAU_IPF(F9),  BALAU_IPF(F10), BALAU_IPF(F11),                    \
	             BALAU_IPF(F12), BALAU_IPF(F13), BALAU_IPF(F14))                                    \
		: BALAU_IMF(F0),  BALAU_IMF(F1),  BALAU_IMF(F2),  BALAU_IMF(F3)                             \
		, BALAU_IMF(F4),  BALAU_IMF(F5),  BALAU_IMF(F6),  BALAU_IMF(F7)                             \
		, BALAU_IMF(F8),  BALAU_IMF(F9),  BALAU_IMF(F10), BALAU_IMF(F11)                            \
		, BALAU_IMF(F12), BALAU_IMF(F13), BALAU_IMF(F14) {}

#define BALAU_InjectConstructNamed_33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	BALAU_InjectNamed_33(TYPE, F0, N0, F1, N1, F2, N2, F3, N3, F4, N4, F5, N5, F6, N6, F7, N7, F8, N8, F9, N9, F10, N10, F11, N11, F12, N12, F13, N13, F14, N14, F15, N15) \
	                                                                                                \
	public: TYPE(BALAU_IPF(F0),  BALAU_IPF(F1),  BALAU_IPF(F2),  BALAU_IPF(F3)                      \
	             BALAU_IPF(F4),  BALAU_IPF(F5),  BALAU_IPF(F6),  BALAU_IPF(F7)                      \
	             BALAU_IPF(F8),  BALAU_IPF(F9),  BALAU_IPF(F10), BALAU_IPF(F11)                     \
	             BALAU_IPF(F12), BALAU_IPF(F13), BALAU_IPF(F14), BALAU_IPF(F15))                    \
		: BALAU_IMF(F0),  BALAU_IMF(F1),  BALAU_IMF(F2),  BALAU_IMF(F3)                             \
		, BALAU_IMF(F4),  BALAU_IMF(F5),  BALAU_IMF(F6),  BALAU_IMF(F7)                             \
		, BALAU_IMF(F8),  BALAU_IMF(F9),  BALAU_IMF(F10), BALAU_IMF(F11)                            \
		, BALAU_IMF(F12), BALAU_IMF(F13), BALAU_IMF(F14), BALAU_IMF(F15) {}

/////////////////////////////////// BalauInjectNamedTypes macros ////////////////////////////////////

#define BALAU_InjectNamedTypes_1(TYPE)                                                              \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE();                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE();                                                                              \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {};                                                                                  \
	}

#define BALAU_InjectNamedTypes_3(TYPE, T0, N0)                                                      \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(BALAU_IGT(T0, N0));                                                         \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(BALAU_IGT(T0, N0));                                                             \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return { BALAU_IKT(T0, N0) };                                                               \
	}

#define BALAU_InjectNamedTypes_5(TYPE, T0, N0, T1, N1)                                              \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(BALAU_IGT(T0, N0), BALAU_IGT(T1, N1));                                      \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(BALAU_IGT(T0, N0), BALAU_IGT(T1, N1));                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return { BALAU_IKT(T0, N0), BALAU_IKT(T1, N1) };                                            \
	}

#define BALAU_InjectNamedTypes_7(TYPE, T0, N0, T1, N1, T2, N2)                                      \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2));                   \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2));                       \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return { BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2) };                         \
	}

#define BALAU_InjectNamedTypes_9(TYPE, T0, N0, T1, N1, T2, N2, T3, N3)                              \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)              \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_11(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4)                     \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4)                                                                     \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_13(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5)             \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5)                                                  \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_15(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6)     \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6)                               \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5), BALAU_IKT(T6, N6)                               \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_17(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5), BALAU_IKT(T6, N6), BALAU_IKT(T7, N7)            \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_19(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
			, BALAU_IGT(T8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
			, BALAU_IGT(T8, N8)                                                                     \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5), BALAU_IKT(T6, N6), BALAU_IKT(T7, N7)            \
			, BALAU_IKT(T8, N8)                                                                     \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_21(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
			, BALAU_IGT(T8, N8), BALAU_IGT(T9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
			, BALAU_IGT(T8, N8), BALAU_IGT(T9, N9)                                                  \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5), BALAU_IKT(T6, N6), BALAU_IKT(T7, N7)            \
			, BALAU_IKT(T8, N8), BALAU_IKT(T9, N9)                                                  \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_23(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
			, BALAU_IGT(T8, N8), BALAU_IGT(T9, N9), BALAU_IGT(T10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2), BALAU_IGT(T3, N3)            \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6), BALAU_IGT(T7, N7)            \
			, BALAU_IGT(T8, N8), BALAU_IGT(T9, N9), BALAU_IGT(T10, N10)                             \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2), BALAU_IKT(T3, N3)            \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5), BALAU_IKT(T6, N6), BALAU_IKT(T7, N7)            \
			, BALAU_IKT(T8, N8), BALAU_IKT(T9, N9), BALAU_IKT(T10, N10)                             \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_25(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)          \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)          \
			, BALAU_IGT(T8, N8), BALAU_IGT(T9, N9), BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0), BALAU_IGT(T1, N1), BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)          \
			, BALAU_IGT(T4, N4), BALAU_IGT(T5, N5), BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)          \
			, BALAU_IGT(T8, N8), BALAU_IGT(T9, N9), BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)        \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0), BALAU_IKT(T1, N1), BALAU_IKT(T2, N2),   BALAU_IKT(T3, N3)          \
			, BALAU_IKT(T4, N4), BALAU_IKT(T5, N5), BALAU_IKT(T6, N6),   BALAU_IKT(T7, N7)          \
			, BALAU_IKT(T8, N8), BALAU_IKT(T9, N9), BALAU_IKT(T10, N10), BALAU_IKT(T11, N11)        \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_27(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12)                                                                   \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0),   BALAU_IKT(T1, N1), BALAU_IKT(T2, N2),   BALAU_IKT(T3, N3)        \
			, BALAU_IKT(T4, N4),   BALAU_IKT(T5, N5), BALAU_IKT(T6, N6),   BALAU_IKT(T7, N7)        \
			, BALAU_IKT(T8, N8),   BALAU_IKT(T9, N9), BALAU_IKT(T10, N10), BALAU_IKT(T11, N11)      \
			, BALAU_IKT(T12, N12)                                                                   \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_29(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12), BALAU_IGT(T13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12), BALAU_IGT(T13, N13)                                              \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0),   BALAU_IKT(T1, N1), BALAU_IKT(T2, N2),   BALAU_IKT(T3, N3)        \
			, BALAU_IKT(T4, N4),   BALAU_IKT(T5, N5), BALAU_IKT(T6, N6),   BALAU_IKT(T7, N7)        \
			, BALAU_IKT(T8, N8),   BALAU_IKT(T9, N9), BALAU_IKT(T10, N10), BALAU_IKT(T11, N11)      \
			, BALAU_IKT(T12, N12), BALAU_IKT(T13, N13)                                              \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_31(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12), BALAU_IGT(T13, N13), BALAU_IGT(T14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12), BALAU_IGT(T13, N13), BALAU_IGT(T14, N14)                         \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0),   BALAU_IKT(T1, N1),   BALAU_IKT(T2, N2),   BALAU_IKT(T3, N3)      \
			, BALAU_IKT(T4, N4),   BALAU_IKT(T5, N5),   BALAU_IKT(T6, N6),   BALAU_IKT(T7, N7)      \
			, BALAU_IKT(T8, N8),   BALAU_IKT(T9, N9),   BALAU_IKT(T10, N10), BALAU_IKT(T11, N11)    \
			, BALAU_IKT(T12, N12), BALAU_IKT(T13, N13), BALAU_IKT(T14, N14)                         \
		};                                                                                          \
	}

#define BALAU_InjectNamedTypes_33(TYPE, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11, T12, N12, T13, N13, T14, N14, T15, N15) \
	public: static TYPE * BALAU_newHeapInstance(const ::Balau::Injector &  BALAU_Injector) {        \
		return new TYPE(                                                                            \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12), BALAU_IGT(T13, N13), BALAU_IGT(T14, N14), BALAU_IGT(T15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static TYPE BALAU_newStackInstance(const ::Balau::Injector &  BALAU_Injector) {         \
		return TYPE(                                                                                \
			  BALAU_IGT(T0, N0),   BALAU_IGT(T1, N1),   BALAU_IGT(T2, N2),   BALAU_IGT(T3, N3)      \
			, BALAU_IGT(T4, N4),   BALAU_IGT(T5, N5),   BALAU_IGT(T6, N6),   BALAU_IGT(T7, N7)      \
			, BALAU_IGT(T8, N8),   BALAU_IGT(T9, N9),   BALAU_IGT(T10, N10), BALAU_IGT(T11, N11)    \
			, BALAU_IGT(T12, N12), BALAU_IGT(T13, N13), BALAU_IGT(T14, N14), BALAU_IGT(T15, N15)    \
		);                                                                                          \
	}                                                                                               \
	                                                                                                \
	public: static std::vector<::Balau::Impl::BindingKey> BALAU_getDependencyKeys() {               \
		return {                                                                                    \
			  BALAU_IKT(T0, N0),   BALAU_IKT(T1, N1),   BALAU_IKT(T2, N2),   BALAU_IKT(T3, N3)      \
			, BALAU_IKT(T4, N4),   BALAU_IKT(T5, N5),   BALAU_IKT(T6, N6),   BALAU_IKT(T7, N7)      \
			, BALAU_IKT(T8, N8),   BALAU_IKT(T9, N9),   BALAU_IKT(T10, N10), BALAU_IKT(T11, N11)    \
			, BALAU_IKT(T12, N12), BALAU_IKT(T13, N13), BALAU_IKT(T14, N14), BALAU_IKT(T15, N15)    \
		};                                                                                          \
	}

//////////////////////////////////////// BalauInject macros /////////////////////////////////////////

#define BALAU_Inject_1(TYPE) \
	BALAU_InjectNamed_1(TYPE)

#define BALAU_Inject_2(TYPE, F0) \
	BALAU_InjectNamed_3(TYPE, F0, "")

#define BALAU_Inject_3(TYPE, F0, F1) \
	BALAU_InjectNamed_5(TYPE, F0, "", F1, "")

#define BALAU_Inject_4(TYPE, F0, F1, F2) \
	BALAU_InjectNamed_7(TYPE, F0, "", F1, "", F2, "")

#define BALAU_Inject_5(TYPE, F0, F1, F2, F3) \
	BALAU_InjectNamed_9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define BALAU_Inject_6(TYPE, F0, F1, F2, F3, F4) \
	BALAU_InjectNamed_11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define BALAU_Inject_7(TYPE, F0, F1, F2, F3, F4, F5) \
	BALAU_InjectNamed_13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define BALAU_Inject_8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	BALAU_InjectNamed_15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define BALAU_Inject_9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	BALAU_InjectNamed_17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define BALAU_Inject_10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	BALAU_InjectNamed_19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define BALAU_Inject_11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	BALAU_InjectNamed_21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define BALAU_Inject_12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	BALAU_InjectNamed_23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define BALAU_Inject_13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	BALAU_InjectNamed_25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define BALAU_Inject_14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	BALAU_InjectNamed_27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define BALAU_Inject_15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	BALAU_InjectNamed_29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define BALAU_Inject_16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	BALAU_InjectNamed_31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define BALAU_Inject_17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	BALAU_InjectNamed_33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

//////////////////////////////////// BalauInjectConstruct macros ////////////////////////////////////

#define BALAU_InjectConstruct_1(TYPE) \
	BALAU_InjectConstructNamed_1(TYPE)

#define BALAU_InjectConstruct_2(TYPE, F0) \
	BALAU_InjectConstructNamed_3(TYPE, F0, "")

#define BALAU_InjectConstruct_3(TYPE, F0, F1) \
	BALAU_InjectConstructNamed_5(TYPE, F0, "", F1, "")

#define BALAU_InjectConstruct_4(TYPE, F0, F1, F2) \
	BALAU_InjectConstructNamed_7(TYPE, F0, "", F1, "", F2, "")

#define BALAU_InjectConstruct_5(TYPE, F0, F1, F2, F3) \
	BALAU_InjectConstructNamed_9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define BALAU_InjectConstruct_6(TYPE, F0, F1, F2, F3, F4) \
	BALAU_InjectConstructNamed_11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define BALAU_InjectConstruct_7(TYPE, F0, F1, F2, F3, F4, F5) \
	BALAU_InjectConstructNamed_13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define BALAU_InjectConstruct_8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	BALAU_InjectConstructNamed_15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define BALAU_InjectConstruct_9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	BALAU_InjectConstructNamed_17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define BALAU_InjectConstruct_10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	BALAU_InjectConstructNamed_19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define BALAU_InjectConstruct_11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	BALAU_InjectConstructNamed_21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define BALAU_InjectConstruct_12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	BALAU_InjectConstructNamed_23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define BALAU_InjectConstruct_13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	BALAU_InjectConstructNamed_25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define BALAU_InjectConstruct_14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	BALAU_InjectConstructNamed_27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define BALAU_InjectConstruct_15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	BALAU_InjectConstructNamed_29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define BALAU_InjectConstruct_16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	BALAU_InjectConstructNamed_31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define BALAU_InjectConstruct_17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	BALAU_InjectConstructNamed_33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

////////////////////////////////////// BalauInjectTypes macros //////////////////////////////////////

#define BALAU_InjectTypes_1(TYPE) \
	BALAU_InjectNamedTypes_1(TYPE)

#define BALAU_InjectTypes_2(TYPE, F0) \
	BALAU_InjectNamedTypes_3(TYPE, F0, "")

#define BALAU_InjectTypes_3(TYPE, F0, F1) \
	BALAU_InjectNamedTypes_5(TYPE, F0, "", F1, "")

#define BALAU_InjectTypes_4(TYPE, F0, F1, F2) \
	BALAU_InjectNamedTypes_7(TYPE, F0, "", F1, "", F2, "")

#define BALAU_InjectTypes_5(TYPE, F0, F1, F2, F3) \
	BALAU_InjectNamedTypes_9(TYPE, F0, "", F1, "", F2, "", F3, "")

#define BALAU_InjectTypes_6(TYPE, F0, F1, F2, F3, F4) \
	BALAU_InjectNamedTypes_11(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "")

#define BALAU_InjectTypes_7(TYPE, F0, F1, F2, F3, F4, F5) \
	BALAU_InjectNamedTypes_13(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "")

#define BALAU_InjectTypes_8(TYPE, F0, F1, F2, F3, F4, F5, F6) \
	BALAU_InjectNamedTypes_15(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "")

#define BALAU_InjectTypes_9(TYPE, F0, F1, F2, F3, F4, F5, F6, F7) \
	BALAU_InjectNamedTypes_17(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "")

#define BALAU_InjectTypes_10(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8) \
	BALAU_InjectNamedTypes_19(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "")

#define BALAU_InjectTypes_11(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
	BALAU_InjectNamedTypes_21(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "")

#define BALAU_InjectTypes_12(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
	BALAU_InjectNamedTypes_23(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "")

#define BALAU_InjectTypes_13(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) \
	BALAU_InjectNamedTypes_25(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "")

#define BALAU_InjectTypes_14(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) \
	BALAU_InjectNamedTypes_27(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "")

#define BALAU_InjectTypes_15(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) \
	BALAU_InjectNamedTypes_29(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "")

#define BALAU_InjectTypes_16(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) \
	BALAU_InjectNamedTypes_31(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "")

#define BALAU_InjectTypes_17(TYPE, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) \
	BALAU_InjectNamedTypes_33(TYPE, F0, "", F1, "", F2, "", F3, "", F4, "", F5, "", F6, "", F7, "", F8, "", F9, "", F10, "", F11, "", F12, "", F13, "", F14, "", F15, "")

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define BALAU_Inject_(N)                    BALAU_Inject_##N
#define BALAU_Inject_Eval(N)                BALAU_Inject_(N)

#define BALAU_InjectNamed_(N)               BALAU_InjectNamed_##N
#define BALAU_InjectNamed_Eval(N)           BALAU_InjectNamed_(N)

#define BALAU_InjectConstruct_(N)           BALAU_InjectConstruct_##N
#define BALAU_InjectConstruct_Eval(N)       BALAU_InjectConstruct_(N)

#define BALAU_InjectConstructNamed_(N)      BALAU_InjectConstructNamed_##N
#define BALAU_InjectConstructNamed_Eval(N)  BALAU_InjectConstructNamed_(N)

#define BALAU_InjectTypes_(N)               BALAU_InjectTypes_##N
#define BALAU_InjectTypes_Eval(N)           BALAU_InjectTypes_(N)

#define BALAU_InjectNamedTypes_(N)          BALAU_InjectNamedTypes_##N
#define BALAU_InjectNamedTypes_Eval(N)      BALAU_InjectNamedTypes_(N)

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define BALAU_Inject(...)                   BALAU_Inject_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectNamed(...)              BALAU_InjectNamed_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectConstruct(...)          BALAU_InjectConstruct_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectConstructNamed(...)     BALAU_InjectConstructNamed_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectTypes(...)              BALAU_InjectTypes_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)
#define BALAU_InjectNamedTypes(...)         BALAU_InjectNamedTypes_Eval(BalauVariadicNArg(__VA_ARGS__))(__VA_ARGS__)

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__INJECTOR_MACROS
