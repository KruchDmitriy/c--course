#include <iostream>

#include "graph.h"

int main() {
	using namespace std;

	cout << "hi" << endl;

	au::graph<int, int> g;
	auto it1 = g.add_vertex(1);
	auto it2 = g.add_vertex(2);
	auto it3 = g.add_vertex(3);
	auto it4 = g.add_vertex(4);

	cout << *it1 << endl;
	cout << *it2 << endl;
	g.edge_begin(it1);
	g.edge_end(it1);

    auto eit1 = g.add_edge(it1, it2, 12);
	g.add_edge(it1, it3, 13);
	g.add_edge(it1, it4, 14);

    eit1->from();
    eit1->to();
    eit1.data();
    it1.data();
    it1->first;
//     cout << *eit1 << endl;
//     g.remove_edge(eit1);

     g.remove_vertex(it2);

	for (auto edge = g.edge_begin(it1); edge != g.edge_end(it1); ++edge) {
		cout << *edge << endl;
	}

     auto eit = g.find_edge(it1, it3);
     // cout << *eit << endl;

     g.remove_edge(eit);

     for (auto edge = g.edge_begin(it1); edge != g.edge_end(it1); ++edge) {
        cout << *edge << endl;
     }

	return 0;
}

// #include <iostream>
// #include "graph.h"
// #include "assert.h"
// #include "vector"

// using namespace std;

// void print(auto begin, auto end) {
//     for(auto i = begin; i != end; i++) {
//         cout<<*i<<" ";
//     }
//     cout<<endl;
// }

// // void test_add() {
// //     au::graph<int, int> g;

// //     auto vertex_1 = g.add_vertex (1);
// //     auto vertex_2 = g.add_vertex (2);
// //     auto vertex_3 = g.add_vertex (3);

// //     auto edge_1_2 = g.add_edge (vertex_1, vertex_2, 12);
// //     auto edge_1_3 = g.add_edge (vertex_1, vertex_3, 13);
// //     auto edge_2_3 = g.add_edge (vertex_2, vertex_3, 23);

// // //    cout<<*edge_1_2<<" " <<*edge_1_3<<" "<<*edge_2_3<<endl;

// //     auto vertex_begin = g.vertex_begin ();
// //     auto vertex_end = g.vertex_end ();

// //     // auto it_vertex = vertex_begin;
// //     // for (size_t i = 1; it_vertex!= vertex_end; i++, it_vertex++ ) {
// //     //     assert( i == *it_vertex);
// //     // }

// //     auto edge_begin = g.edge_begin (vertex_1);
// //     auto edge_end = g.edge_end (vertex_1);


// //     std::vector<int> edge = {13, 12};
// //     auto it_edge = edge_begin;
// //     for (size_t i = 0; i< edge.size (); i++, it_edge++) {

// // //        cout<<edge[i] << "==" <<*it_edge<<" ";
// //         assert (edge[i] == *it_edge);
// //     }

// // }

// void test_remove_edge() {
//     au::graph<int, int> g;

//     auto vertex_1 = g.add_vertex (1);
//     auto vertex_2 = g.add_vertex (2);
//     auto vertex_3 = g.add_vertex (3);

//     auto edge_1_2 = g.add_edge (vertex_1, vertex_2, 12);
//     auto edge_1_3 = g.add_edge (vertex_1, vertex_3, 13);
//     g.add_edge (vertex_2, vertex_3, 23);

// //    cout<<*edge_1_2<<" " <<*edge_1_3<<" "<<*edge_2_3<<endl;

//     g.remove_edge (edge_1_2);

//     auto edge_begin = g.edge_begin (vertex_2);
// //    std::cout<<*edge_begin<<" ";
//     auto edge_end = g.edge_end (vertex_2);

//     std::vector<int> edge = {23};
//     for (size_t i = 0; i< edge.size ();i++, edge_begin++) {
//         assert (edge[i] == *edge_begin);
//     }

//     edge = {13, 23};
//     edge_begin = g.edge_begin (vertex_1);
//     edge_end = g.edge_end (vertex_1);
//     for (size_t i = 0; i < edge.size (); i++) {
//    		std::cout << *edge_begin << " " << endl;
//         assert (edge[i] == *edge_begin);
//         edge_begin++;
//     }
//     // assert(edge_begin == edge_end);

//     edge = {23};
//     edge_begin = g.edge_begin (vertex_2);
//     edge_end = g.edge_end (vertex_2);
//     for (size_t i = 0; i< edge.size ();i++, edge_begin++) {
//         assert (edge[i] == *edge_begin);
//     }

//     g.remove_edge(edge_1_3);
//     edge_begin = g.edge_begin (vertex_1);
//     edge_end = g.edge_end (vertex_1);

// 	cout<< "vetrex " << *edge_begin << endl;
//     assert(edge_begin == edge_end);

// }

// int main()
// {

//     using namespace au;

//     // test_add ();

//     test_remove_edge ();

//     au::graph<int, int>* g = new au::graph<int, int>();
//     auto iter_0 = g->add_vertex(0);
//     auto iter_1 = g->add_vertex (1);

//     auto edge_iter = g->add_edge (iter_0, iter_1,10);
//     auto end = g->edge_end (iter_1);

//     for (auto i = edge_iter; i!=end; i++) {
//         std::cout<<*edge_iter<<endl;
//     }
//     auto iter_2 = g->add_vertex (2);
//     auto edge_iter_2 = g->add_edge (iter_0, iter_2, 20);

//     end = g->edge_end (iter_2);
//     print (edge_iter_2, end);

//     auto begin_vertex = g->vertex_begin ();
//     auto end_vertex = g->vertex_end ();

//     print (begin_vertex, end_vertex);

//     std::cout<<std::endl;

// //    g->remove_edge (edge_iter_2);

//     auto iter = g->find_vertex (1);
//     end_vertex = g->vertex_end ();
//     print (iter, end_vertex);


// //    g->remove_vertex (iter_0);

//     auto it = g->find_edge (iter_0, iter_1);

//     return 0;
// }
