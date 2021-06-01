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
// Original dependency example from the Boost Graph library.
//
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <Balau/Type/ToString.hpp>

namespace Balau::Container {

struct Dep {
	std::string_view name;
};

inline bool operator == (const Dep & lhs, const Dep & rhs) {
	return lhs.name == rhs.name;
}

inline std::string toString(const Dep & obj) {
	return std::string(obj.name);
}

} // namespace Balau::Container

namespace std {

template <> struct hash<Balau::Container::Dep> {
	size_t operator () (const Balau::Container::Dep & dep) const noexcept {
		return hash<std::string_view>()(dep.name);
	}
};

} // namespace std

#include <TestResources.hpp>
#include "../../../../main/cpp/Balau/Container/DependencyGraph.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <time.h>

namespace Balau {

using Testing::is;
using Testing::isNotNull;

namespace Container {

const char * dax_h       = "dax.h";
const char * yow_h       = "yow.h";
const char * boz_h       = "boz.h";
const char * zow_h       = "zow.h";
const char * foo_cpp     = "foo.cpp";
const char * foo_o       = "foo.o";
const char * bar_cpp     = "bar.cpp";
const char * bar_o       = "bar.o";
const char * libfoobar_a = "libfoobar.a";
const char * zig_cpp     = "zig.cpp";
const char * zig_o       = "zig.o";
const char * zag_cpp     = "zag.cpp";
const char * zag_o       = "zag.o";
const char * libzigzag_a = "libzigzag.a";
const char * killerapp   = "killerapp";

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

std::unordered_map<const char *, Vertex> verticesByName;
std::unordered_map<Vertex, const char *> namesByVertex;

struct print_visitor : public boost::bfs_visitor<> {
	template <class Vertex, class Graph> void discover_vertex(Vertex v, Graph &) {
		std::cout << namesByVertex.at(v) << " ";
	}
};

struct cycle_detector : public boost::dfs_visitor<> {
	cycle_detector(bool & has_cycle)
		: m_has_cycle(has_cycle) {
	}

	template <class Edge, class Graph> void back_edge(Edge, Graph &) {
		m_has_cycle = true;
	}

	protected: bool & m_has_cycle;
};

class A {
	public: double d;
};

void addVertex(Graph & g, const char * name) {
	Graph::vertex_descriptor vertex = boost::add_vertex(g);
	verticesByName[name] = vertex;
	namesByVertex[vertex] = name;
}

void addEdge(Graph & g, const char * from, const char * to) {
	boost::add_edge(verticesByName[from], verticesByName[to], g);
}

struct DependencyGraphTest : public Testing::TestGroup<DependencyGraphTest> {
	DependencyGraphTest() {
		RegisterTestCase(test_prep);
		RegisterTestCase(test);
	}

	void test_prep() {
		Graph g;

		addVertex(g, dax_h);
		addVertex(g, yow_h);
		addVertex(g, boz_h);
		addVertex(g, zow_h);
		addVertex(g, foo_cpp);
		addVertex(g, foo_o);
		addVertex(g, bar_cpp);
		addVertex(g, bar_o);
		addVertex(g, libfoobar_a);
		addVertex(g, zig_cpp);
		addVertex(g, zig_o);
		addVertex(g, zag_cpp);
		addVertex(g, zag_o);
		addVertex(g, libzigzag_a);
		addVertex(g, killerapp);

		addEdge(g, dax_h, foo_cpp);
		addEdge(g, dax_h, bar_cpp);
		addEdge(g, dax_h, yow_h);
		addEdge(g, yow_h, bar_cpp);
		addEdge(g, yow_h, zag_cpp);
		addEdge(g, boz_h, bar_cpp);
		addEdge(g, boz_h, zig_cpp);
		addEdge(g, boz_h, zag_cpp);
		addEdge(g, zow_h, foo_cpp);
		addEdge(g, foo_cpp, foo_o);
		addEdge(g, foo_o, libfoobar_a);
		addEdge(g, bar_cpp, bar_o);
		addEdge(g, bar_o, libfoobar_a);
		addEdge(g, libfoobar_a, libzigzag_a);
		addEdge(g, zig_cpp, zig_o);
		addEdge(g, zig_o, libzigzag_a);
		addEdge(g, zag_cpp, zag_o);
		addEdge(g, zag_o, libzigzag_a);
		addEdge(g, libzigzag_a, killerapp);

		// Determine ordering for a full recompilation
		// and the order with files that can be compiled in parallel
		{
			std::list<Vertex> make_order;

			topological_sort(g, std::front_inserter(make_order));
			std::cout << "make ordering: ";

			for (auto i = make_order.begin(); i != make_order.end(); ++i) {
				std::cout << namesByVertex[*i] << " ";
			}

			std::cout << std::endl << std::endl;

			// Parallel compilation ordering
			std::vector<int> time(namesByVertex.size(), 0);

			for (auto i = make_order.begin(); i != make_order.end(); ++i) {
				// Walk through the in_edges an calculate the maximum time.
				if (in_degree(*i, g) > 0) {
					Graph::in_edge_iterator j, j_end;
					int maxdist = 0;
					// Through the order from topological sort, we are sure that every
					// time we are using here is already initialized.
					for (boost::tie(j, j_end) = in_edges(*i, g); j != j_end; ++j) {
						maxdist = (std::max)(time[source(*j, g)], maxdist);
					}
					time[*i] = maxdist + 1;
				}
			}

			std::cout << "parallel make ordering, " << std::endl
			     << "vertices with same group number can be made in parallel" << std::endl;

			{
				boost::graph_traits<Graph>::vertex_iterator i, iend;

				for (boost::tie(i, iend) = vertices(g); i != iend; ++i) {
					std::cout << "time_slot[" << namesByVertex[*i] << "] = " << time[*i] << std::endl;
				}
			}

		}

		std::cout << std::endl;

		// if I change yow.h what files need to be re-made?
		{
			std::cout << "A change to yow.h will cause what to be re-made?" << std::endl;
			print_visitor vis;
			breadth_first_search(g, vertex(verticesByName[yow_h], g), visitor(vis));
			std::cout << std::endl;
		}

		std::cout << std::endl;

		// are there any cycles in the graph?
		{
			bool has_cycle = false;
			cycle_detector vis(has_cycle);
			depth_first_search(g, visitor(vis));
			std::cout << "The graph has a cycle? " << has_cycle << std::endl;
		}

		std::cout << std::endl;

		// add a dependency going from bar.cpp to dax.h
		{
			std::cout << "adding edge bar_cpp -> dax_h" << std::endl;
			boost::add_edge(verticesByName[bar_cpp], verticesByName[dax_h], g);
		}

		std::cout << std::endl;

		// are there any cycles in the graph?
		{
			bool has_cycle = false;
			cycle_detector vis(has_cycle);
			depth_first_search(g, visitor(vis));
			std::cout << "The graph has a cycle now? " << has_cycle << std::endl;
		}
	}

	void test() {
		// Some dependencies.
		std::vector<Dep> deps {
			  { "zero"  }
			, { "one"   }
			, { "two"   }
			, { "three" }
			, { "four"  }
			, { "five"  }
		};

		// Some relationships between the dependencies.
		std::vector<std::pair<Dep, Dep>> relationships {
			  { deps[0], deps[1] }
			, { deps[0], deps[4] }
			, { deps[1], deps[2] }
			, { deps[1], deps[3] }
			, { deps[1], deps[4] }
			, { deps[2], deps[4] }
			, { deps[3], deps[5] }
			, { deps[4], deps[5] }
		};

		std::vector<Dep> expectedDirectDependenciesOf5 { deps[3], deps[4] };

		std::vector<Dep> expectedDependencyOrder {
			// Two and three are on the same level.
			deps[0], deps[1], deps[3], deps[2], deps[4], deps[5]
		};

		std::vector<std::vector<Dep>> expectedParallelDependencyOrder {
			  { deps[0] }
			, { deps[1] }
			, { deps[2], deps[3] }
			, { deps[4] }
			, { deps[5] }
		};

		DependencyGraph<Dep> dependencyGraph;

		// Add the dependencies.
		for (auto d : deps) {
			dependencyGraph.addDependency(d);
		}

		// Then add the relationships between them.
		for (auto r : relationships) {
			dependencyGraph.addRelationship(r.first, r.second);
		}

		// Perform assertions.

		auto actualDirectDependenciesOf5 = dependencyGraph.directDependenciesOf(deps[5]);
		auto actualDependencyOrder = dependencyGraph.dependencyOrder();
		auto actualParallelDependencyOrder = dependencyGraph.parallelDependencyOrder();

		AssertThat(dependencyGraph.hasCycles(), is(false));
		AssertThat(actualDirectDependenciesOf5, is(expectedDirectDependenciesOf5));
		AssertThat(actualDependencyOrder, is(expectedDependencyOrder));
		AssertThat(actualParallelDependencyOrder, is(expectedParallelDependencyOrder));

		// Remove a dependency and perform assertions again.

		dependencyGraph.removeDependency(deps[3]);

		std::vector<Dep> modifiedExpectedDirectDependenciesOf5 { deps[4] };

		std::vector<Dep> modifiedExpectedDependencyOrder {
			deps[0], deps[1], deps[2], deps[4], deps[5]
		};

		std::vector<std::vector<Dep>> modifiedExpectedParallelDependencyOrder {
			  { deps[0] }
			, { deps[1] }
			, { deps[2] }
			, { deps[4] }
			, { deps[5] }
		};

		auto modifiedActualDirectDependenciesOf5 = dependencyGraph.directDependenciesOf(deps[5]);
		auto modifiedActualDependencyOrder = dependencyGraph.dependencyOrder();
		auto modifiedActualParallelDependencyOrder = dependencyGraph.parallelDependencyOrder();

		AssertThat(dependencyGraph.hasCycles(), is(false));
		AssertThat(modifiedActualDirectDependenciesOf5, is(modifiedExpectedDirectDependenciesOf5));
		AssertThat(modifiedActualDependencyOrder, is(modifiedExpectedDependencyOrder));
		AssertThat(modifiedActualParallelDependencyOrder, is(modifiedExpectedParallelDependencyOrder));
	}
};

} // namespace Container

} // namespace Balau
