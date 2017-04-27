#pragma once

#include <set>
#include <map>
#include <list>

namespace au {

// 1. std::unordered_set-like interface for vertex, vertex is not
// divided into key and value
// 2. vertex_data is hash'able by std::hash
// 3. not a multi graph
// 4. both vertex and edge iterators provides 'data' field with
// corresponding vertex and edge data
// 5. edge iterator supports 'from' and 'to' fields

template<class vertex_data, class edge_data>
class graph {
private:
	struct adj_list;
	struct edge;

    using vertex_data_iterator = typename std::map<vertex_data, adj_list>::iterator;
	using edge_data_iterator   = typename std::list<edge>::iterator;
    using const_vertex_data_iterator = typename std::map<vertex_data, adj_list>::const_iterator;
    using const_edge_data_iterator   = typename std::list<edge>::const_iterator;

	struct edge {
		edge(vertex_data_iterator from, vertex_data_iterator to, edge_data data)
        : from_(from)
        , to_(to)
        , data_(data)
		{}

        vertex_data_iterator from() {
            return from_;
        }

        vertex_data_iterator to() {
            return to_;
        }

        edge_data& get_data() {
            return data_;
        }

    private:
        vertex_data_iterator from_;
        vertex_data_iterator to_;
        edge_data data_;
	};

    struct vertex {
        vertex(vertex_data_iterator vit)
        : vit(vit)
        {}

    private:
        vertex_iterator vit;
    };

	struct adj_list {
		adj_list(const edge_data_iterator& end_iterator) {
			begin = end = end_iterator;
		}
		edge_data_iterator begin;
		edge_data_iterator end;
	};

	std::list<edge> edges;
	std::map<vertex_data, adj_list> vertexes;

	template <class iterator>
	class graph_iterator {
	private:
		iterator it;

//        static edge_data& get_value(edge_data_iterator it) {
//            return it->get_data();
//        }

//        static const vertex_data& get_value(vertex_data_iterator it) {
//            return it->first;
//        }
	public:
		graph_iterator(iterator it)
		: it(it)
		{}

		graph_iterator(const graph_iterator& it) = default;

		auto operator *() {
            return it->data();
		}

		auto operator *() const {
            return it->data();
        }

        auto data() {
            return it->data();
        }

        auto data() const {
            return it->data();
        }

        iterator& operator ->() {
            return it;
        }

		graph_iterator& operator++() {
			++it;
			return *this;
		}

		graph_iterator operator++(int) {
			graph_iterator tmp = *this;
			it++;
			return tmp;
		}

		bool operator ==(const graph_iterator& other) {
			return it == other.it;
		}

		bool operator !=(const graph_iterator& other) {
			return !(*this == other);
		}

		friend class graph;
	};

public:
	// minimum interface
	// both const and non-const implementations
	using vertex_iterator = graph_iterator<vertex_data_iterator>;
	using edge_iterator   = graph_iterator<edge_data_iterator>;

	using vertex_const_iterator = graph_iterator<const_vertex_data_iterator>;
	using edge_const_iterator   = graph_iterator<const_edge_data_iterator>;

	graph() {}
	graph(const graph& other) 			  = default;
	graph(graph&& other) 				  = default;
	graph& operator= (const graph& other) = default;
	graph& operator= (graph&& other)      = default;

	vertex_iterator add_vertex(vertex_data vdata) {
		auto pair = vertexes.insert(std::make_pair<vertex_data, adj_list>(
			std::move(vdata), adj_list(edges.end())));

		return vertex_iterator(pair.first);
	}

	edge_iterator add_edge(vertex_iterator const& from,
						   vertex_iterator const& to,
						   edge_data edata) {

        auto it = vertexes.find(*from);
        auto edge_it = edges.insert(
                it->second.begin, edge(from.it, to.it, edata));

        it->second.begin = edge_it;

        if (it->second.end == edges.end()) {
            it->second.end = it->second.begin;
        }

        return edge_iterator(edge_it);
    }

	void remove_vertex(vertex_iterator vit) {
		std::list<edge_data_iterator> removed_edges;

		for (edge_data_iterator edge = edges.begin(); edge != edges.end(); ++edge) {
            if (edge->from() == vit.it || edge->to() == vit.it) {
				removed_edges.push_back(edge);
			}
		}

		for (auto edge : removed_edges) {
			remove_edge(edge);
		}

		vertexes.erase(vit.it);
	}

	void remove_edge(edge_iterator eit) {
        auto from = eit.it->from();
		adj_list& list = from->second;

        if (list.begin == list.end) {
            list.begin = edges.end();
            list.end   = edges.end();
            return;
        }

		if (list.begin == eit.it) {
			++list.begin;
        } else if (list.end == eit.it) {
			--list.end;
		}

		edges.erase(eit.it);
	}

	// getters find, begin and end should support
	// both const and non-const versions
	vertex_iterator find_vertex(const vertex_data& vdata) {
		return vertex_iterator(vertexes.find(vdata));
	}

	edge_iterator find_edge(vertex_iterator from, vertex_iterator to) {
		auto end = edge_end(from);
		auto edge = edge_begin(from);
		for (; edge != end; ++edge) {
            if (edge.it->to() == to.it) {
				break;
			}
		}
		return edge;
	}

	vertex_iterator vertex_begin() {
		return vertex_iterator(vertexes.begin());
	}

	vertex_iterator vertex_end() {
		return vertex_iterator(vertexes.end());
	}

	edge_iterator edge_begin(vertex_iterator from) {
		auto adj_list = from.it->second;
		return edge_iterator(adj_list.begin);
	}

	edge_iterator edge_end(vertex_iterator from) {
		auto adj_list = from.it->second;
        return edge_iterator(++adj_list.end);
	}
};



} // namespace au
