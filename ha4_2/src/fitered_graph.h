#pragma once

namespace au {

	template<class graph, class vertex_filter, class edge_filter>
	struct filtered_graph {
		using vertex_iterator = impl_defined;
		using edge_iterator = impl_defined;

		filtered_graph(
			graph const&, // do not make copy of graph
			/* filters either by l-value, or by reference */);

		// only const getters
		vertex_iterator find_vertex(typename graph::vertex_data const&) const;
		edge_iterator   find_edge  (vertex_iterator from, vertex_iterator to) const;

		vertex_iterator vertex_begin() const;
		vertex_iterator vertex_end  () const;

		edge_iterator edge_begin(vertex_iterator from) const;
		edge_iterator edge_end  (vertex_iterator from) const;
	};

} // namespace au
