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

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST_DATA
#define COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST_DATA

#include <Balau/Exception/ContainerExceptions.hpp>
#include <Balau/Util/Strings.hpp>

struct A {
	int value;
};

struct B {
	int value;
};

inline std::string toString(const A & a) {
	return toString(a.value);
}

inline std::string toString(const B & b) {
	return toString(b.value);
}

// Custom comparison functions.

inline bool operator == (const A & lhs, const A & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator == (const A & lhs, const B & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator == (const B & lhs, const A & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator == (const B & lhs, const B & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator != (const A & lhs, const A & rhs) {
	return lhs.value != rhs.value;
}

inline bool operator != (const A & lhs, const B & rhs) {
	return lhs.value != rhs.value;
}

inline bool operator != (const B & lhs, const A & rhs) {
	return lhs.value != rhs.value;
}

inline bool operator != (const B & lhs, const B & rhs) {
	return lhs.value != rhs.value;
}

inline bool operator > (const A & lhs, const A & rhs) {
	return lhs.value > rhs.value;
}

inline bool operator > (const A & lhs, const B & rhs) {
	return lhs.value > rhs.value;
}

inline bool operator > (const B & lhs, const A & rhs) {
	return lhs.value > rhs.value;
}

inline bool operator > (const B & lhs, const B & rhs) {
	return lhs.value > rhs.value;
}

inline bool operator >= (const A & lhs, const A & rhs) {
	return lhs.value >= rhs.value;
}

inline bool operator >= (const A & lhs, const B & rhs) {
	return lhs.value >= rhs.value;
}

inline bool operator >= (const B & lhs, const A & rhs) {
	return lhs.value >= rhs.value;
}

inline bool operator >= (const B & lhs, const B & rhs) {
	return lhs.value >= rhs.value;
}

inline bool operator < (const A & lhs, const A & rhs) {
	return lhs.value < rhs.value;
}

inline bool operator < (const A & lhs, const B & rhs) {
	return lhs.value < rhs.value;
}

inline bool operator < (const B & lhs, const A & rhs) {
	return lhs.value < rhs.value;
}

inline bool operator < (const B & lhs, const B & rhs) {
	return lhs.value < rhs.value;
}

inline bool operator <= (const A & lhs, const A & rhs) {
	return lhs.value <= rhs.value;
}

inline bool operator <= (const A & lhs, const B & rhs) {
	return lhs.value <= rhs.value;
}

inline bool operator <= (const B & lhs, const A & rhs) {
	return lhs.value <= rhs.value;
}

inline bool operator <= (const B & lhs, const B & rhs) {
	return lhs.value <= rhs.value;
}

inline A operator + (const A & lhs, int rhs) {
	return A { lhs.value + rhs };
}

inline A operator - (const A & lhs, int rhs) {
	return A { lhs.value - rhs };
}

inline B operator + (const B & lhs, int rhs) {
	return B { lhs.value + rhs };
}

inline B operator - (const B & lhs, int rhs) {
	return B { lhs.value - rhs };
}

struct C {
	std::string value;

	size_t length() const {
		return value.length();
	}

	C substr(size_t start, size_t count) const {
		return C { value.substr(start, count) };
	}

	std::string::const_iterator begin() const {
		return value.begin();
	}

	std::string::const_iterator end() const {
		return value.end();
	}
};

struct D {
	std::string value;

	size_t length() const {
		return value.length();
	}

	D substr(size_t start, size_t count) const {
		return D { value.substr(start, count) };
	}

	std::string::const_iterator begin() const {
		return value.begin();
	}

	std::string::const_iterator end() const {
		return value.end();
	}
};

inline std::string toString(const C & c) {
	return toString(c.value);
}

inline std::string toString(const D & d) {
	return toString(d.value);
}

inline bool operator == (const C & lhs, const C & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator == (const C & lhs, const D & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator == (const D & lhs, const C & rhs) {
	return lhs.value == rhs.value;
}

inline bool operator == (const D & lhs, const D & rhs) {
	return lhs.value == rhs.value;
}

namespace Balau::Testing {

inline bool contains(const C & lhs, const C & rhs) {
	return Util::Strings::contains(lhs.value, rhs.value);
}

inline bool contains(const C & lhs, const D & rhs) {
	return Util::Strings::contains(lhs.value, rhs.value);
}

inline bool contains(const D & lhs, const C & rhs) {
	return Util::Strings::contains(lhs.value, rhs.value);
}

inline bool contains(const D & lhs, const D & rhs) {
	return Util::Strings::contains(lhs.value, rhs.value);
}

} // namespace Balau::Testing

inline void foo() {
	ThrowBalauException(Balau::Exception::SizeException, "testing");
}

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST_DATA
