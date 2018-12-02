// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ObjectTrieTest.hpp"

#include "Balau/Container/ObjectTrie.hpp"

namespace Balau {

using Testing::assertThat;
using Testing::is;
using Testing::isNotNull;

namespace Container {

using UIntTrie = ObjectTrie<unsigned int>;
using UIntTrieNode = ObjectTrieNode<unsigned int>;

void populateUIntTrie(UIntTrie & trie, unsigned int dataCounter = 0) {
	UIntTrieNode & root1 = trie.root();

	root1.value = dataCounter++;

	root1.add(dataCounter++);
	root1.add(dataCounter++);
	root1.add(dataCounter++);

	UIntTrieNode & root2 = trie.root();

	assertThat(&root1, is(&root2));
	assertThat(root1.count(), is(3U));

	UIntTrieNode & child0 = root2[0];

	assertThat(child0.count(), is(0U));

	child0.add(dataCounter++);

	assertThat(child0.count(), is(1U));

	child0.add(dataCounter++);

	assertThat(child0.count(), is(2U));

	child0.add(dataCounter++);

	assertThat(child0.count(), is(3U));

	UIntTrieNode & child1 = root2[1];

	child1.add(dataCounter++);
	child1.add(dataCounter++);
	child1.add(dataCounter++);

	UIntTrieNode & child11 = child1[1];

	child11.add(dataCounter++);
	child11.add(dataCounter++);
	child11.add(dataCounter);
}

void updateUIntTrie(UIntTrie & trie, unsigned int dataCounter = 0) {
	UIntTrieNode & root = trie.root();
	UIntTrieNode & child0 = root[0];
	UIntTrieNode & child1 = root[1];
	UIntTrieNode & child11 = child1[1];

	root.value = dataCounter++;

	root[0].value = dataCounter++;
	root[1].value = dataCounter++;
	root[2].value = dataCounter++;

	child0[0].value = dataCounter++;
	child0[1].value = dataCounter++;
	child0[2].value = dataCounter++;

	child1[0].value = dataCounter++;
	child1[1].value = dataCounter++;
	child1[2].value = dataCounter++;

	child11[0].value = dataCounter++;
	child11[1].value = dataCounter++;
	child11[2].value = dataCounter;
}

void assertUIntTrie(const UIntTrie & trie, unsigned int dataCounter = 0) {
	const UIntTrieNode & root = trie.root();
	const UIntTrieNode & child0 = root[0];
	const UIntTrieNode & child1 = root[1];
	const UIntTrieNode & child11 = child1[1];

	assertThat("R", root.value, is(dataCounter++));

	assertThat("C0", root[0].value, is(dataCounter++));
	assertThat("C1", root[1].value, is(dataCounter++));
	assertThat("C2", root[2].value, is(dataCounter++));

	assertThat("C00", child0[0].value, is(dataCounter++));
	assertThat("C01", child0[1].value, is(dataCounter++));
	assertThat("C02", child0[2].value, is(dataCounter++));

	assertThat("C10", child1[0].value, is(dataCounter++));
	assertThat("C11", child1[1].value, is(dataCounter++));
	assertThat("C12", child1[2].value, is(dataCounter++));

	assertThat("C110", child11[0].value, is(dataCounter++));
	assertThat("C111", child11[1].value, is(dataCounter++));
	assertThat("C112", child11[2].value, is(dataCounter));
}

void ObjectTrieTest::uIntTrieBuild() {
	UIntTrie trie(0);
	populateUIntTrie(trie);
	assertUIntTrie(trie);
}

void ObjectTrieTest::uIntTrieCopy() {
	UIntTrie trie(0);
	populateUIntTrie(trie);
	assertUIntTrie(trie);
	const UIntTrie & constTree = trie;
	UIntTrie trieCopy = constTree; // NOLINT
	assertUIntTrie(constTree);
	assertUIntTrie(trieCopy);
	updateUIntTrie(trieCopy, 42);

	assertUIntTrie(trie);
	assertUIntTrie(constTree);
	assertUIntTrie(trieCopy, 42);
}

const unsigned int UINT_DEPTH_ITERATION_DATA[] = {
	   0 // R
	,  1 // R -> C0
	,  4 // R -> C0 -> C00
	,  5 // R -> C0 -> C01
	,  6 // R -> C0 -> C02
	,  2 // R -> C1
	,  7 // R -> C1 -> C10
	,  8 // R -> C1 -> C11
	, 10 // R -> C1 -> C11 -> C110
	, 11 // R -> C1 -> C11 -> C111
	, 12 // R -> C1 -> C11 -> C112
	,  9 // R -> C1 -> C12
	,  3 // R -> C2
};

const unsigned int UINT_BREADTH_ITERATION_DATA[] = {
	   0 // R
	,  1 // R -> C0
	,  2 // R -> C1
	,  3 // R -> C2
	,  4 // R -> C0 -> C00
	,  5 // R -> C0 -> C01
	,  6 // R -> C0 -> C02
	,  7 // R -> C1 -> C10
	,  8 // R -> C1 -> C11
	,  9 // R -> C1 -> C12
	, 10 // R -> C1 -> C11 -> C110
	, 11 // R -> C1 -> C11 -> C111
	, 12 // R -> C1 -> C11 -> C112
};

void ObjectTrieTest::uIntTreeDepthIterate() {
	UIntTrie trie(0);
	populateUIntTrie(trie);
	UIntTrie::DepthIterator iter = trie.begin();
	UIntTrie::DepthIterator end = trie.end();
	size_t iterationCount = 0;

	while (iter != end) {
		assertThat(iter->value, is(UINT_DEPTH_ITERATION_DATA[iterationCount]));
		++iterationCount;
		++iter;
	}
}

void ObjectTrieTest::uIntTreeDepthIterateForLoop() {
	UIntTrie trie(0);
	populateUIntTrie(trie);
	size_t iterationCount = 0;

	for (UIntTrieNode & node : trie) {
		assertThat(node.value, is(UINT_DEPTH_ITERATION_DATA[iterationCount]));
		++iterationCount;
	}
}

void ObjectTrieTest::uIntTreeBreadthIterate() {
	UIntTrie trie(0);
	populateUIntTrie(trie);
	UIntTrie::BreadthIterator iter = trie.breadthBegin();
	UIntTrie::BreadthIterator end = trie.breadthEnd();
	size_t iterationCount = 0;

	while (iter != end) {
		assertThat(iter->value, is(UINT_BREADTH_ITERATION_DATA[iterationCount]));
		++iterationCount;
		++iter;
	}
}

void ObjectTrieTest::fluentBuild() {
	using Value = ObjectTrieTestValue;
	using Trie = ObjectTrie<Value>;
	using Node = ObjectTrieNode<Value>;

	Trie trie;

	trie.add({ 'a', 1 }
		, Node::child({ 'a', 11 })
		, Node::child({ 'b', 12 })
		, Node::child({ 'c', 13 })
		, Node::child({ 'd', 14 })
	).add({ 'b', 2 }
		, Node::child({ 'a', 21 })
		, Node::child({ 'b', 22 }
			, Node::child({ 'a', 221 })
			, Node::child({ 'b', 222 })
			, Node::child({ 'c', 223 })
		)
	).add({ 'c', 3 }
		, Node::child({ 'a', 31 })
		, Node::child({ 'b', 32 })
		, Node::child({ 'c', 33 })
		, Node::child({ 'd', 34 })
		, Node::child({ 'e', 35 })
	);

	assertThat(trie.root().count(), is(3U));

	assertThat(trie.root().value, is(Value { '\0', 0 }));

	auto & c0 = trie[0];
	auto & c1 = trie[1];
	auto & c2 = trie[2];

	assertThat(c0.count(), is(4U));
	assertThat(c1.count(), is(2U));
	assertThat(c2.count(), is(5U));

	assertThat(c0.value, is(Value { 'a', 1 }));
	assertThat(c1.value, is(Value { 'b', 2 }));
	assertThat(c2.value, is(Value { 'c', 3 }));

	assertThat(c0[0].value, is(Value { 'a', 11 }));
	assertThat(c0[1].value, is(Value { 'b', 12 }));
	assertThat(c0[2].value, is(Value { 'c', 13 }));
	assertThat(c0[3].value, is(Value { 'd', 14 }));

	assertThat(c1[0].value, is(Value { 'a', 21 }));
	assertThat(c1[1].value, is(Value { 'b', 22 }));

	assertThat(c2[0].value, is(Value { 'a', 31 }));
	assertThat(c2[1].value, is(Value { 'b', 32 }));
	assertThat(c2[2].value, is(Value { 'c', 33 }));
	assertThat(c2[3].value, is(Value { 'd', 34 }));
	assertThat(c2[4].value, is(Value { 'e', 35 }));

	assertThat(c1[1].count(), is(3U));

	assertThat(c1[1][0].value, is(Value { 'a', 221 }));
	assertThat(c1[1][1].value, is(Value { 'b', 222 }));
	assertThat(c1[1][2].value, is(Value { 'c', 223 }));
}

} // namespace Container

} // namespace Balau
