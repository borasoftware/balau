// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__DEPENDENCY_GRAPH_TEST
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__DEPENDENCY_GRAPH_TEST

#include <Balau/Type/ToString.hpp>

namespace Balau {

namespace Container {

struct Dep {
	std::string_view name;
};

inline bool operator == (const Dep & lhs, const Dep & rhs) {
	return lhs.name == rhs.name;
}

inline std::string toString(const Dep & obj) {
	return std::string(obj.name);
}

} // namespace Container

} // namespace Balau

namespace std {

template <> struct hash<Balau::Container::Dep> {
	size_t operator () (const Balau::Container::Dep & dep) const noexcept {
		return hash<std::string_view>()(dep.name);
	}
};

} // namespace std

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Container {

struct DependencyGraphTest : public Testing::TestGroup<DependencyGraphTest> {
	DependencyGraphTest() {
		registerTest(&DependencyGraphTest::test_prep, "test_prep");
		registerTest(&DependencyGraphTest::test, "test");
	}

	void test_prep();
	void test();
};

} // namespace Container

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__DEPENDENCY_GRAPH_TEST
