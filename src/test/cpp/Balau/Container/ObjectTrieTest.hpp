// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__BALAU_TRIE_TEST
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__BALAU_TRIE_TEST

#include <Balau/Type/ToString.hpp>

struct ObjectTrieTestValue {
	char key;
	int value;

	bool operator == (const ObjectTrieTestValue & rhs) const {
		return key == rhs.key;
	}
};

inline std::string toString(const ObjectTrieTestValue & value) {
	return toString("{ ", value.key, ", ", value.value, " }");
}

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Container {

struct ObjectTrieTest : public Testing::TestGroup<ObjectTrieTest> {
	explicit ObjectTrieTest(Testing::TestRunner & runner) : TestGroup(runner) {
		registerTest(&ObjectTrieTest::uIntTrieBuild,               "uIntTrieBuild");
		registerTest(&ObjectTrieTest::uIntTrieCopy,                "uIntTrieCopy");
		registerTest(&ObjectTrieTest::uIntTreeDepthIterate,        "uIntTreeDepthIterate");
		registerTest(&ObjectTrieTest::uIntTreeDepthIterateForLoop, "uIntTreeDepthIterateForLoop");
		registerTest(&ObjectTrieTest::uIntTreeBreadthIterate,      "uIntTreeBreadthIterate");
		registerTest(&ObjectTrieTest::fluentBuild,                 "fluentBuild");
	}

	void uIntTrieBuild();
	void uIntTrieCopy();
	void uIntTreeDepthIterate();
	void uIntTreeDepthIterateForLoop();
	void uIntTreeBreadthIterate();
	void fluentBuild();
};

} // namespace Container

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__BALAU_TRIE_TEST
