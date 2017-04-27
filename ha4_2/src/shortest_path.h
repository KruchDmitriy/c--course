#pragma once

namespace au {

// returns true, if path is found, false otherwise
template<class graph, class edge_len, class path_visitor>
bool find_shortest_path(
	graph const&,
	typename graph::vertex_const_iterator from,
	typename graph::vertex_const_iterator to,
	edge_len && len_functor,
	path_visitor&& visitor);

} // namespace au
