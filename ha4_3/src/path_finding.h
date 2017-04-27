#pragma once

#include <limits>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <cmath>

namespace au {

template<class graph, class edge_len, class path_visitor>
bool find_shortest_path(
    graph const& g,
	typename graph::vertex_const_iterator from,
	typename graph::vertex_const_iterator to,
	edge_len && len_functor,
    path_visitor&& visitor) {

    const size_t VERTEX_COUNT = std::distance(g.vertex_begin(), g.vertex_end());
    const double MAX_DOUBLE   = std::numeric_limits<double>::max();
    using vertex_data = typename std::remove_const<typename graph::vertex_data>::type;

    std::unordered_map<vertex_data, double>      dist(VERTEX_COUNT);
    std::unordered_map<vertex_data, vertex_data> ancestor(VERTEX_COUNT);
    std::unordered_set<vertex_data>              visited (VERTEX_COUNT);

    for (auto v = g.vertex_begin(); v != g.vertex_end(); ++v) {
        ancestor[*v] = *v;
        dist[*v]     = MAX_DOUBLE;
    }

    dist[*from] = 0.;

    auto cur_vertex = from;
    size_t iteration = 0;
    while (visited.find(*to) == visited.end() && iteration < VERTEX_COUNT) {
        auto min_vertex = g.vertex_begin();
        double min_dist = MAX_DOUBLE;
        for (auto v = g.vertex_begin(); v != g.vertex_end(); ++v) {
            if (visited.find(*v) == visited.end() &&
                dist[*v] < min_dist)
            {
                min_dist = dist[*v];
                min_vertex = v;
            }
        }

        cur_vertex = min_vertex;
        if (dist[*min_vertex] == MAX_DOUBLE) {
            break;
        }
        visited.insert(*min_vertex);

        for (auto e = g.edge_begin(cur_vertex); e != g.edge_end(cur_vertex); ++e) {
            double new_length = dist[*e.from()] + len_functor(*e);

            if (visited.find(*e.to()) == visited.end() &&
                new_length < dist[*e.to()])
            {
                dist[*e.to()] = new_length;
                ancestor[*e.to()] = *cur_vertex;
            }
        }

        iteration++;
    }

    if (dist[*to] == MAX_DOUBLE) {
        return false;
    }

    std::list<typename graph::edge_const_iterator> path;
    cur_vertex = to;
    while (*cur_vertex != ancestor[*cur_vertex]) {
        auto anc = g.find_vertex(ancestor[*cur_vertex]);
        path.push_front(g.find_edge(anc, cur_vertex));
        cur_vertex = anc;
    }

    for (auto edge : path) {
        visitor(edge);
    }

    return true;
}

} // namespace au
