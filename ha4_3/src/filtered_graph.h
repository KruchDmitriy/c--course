#pragma once

#include "graph.h"
#include <functional>

namespace au {
    template<class graph, class vertex_filter, class edge_filter>
    class filtered_graph {
    private:
        const graph& g;
        vertex_filter v_filter;
        edge_filter   e_filter;

        template <class iter, class value_type>
        using filtered_iterator = typename graph::template graph_iterator<iter, value_type>;

    public:
        using vertex_iterator = filtered_iterator<
                                    typename graph::vertex_const_iterator,
                                    const typename graph::vertex_data
                                >;
        using edge_iterator   = filtered_iterator<
                                    typename graph::edge_const_iterator,
                                    const typename graph::edge_data
                                >;

        using vertex_const_iterator = vertex_iterator;
        using edge_const_iterator   = edge_iterator;

        using vertex_data = const typename graph::vertex_data;
        using edge_data   = const typename graph::edge_data;

    private:
        std::function<bool(typename graph::vertex_const_iterator)> vertex_iter_filter =
            [&](typename graph::vertex_const_iterator it) {
                return v_filter(*it);
            };

        std::function<bool(typename graph::edge_const_iterator)> edge_iter_filter =
            [&](typename graph::edge_const_iterator it) {
                return e_filter(*it) && v_filter(*it.from()) && v_filter(*it.to());
            };

    public:
        filtered_graph(graph const& g, vertex_filter v_filter, edge_filter e_filter)
            : g(g)
            , v_filter(std::move(v_filter))
            , e_filter(std::move(e_filter))
        {}

        vertex_iterator find_vertex(typename graph::vertex_data const& vdata) const {
            auto it = g.find_vertex(vdata);

            if (it == g.vertex_end()) {
                return vertex_end();
            }

            return v_filter(*it) ? vertex_iterator(it, g.vertex_end()) : vertex_end();
        }

        edge_iterator   find_edge  (vertex_iterator from, vertex_iterator to) const {
            auto it = g.find_edge(from.get_inner(), to.get_inner());

            if (it == g.edge_end(from.get_inner())) {
                return edge_end(from);
            }

            return e_filter(*it) ? edge_iterator(it, g.edge_end(from.get_inner())) : edge_end(from);
        }

        vertex_iterator vertex_begin() const {
            for (auto v = g.vertex_begin(); v != g.vertex_end(); ++v) {
                if (vertex_iter_filter(v)) {
                    return vertex_iterator(v, g.vertex_end(), vertex_iter_filter);
                }
            }

            return vertex_end();
        }

        vertex_iterator vertex_end  () const {
            return vertex_iterator(g.vertex_end(), g.vertex_end(), vertex_iter_filter);
        }

        edge_iterator edge_begin(vertex_iterator from) const {
            for (auto e = g.edge_begin(from.get_inner()); e != g.edge_end(from.get_inner()); ++e) {
                if (edge_iter_filter(e)) {
                    return edge_iterator(e, g.edge_end(from.get_inner()), edge_iter_filter);
                }
            }

            return edge_end(from);
        }

        edge_iterator edge_end  (vertex_iterator from) const {
            return edge_iterator(g.edge_end(from.get_inner()),
                                 g.edge_end(from.get_inner()),
                                 edge_iter_filter);
        }
	};

} // namespace au
