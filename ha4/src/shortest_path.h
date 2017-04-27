#pragma once

#include <limits>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace au {

// returns true, if path is found, false otherwise
template<class graph, class edge_len, class path_visitor>
bool find_shortest_path(
    graph const& g,
	typename graph::vertex_const_iterator from,
	typename graph::vertex_const_iterator to,
	edge_len && len_functor,
    path_visitor&& visitor) {

    const size_t VERTEX_COUNT = std::distance(g.vertex_cbegin(), g.vertex_cend());
    const double MAX_DOUBLE   = std::numeric_limits<double>::max();
    using vertex_data = typename graph::vertex_data;

    std::unordered_map<vertex_data, double>      dist    (VERTEX_COUNT);
    std::unordered_map<vertex_data, vertex_data> ancestor(VERTEX_COUNT);
    std::unordered_set<vertex_data>              visited (VERTEX_COUNT);

    for (auto v = g.vertex_cbegin(); v != g.vertex_cend(); ++v) {
        dist[v] = MAX_DOUBLE;
        visited.insert(*from);
    }

    ancestor[*from] = *from;

    auto cur_vertex = from;
    while (visited.find(*to) == visited.end()) {
        for (auto e = g.edge_cbegin(cur_vertex); e != g.edge_cend(cur_vertex); ++e) {
            double new_length = dist[e->from()] + len_functor(e);
            if (visited.find(e->to()) == visited.end() && new_length < dist[e->to()]) {
                dist[e->to()] = new_length;
            }

            double max_dist  = MAX_DOUBLE;
            auto prev_vertex = cur_vertex;
            for (auto v = g.vertex_cbegin(); v != g.vertex_cend(); ++v) {
                if (visited.find(v) == visited.end() && dist[v] < max_dist) {
                    cur_vertex = v;
                }
            }

            visited.insert(*cur_vertex);
            ancestor[*cur_vertex] = *prev_vertex;
        }
    }

    bool is_connected_graph = ancestor.find(to) != ancestor.end();
    if (!is_connected_graph) {
        return false;
    }

    cur_vertex = to;
    while (*cur_vertex != ancestor[*cur_vertex]) {
        visitor(g.find_edge(*cur_vertex, ancestor[*cur_vertex]));
        cur_vertex = g.find_vertex(ancestor[*cur_vertex]);
    }

    return true;
}

} // namespace au
