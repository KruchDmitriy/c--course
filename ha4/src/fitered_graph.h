#pragma once

#include "graph.h"

namespace au {
    template<class graph, class vertex_filter, class edge_filter>
    class filtered_graph {
    private:
        const graph& g;
        vertex_filter v_filter;
        edge_filter   e_filter;

        template <class iter, class data>
        using filtered_iterator = typename graph::template graph_iterator<iter, data>;

    public:
        using vertex_iterator = filtered_iterator<
                                    typename graph::const_vertex_iterator,
                                    typename graph::vertex_data
                                >;
        using edge_iterator   = filtered_iterator<
                                    typename graph::const_edge_iterator,
                                    typename graph::edge_data
                                >;

    public:
        filtered_graph(graph const& g, vertex_filter v_filter, edge_filter e_filter)
            : g(g)
            , v_filter(std::move(v_filter))
            , e_filter(std::move(e_filter))
        {}

        vertex_iterator find_vertex(typename graph::vertex_data const& vdata) const {
            auto it = g.find_vertex(vdata);

            if (it == g.vertex_cend()) {
                return vertex_end();
            }

            return v_filter(*it) ? vertex_iterator(it, g.vertex_cend()) : vertex_end();
        }

        edge_iterator   find_edge  (vertex_iterator from, vertex_iterator to) const {
            auto it = g.find_edge(from.get_inner(), to.get_inner());

            if (it == g.edge_cend(from.get_inner())) {
                return edge_end(from);
            }

            return e_filter(*it) ? edge_iterator(it, g.edge_cend(from.get_inner())) : edge_end(from);
        }

        vertex_iterator vertex_begin() const {
            for (auto v = g.vertex_cbegin(); v != g.vertex_cend(); ++v) {
                if (v_filter(*v)) {
                    return vertex_iterator(v, g.vertex_cend(), v_filter);
                }
            }

            return vertex_end();
        }

        vertex_iterator vertex_end  () const {
            return vertex_iterator(g.vertex_cend(), g.vertex_cend(), v_filter);
        }

        edge_iterator edge_begin(vertex_iterator from) const {
            for (auto e = g.edge_cbegin(from.get_inner()); e != g.edge_cend(from.get_inner()); ++e) {
                if (e_filter(*e)) {
                    return edge_iterator(e, g.edge_cend(from.get_inner()), e_filter);
                }
            }

            return edge_end(from);
        }

        edge_iterator edge_end  (vertex_iterator from) const {
            return edge_iterator(g.edge_cend(from.get_inner()),
                                 g.edge_cend(from.get_inner()),
                                 e_filter);
        }
	};

} // namespace au
