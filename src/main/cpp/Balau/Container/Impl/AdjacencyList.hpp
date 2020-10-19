// @formatter:off
//
// Balau core C++ library
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
#ifndef COM_BORA_SOFTWARE__BALAU_CONTAINER_IMPL__ADJACENCY_LIST
#define COM_BORA_SOFTWARE__BALAU_CONTAINER_IMPL__ADJACENCY_LIST

#include <Balau/Application/Impl/Binding.hpp>

namespace Balau::Container {

template <typename VertexDataT, typename EdgeDataT> class AdjacencyList;

template <typename VertexDataT, typename EdgeDataT> class Vertex {
	public: Vertex(const Vertex & copy) noexcept : owner(copy.owner) {}
	public: Vertex(Vertex && rhs) noexcept : owner(rhs.owner) {}
	public: ~Vertex() noexcept = default;

	///
	/// Get a non-const reference to the vertex's data object.
	///
	public: VertexDataT & getData() noexcept;

	///
	/// Get a const reference to the vertex's data object.
	///
	public: const VertexDataT & getData() const noexcept;

	private: Vertex(AdjacencyList<VertexDataT, EdgeDataT> & owner_, unsigned int index_) noexcept
		: owner(owner_)
		, index(index_) {}

	private: AdjacencyList<VertexDataT, EdgeDataT> & owner;
	private: unsigned int index;

	friend class AdjacencyList<VertexDataT, EdgeDataT>;
};

template <typename VertexDataT, typename EdgeDataT> class Edge {
	public: Edge(const Edge & copy) noexcept : owner(copy.owner) {}
	public: Edge(Edge && rhs) noexcept : owner(rhs.owner) {}

	///
	/// Get a const reference to the edge's data object.
	///
	public: EdgeDataT & getData() noexcept;

	public: const EdgeDataT & getData() const noexcept;

	private: Edge(AdjacencyList<VertexDataT, EdgeDataT> & owner_,
	              unsigned int edgeIndex_,
	              unsigned int out_,
	              unsigned int in_)
		: owner(owner_)
		, edgeIndex(edgeIndex_)
		, out(out_)
		, in(in_) {}

	private: AdjacencyList<VertexDataT, EdgeDataT> & owner;
	private: unsigned int edgeIndex;
	private: unsigned int out;
	private: unsigned int in;

	friend class AdjacencyList<VertexDataT, EdgeDataT>;
};

template <typename VertexDataT, typename EdgeDataT> class AdjacencyList {
	public: using VertexT = Container::Vertex<VertexDataT, EdgeDataT>;
	public: using EdgeT = Container::Edge<VertexDataT, EdgeDataT>;

	public: AdjacencyList()
		: edgeMapping([this] (const EdgeDataT & lhs, unsigned int rhs) { return lhs < edgeData[rhs]; })
		, edgesOut([this] (const VertexDataT & lhs, unsigned int rhs) { return lhs < vertexData[rhs]; })
		, edgesIn([this] (const VertexDataT & lhs, unsigned int rhs) { return lhs < vertexData[rhs]; }) {}

	public: bool contains(const VertexDataT & data) noexcept {
		return edgesOut.find(data) != edgesOut.end();
	}

	public: VertexT addVertex(VertexDataT data) {
		const auto vertexIndex = data.size() + 1;
		vertexData.emplace_back(std::move(data));
		edgesOut.insert(std::make_pair(vertexIndex, std::vector<unsigned int>()));
		edgesIn.insert(std::make_pair(vertexIndex, std::vector<unsigned int>()));
		return VertexT(this, vertexIndex);
	}

	public: EdgeT addEdge(EdgeDataT data, const VertexT & out, const VertexT & in) {
		const auto edgeIndex = data.size() + 1;
		edgeData.emplace_back(std::move(data));
		edgeMapping.insert(std::make_pair(edgeIndex, { out.index, in.index }));
		edgesOut.insert(std::make_pair(edgeIndex, { out.index }));
		edgesOut.insert(std::make_pair(edgeIndex, { in.index }));
		return EdgeT(this, edgeIndex, out.index, in.index);
	}

	public: void removeVertex(const VertexT & dependency, bool throwIfNotExists = true) {
		vertexData.erase(dependency.index);
		edgesOut.erase(dependency.index);
		edgesIn.erase(dependency.index);
	}

	public: void addEdge(const VertexT & independent, const VertexT & dependent) {
	}

	public: void addEdge(const VertexT & independent, const VertexT & dependent, const EdgeT & data) {
	}

	public: void removeEdge(const VertexT & independent, const VertexT & dependent) {
		// TODO
	}

	public: EdgeT getRelation(const VertexT & independent, const VertexT & dependent) const {
		// TODO
	}

	public: bool contains(const VertexT & dependency) const {
		// TODO
	}

	////////////////////////// Private implementation /////////////////////////

	friend class Vertex<VertexDataT, EdgeDataT>;
	friend class Edge<VertexDataT, EdgeDataT>;

	private: VertexDataT & getVertexData(unsigned int index) noexcept {
		return vertexData[index];
	}

	private: const VertexDataT & getVertexData(unsigned int index) const noexcept {
		return vertexData[index];
	}

	private: EdgeDataT & getEdgeData(unsigned int index) noexcept {
		return edgeData[index];
	}

	private: const EdgeDataT & getEdgeData(unsigned int index) const noexcept {
		return edgeData[index];
	}

	//
	// The data for each vertex.
	//
	private: std::vector<VertexDataT> vertexData;

	//
	// The data for each edge.
	//
	private: std::vector<EdgeDataT> edgeData;

	//
	// key = edge index
	// value = ordered pair of vertex indices
	//
	private: std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> edgeMapping;

	//
	// key = vertex index
	// value = vector of out edge indices
	//
	private: std::unordered_map<unsigned int, std::vector<unsigned int>> edgesOut;

	//
	// key = vertex index
	// value = vector of in edge indices
	//
	private: std::unordered_map<unsigned int, std::vector<unsigned int>> edgesIn;
};

template <typename VertexDataT, typename EdgeDataT>
inline VertexDataT & Vertex<VertexDataT, EdgeDataT>::getData() noexcept {
	owner.getVertexData();
}

template <typename VertexDataT, typename EdgeDataT>
inline const VertexDataT & Vertex<VertexDataT, EdgeDataT>::getData() const noexcept {
	owner.getVertexData();
}

template <typename VertexDataT, typename EdgeDataT>
inline EdgeDataT & Edge<VertexDataT, EdgeDataT>::getData() noexcept {
	owner.getEdgeData();
}

template <typename VertexDataT, typename EdgeDataT>
inline const EdgeDataT & Edge<VertexDataT, EdgeDataT>::getData() const noexcept {
	owner.getEdgeData();
}

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_CONTAINER_IMPL__ADJACENCY_LIST
