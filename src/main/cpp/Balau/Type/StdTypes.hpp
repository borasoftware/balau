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
/// @file StdTypes.hpp
///
/// Core includes, typedefs and functions.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__STD_TYPES
#define COM_BORA_SOFTWARE__BALAU_TYPE__STD_TYPES

// Switches for optional features.
#include <Balau/BalauConfig.hpp>

/////////////////////////// Additional documentation //////////////////////////

///
/// @mainpage
///
/// Developer manual: https://borasoftware.com/doc/balau/latest/manual
/// <p>
/// API documentation: https://borasoftware.com/doc/balau/latest/api
/// <p>
/// Main %Balau Git repository: https://github.com/borasoftware/balau
/// <p>
/// License: https://borasoftware.com/licenses/balau-license.html
/// <p>
/// %Balau is a C++ application framework designed for enterprise C++ software development.
///
/// Following the recent revisions of the language, C++ has matured to become an attractive
/// candidate for rapid enterprise application development. %Balau provides tools designed
/// to support the rapid development of high performance C++ enterprise applications.
///
/// %Balau builds on the foundations of the ICU and Boost projects, and focuses on using
/// modern C++17 features and the standard unicode string classes.
///
/// The library has been conceived for the development of applications that have a
/// dependency injection based architecture, have complex logging requirements, and will
/// be developed with a test driven development methodology. %Balau has also been designed
/// to provide a base on which Unicode aware C++ software applications may be written
/// with ease.
///
/// Four key components of the %Balau library are
/// the <a href="classBalau_1_1Injector.html">injector</a>,
/// the <a href="classBalau_1_1EnvironmentConfiguration.html">environment configuration</a> framework,
/// the <a href="classBalau_1_1Logger.html">logging system</a>, and
/// the <a href="classBalau_1_1Testing_1_1TestRunner.html">test runner</a>. In
/// this respect, part of %Balau is a C++ equivalent to the de facto standard Java
/// application development components consisting of the Java standard library for
/// Unicode support, Guice/Spring for dependency injection and environment configuration,
/// Log4j/Slf4j/Logback for logging, and JUnit/TestNG for testing.
///
/// In addition to the injector, environment configuration, logger, and test runner, %Balau
/// also provides a set of components and utilities with simple APIs, including an
/// HTTP/WebSocket web application framework.
///

/// @namespace Balau The root %Balau namespace.

/// @namespace Balau::Concurrent Concurrency control classes.

/// @namespace Balau::Container Various container classes, apart from interprocess containers.

/// @namespace Balau::Documentation Classes used for providing documentation.

/// @namespace Balau::Exception All exception classes.

/// @namespace Balau::Interprocess %Interprocess functionality including interprocess containers.

/// @namespace Balau::Lang Parsing tools and parser implementations.

/// @namespace Balau::Lang::Property The %Property parser.

/// @namespace Balau::Lang::Property::AST The %Property parser abstract syntax tree node classes.

/// @namespace Balau::Network Components and utilities working on network data transmission.

/// @namespace Balau::Network::Http Components and utilities working on HTTP data transmission.

/// @namespace Balau::Network::Http::HttpWebApps A set of HTTP web apps provided by the %Balau library.

/// @namespace Balau::Network::Http::WsWebApps A set of WebSocket web apps provided by the %Balau library.

/// @namespace Balau::Resource The unified resource class hierarchy.

/// @namespace Balau::System %System utilities and wrappers.

/// @namespace Balau::Testing The test runner and test assertion functions.

/// @namespace Balau::Testing::TestRenderers Pre-defined renderers used to print test assertion failures.

/// @namespace Balau::Type The character utilities and other basic functionality.

/// @namespace Balau::Util Utility functions.

///////////////////////////////////////////////////////////////////////////////

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <cassert>
#include <climits>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <typeindex>
#include <utility>
#include <vector>

//////////////////////////// Check primitive sizes ////////////////////////////

static_assert(CHAR_BIT                   == 8, "char size is not 8 bits");

static_assert(sizeof(signed char)        == 1, "signed char size is not 8 bits");
static_assert(sizeof(signed short)       == 2, "signed short size is not 16 bits");
static_assert(sizeof(signed int)         == 4, "signed int size is not 32 bits");
static_assert(sizeof(signed long long)   == 8, "signed long long size is not 64 bits");

static_assert(sizeof(unsigned char)      == 1, "unsigned char size is not 8 bits");
static_assert(sizeof(unsigned short)     == 2, "unsigned short size is not 16 bits");
static_assert(sizeof(unsigned int)       == 4, "unsigned int size is not 32 bits");
static_assert(sizeof(unsigned long long) == 8, "unsigned long long size is not 64 bits");

static_assert(sizeof(int8_t)             == 1, "int8_t is not 8 bits");
static_assert(sizeof(int16_t)            == 2, "int16_t is not 16 bits");
static_assert(sizeof(int32_t)            == 4, "int32_t is not 32 bits");
static_assert(sizeof(int64_t)            == 8, "int64_t is not 64 bits");

static_assert(sizeof(uint8_t)            == 1, "uint8_t is not 8 bits");
static_assert(sizeof(uint16_t)           == 2, "uint16_t is not 16 bits");
static_assert(sizeof(uint32_t)           == 4, "uint32_t is not 32 bits");
static_assert(sizeof(uint64_t)           == 8, "uint64_t is not 64 bits");

///////////////////////////// Additional pre-defs /////////////////////////////

#if INTPTR_MAX == INT32_MAX
	#define BalauPointerSize_ 32
#elif INTPTR_MAX == INT64_MAX
	#define BalauPointerSize_ 64
#else
	#error "Pointer size not supported for this platform."
#endif

///
/// The pointer size in bits of the platform.
///
#define BalauPointerSize BalauPointerSize_

////////////////////////////////// Backports //////////////////////////////////

#ifdef BALAU_STD_STRING_VIEW_AVAILABLE
	#include <string_view>
#else
	#include <Balau/Type/Impl/StringView.hpp>
#endif

///////////////////////////////////////////////////////////////////////////////

///
/// The version of the Balau library.
///
extern const std::string BalauVersion;

///////////////////////////////////////////////////////////////////////////////

namespace std { // NOLINT

// Missing standard library stream typedefs.

using u16istream = basic_istream<char16_t>;
using u16ostream = basic_ostream<char16_t>;

using u16ifstream = basic_ifstream<char16_t>;
using u16ofstream = basic_ofstream<char16_t>;

using u16istringstream = basic_istringstream<char16_t>;
using u16ostringstream = basic_ostringstream<char16_t>;

using u32istream = basic_istream<char32_t>;
using u32ostream = basic_ostream<char32_t>;

using u32ifstream = basic_ifstream<char32_t>;
using u32ofstream = basic_ofstream<char32_t>;

using u32istringstream = basic_istringstream<char32_t>;
using u32ostringstream = basic_ostringstream<char32_t>;

// Additional hash / equal_to functions.

template <typename T> struct hash<const vector<T>> {
	size_t operator () (const vector<T> & vector) const noexcept {
		hash<T> elementHashFunction;

		size_t h = vector.size();
		for (auto & element : vector) {
			h ^= elementHashFunction(element) + 0x9e3779b9 + (h << 6U) + (h >> 2U);
		}

		return h;
	}
};

template <> struct hash<char *> {
	size_t operator () (char * const & ptr) const noexcept {
		return (((size_t) ptr) >> 6U) * 31U;
	}
};

template <typename T> struct equal_to<vector<T>> {
	bool operator () (const vector<T> & lhs, const vector<T> & rhs) const {
		if (lhs.size() != rhs.size()) {
			return false;
		}

		for (size_t m = 0; m < lhs.size(); m++) {
			if (!equal_to(lhs[m], rhs[m])) {
				return false;
			}
		}

		return true;
	}
};

} // namespace std

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__STD_TYPES
