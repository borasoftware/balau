// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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

namespace Balau {

namespace Testing {

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

} // namespace Testing

} // namespace Balau

inline void foo() {
	ThrowBalauException(Balau::Exception::SizeException, "testing");
}

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__ASSERTIONS_TEST_DATA
