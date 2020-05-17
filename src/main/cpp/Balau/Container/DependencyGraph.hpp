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
// https://www.boost.org/doc/libs/1_68_0/libs/graph/doc/file_dependency_example.html
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

namespace Balau::Container {

///
/// A graph that models the dependencies between a set of objects of type VertexT.
///
/// The dependency graph provides information on:
///  - the direct dependencies of nodes;
///  - the dependency order of the set of nodes;
///  - the parallel dependency order of the set of nodes;
///  - whether the dependency tree has any cycles.
///
/// The type VertexT is usually defined to be an arbitrary type, but it may also be
/// a Boost property list if vertex property lists are required.
///
/// In addition to vertex objects, edge data may also be attached to the edges of the
/// graph if required. In order to do this, the additional optional type EdgeT should
/// be defined.
///
/// Finally, a single graph data object may also be attached to the graph if required.
/// This allows a bundle of data to be closely associated with the graph, without
/// requiring the developer to define a custom class containing the bundle and the
/// graph itself. In order attach a single object to the graph, the additional optional
/// type GraphT should be defined.
///
/// This class was initially conceived to support the injector, but may be used
/// for any set of dependencies by creating the appropriate object type VertexT.
///
/// As two copies of the objects of type VertexT are maintained in the dependency graph
/// class, the type VertexT should be a compact representation of the concept being modelled.
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
template <typename VertexT = boost::no_property, typename EdgeT = boost::no_property, typename GraphT = boost::no_property>
class DependencyGraph {
	private: using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS, VertexT, EdgeT, GraphT>;
	private: using Vertex = typename Graph::vertex_descriptor;
	private: using Edge = typename Graph::edge_descriptor;
	private: using VertexIterator = typename Graph::vertex_iterator;
	private: using InEdgeIterator = typename Graph::in_edge_iterator;
	private: using OutEdgeIterator = typename Graph::out_edge_iterator;

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
			iterator ret = *this;
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
		public: VertexT & operator * () {
			return graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const VertexT & operator * () const {
			return graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: VertexT & operator -> () {
			return &graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const VertexT & operator -> () const {
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

		friend class DependencyGraph<VertexT>;

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
		public: const VertexT & operator * () const {
			return graph[*vertexIter];
		}

		///
		/// Dereference the iterator in order to obtain the object pointed to.
		///
		public: const VertexT & operator -> () const {
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

		friend class DependencyGraph<VertexT>;

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
	public: void addDependency(const VertexT & dependency, bool throwIfExists = true) {
		if (reverseLookup.find(dependency) != reverseLookup.end()) {
			if (throwIfExists) {
				ThrowBalauException(Exception::ItemExistsException<VertexT>, dependency, "");
			} else {
				return;
			}
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
	public: void removeDependency(const VertexT & dependency, bool throwIfNotExists = true) {
		//logGraph("Before remove");

		auto itemIter = reverseLookup.find(dependency);

		if (itemIter == reverseLookup.end()) {
			if (throwIfNotExists) {
				ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, dependency, "");
			} else {
				return;
			}
		}

		const auto vertex = itemIter->second;
		boost::clear_vertex(vertex, graph); // Removes edges to/from the vertex.
		boost::remove_vertex(vertex, graph); // Removes the vertex.
		rebuildReverseLookup();

		//logGraph("After rebuild");
	}

	///
	/// Add a dependency relationship between two existing dependencies.
	///
	public: void addRelationship(const VertexT & independent, const VertexT & dependent) {
		auto independentIter = reverseLookup.find(independent);
		auto dependentIter = reverseLookup.find(dependent);

		if (independentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, independent, "");
		}

		if (dependentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, dependent, "");
		}

		boost::add_edge(independentIter->second, dependentIter->second, graph);
	}

	///
	/// Add a dependency relationship between two existing dependencies, with edge data.
	///
	public: void addRelationship(const VertexT & independent, const VertexT & dependent, const EdgeT & edgeData) {
		auto independentIter = reverseLookup.find(independent);
		auto dependentIter = reverseLookup.find(dependent);

		if (independentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, independent, "");
		}

		if (dependentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, dependent, "");
		}

		boost::add_edge(independentIter->second, dependentIter->second, edgeData, graph);
	}

	///
	/// Remove a dependency relationship between two existing dependencies.
	///
	public: void removeRelationship(const VertexT & independent, const VertexT & dependent) {
		auto independentIter = reverseLookup.find(independent);
		auto dependentIter = reverseLookup.find(dependent);

		if (independentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, independent, "");
		}

		if (dependentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, dependent, "");
		}

		boost::remove_edge(independentIter->second, dependentIter->second, graph);
	}

	public: EdgeT getRelation(const VertexT & independent, const VertexT & dependent) const {
		auto independentIter = reverseLookup.find(independent);
		auto dependentIter = reverseLookup.find(dependent);

		if (independentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, independent, "");
		}

		if (dependentIter == reverseLookup.end()) {
			ThrowBalauException(Exception::ItemDoesNotExistException<VertexT>, dependent, "");
		}

		auto independentVertex = independentIter->second;
		auto dependentVertex = dependentIter->second;
		auto e = boost::edge(independentVertex, dependentVertex, graph);

		if (!e.second) {
			_ThrowBalauException_generateStackTrace
			throw Exception::RelationshipDoesNotExistException<VertexT, VertexT>(
				__FILE__, __LINE__, st, independent, dependent, ""
			);
		}

		return graph[e.first];
	}

	///
	/// Does the graph have the specified dependency?
	///
	public: bool hasDependency(const VertexT & dependency) const {
		return reverseLookup.find(dependency) != reverseLookup.end();
	}

	///
	/// What are the direct dependencies of the specified dependency.
	///
	public: std::vector<VertexT> directDependenciesOf(const VertexT & dependency) const {
		auto vertex = boost::vertex(reverseLookup.at(dependency), graph);
		InEdgeIterator iter, end;
		std::vector<VertexT> ret;

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
	public: const VertexT * lookupDependency(const VertexT & key) const {
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
	public: std::vector<VertexT> dependencyOrder() const {
		std::vector<VertexT> ret;
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
	public: std::vector<std::vector<VertexT>> parallelDependencyOrder() const {
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

		std::vector<std::vector<VertexT>> results(levelCount + 1, std::vector<VertexT>());
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
		std::vector<std::pair<VertexT, VertexT>> cycleEdges;
		CycleDetector cycleDetector(cycleEdges);
		boost::depth_first_search(graph, boost::visitor(cycleDetector));
		return !cycleEdges.empty();
	}

	///
	/// Does the dependency graph have any cycles?
	///
	/// @param cycleEdges any edges that are found to have cycles are added to this vector
	///
	public: bool hasCycles(std::vector<std::pair<VertexT, VertexT>> & cycleEdges) const {
		const auto sz = cycleEdges.size();
		CycleDetector cycleDetector(cycleEdges);
		boost::depth_first_search(graph, boost::visitor(cycleDetector));
		return cycleEdges.size() > sz;
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
		if (!Impl::ContainerLogger::log().enabled(level)) {
			return;
		}

		std::ostringstream builder;

		builder << title << "\n";

		VertexIterator vertexIter, vertexEnd;
		InEdgeIterator edgeIter, edgeEnd;

		using ::toString;

		const int indexLength = (int) toString(reverseLookup.size()).length();

		for (boost::tie(vertexIter, vertexEnd) = vertices(graph); vertexIter != vertexEnd; ++vertexIter) {
			auto vertex = *vertexIter;
			auto value = graph[vertex];

			builder << std::right << std::setw(indexLength) << vertex << ": " << toString(value) << "\n";

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

        Impl::ContainerLogger::log().log(level, builder.str().c_str());
	}

	////////////////////////// Private implementation /////////////////////////

	private: struct CycleDetector : public boost::dfs_visitor<> {
		explicit CycleDetector(std::vector<std::pair<VertexT, VertexT>> & cycleEdges_) : cycleEdges(cycleEdges_) {}

		template <class Edge, class Graph> void back_edge(Edge e, Graph & g) {
			VertexT s = g[source(e, g)];
			VertexT t = g[target(e, g)];
			cycleEdges.emplace_back(std::pair<VertexT, VertexT>(s, t));
		}

		private: std::vector<std::pair<VertexT, VertexT>> & cycleEdges;
	};

	// This would not be required if the graph library had reverse vertex
	// property lookup (maybe it has this hidden somewhere).
	private: void rebuildReverseLookup() {
		reverseLookup.clear();
		VertexIterator vertexIter, vertexEnd;

		for (boost::tie(vertexIter, vertexEnd) = vertices(graph); vertexIter != vertexEnd; ++vertexIter) {
			auto vertex = *vertexIter;
			auto value = graph[vertex];

			reverseLookup.insert(std::pair<VertexT, Vertex>(value, vertex));
		}
	}

	private: Graph graph;
	private: std::unordered_map<VertexT, Vertex> reverseLookup;
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER_DEPENDENCY_GRAPH
