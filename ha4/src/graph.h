#pragma once

#include <unordered_set>
#include <list>
//#include "filtered_graph.h"

//// 1. std::unordered_set-like interface for vertex, vertex is not
//// divided into key and value
//// 2. vertex_data is hash'able by std::hash
//// 3. not a multi graph
//// 4. both vertex and edge iterators provides 'data' field with
//// corresponding vertex and edge data
//// 5. edge iterator supports 'from' and 'to' fields

namespace au {
template <class v_data, class e_data>
class graph {
public:
    using vertex_data = v_data;
    using edge_data   = e_data;

private:
    struct vertex;
    struct hasher;
    struct edge;

    using vertex_data_iterator = typename std::unordered_set<vertex, hasher>::iterator;
    using edge_data_iterator   = typename std::list<edge>::iterator;
    using const_vertex_data_iterator = typename std::unordered_set<vertex, hasher>::const_iterator;
    using const_edge_data_iterator   = typename std::list<edge>::const_iterator;

    struct edge {
        edge(vertex_data&& from, vertex_data&& to, edge_data data)
        : from_(std::move(from))
        , to_(std::move(to))
        , data_(std::move(data))
        {}

        vertex_data& from() {
            return from_;
        }

        vertex_data& to() {
            return to_;
        }

        edge_data& data() {
            return data_;
        }

        const edge_data& data() const {
            return data_;
        }

    private:
        vertex_data from_;
        vertex_data to_;
        edge_data data_;
    };

    struct vertex {
        vertex(vertex_data data)
            : data_(std::move(data))
        {}

        vertex(const vertex& v) = default;
        vertex(vertex&& v)      = default;

        vertex& operator = (const vertex& v) = default;
        vertex& operator = (vertex&& v)      = default;

        const vertex_data& data() const {
            return data_;
        }

        std::list<edge>& get_adj_list() const {
            return adj_list;
        }

        bool operator == (const vertex& l) const {
            return true;
        }
    private:
        vertex_data data_;
        mutable std::list<edge> adj_list;
    };

    struct hasher {
        size_t operator()(const vertex& v) const noexcept {
            return std::hash<vertex_data>()(v.data());
        }
    };

    std::unordered_set<vertex, hasher> vertexes;

public:
    template <class iterator, class data>
    class graph_iterator {
    private:
        iterator it;
        iterator end;
        std::function<bool(data)> filter_;
    public:
        graph_iterator(iterator it, iterator end,
                       std::function<bool(data)> f = [](data) {return true;})
            : it(std::move(it))
            , end(end)
            , filter_(f)
        {}

        graph_iterator(const graph_iterator& it) = default;

        iterator get_inner() {
            return it;
        }

        auto operator *() {
            return it->data();
        }

        auto operator *() const {
            return it->data();
        }

        iterator& operator ->() {
            return it;
        }

        graph_iterator& operator++() {
            ++it;
            while ((it != end) && !filter_(  it->data()  )) ++it;
            return *this;
        }

        graph_iterator operator++(int) {
            graph_iterator tmp = *this;
            ++it;
            while ((it != end) && !filter_(  it->data()  )) ++it;
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

    using vertex_iterator = graph_iterator<vertex_data_iterator, vertex_data>;
    using edge_iterator   = graph_iterator<edge_data_iterator  , edge_data>;

    using const_vertex_iterator = graph_iterator<const_vertex_data_iterator, vertex_data>;
    using const_edge_iterator   = graph_iterator<const_edge_data_iterator  , edge_data>;

    graph() {}
    graph(const graph& other) 			  = default;
    graph(graph&& other) 				  = default;
    graph& operator= (const graph& other) = default;
    graph& operator= (graph&& other)      = default;

    vertex_iterator add_vertex(vertex_data vdata) {
        auto it = vertexes.insert(vertex(std::move(vdata)));
        return vertex_iterator(it.first, vertexes.end());
    }

    edge_iterator add_edge(vertex_iterator& from,
                           vertex_iterator& to,
                           edge_data edata) {
        std::list<edge>& adj_list = from->get_adj_list();
        adj_list.push_back({*from, *to, edata});
        return edge_iterator(--adj_list.end(), adj_list.end());
    }

    void remove_vertex(vertex_iterator vit) {
        std::list<edge_data_iterator> removed_edges;

        for (auto& vertex : vertexes) {
            std::list<edge>& adj_list = vertex.get_adj_list();
            for (auto edge = adj_list.begin(); edge != adj_list.end();) {
                auto from_it = vertexes.find(edge->from());
                auto to_it   = vertexes.find(edge->to());
                if (from_it == vit.it || to_it == vit.it) {
                    adj_list.erase(edge++);
                } else {
                    ++edge;
                }
            }
        }

        vertexes.erase(vit.it);
    }

    void remove_edge(edge_iterator eit) {
        vertex_data& from = eit->from();
        std::list<edge>& adj_list = vertexes.find(from)->get_adj_list();
        adj_list.erase(eit.it);
    }

    vertex_iterator find_vertex(vertex_data& vdata) {
        return vertex_iterator(vertexes.find(vdata), vertexes.end());
    }

    const_vertex_iterator find_vertex(const vertex_data& vdata) const {
        return const_vertex_iterator(vertexes.find(vdata), vertexes.end());
    }

    edge_iterator find_edge(vertex_iterator from, vertex_iterator to) {
        auto end = edge_end(from);
        auto edge = edge_begin(from);
        for (; edge != end; ++edge) {
            if (vertexes.find(edge->to()) == to.it) {
                break;
            }
        }
        return edge;
    }

    const_edge_iterator find_edge(const_vertex_iterator from, const_vertex_iterator to) const {
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

    const_vertex_iterator vertex_cbegin() const {
        return const_vertex_iterator(vertexes.cbegin(), vertexes.cend());
    }

    vertex_iterator vertex_end() {
        return vertex_iterator(vertexes.end(), vertexes.end());
    }

    const_vertex_iterator vertex_cend() const {
        return const_vertex_iterator(vertexes.cend(), vertexes.end());
    }

    edge_iterator edge_begin(vertex_iterator from) {
        std::list<edge>& adj_list = from->get_adj_list();
        return edge_iterator(adj_list.begin(), adj_list.end());
    }

    const_edge_iterator edge_cbegin(const_vertex_iterator from) const {
        std::list<edge>& adj_list = from->get_adj_list();
        return const_edge_iterator(adj_list.begin(), adj_list.end());
    }

    edge_iterator edge_end(vertex_iterator from) {
        std::list<edge>& adj_list = from->get_adj_list();
        return edge_iterator(adj_list.end(), adj_list.end());
    }

    const_edge_iterator edge_cend(const_vertex_iterator from) const {
        std::list<edge>& adj_list = from->get_adj_list();
        return const_edge_iterator(adj_list.end(), adj_list.end());
    }
};

} // namespace au
