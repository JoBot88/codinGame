#ifndef JOHNSON
#define JOHNSON

#include "preprocess_generic.hpp"
#include "bellman-ford_generic.hpp"

#include <vector>
#include <iostream>

/*
    This module provides an implementation of the Johnson's algorithm for computing
    all shortest-paths in a graph that does not contain any negative-weight cycle
*/
template<typename Tvertex, typename Tweight>
std::vector<Tweight> reweight(graph<Tvertex, Tweight>& my_graph);
/*
    Aim : compute new weights for the graph in order to then apply Dijkstra's algorithm
    Args : the graph
    Return value : none
    Complexity : O(nm)    (reduces to one invocation of Bellman-Ford algorithm)
*/

template<typename Tvertex, typename Tweight>
std::vector<Tweight> reweight(graph<Tvertex, Tweight>& my_graph){
    
    my_graph.num_vertices_++;        // we add a fictitious vertex

    for(size_t i = 0; i < my_graph.num_vertices_-1; i++)
        my_graph.edges_.push_back({my_graph.num_vertices_-1, i, 0});        // add fictitious edges from fictitious vertex
                                                                            // to every other vertex with weight 0

    std::vector<Tweight> new_weights = bellman_ford(my_graph, my_graph.num_vertices_-1); // apply bellman-ford on the graph using the fictitious vertex
                                                                                        // as the source

    // apply for each edge and then run n*dijkstra
 
    return new_weights;
}

#endif