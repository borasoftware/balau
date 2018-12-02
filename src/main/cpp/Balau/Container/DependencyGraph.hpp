// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//
// See the Boost Graph dependency example for inspiration and background information.
// https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/file_dependency_example.html
//
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

///
/// @file DependencyGraph.hpp
///
/// A graph that models the dependencies between a set of objects of type T.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER__DEPENDENCY_GRAPH
#define COM_BORA_SOFTWARE__BALAU_CONTAINER__DEPENDENCY_GRAPH

#include <Balau/Container/Impl/ContainerLogger.hpp>
#include <Balau/Exception/ContainerExceptions.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>

#include <iomanip>

// Avoid false positive.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau::Container {

///
/// A graph that models the dependencies between a set of objects of type T.
///
/// This class was initially conceived to support the injector, but may be used
/// for any set of dependencies by creating the appropriate object type T.
///
/// As two copies of the objects of type T are maintained in the dependency graph
/// class, the type T should be a compact representation of the concept being modelled.
/// If this is not possible, a compact proxy object should be used and a single copy
/// of the real objects should be maintained in an external structure instead.
///
/// It is best to avoid calling DependencyGraph::removeDependency on larger dependency
/// graphs, as the method is O(n) due to the need to rebuild the reverse lookup map
/// on each call.
///
/// This data structure is not thread safe.
///
/// The %DependencyGraph class uses the Boost Graph Library and is inspired from the
/// Boost Graph library dependency example (Copyright 1997, 1998, 1999, 2000 University
/// of Notre Dame; authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek).
///
template <typename T> class DependencyGraph {
	private: using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS, T>;
	private: using InEdgeIterator = typename Graph::in_edge_iterator;
	private: using OutEdgeIterator = typename Graph::out_edge_iterator;
	private: using Vertex = typename Graph::vertex_descriptor;
	private: using VertexIterator = typename Graph::vertex_iterator;

	///
	/// The %DependencyGraph non-const iterator.
	///
	public: class iterator {
		///
		/// Create an iterator by copying the supplied iterator.
		///
		public: iterator(const iterator & copy)
			: vertexIter(copy.vertexIter) {}

		///
		/// Set the current iterator to a copy of the supplied iterator.
		///
		public: iterator & operator = (const iterator & copy) {
			vertexIter = copy.vertexIter;
			return *this;
		}

		///
		/// Increment the iterator (post increment).
		///
		public: iterator operator ++ (int) {
			const_iterator ret = *this;
			++vertexIter;
			return ret;
		}

		///
		/// Increment the iterator.
		///
		public: iterator & operator ++ () {
			++vertexIter;
			return *this;
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: T & operator * () {
			return graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const T & operator * () const {
			return graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: T & operator -> () {
			return &graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const T & operator -> () const {
			return &graph[*vertexIter];
		}

		///
		/// returns true if the current iterator is equal to the supplied iterator.
		///
		public: bool operator == (const iterator & rhs) const {
			return vertexIter == rhs.vertexIter;
		}

		///
		/// returns true if the current iterator is not equal to the supplied iterator.
		///
		public: bool operator != (const iterator & rhs) const {
			return vertexIter != rhs.vertexIter;
		}

		friend class DependencyGraph<T>;

		private: explicit iterator(Graph & graph_, VertexIterator vertexIter_)
			: graph(graph_)
			  , vertexIter(vertexIter_) {}

		private: Graph & graph;
		private: VertexIterator vertexIter;
	};

	///
	/// The %DependencyGraph const iterator.
	///
	public: class const_iterator {
		///
		/// Create an iterator by copying the supplied iterator.
		///
		public: const_iterator(const const_iterator & copy)
			: vertexIter(copy.vertexIter) {}

		///
		/// Set the current iterator to a copy of the supplied iterator.
		///
		public: const_iterator & operator = (const const_iterator & copy) {
			vertexIter = copy.vertexIter;
			return *this;
		}

		///
		/// Increment the iterator (post increment).
		///
		public: const_iterator operator ++ (int) {
			const_iterator ret = *this;
			++vertexIter;
			return ret;
		}

		///
		/// Increment the iterator.
		///
		public: const_iterator & operator ++ () {
			++vertexIter;
			return *this;
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const T & operator * () const {
			return graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const T & operator -> () const {
			return &graph[*vertexIter];
		}

		///
		/// returns true if the current iterator is equal to the supplied iterator.
		///
		public: bool operator == (const const_iterator & rhs) const {
			return vertexIter == rhs.vertexIter;
		}

		///
		/// returns true if the current iterator is not equal to the supplied iterator.
		///
		public: bool operator != (const const_iterator & rhs) const {
			return vertexIter != rhs.vertexIter;
		}

		friend class DependencyGraph<T>;

		private: explicit const_iterator(const Graph & graph_, VertexIterator vertexIter_)
			: graph(graph_)
			  , vertexIter(vertexIter_) {}

		private: const Graph & graph;
		private: VertexIterator vertexIter;
	};

	///////////////////////////// Public methods //////////////////////////////

	///
	/// Add a dependency.
	///
	/// If the dependency already exists and throwIfExists is set to false, the result
	/// will be a NOP.
	///
	/// @param dependency the dependency to add
	/// @param throwIfExists set to true to throw if the dependency already exists
	/// @throw ItemExistsException if throwIfExists is set to true and the item already exists
	///
	public: void addDependency(const T & dependency, bool throwIfExists = true) {
		if (reverseLookup.find(dependency) != reverseLookup.end()) {
			ThrowBalauException(Exception::ItemExistsException, dependency, "");
		}

		Vertex vertex = boost::add_vertex(graph);
		reverseLookup[dependency] = vertex;
		graph[vertex] = dependency;
	}

	///
	/// Remove a dependency.
	///
	/// If the dependency does not exist and throwIfNotExists is set to false, the result
	/// will be a NOP.
	///
	/// This method is O(n) due to the need to rebuild the reverse lookup map on each call.
	///
	/// @param dependency the dependency to remove
	/// @param throwIfNotExists set to true to throw if the dependency does not exist
	/// @throw ItemDoesNotExistException if throwIfNotExists is set to true and the item does not exist
	///
	public: void removeDependency(const T & dependency, bool throwIfNotExists = true) {
		//logGraph("Before remove");

		auto itemIter = reverseLookup.find(dependency);

		if (itemIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException, dependency, "");
		}

		const auto vertex = itemIter->second;

		boost::clear_vertex(vertex, graph); // Removes edges to/from the vertex.
		boost::remove_vertex(vertex, graph); // Removes the vertex.
		rebuildReverseLookup();

		//logGraph("After rebuild");
	}

	///
	/// Does the graph have the specified dependency?
	///
	public: bool hasDependency(const T & dependency) const {
		return reverseLookup.find(dependency) != reverseLookup.end();
	}

	///
	/// Add a dependency relationship between two existing dependencies.
	///
	public: void addRelationship(const T & independent, const T & dependent) {
		// TODO check for exist edge..
		boost::add_edge(reverseLookup.at(independent), reverseLookup.at(dependent), graph);
	}

	///
	/// What are the direct dependencies of the specified dependency.
	///
	public: std::vector<T> directDependenciesOf(const T & dependency) const {
		auto vertex = boost::vertex(reverseLookup.at(dependency), graph);
		InEdgeIterator iter, end;
		std::vector<T> ret;

		for (boost::tie(iter, end) = boost::in_edges(vertex, graph); iter != end; ++iter) {
			auto s = source(*iter, graph);
			ret.emplace_back(graph[s]);
		}

		return ret;
	}

	///
	/// Lookups up the dependency that matches the supplied candidate object.
	///
	/// @return a const pointer to the dependency object, or nullptr if the key is not in the graph
	///
	public: const T * lookupDependency(const T & key) const {
		auto iter = reverseLookup.find(key);

		if (iter != reverseLookup.end()) {
			return &iter->first;
		} else {
			return nullptr;
		}
	}

	///
	/// Calculate the dependency order of the dependencies.
	///
	public: std::vector<T> dependencyOrder() const {
		std::vector<T> ret;
		std::list<Vertex> depOrder;
		boost::topological_sort(graph, std::front_inserter(depOrder));

		for (auto vertex : depOrder) {
			ret.push_back(graph[vertex]);
		}

		return ret;
	}

	///
	/// Calculate the parallel dependency order of the dependencies.
	///
	/// This method separates dependencies into steps, where each step contains a set
	/// of independent dependencies.
	///
	public: std::vector<std::vector<T>> parallelDependencyOrder() const {
		std::list<Vertex> depOrder;
		std::vector<Vertex> order(reverseLookup.size(), 0);
		boost::topological_sort(graph, std::front_inserter(depOrder));
		size_t levelCount = 0;

		for (auto vertex : depOrder) {
			if (in_degree(vertex, graph) > 0) {
				size_t maximum = 0;
				InEdgeIterator edgeIter, edgeEnd;

				for (boost::tie(edgeIter, edgeEnd) = in_edges(vertex, graph); edgeIter != edgeEnd; ++edgeIter) {
					auto t = order[source(*edgeIter, graph)];

					if (maximum < t) {
						maximum = t;
					}
				}

				order[vertex] = maximum + 1;

				if (levelCount < order[vertex]) {
					levelCount = order[vertex];
				}
			}
		}

		std::vector<std::vector<T>> results(levelCount + 1, std::vector<T>());
		VertexIterator vertexIter, vertexEnd;

		for (boost::tie(vertexIter, vertexEnd) = vertices(graph); vertexIter != vertexEnd; ++vertexIter) {
			const auto position = order[*vertexIter];
			results[position].emplace_back(graph[*vertexIter]);
		}

		return results;
	}

	///
	/// Does the dependency graph have any cycles?
	///
	public: bool hasCycles() const {
		bool hasCycle = false;
		CycleDetector cycleDetector(hasCycle);
		boost::depth_first_search(graph, boost::visitor(cycleDetector));
		return hasCycle;
	}

	///
	/// Get an iterator positioned at the beginning of the dependency graph.
	///
	public: iterator begin() {
		VertexIterator begin, end;
		boost::tie(begin, end) = boost::vertices(graph);
		return iterator(graph, begin);
	}

	///
	/// Get an iterator positioned at the end of the dependency graph.
	///
	public: iterator end() {
		VertexIterator begin, end;
		boost::tie(begin, end) = boost::vertices(graph);
		return iterator(graph, end);
	}

	///
	/// Get a const iterator positioned at the beginning of the dependency graph.
	///
	public: const_iterator begin() const {
		VertexIterator begin, end;
		boost::tie(begin, end) = boost::vertices(graph);
		return const_iterator(graph, begin);
	}

	///
	/// Get a const iterator positioned at the end of the dependency graph.
	///
	public: const_iterator end() const {
		VertexIterator begin, end;
		boost::tie(begin, end) = boost::vertices(graph);
		return const_iterator(graph, end);
	}

	///
	/// Log the dependency tree to the "balau.container" logger if the logger
	/// is enabled for the supplied logging level.
	///
	public: void logGraph(LoggingLevel level, const char * title) {
		if (!Impl::ContainerLogger::log.enabled(level)) {
			return;
		}

		std::ostringstream builder;

		builder << title << "\n";

		VertexIterator vertexIter, vertexEnd;
		InEdgeIterator edgeIter, edgeEnd;

		const int indexLength = (int) ::toString(reverseLookup.size()).length();

		for (boost::tie(vertexIter, vertexEnd) = vertices(graph); vertexIter != vertexEnd; ++vertexIter) {
			auto vertex = *vertexIter;
			auto value = graph[vertex];

			builder << std::right << std::setw(indexLength) << vertex << ": " << ::toString(value) << "\n";

			std::string_view prefix = "    deps: ";
			for (boost::tie(edgeIter, edgeEnd) = in_edges(vertex, graph); edgeIter != edgeEnd; ++edgeIter) {
				auto s = source(*edgeIter, graph);
				builder << prefix << s;
				prefix = ", ";
			}

			if (prefix == ", ") {
				builder << "\n";
			}
		}

		Impl::ContainerLogger::log.trace(builder.str().c_str());
	}

	////////////////////////// Private implementation /////////////////////////

	private: struct CycleDetector : public boost::dfs_visitor<> {
		explicit CycleDetector(bool & hasCycle_) : hasCycle(hasCycle_) {}

		template <class Edge, class Graph> void back_edge(Edge, Graph &) {
			hasCycle = true;
		}

		protected: bool & hasCycle;
	};

	// This would not be required if the graph library had reverse vertex
	// property lookup (maybe it has this hidden somewhere).
	private: void rebuildReverseLookup() {
		reverseLookup.clear();
		VertexIterator vertexIter, vertexEnd;

		for (boost::tie(vertexIter, vertexEnd) = vertices(graph); vertexIter != vertexEnd; ++vertexIter) {
			auto vertex = *vertexIter;
			auto value = graph[vertex];

			reverseLookup.insert(std::pair<T, Vertex>(value, vertex));
		}
	}

	private: Graph graph;
	private: std::unordered_map<T, Vertex> reverseLookup;
};

} // namespace Balau::Container

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER__DEPENDENCY_GRAPH
