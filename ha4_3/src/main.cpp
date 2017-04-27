#include <iostream>
#include <unordered_set>
#include <assert.h>
#include <vector>
#include <set>
#include <functional>

#include "graph.h"
#include "filtered_graph.h"
#include "path_finding.h"

au::graph<int, int> g;
std::set<int> test_vertex = { 0 , 1 , 2 , 3 , 4 };
std::vector<int> test_edges  = { 00, 10, 12, 13, 14, 34 };
std::vector<au::graph<int, int>::vertex_const_iterator> vert_it;

std::function<bool(int)> even_filter = [](int a){ return a % 2 == 0; };
au::filtered_graph<au::graph<int, int>,
                   decltype(even_filter),
                   decltype(even_filter)>
filtered_g(g, even_filter, even_filter);

void init_graph() {
    for (auto vertex : test_vertex) {
        vert_it.push_back(g.add_vertex(vertex));
    }

    g.add_edge(vert_it[0], vert_it[1], test_edges[0]);
    g.add_edge(vert_it[1], vert_it[0], test_edges[1]);
    g.add_edge(vert_it[1], vert_it[2], test_edges[2]);
    g.add_edge(vert_it[1], vert_it[3], test_edges[3]);
    g.add_edge(vert_it[1], vert_it[4], test_edges[4]);
    g.add_edge(vert_it[3], vert_it[4], test_edges[5]);
}

void test_add_remove_vertex() {
    init_graph();

    for (auto vertex : test_vertex) {
        assert(g.find_vertex(vertex) != g.vertex_end());
    }
    for (auto vertex : test_vertex) {
        assert(g.find_vertex(vertex)->data() == vertex);
    }
    assert(g.find_vertex(500) == g.vertex_end());

    for (auto vit : vert_it) {
        g.remove_vertex(vit);
    }

    vert_it.clear();
}

void test_add_remove_edge() {
    init_graph();

    auto eit1 = g.find_edge(vert_it[0], vert_it[1]);
    auto eit2 = g.find_edge(vert_it[1], vert_it[0]);
    auto eit3 = g.find_edge(vert_it[1], vert_it[2]);
    auto eit4 = g.find_edge(vert_it[1], vert_it[3]);
    auto eit5 = g.find_edge(vert_it[1], vert_it[4]);
    auto eit6 = g.find_edge(vert_it[3], vert_it[4]);

    assert(eit1->data() == test_edges[0]);
    assert(*eit1->from() == 0);
    assert(*eit1->to()   == 1);

    assert(eit2->data() == test_edges[1]);
    assert(*eit2->from() == 1);
    assert(*eit2->to()   == 0);

    assert(eit3->data() == test_edges[2]);
    assert(*eit3->from() == 1);
    assert(*eit3->to()   == 2);

    assert(eit4->data() == test_edges[3]);
    assert(*eit4->from() == 1);
    assert(*eit4->to()   == 3);

    assert(eit5->data() == test_edges[4]);
    assert(*eit5->from() == 1);
    assert(*eit5->to()   == 4);

    assert(eit6->data() == test_edges[5]);
    assert(*eit6->from() == 3);
    assert(*eit6->to()   == 4);

    g.remove_edge(eit1);
    g.remove_edge(eit2);
    g.remove_edge(eit3);
    g.remove_edge(eit4);
    g.remove_edge(eit5);
    g.remove_edge(eit6);
}

void test_simple() {
    using namespace std;
    au::graph<int, int> g2;

    auto it1 = g2.add_vertex(1);
    auto it2 = g2.add_vertex(2);
    auto it3 = g2.add_vertex(3);
    auto it4 = g2.add_vertex(4);

    cout << *it1 << endl;
    cout << *it2 << endl;
    cout << *it3 << endl;
    cout << *it4 << endl;
    g2.edge_begin(it1);
    g2.edge_end(it1);
    assert(g2.edge_begin(it1) == g2.edge_end(it1));

    auto eit1 = g2.add_edge(it1, it2, 12);
    g2.add_edge(it1, it3, 13);
    g2.add_edge(it1, it4, 14);

    for (auto edge = g2.edge_begin(it1); edge != g2.edge_end(it1); ++edge) {
        cout << *edge << endl;
    }

    eit1.from();
    eit1.to();
    eit1.data();
    it1.data();

    cout << *eit1 << endl;
    g2.remove_vertex(it2);

    for (auto edge = g2.edge_begin(it1); edge != g2.edge_end(it1); ++edge) {
        cout << *edge << endl;
    }
}

void test_filter_graph() {
    init_graph();

    for (auto vertex : test_vertex) {
        if (vertex % 2 == 0) {
            assert(filtered_g.find_vertex(vertex) != filtered_g.vertex_end());
        }
    }
    for (auto vertex : test_vertex) {
        if (vertex % 2 == 0)
            assert(filtered_g.find_vertex(vertex)->data() == vertex);
    }
    assert(filtered_g.find_vertex(500) == filtered_g.vertex_end());

    for (auto v = filtered_g.vertex_begin(); v != filtered_g.vertex_end(); ++v) {
        assert(*v % 2 == 0);
    }

    for (auto v = filtered_g.vertex_begin(); v != filtered_g.vertex_end(); ++v) {
        for (auto edge = filtered_g.edge_begin(v); edge != filtered_g.edge_end(v); ++edge) {
            std::cout << *edge << std::endl;
            assert(*edge % 2 == 0);
        }
    }
}

//int main() {
//    using namespace std;

//    test_add_remove_vertex();
//    test_add_remove_edge();
//    test_simple();
//    test_filter_graph();

//    return 0;
//}
