#pragma once

#include <unordered_map>
#include <list>
#include <functional>


namespace au {
template <class v_data, class e_data>
class graph {
private:
    struct vertex;
    struct edge;

    using vertex_data_iterator = typename std::list<vertex>::iterator;
    using edge_data_iterator   = typename std::list<edge  >::iterator;
    using vertex_data_const_iterator = typename std::list<vertex>::const_iterator;
    using edge_data_const_iterator   = typename std::list<edge  >::const_iterator;

public:
    using vertex_data = v_data;
    using edge_data   = e_data;

    template <class it, class vtype>
    struct graph_iterator;

    using vertex_iterator = graph_iterator<vertex_data_const_iterator, const vertex_data>;
    using edge_iterator   = graph_iterator<edge_data_iterator        , edge_data>;

    using vertex_const_iterator = vertex_iterator;
    using edge_const_iterator   = graph_iterator<edge_data_const_iterator, const edge_data>;

private:
    struct edge {
        edge(const vertex_iterator& from, const vertex_iterator& to, edge_data data)
        : from_(from)
        , to_  (to)
        , data_(std::move(data))
        {}

        vertex_iterator from() const {
            return from_;
        }

        vertex_iterator to() const {
            return to_;
        }

        edge_data& data() {
            return data_;
        }

        const edge_data& data() const {
            return data_;
        }

    private:
        const vertex_iterator from_;
        const vertex_iterator to_;
        edge_data data_;
    };

    struct vertex {
        vertex(vertex_data data)
            : data_(std::move(data))
        {}

        const vertex_data& data() const {
            return data_;
        }
    private:
        vertex_data data_;
    };

    std::list<vertex> vertexes;
    std::unordered_map<vertex_data, vertex_data_iterator> vertex_to_iter;
    std::unordered_map<vertex_data, std::list<edge>     > vertex_to_adj_list;

    std::list<edge>& get_adj_list(vertex_iterator it) {
        return vertex_to_adj_list.find(*it)->second;
    }

    const std::list<edge>& get_adj_list(vertex_iterator it) const {
        return vertex_to_adj_list.find(*it)->second;
    }

    /* full specialization is forbidden in inner classes =(, bool is a workaround */
    template <bool a, class iter>
    struct get_const_iter {
        using const_iter = iter;
    };

    template <bool a>
    struct get_const_iter<a, edge_data_iterator> {
        using const_iter = edge_data_const_iterator;
    };

    template <bool a>
    struct get_const_iter<a, vertex_data_iterator> {
        using const_iter = vertex_data_const_iterator;
    };

    template <bool a>
    struct get_const_iter<a, edge_iterator> {
        using const_iter = edge_const_iterator;
    };

public:
    template <class iterator, class v_type>
    class graph_iterator {
    private:
        using base = std::iterator<std::forward_iterator_tag, v_type>;
        using const_iterator = typename get_const_iter<true, iterator>::const_iter;

        iterator it;
        iterator end;
        std::function<bool(const_iterator)> filter;

        static constexpr bool is_edge_data = std::is_same<edge_data, v_type>::value;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = v_type;
        using difference_type   = typename base::difference_type;
        using reference         = typename base::reference;
        using pointer           = typename base::pointer;

        graph_iterator() {}

        graph_iterator(iterator it, iterator end,
                       std::function<bool(const_iterator)> f = [](const_iterator) {return true;})
            : it(std::move(it))
            , end(end)
            , filter(f)
        {}

        graph_iterator(const graph_iterator& it) = default;
        graph_iterator& operator =(const graph_iterator& it) = default;

        template <class non_const_iter>
        graph_iterator(non_const_iter& it)
            : it(it.it)
            , end(it.end)
            , filter(it.filter)
        {}

        graph_iterator(graph_iterator&& it) = default;

        value_type& operator *() {
            return it->data();
        }

        value_type& operator *() const {
            return it->data();
        }

        iterator& operator ->() {
            return it;
        }

        const iterator& operator ->() const {
            return it;
        }

        graph_iterator& operator++() {
            ++it;
            while ((it != end) && !filter(  it  )) ++it;
            return *this;
        }

        graph_iterator operator++(int) {
            graph_iterator tmp = *this;
            ++it;
            while ((it != end) && !filter(  it  )) ++it;
            return tmp;
        }

        bool operator ==(const graph_iterator& other) {
            return it == other.it;
        }

        bool operator !=(const graph_iterator& other) {
            return !(*this == other);
        }

        template <class = std::enable_if<is_edge_data>>
        vertex_iterator from() const {
            return it->from();
        }

        template <class = std::enable_if<is_edge_data>>
        vertex_iterator to() const {
            return it->to();
        }

        value_type& data() {
            return it->data();
        }

        value_type& data() const {
            return it->data();
        }

        iterator get_inner() {
            return it;
        }

        friend class graph;
    };

    graph() {}
    graph(const graph& other) 			  = default;
    graph(graph&& other) 				  = default;
    graph& operator= (const graph& other) = default;
    graph& operator= (graph&& other)      = default;

    vertex_iterator add_vertex(vertex_data vdata) {
        vertexes.push_back(vertex(std::move(vdata)));

        auto vit = --vertexes.end();
        vertex_to_iter.insert({vdata, vit});
        vertex_to_adj_list.insert({vdata, std::list<edge>()});

        return vertex_iterator(vit, vertexes.end());
    }

    edge_iterator add_edge(vertex_iterator from, vertex_iterator to, edge_data edata) {
        std::list<edge>& adj_list = get_adj_list(from);
        adj_list.push_back({from, to, edata});

        return edge_iterator(--adj_list.end(), adj_list.end());
    }

    void remove_vertex(vertex_iterator vit) {
        std::list<edge_iterator> edges_to_remove;

        for (auto vertex = vertex_begin(); vertex != vertex_end(); ++vertex) {
            for (auto edge = edge_begin(vertex); edge != edge_end(vertex); ++edge) {
                auto from = edge.from();
                auto to   = edge.to();
                if (from == vit || to == vit) {
                    edges_to_remove.push_back(edge);
                }
            }
        }

        for (auto edge : edges_to_remove) {
            remove_edge(edge);
        }

        vertex_to_iter.erase(*vit);
        vertex_to_adj_list.erase(*vit);
        vertexes.erase(vit.get_inner());
    }

    void remove_edge(edge_iterator eit) {
        auto vit = eit->from();
        std::list<edge>& adj_list = get_adj_list(vit);
        adj_list.erase(eit.get_inner());
    }

    vertex_iterator find_vertex(const vertex_data& vdata) {
        auto it = vertex_to_iter.find(vdata);
        if (it == vertex_to_iter.end()) {
            return vertex_iterator(vertexes.end(), vertexes.end());
        }

        return vertex_iterator(it->second, vertexes.end());
    }

    vertex_const_iterator find_vertex(const vertex_data& vdata) const {
        auto it = vertex_to_iter.find(vdata);
        if (it == vertex_to_iter.end()) {
            return vertex_const_iterator(vertexes.end(), vertexes.end());
        }

        return vertex_const_iterator(it->second, vertexes.end());
    }

    edge_iterator find_edge(vertex_iterator from, vertex_iterator to) {
        auto end  = edge_end(from);
        auto edge = edge_begin(from);

        for (; edge != end; ++edge) {
            if (edge->to() == to) {
                break;
            }
        }

        return edge;
    }

    edge_const_iterator find_edge(vertex_const_iterator from, vertex_const_iterator to) const {
        auto end  = edge_end(from);
        auto edge = edge_begin(from);

        for (; edge != end; ++edge) {
            if (edge->to() == to) {
                break;
            }
        }

        return edge;
    }

    vertex_iterator vertex_begin() {
        return vertex_iterator(vertexes.begin(), vertexes.end());
    }

    vertex_const_iterator vertex_begin() const {
        return vertex_const_iterator(vertexes.begin(), vertexes.end());
    }

    vertex_iterator vertex_end() {
        return vertex_iterator(vertexes.end(), vertexes.end());
    }

    vertex_iterator vertex_end() const {
        return vertex_iterator(vertexes.end(), vertexes.end());
    }

    edge_iterator edge_begin(vertex_iterator from) {
        std::list<edge>& adj_list = get_adj_list(from);
        return edge_iterator(adj_list.begin(), adj_list.end());
    }

    edge_const_iterator edge_begin(vertex_const_iterator from) const {
        const std::list<edge>& adj_list = get_adj_list(from);
        return edge_const_iterator(adj_list.begin(), adj_list.end());
    }

    edge_iterator edge_end(vertex_iterator from) {
        std::list<edge>& adj_list = get_adj_list(from);
        return edge_iterator(adj_list.end(), adj_list.end());
    }

    edge_const_iterator edge_end(vertex_const_iterator from) const {
        const std::list<edge>& adj_list = get_adj_list(from);
        return edge_const_iterator(adj_list.end(), adj_list.end());
    }
};

} // namespace au
