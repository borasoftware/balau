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

///
/// @file ObjectTrie.hpp
///
/// An object based trie used for parent-child hierarchies.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__BALAU_TRIE
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__BALAU_TRIE

#include <Balau/Util/Vectors.hpp>

// Avoid false positives.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Container {

template <typename T> class ObjectTrie;

///
/// The node type contained in the object trie.
///
/// The data type stored in nodes must have a default constructor.
///
/// Nodes are exposed by the trie via the root() method, and from within the
/// node type itself via the getChild() method.
///
/// Node manipulation is not exposed outside of the trie implementation.
///
template <typename T> class ObjectTrieNode {
	// Used in the fluent API.
	private: class ObjectAdder {
		friend class ObjectTrieNode<T>;
		T value;
		std::vector<ObjectAdder> childAdders;

		ObjectAdder(T && value_, std::vector<ObjectAdder> && childAdders_)
			: value(std::move(value_))
			, childAdders(std::move(childAdders_)) {}

		public: ObjectAdder(ObjectAdder && ) noexcept = default;
	};

	///
	/// Add a child of the child being added.
	///
	public: static ObjectAdder child(T && value) {
		return ObjectAdder(std::move(value), {});
	}

	///
	/// Add a child of the child being added, plus descendants of the child.
	///
	public: template <typename ... ObjectAdderT>
	static ObjectAdder child(T && value, ObjectAdderT && ... childAdder) {
		return ObjectAdder(std::move(value), Util::Vectors::pushBack(std::move(childAdder) ... ));
	}

	///
	/// Append a list of children to the node.
	///
	/// @return the current node, allowing chaining calls to be made
	///
	public: template <typename ... ObjectAdderT>
	ObjectTrieNode<T> & add(T && value, ObjectAdderT && ... childAdder) {
		auto & child = addAndReturnChild(std::move(value));
		child.add(Util::Vectors::pushBack(std::move(childAdder) ... ));
		return *this;
	}

	///
	/// Append a vector of children to the node.
	///
	/// @return the current node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & add(std::vector<ObjectAdder> && childAdders) {
		for (auto & childAdder : childAdders) {
			children.emplace_back(ObjectTrieNode<T>(std::move(childAdder.value)));
			children.back().add(std::move(childAdder.childAdders));
		}

		return *this;
	}

	///
	/// The publicly accessible data contained in the node.
	///
	public: T value;

	///
	/// Append a child containing the supplied data.
	///
	/// The data is moved into the new value.
	///
	/// @param value the value that will be added
	/// @return the current node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & add(T && value) {
		addAndReturnChild(std::move(value));
		return *this;
	}

	///
	/// Append a child containing the supplied data.
	///
	/// The data is copied into the new value.
	///
	/// @param value the value that will be added
	/// @return the current node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & add(const T & value) {
		addAndReturnChild(value);
		return *this;
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new or existing value.
	///
	/// @param value the value that will be added or will replace
	/// @return the current node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & addOrReplace(T && value) {
		ObjectTrieNode<T> * match = find(value);

		if (match) {
			match->value = std::move(value);
		} else {
			addAndReturnChild(std::move(value));
		}

		return *this;
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new or existing value.
	///
	/// @param value the value that will be added or will replace
	/// @return the current node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & addOrReplace(const T & value) {
		ObjectTrieNode<T> * match = find(value);

		if (match) {
			match->value = value;
		} else {
			addAndReturnChild(value);
		}

		return *this;
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @return the current node, allowing chaining calls to be made
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplace(T && value, CompareT compare) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			match->value = std::move(value);
		} else {
			addAndReturnChild(std::move(value));
		}

		return *this;
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @return the current node, allowing chaining calls to be made
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplace(const T & value, CompareT compare) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			match->value = value;
		} else {
			addAndReturnChild(value);
		}

		return *this;
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The supplied replacer is used to perform assignment of the values when a match is found.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @tparam ReplaceT the type of the replacer function
	/// @param replace the replacer function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @return the current node, allowing chaining calls to be made
	///
	public: template <typename CompareT, typename ReplaceT, ReplaceT replace>
	ObjectTrieNode<T> & addOrReplace(T && value, CompareT compare) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			replace(match->value, std::move(value));
		} else {
			addAndReturnChild(std::move(value));
		}

		return *this;
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The supplied replacer is used to perform assignment of the values when a match is found.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @tparam ReplaceT the type of the replacer function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @param replace the replacer function
	/// @return the current node, allowing chaining calls to be made
	///
	public: template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplace(const T & value, CompareT compare, ReplaceT replace) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			replace(match->value, value);
		} else {
			addAndReturnChild(value);
		}

		return *this;
	}

	///
	/// Append a child containing a copy of the supplied data.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The data is copied into the new value.
	///
	/// @param value the value that will be added
	/// @return the newly created node
	///
	public: ObjectTrieNode<T> & addAndReturnChild(const T & value) {
		children.emplace_back(ObjectTrieNode<T>(value));
		return children.back();
	}

	///
	/// Append a child containing the supplied data.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The data is moved into the new value.
	///
	/// @param value the value that will be added
	/// @return the newly created node
	///
	public: ObjectTrieNode<T> & addAndReturnChild(T && value) {
		children.emplace_back(ObjectTrieNode<T>(std::move(value)));
		return children.back();
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new or existing value.
	///
	/// @param value the value that will be added or will replace
	/// @return the newly created or modified node
	///
	public: ObjectTrieNode<T> & addOrReplaceAndReturnChild(const T & value) {
		ObjectTrieNode<T> * match = find(value);

		if (match) {
			match->value = value;
			return *match;
		} else {
			return addAndReturnChild(value);
		}
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new or existing value.
	///
	/// @param value the value that will be added or will replace
	/// @return the newly created or modified node
	///
	public: ObjectTrieNode<T> & addOrReplaceAndReturnChild(T && value) {
		ObjectTrieNode<T> * match = find(value);

		if (match) {
			match->value = std::move(value);
			return *match;
		} else {
			return addAndReturnChild(std::move(value));
		}
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @return the newly created or modified node
	///
	public:  template <typename CompareT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(const T & value, CompareT compare) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			match->value = value;
			return *match;
		} else {
			return addAndReturnChild(value);
		}
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @return the newly created or modified node
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(T && value, CompareT compare) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			match->value = std::move(value);
			return *match;
		} else {
			return addAndReturnChild(std::move(value));
		}
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The supplied replacer is used to perform assignment of the values when a match is found.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @tparam ReplaceT the type of the replacer function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @param replace the replacer function
	/// @return the newly created or modified node
	///
	public:  template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(const T & value, CompareT compare, ReplaceT replace) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			replace(match->value, value);
			return *match;
		} else {
			return addAndReturnChild(value);
		}
	}

	///
	/// Append a child containing the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The supplied replacer is used to perform assignment of the values when a match is found.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new or existing value.
	///
	/// @tparam CompareT the type of the comparison function
	/// @tparam ReplaceT the type of the replacer function
	/// @param value the value that will be added or will replace
	/// @param compare the comparison function
	/// @param replace the replacer function
	/// @return the newly created or modified node
	///
	public: template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(T && value, CompareT compare, ReplaceT replace) {
		ObjectTrieNode<T> * match = find(value, compare);

		if (match) {
			replace(match->value, std::move(value));
			return *match;
		} else {
			return addAndReturnChild(std::move(value));
		}
	}

	///
	/// Locates the first child matching the supplied value or return nullptr if not found.
	///
	/// @param value the value to compare
	/// @return the matching child or nullptr if no match was found.
	///
	public: ObjectTrieNode<T> * find(const T & value) {
		for (size_t m = 0; m < children.size(); m++) {
			if (children[m].value == value) {
				return &children[m];
			}
		}

		return nullptr;
	}

	///
	/// Locates the first child matching the supplied value or return nullptr if not found.
	///
	/// @param value the value to compare
	/// @return the matching child or nullptr if no match was found.
	///
	public: const ObjectTrieNode<T> * find(const T & value) const {
		for (size_t m = 0; m < children.size(); m++) {
			if (children[m].value == value) {
				return &children[m];
			}
		}

		return nullptr;
	}

	///
	/// Locates the first child matching the supplied value or return nullptr if not found.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// @tparam CompareT the type of the comparison function
	/// @param value the value to compare
	/// @param compare the comparison function between type U and type T
	/// @return the matching child or nullptr if no match was found.
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> * find(const T & value, CompareT compare) {
		for (size_t m = 0; m < children.size(); m++) {
			if (compare(children[m].value, value)) {
				return &children[m];
			}
		}

		return nullptr;
	}

	///
	/// Locates the first child matching the supplied value or return nullptr if not found.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// @tparam CompareT the type of the comparison function
	/// @param value the value to compare
	/// @param compare the comparison function between type U and type T
	/// @return the matching child or nullptr if no match was found.
	///
	public: template <typename CompareT>
	const ObjectTrieNode<T> * find(const T & value, CompareT compare) const {
		for (size_t m = 0; m < children.size(); m++) {
			if (compare(children[m].value, value)) {
				return &children[m];
			}
		}

		return nullptr;
	}

	///
	/// Locates the first child matching the supplied value or return nullptr if not found.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// @tparam U the type of the value to compare
	/// @tparam CompareT the type of the comparison function
	/// @param value the value to compare
	/// @param compare the comparison function between type U and type T
	/// @return the matching child or nullptr if no match was found.
	///
	public: template <typename U, typename CompareT>
	ObjectTrieNode<T> * find(const U & value, CompareT compare) {
		for (size_t m = 0; m < children.size(); m++) {
			if (compare(children[m].value, value)) {
				return &children[m];
			}
		}

		return nullptr;
	}

	///
	/// Locates the first child matching the supplied value or return nullptr if not found.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// @tparam U the type of the value to compare
	/// @tparam CompareT the type of the comparison function
	/// @param value the value to compare
	/// @param compare the comparison function between type U and type T
	/// @return the matching child or nullptr if no match was found.
	///
	public: template <typename U, typename CompareT>
	const ObjectTrieNode<T> * find(const U & value, CompareT compare) const {
		for (size_t m = 0; m < children.size(); m++) {
			if (compare(children[m].value, value)) {
				return &children[m];
			}
		}

		return nullptr;
	}

	///
	/// Locates the first child matching the supplied value or creates a one.
	///
	/// The supplied value will be copied if no match was found and a new node is created.
	///
	/// @param value the value to compare and copy if a new node is created
	/// @return the matching child or the newly created one.
	///
	public: ObjectTrieNode<T> & findOrAddChild(const T & value) {
		for (size_t m = 0; m < children.size(); m++) {
			if (children[m].value == value) {
				return children[m];
			}
		}

		children.emplace_back(ObjectTrieNode<T>(value));
		return children.back();
	}

	///
	/// Locates the first child matching the supplied value or creates a one.
	///
	/// The supplied value will be moved if no match was found and a new node is created.
	///
	/// @param value the value to compare and move if a new node is created
	/// @return the matching child or the newly created one.
	///
	public: ObjectTrieNode<T> & findOrAddChild(T && value) {
		for (size_t m = 0; m < children.size(); m++) {
			if (children[m].value == value) {
				return children[m];
			}
		}

		children.emplace_back(ObjectTrieNode<T>(std::move(value)));
		return children.back();
	}

	///
	/// Locates the first child matching the supplied value or creates one.
	///
	/// A new value will be created from the supplied value if no match was
	/// found and a new node is created.
	///
	/// @tparam U the type of the value to compare
	/// @tparam CompareT the type of the comparison function
	/// @param value the value to compare
	/// @param compare the comparison function between type U and type T
	/// @return the matching child or the newly created one.
	///
	public: template <typename U, typename CompareT>
	ObjectTrieNode<T> & findOrAddChild(const U & value, CompareT compare) {
		for (size_t m = 0; m < children.size(); m++) {
			if (compare(children[m].value, value)) {
				return children[m];
			}
		}

		children.emplace_back(ObjectTrieNode<T>(T(value)));
		return children.back();
	}

	///
	/// Locates the first child matching the supplied value or creates one.
	///
	/// A new value will be created from the supplied value via the create
	/// function if no match was found and a new node is created.
	///
	/// @tparam U the type of the value to compare
	/// @tparam CompareT the type of the comparison function
	/// @tparam Create the function used to create a new value
	/// @param value the value to compare
	/// @param compare the comparison function between type U and type T
	/// @return the matching child or the newly created one.
	///
	public: template <typename U, typename CompareT, typename Create>
	ObjectTrieNode<T> & findOrAddChild(const U & value, CompareT compare, Create create) {
		for (size_t m = 0; m < children.size(); m++) {
			if (compare(children[m].value, value)) {
				return children[m];
			}
		}

		children.emplace_back(ObjectTrieNode<T>(create(value)));
		return children.back();
	}

	///
	/// Get a reference to the specified child.
	///
	public: ObjectTrieNode<T> & operator [] (size_t index) {
		return children[index];
	}

	///
	/// Get a const reference to the specified child.
	///
	public: const ObjectTrieNode<T> & operator [] (size_t index) const {
		return children[index];
	}

	///
	/// Get the number of children in the node.
	///
	public: size_t count() const {
		return children.size();
	}

	///
	/// Returns true if the trie node is empty.
	///
	public: bool empty() const {
		return children.empty();
	}

	///
	/// Get a depth iterator positioned at the beginning of the trie.
	///
	public: ObjectTrieNode<T> * begin() {
		return &children[0];
	}

	///
	/// Get a const depth iterator positioned at the beginning of the trie.
	///
	public: const ObjectTrieNode<T> * begin() const {
		return &children[0];
	}

	///
	/// Get an iterator positioned at the end of the vector of children.
	///
	public: ObjectTrieNode<T> * end() {
		return &children.back();
	}

	///
	/// Get a const iterator positioned at the end of the vector of children.
	///
	public: const ObjectTrieNode<T> * end() const {
		return &children.back();
	}

	////////////////////////// Private implementation /////////////////////////

	// Used to support the child array allocation.
	private: explicit ObjectTrieNode()
		: value() {}

	// Required public for std::vector
	public: ObjectTrieNode(ObjectTrieNode<T> && rhs) noexcept
		: value(std::move(rhs.value))
		, children(std::move(rhs.children)) {}

	// Required public for std::vector
	public: ObjectTrieNode(const ObjectTrieNode<T> & copy)
		: value(copy.value)
		, children(copy.children) {}

	private: explicit ObjectTrieNode(const T & value_)
		: value(value_) {}

	private: explicit ObjectTrieNode(T && value_)
		: value(std::move(value_)) {}

	private: ObjectTrieNode<T> & operator = (const ObjectTrieNode<T> & copy) {
		value = copy.value;
		children = copy.children;
		return *this;
	}

	// Required for std::move.
	public: ObjectTrieNode<T> & operator = (ObjectTrieNode<T> && rhs) noexcept {
		value = std::move(rhs.value);
		children = std::move(rhs.children);
		return *this;
	}

	// Used in the public cascade copy method.
	private: void cascadeChildren(const ObjectTrieNode<T> & toCascade) {
		for (size_t m = 0; m < toCascade.children.size(); m++) {
			const ObjectTrieNode<T> & childToCascade = toCascade.children[m];
			ObjectTrieNode<T> * selfChild = find(childToCascade.value);

			if (selfChild) {
				selfChild->value = childToCascade.value;
				selfChild->cascadeChildren(childToCascade);
			} else {
				addAndReturnChild(childToCascade.value).cascadeChildren(childToCascade);
			}
		}
	}

	// Used in the public cascade move method.
	private: template <typename U> void cascadeChildren(ObjectTrieNode<T> & toCascade, U moveFunction) {
		for (size_t m = 0; m < toCascade.children.size(); m++) {
			ObjectTrieNode<T> & childToCascade = toCascade.children[m];
			ObjectTrieNode<T> * selfChild = find(childToCascade.value);

			if (selfChild) {
				selfChild->value = moveFunction(childToCascade.value);
				selfChild->cascadeChildren(childToCascade);
			} else {
				addAndReturnChild(moveFunction(childToCascade.value)).cascadeChildren(childToCascade);
			}
		}
	}

	template <typename U> friend class ObjectTrie;

	private: std::vector<ObjectTrieNode<T>> children;
};

///////////////////////////////////////////////////////////////////////////////

///
/// An object based trie used for parent-child hierarchies.
///
/// A node's children are ordered in the order of appending.
///
/// The trie has depth first and breadth first iterators.
///
/// The nodes of an object trie are single linked. If the ancestors of a node are
/// required during iteration, the depth first iterator provides ancestor methods
/// that access the iterator's internal node stack.
///
/// This data structure is useful when the parent-child relationships between
/// nodes methods of instances are semantically important and must not be changed
/// via tree balancing. If this is not the case, a B-tree or red–black tree would
/// most likely be more appropriate.
///
/// The trie implementation is not optimised or compressed, thus use cases
/// requiring the most optimised trie representation would most likely be better
/// using a non-object based trie implementation.
///
/// An object of type T is contained in each trie node. Trie nodes also contain a
/// heap based vector of child nodes.
///
/// When using the search algorithms of the object trie, the standard operator ==
/// function/method is used for the object type T. Alternative methods are also
/// provided that accept a custom compare function/lambda.
///
/// The object type T should normally provide a key type field that is compared
/// within the == operator, and one or more body fields that are not accessed by
/// the object trie logic.
///
/// @todo Add cascade methods with custom comparators.
///
template <typename T> class ObjectTrie {
	///
	/// A depth first iterator for the trie.
	///
	public: class DepthIterator {
		private: class IteratorNode {
			public: ObjectTrieNode<T> * node;
			// The index is offset by +1 in order to assign 0 to the current node.
			public: size_t index;

			public: IteratorNode(ObjectTrieNode<T> * node_, size_t index_) : node(node_), index(index_) {}
		};

		///
		/// Make a copy of the supplied iterator.
		///
		public: DepthIterator(const DepthIterator & copy)
			: nodeStack(copy.nodeStack) {}

		///
		/// Make a copy of the supplied iterator.
		///
		public: DepthIterator & operator = (const DepthIterator & copy) {
			nodeStack = copy.nodeStack;
			return *this;
		}

		///
		/// Increment the iterator.
		///
		public: DepthIterator & operator ++ () {
			if (!nodeStack.size()) {
				return endMarker;
			}

			while (nodeStack.size() > 0) {
				if (nodeStack.back().index < nodeStack.back().node->children.size()) {
					++nodeStack.back().index;
					nodeStack.push_back({ &nodeStack.back().node->children[nodeStack.back().index - 1], 0 });
					return *this;
				} else {
					nodeStack.pop_back();
 				}
			}

			return endMarker;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: ObjectTrieNode<T> & operator * () {
			return *nodeStack.back().node;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> & operator * () const {
			return *nodeStack.back().node;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: ObjectTrieNode<T> * operator -> () {
			return nodeStack.back().node;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> * operator -> () const {
			return nodeStack.back().node;
		}

		///
		/// Get a pointer to the nth ancestor of the current node.
		///
		/// Nullptr is returned if such an ancestor does not exist.
		///
		/// @param level the ancestor distance
		/// @return the ancestor or nullptr if such an ancestor does not exist
		///
		public: ObjectTrieNode<T> * getAncestor(size_t level) {
			if (nodeStack.size() <= level) {
				return nullptr;
			}

			return nodeStack[nodeStack.size() - 1 - level].node;
		}

		///
		/// Get a const pointer to the nth ancestor of the current node.
		///
		/// Nullptr is returned if such an ancestor does not exist.
		///
		/// @param level the ancestor distance
		/// @return the ancestor or nullptr if such an ancestor does not exist
		///
		public: const ObjectTrieNode<T> * getAncestor(size_t level) const {
			if (nodeStack.size() <= level) {
				return nullptr;
			}

			return nodeStack[nodeStack.size() - 1 - level].node;
		}

		///
		/// Populate the supplied const pointer vector with the node's ancestors and the current node.
		///
		/// @return a const pointer vector to the node's ancestors and the current node
		///
		public: void getAncestors(std::vector<const ObjectTrieNode<T> *> & output) const {
			output.clear();

			for (const auto & iteratorNode : nodeStack) {
				output.emplace_back(iteratorNode.node);
			}
		}

		///
		/// Compare the current iterator and the supplied iterator for equality.
		///
		public: bool operator == (const DepthIterator & rhs) const {
			return nodeStack.size() == rhs.nodeStack.size()
				&& (!nodeStack.size() || nodeStack.back().node == rhs.nodeStack.back().node);
		}

		///
		/// Compare the current iterator and the supplied iterator for inequality.
		///
		public: bool operator != (const DepthIterator & rhs) const {
			return ! operator == (rhs);
		}

		friend class ObjectTrie<T>;

		private: DepthIterator() = default;

		private: explicit DepthIterator(ObjectTrie<T> * parent)
			: nodeStack({ IteratorNode(&parent->trieRoot, 0) }) {}

		private: static DepthIterator endMarker;
		private: std::vector<IteratorNode> nodeStack;
	};

	///
	/// A depth first const iterator for the trie.
	///
	public: class ConstDepthIterator {
		private: class IteratorNode {
			public: const ObjectTrieNode<T> * node;
			// The index is offset by +1 in order to assign 0 to the current node.
			public: size_t index;

			public: IteratorNode(const ObjectTrieNode<T> * node_, size_t index_) : node(node_), index(index_) {}
		};

		///
		/// Make a copy of the supplied iterator.
		///
		public: ConstDepthIterator(const ConstDepthIterator & copy)
			: nodeStack(copy.nodeStack) {}

		///
		/// Make a copy of the supplied iterator.
		///
		public: ConstDepthIterator & operator = (const ConstDepthIterator & copy) {
			nodeStack = copy.nodeStack;
			return *this;
		}

		///
		/// Increment the iterator.
		///
		public: ConstDepthIterator & operator ++ () {
			if (!nodeStack.size()) {
				return endMarker;
			}

			while (nodeStack.size() > 0) {
				if (nodeStack.back().index < nodeStack.back().node->children.size()) {
					++nodeStack.back().index;
					nodeStack.push_back({ &nodeStack.back().node->children[nodeStack.back().index - 1], 0 });
					return *this;
				} else {
					nodeStack.pop_back();
 				}
			}

			return endMarker;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> & operator * () const {
			return *nodeStack.back().node;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> * operator -> () const {
			return nodeStack.back().node;
		}

		///
		/// Get a const pointer to the nth ancestor of the current node.
		///
		/// Nullptr is returned if such an ancestor does not exist.
		///
		/// @param level the ancestor distance
		/// @return the ancestor or nullptr if such an ancestor does not exist
		///
		public: const ObjectTrieNode<T> * getAncestor(size_t level) const {
			if (nodeStack.size() <= level) {
				return nullptr;
			}

			return nodeStack[nodeStack.size() - 1 - level].node;
		}

		///
		/// Populate the supplied const pointer vector with the node's ancestors and the current node.
		///
		/// @return a const pointer vector to the node's ancestors and the current node
		///
		public: void getAncestors(std::vector<ObjectTrieNode<T> * const> & output) const {
			output.clear();

			for (const auto & iteratorNode : nodeStack) {
				output.emplace_back(iteratorNode.node);
			}
		}

		///
		/// Compare the current iterator and the supplied iterator for equality.
		///
		public: bool operator == (const ConstDepthIterator & rhs) const {
			return nodeStack.size() == rhs.nodeStack.size()
				&& (!nodeStack.size() || nodeStack.back().node == rhs.nodeStack.back().node);
		}

		///
		/// Compare the current iterator and the supplied iterator for inequality.
		///
		public: bool operator != (const ConstDepthIterator & rhs) const {
			return ! operator == (rhs);
		}

		friend class ObjectTrie<T>;

		private: ConstDepthIterator() = default;

		private: explicit ConstDepthIterator(const ObjectTrie<T> * parent)
			: nodeStack({ IteratorNode(&parent->trieRoot, 0) }) {}

		private: static ConstDepthIterator endMarker;
		private: std::vector<IteratorNode> nodeStack;
	};

	///
	/// Default (depth first) iterator.
	///
	/// For breadth first iteration, use breadth_iterator.
	///
	using iterator = DepthIterator;

	///
	/// Default (depth first) const iterator.
	///
	/// For breadth first const iteration, use const_breadth_iterator.
	///
	using const_iterator = ConstDepthIterator;

	///
	/// A breadth first iterator for the trie.
	///
	public: class BreadthIterator {
		///
		/// Make a copy of the supplied iterator.
		///
		public: BreadthIterator(const BreadthIterator & copy)
			: nodeQueue(copy.nodeQueue) {}

		///
		/// Make a copy of the supplied iterator.
		///
		public: BreadthIterator & operator = (const BreadthIterator & copy) {
			nodeQueue = copy.nodeQueue;
			return *this;
		}

		///
		/// Increment the iterator.
		///
		public: BreadthIterator & operator ++ () {
			if (!nodeQueue.size()) {
				return endMarker;
			}

			ObjectTrieNode<T> * previous = nodeQueue.front();
			nodeQueue.pop();

			for (size_t m = 0; m < previous->children.size(); m++) {
				nodeQueue.push(&previous->children[m]);
			}

			if (!nodeQueue.size()) {
				return endMarker;
			}

			return *this;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: ObjectTrieNode<T> & operator * () {
			return *nodeQueue.front();
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> & operator * () const {
			return *nodeQueue.front();
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: ObjectTrieNode<T> * operator -> () {
			return nodeQueue.front();
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> * operator -> () const {
			return nodeQueue.front();
		}

		///
		/// Compare the current iterator and the supplied iterator for equality.
		///
		public: bool operator == (const BreadthIterator & rhs) const {
			return nodeQueue.size() == rhs.nodeQueue.size()
				&& (!nodeQueue.size() || nodeQueue.front() == rhs.nodeQueue.front());
		}

		///
		/// Compare the current iterator and the supplied iterator for inequality.
		///
		public: bool operator != (const BreadthIterator & rhs) const {
			return ! operator == (rhs);
		}

		friend class ObjectTrie<T>;

		private: BreadthIterator() = default;

		private: explicit BreadthIterator(ObjectTrie<T> * parent)
			: nodeQueue({ &parent->trieRoot }) {}

		private: static BreadthIterator endMarker;
		private: std::queue<ObjectTrieNode<T> *> nodeQueue;
	};

	///
	/// A breadth first const iterator for the trie.
	///
	public: class ConstBreadthIterator {
		///
		/// Make a copy of the supplied iterator.
		///
		public: ConstBreadthIterator(const ConstBreadthIterator & copy)
			: nodeQueue(copy.nodeQueue) {}

		///
		/// Make a copy of the supplied iterator.
		///
		public: ConstBreadthIterator & operator = (const ConstBreadthIterator & copy) {
			nodeQueue = copy.nodeQueue;
			return *this;
		}

		///
		/// Increment the iterator.
		///
		public: ConstBreadthIterator & operator ++ () {
			if (!nodeQueue.size()) {
				return endMarker;
			}

			const ObjectTrieNode<T> * previous = nodeQueue.front();
			nodeQueue.pop();

			for (size_t m = 0; m < previous->children.size(); m++) {
				nodeQueue.push(&previous->children[m]);
			}

			if (!nodeQueue.size()) {
				return endMarker;
			}

			return *this;
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> & operator * () const {
			return *nodeQueue.front();
		}

		///
		/// Dereference the node pointed to by the iterator.
		///
		public: const ObjectTrieNode<T> * operator -> () const {
			return nodeQueue.front();
		}

		///
		/// Compare the current iterator and the supplied iterator for equality.
		///
		public: bool operator == (const ConstBreadthIterator & rhs) const {
			return nodeQueue.size() == rhs.nodeQueue.size()
				&& (!nodeQueue.size() || nodeQueue.front() == rhs.nodeQueue.front());
		}

		///
		/// Compare the current iterator and the supplied iterator for inequality.
		///
		public: bool operator != (const ConstBreadthIterator & rhs) const {
			return ! operator == (rhs);
		}

		friend class ObjectTrie<T>;

		private: ConstBreadthIterator() = default;

		private: explicit ConstBreadthIterator(const ObjectTrie<T> * parent)
			: nodeQueue({ &parent->trieRoot }) {}

		private: static ConstBreadthIterator endMarker;
		private: std::queue<const ObjectTrieNode<T> *> nodeQueue;
	};

	///
	/// Construct the trie with a root value equal to the default construction of T.
	///
	public: ObjectTrie() = default;

	///
	/// Construct the trie by making a copy of the supplied root value.
	///
	/// This constructor is used when the object type T does not have a default
	/// constructor and the supplied object will be copied.
	///
	public: explicit ObjectTrie(const T & trieRoot_) : trieRoot(trieRoot_) {}

	///
	/// Construct the trie by moving the supplied root value.
	///
	/// This constructor is used when the object type T does not have a default
	/// constructor and the supplied object will be moved.
	///
	public: explicit ObjectTrie(T && trieRoot_) : trieRoot(std::move(trieRoot_)) {}

	///
	/// Create a trie by copying an existing trie.
	///
	public: ObjectTrie(const ObjectTrie<T> & copy) : trieRoot(copy.trieRoot) {}

	///
	/// Move a trie.
	///
	public: ObjectTrie(ObjectTrie<T> && rhs) noexcept : trieRoot(std::move(rhs.trieRoot)) {}

	///
	/// Move a trie to an existing trie.
	///
	public: ObjectTrie<T> & operator = (ObjectTrie<T> && rhs) noexcept {
		trieRoot = std::move(rhs.trieRoot);
		return *this;
	}

	///
	/// Copy a trie to an existing trie.
	///
	public: ObjectTrie<T> & operator = (const ObjectTrie<T> & rhs) {
		trieRoot = rhs.trieRoot;
		return *this;
	}

	///
	/// Get the root node of the trie.
	///
	public: ObjectTrieNode<T> & root() {
		return trieRoot;
	}

	///
	/// Get the root node of the trie.
	///
	public: const ObjectTrieNode<T> & root() const {
		return trieRoot;
	}

	///
	/// Append a child containing the supplied data to the root node, plus children of the new child.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: template <typename ... ObjectAdderT>
	ObjectTrieNode<T> & add(T && value, ObjectAdderT && ... childNode) {
		auto & child = trieRoot.addAndReturnChild(std::move(value));
		child.add(Util::Vectors::pushBack(std::move(childNode) ... ));
		return trieRoot;
	}

	///
	/// Append a child containing a copy of the supplied data to the root node.
	/// The data is copied into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & add(const T & value) {
		return trieRoot.add(value);
	}

	///
	/// Append a child containing the supplied data to the root node.
	/// The data is moved into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & add(T && value) {
		return trieRoot.add(std::move(value));
	}

	///
	/// Append a child containing the supplied data to the root node if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & addOrReplace(const T & value) {
		return trieRoot.addOrReplace(value);
	}

	///
	/// Append a child containing the supplied data to the root node if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: ObjectTrieNode<T> & addOrReplace(T && value) {
		return trieRoot.addOrReplace(std::move(value));
	}

	///
	/// Append a child containing the supplied data to the root node if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplace(const T & value, CompareT compare) {
		return trieRoot.addOrReplace(value, compare);
	}

	///
	/// Append a child containing the supplied data to the root node if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplace(T && value, CompareT compare) {
		return trieRoot.addOrReplace(std::move(value), compare);
	}

	///
	/// Append a child containing the supplied data to the root node if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The supplied replacer is used to perform assignment of the values when a match is found.
	///
	/// The data is copied into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplace(const T & value, CompareT compare, ReplaceT replace) {
		return trieRoot.addOrReplace(value, compare, replace);
	}

	///
	/// Append a child containing the supplied data to the root node if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The supplied replacer is used to perform assignment of the values when a match is found.
	///
	/// The data is moved into the new value.
	///
	/// @return the root node, allowing chaining calls to be made
	///
	public: template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplace(T && value, CompareT compare, ReplaceT replace) {
		return trieRoot.addOrReplace(std::move(value), compare, replace);
	}

	///
	/// Append a root node child containing a copy of the supplied data.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The data is copied into the new value.
	///
	/// @return the newly created node
	///
	public: ObjectTrieNode<T> & addAndReturnChild(const T & value) {
		return trieRoot.addAndReturnChild(value);
	}

	///
	/// Append a root node child containing a the supplied data.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The data is moved into the new value.
	///
	/// @return the newly created node
	///
	public: ObjectTrieNode<T> & addAndReturnChild(T && value) {
		return trieRoot.addAndReturnChild(std::move(value));
	}

	///
	/// Append a root node child containing a copy of the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new value.
	///
	/// @return the newly created or replaced node
	///
	public: ObjectTrieNode<T> & addOrReplaceAndReturnChild(const T & value) {
		return trieRoot.addOrReplaceAndReturnChild(value);
	}

	///
	/// Append a root node child containing a copy of the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The operator == method is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new value.
	///
	/// @return the newly created or replaced node
	///
	public: ObjectTrieNode<T> & addOrReplaceAndReturnChild(T && value) {
		return trieRoot.addOrReplaceAndReturnChild(std::move(value));
	}

	///
	/// Append a root node child containing a copy of the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new value.
	///
	/// @return the newly created or replaced node
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(const T & value, CompareT compare) {
		return trieRoot.addOrReplaceAndReturnChild(value, compare);
	}

	///
	/// Append a root node child containing a copy of the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new value.
	///
	/// @return the newly created or replaced node
	///
	public: template <typename CompareT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(T && value, CompareT compare) {
		return trieRoot.addOrReplaceAndReturnChild(std::move(value), compare);
	}

	///
	/// Append a root node child containing a copy of the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is copied into the new value.
	///
	/// @return the newly created or replaced node
	///
	public: template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(const T & value, CompareT compare, ReplaceT replace) {
		return trieRoot.addOrReplaceAndReturnChild(value, compare, replace);
	}

	///
	/// Append a root node child containing a copy of the supplied data if a child equal to the supplied value is not present, otherwise replace it.
	///
	/// This is an alternative form of the add() method, that returns
	/// the newly created child instead of the current node.
	///
	/// The supplied comparator is used to compare values.
	///
	/// The operator = method is used to assign the supplied value to the
	/// existing one when a matching value is is found.
	///
	/// The data is moved into the new value.
	///
	/// @return the newly created or replaced node
	///
	public: template <typename CompareT, typename ReplaceT>
	ObjectTrieNode<T> & addOrReplaceAndReturnChild(T && value, CompareT compare, ReplaceT replace) {
		return trieRoot.addOrReplaceAndReturnChild(std::move(value), compare, replace);
	}

	///
	/// Get a reference to the specified child in the root node.
	///
	public: ObjectTrieNode<T> & operator [] (size_t index) {
		return trieRoot.children[index];
	}

	///
	/// Get a const reference to the specified child in the root node.
	///
	public: const ObjectTrieNode<T> & operator [] (size_t index) const {
		return trieRoot.children[index];
	}

	///
	/// Get the number of children in the root node.
	///
	public: size_t count() const {
		return trieRoot.children.size();
	}

	///
	/// Returns true if the trie is empty.
	///
	public: bool empty() const {
		return trieRoot.children.empty();
	}

	///
	/// Get a depth iterator positioned at the beginning of the trie.
	///
	public: DepthIterator begin() {
		return depthBegin();
	}

	///
	/// Get a const depth iterator positioned at the beginning of the trie.
	///
	public: ConstDepthIterator begin() const {
		return depthBegin();
	}

	///
	/// Get a depth iterator positioned at the end of the trie.
	///
	public: DepthIterator end() {
		return depthEnd();
	}

	///
	/// Get a const depth iterator positioned at the end of the trie.
	///
	public: ConstDepthIterator end() const {
		return depthEnd();
	}

	///
	/// Get a depth iterator positioned at the beginning of the trie.
	///
	public: DepthIterator depthBegin() {
		return DepthIterator(this);
	}

	///
	/// Get a const depth iterator positioned at the beginning of the trie.
	///
	public: ConstDepthIterator depthBegin() const {
		return ConstDepthIterator(this);
	}

	///
	/// Get a depth iterator positioned at the end of the trie.
	///
	public: DepthIterator depthEnd() {
		return DepthIterator::endMarker;
	}

	///
	/// Get a const depth iterator positioned at the end of the trie.
	///
	public: ConstDepthIterator depthEnd() const {
		return ConstDepthIterator::endMarker;
	}

	///
	/// Get a breadth iterator positioned at the beginning of the trie.
	///
	public: BreadthIterator breadthBegin() {
		return BreadthIterator(this);
	}

	///
	/// Get a const breadth iterator positioned at the beginning of the trie.
	///
	public: ConstBreadthIterator breadthBegin() const {
		return ConstBreadthIterator(this);
	}

	///
	/// Get a breadth iterator positioned at the end of the trie.
	///
	public: BreadthIterator breadthEnd() {
		return BreadthIterator::endMarker;
	}

	///
	/// Get a const breadth iterator positioned at the end of the trie.
	///
	public: ConstBreadthIterator breadthEnd() const {
		return ConstBreadthIterator::endMarker;
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the exact node if all
	/// values were found or nullptr if not all values were matched.
	///
	/// @param values a vector of values to search for
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	ObjectTrieNode<T> * find(const ContainerT<U, UC ...> & values, bool skipRoot = true) {
		return find(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; }, skipRoot);
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the exact node if all
	/// values were found or nullptr if not all values were matched.
	///
	/// @param values a vector of values to search for
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	const ObjectTrieNode<T> * find(const ContainerT<U, UC ...> & values, bool skipRoot = true) const {
		return find(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; }, skipRoot);
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The supplied search vector contains values that are used in the custom compare function.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the exact node if all
	/// values were found or nullptr if not all values were matched.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	ObjectTrieNode<T> * find(const ContainerT<U, UC ...> & values, CompareT compare, bool skipRoot = true) {
		if (values.empty()) {
			return nullptr;
		}

		auto iter = values.begin();

		if (!skipRoot) {
			if (!(compare(trieRoot.value, *iter))) {
				return nullptr;
			}

			++iter;
		}

		ObjectTrieNode<T> * node = &trieRoot;

		while (iter != values.end()) {
			node = node->template find<U, CompareT>(*iter, compare);

			if (node == nullptr) {
				return nullptr;
			}

			++iter;
		}

		return node;
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The supplied search vector contains values that are used in the custom compare function.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the exact node if all
	/// values were found or nullptr if not all values were matched.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	const ObjectTrieNode<T> * find(const ContainerT<U, UC ...> & values, CompareT compare, bool skipRoot = true) const {
		if (values.empty()) {
			return nullptr;
		}

		auto iter = values.begin();

		if (!skipRoot) {
			if (!(compare(trieRoot.value, *iter))) {
				return nullptr;
			}

			++iter;
		}

		const ObjectTrieNode<T> * node = &trieRoot;;

		while (iter != values.end()) {
			node = node->template find<U, CompareT>(*iter, compare);

			if (node == nullptr) {
				return nullptr;
			}

			++iter;
		}

		return node;
	}


	///
	/// Descend into the trie, locating matches of the supplied values, or creating them if they do not exist.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Any nodes that are not found are created.
	/// Returns the node that was found or the created node if the node or one or more of its
	/// parent were not present.
	///
	/// As the root node of the trie is fixed, the findOrAdd algorithm never checks the
	/// first value against the root node of the trie, i.e. skipRoot is always true.
	///
	/// If the supplied values container is empty, the root node is returned.
	///
	/// @param values a vector of values to search for
	/// @return a reference to the matching or created node
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	ObjectTrieNode<T> & findOrAdd(const ContainerT<U, UC ...> & values) {
		return findOrAdd(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; });
	}

	///
	/// Descend into the trie, locating matches of the supplied values, or creating them if they do not exist.
	///
	/// The supplied search vector contains values that are used in the custom compare function.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Any nodes that are not found are created.
	/// Returns the node that was found or the created node if the node or one or more of its
	/// parent were not present.
	///
	/// As the root node of the trie is fixed, the findOrAdd algorithm never checks the
	/// first value against the root node of the trie, i.e. skipRoot is always true.
	///
	/// If the supplied values container is empty, the root node is returned.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @return a reference to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	ObjectTrieNode<T> & findOrAdd(const ContainerT<U, UC ...> & values, CompareT compare) {
		ObjectTrieNode<T> * node = &trieRoot;

		if (values.empty()) {
			return *node;
		}

		auto iter = values.begin();

		while (iter != values.end()) {
			node = &node->template findOrAddChild<U, CompareT>(*iter, compare);
			++iter;
		}

		return *node;
	}

	///
	/// Descend into the trie, locating matches of the supplied values, or creating them if they do not exist.
	///
	/// The supplied search vector contains values that are used in the custom compare function.
	///
	/// The create function is used to create new nodes. The create function's argument
	/// is the current value as supplied in the values container.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Any nodes that are not found are created.
	/// Returns the node that was found or the created node if the node or one or more of its
	/// parent were not present.
	///
	/// As the root node of the trie is fixed, the findOrAdd algorithm never checks the
	/// first value against the root node of the trie, i.e. skipRoot is always true.
	///
	/// If the supplied values container is empty, the root node is returned.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @tparam Create the function used to create new nodes
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @return a reference to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT, typename Create>
	ObjectTrieNode<T> & findOrAdd(const ContainerT<U, UC ...> & values, CompareT compare, Create create) {
		ObjectTrieNode<T> * node = &trieRoot;

		if (values.empty()) {
			return *node;
		}

		auto iter = values.begin();

		while (iter != values.end()) {
			node = &node->template findOrAddChild<U, CompareT>(*iter, compare, create);
			++iter;
		}

		return *node;
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// or the exact node if all values were found.
	///
	/// Returns nullptr if no matching nodes were found.
	///
	/// @param values a vector of values to search for
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	ObjectTrieNode<T> * findNearest(const ContainerT<U, UC ...> & values, bool skipRoot = true) {
		return findNearest(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; }, skipRoot);
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// or the exact node if all values were found.
	///
	/// Returns nullptr if no matching nodes were found.
	///
	/// @param values a vector of values to search for
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	const ObjectTrieNode<T> * findNearest(const ContainerT<U, UC ...> & values, bool skipRoot = true) const {
		return findNearest(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; }, skipRoot);
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// or the exact node if all values were found.
	///
	/// Returns nullptr if no matching nodes were found.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	ObjectTrieNode<T> * findNearest(const ContainerT<U, UC ...> & values, CompareT compare, bool skipRoot = true) {
		if (values.empty()) {
			return nullptr;
		}

		auto iter = values.begin();
		ObjectTrieNode<T> * nodeToSearch = &trieRoot;
		ObjectTrieNode<T> * currentNode = nullptr;

		if (!skipRoot) {
			if (!compare(trieRoot.value, *iter)) {
				return nullptr;
			}

			++iter;
			currentNode = &trieRoot;
		}

		ObjectTrieNode<T> * nextNode;

		while (iter != values.end()) {
			nextNode = nodeToSearch->find(*iter, compare);

			if (nextNode == nullptr) {
				return currentNode;
			}

			++iter;
			currentNode = nextNode;
			nodeToSearch = nextNode;
		}

		return currentNode;
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// or the exact node if all values were found.
	///
	/// Returns nullptr if no matching nodes were found.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	const ObjectTrieNode<T> * findNearest(const ContainerT<U, UC ...> & values, CompareT compare, bool skipRoot = true) const {
		if (values.empty()) {
			return nullptr;
		}

		auto iter = values.begin();
		const ObjectTrieNode<T> * nodeToSearch = &trieRoot;
		const ObjectTrieNode<T> * currentNode = nullptr;

		if (!skipRoot) {
			if (!compare(trieRoot.value, *iter)) {
				return nullptr;
			}

			++iter;
			currentNode = &trieRoot;
		}

		const ObjectTrieNode<T> * nextNode;

		while (iter != values.end()) {
			nextNode = nodeToSearch->find(*iter, compare);

			if (nextNode == nullptr) {
				return currentNode;
			}

			++iter;
			currentNode = nextNode;
			nodeToSearch = nextNode;
		}

		return currentNode;
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// if the node is a leaf node.
	///
	/// Returns nullptr if no matching leaf node was found.
	///
	/// @param values a vector of values to search for
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	ObjectTrieNode<T> * findNearestLeaf(const ContainerT<U, UC ...> & values, bool skipRoot = true) {
		return findNearestLeaf(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; }, skipRoot);
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The default operator == compare function is used to determine equality.
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// if the node is a leaf node.
	///
	/// Returns nullptr if no matching leaf node was found.
	///
	/// @param values a vector of values to search for
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC>
	const ObjectTrieNode<T> * findNearestLeaf(const ContainerT<U, UC ...> & values, bool skipRoot = true) const {
		return findNearestLeaf(values, [] (auto & lhs, auto & rhs) { return lhs == rhs; }, skipRoot);
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// if the node is a leaf node.
	///
	/// Returns nullptr if no matching leaf node was found.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	ObjectTrieNode<T> * findNearestLeaf(const ContainerT<U, UC ...> & values, CompareT compare, bool skipRoot = true) {
		auto node = findNearest(values, compare, skipRoot);
		return node != nullptr && node->children.empty() ? node : nullptr;
	}

	///
	/// Descend into the trie, locating matches of the supplied values.
	///
	/// The supplied comparator is used to determine equality (true == equal).
	///
	/// Descends into the trie, locating at each node the first child that matches
	/// the next value in the supplied value chain. Returns the last matched node
	/// if the node is a leaf node.
	///
	/// Returns nullptr if no matching leaf node was found.
	///
	/// @tparam U the type of the values used in the compare function
	/// @tparam CompareT the compare function type
	/// @param values a vector of values to search for
	/// @param compare the compare function
	/// @param skipRoot does not check the first value against the root node of the trie
	/// @return a pointer to the matching node or nullptr if no node was found
	///
	public: template <typename U, template <typename ...> class ContainerT, typename ... UC, typename CompareT>
	const ObjectTrieNode<T> * findNearestLeaf(const ContainerT<U, UC ...> & values, CompareT compare, bool skipRoot = true) const {
		auto node = findNearest(values, compare, skipRoot);
		return node != nullptr && node->children.empty() ? node : nullptr;
	}

	///
	/// Cascades the supplied trie onto this trie, copying the values.
	///
	/// In order that anything is cascaded, the values of the root nodes must match via
	/// the value type's operator ==, otherwise the result is a nop. It is the responsibility
	/// of the end developer to use a value type that has a suitable operator ==.
	///
	/// @param toCascade the source trie
	///
	public: void cascade(const ObjectTrie<T> & toCascade) {
		cascade(toCascade, stdCopyFunction);
	}

	///
	/// Cascades the supplied trie onto this trie, moving the source values.
	///
	/// In order that anything is cascaded, the values of the root nodes must match via
	/// the value type's operator ==, otherwise the result is a nop. It is the responsibility
	/// of the end developer to use a value type that has a suitable operator ==.
	///
	/// By the end of this call, the source trie's values will be partially or fully emptied.
	///
	/// @param toCascade the source trie
	///
	public: void cascade(ObjectTrie<T> && toCascade) {
		cascade(std::move(toCascade), stdMoveFunction);
	}

	///
	/// Cascades the supplied trie onto this trie. The values are copied via the supplied function.
	///
	/// In order that anything is cascaded, the values of the root nodes must match via
	/// the value type's operator ==, otherwise the result is a nop. It is the responsibility
	/// of the end developer to use a value type that has a suitable operator ==.
	///
	/// @param toCascade the source trie
	/// @param moveFunction the function used to copy the values
	///
	public: template <typename U> void cascade(const ObjectTrie<T> & toCascade, U copyFunction) {
		if (!(trieRoot.value == toCascade.trieRoot.value)) {
			return; // NOP
		}

		trieRoot.value = copyFunction(toCascade.trieRoot.value);
		trieRoot.cascadeChildren(toCascade.trieRoot);
	}

	///
	/// Cascades the supplied trie onto this trie. The values are moved via the supplied function.
	/// In order that anything is cascaded, the values of the root nodes must match via
	/// the value type's operator ==, otherwise the result is a nop. It is the responsibility
	/// of the end developer to use a value type that has a suitable operator ==.
	///
	/// By the end of this call, the source trie's values will be partially or fully emptied.
	///
	/// @param toCascade the source trie
	/// @param moveFunction the function used to move the values
	///
	public: template <typename U> void cascade(ObjectTrie<T> && toCascade, U moveFunction) {
		if (!(trieRoot.value == toCascade.trieRoot.value)) {
			return; // NOP
		}

		trieRoot.value = moveFunction(toCascade.trieRoot.value);
		trieRoot.cascadeChildren(toCascade.trieRoot, moveFunction);
	}

	////////////////////////// Private implementation /////////////////////////

	private: static const T & stdCopyFunction(const T & in) {
		return in;
	}

	private: static T && stdMoveFunction(T & in) {
		return std::move(in);
	}

	private: ObjectTrieNode<T> trieRoot;
};

template <typename T>
typename ObjectTrie<T>::DepthIterator ObjectTrie<T>::DepthIterator::endMarker;

template <typename T>
typename ObjectTrie<T>::ConstDepthIterator ObjectTrie<T>::ConstDepthIterator::endMarker;

template <typename T>
typename ObjectTrie<T>::BreadthIterator ObjectTrie<T>::BreadthIterator::endMarker;

template <typename T>
typename ObjectTrie<T>::ConstBreadthIterator ObjectTrie<T>::ConstBreadthIterator::endMarker;

///
/// Print the contents of the trie to the stream via a breadth first traversal.
///
template <typename T>
std::ostream & operator << (std::ostream & stream, const ObjectTrie<T> & trie) {
	typename ObjectTrie<T>::ConstBreadthIterator iter = trie.breadthBegin();
	typename ObjectTrie<T>::ConstBreadthIterator end = trie.breadthEnd();

	while (iter != end) {
		stream << iter->value << "\n";
		++iter;
	}

	return stream;
}

} // namespace Balau::Container

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__BALAU_TRIE
