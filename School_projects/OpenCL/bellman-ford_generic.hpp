#ifndef BELLMAN_FORD 
#define BELLMAN_FORD

#include "preprocess_generic.hpp"
#include <vector>
#include <limits>
#include <iostream>
/*
    This module implements the Bellman-Ford algorithm

    For the complexity, n denotes the number of vertices of the graph, m the number of edges
*/
template<typename Tvertex, typename Tweight>
std::vector<Tweight> bellman_ford(graph<Tvertex, Tweight>& my_graph, Tvertex source);
/*
    Aim : compute the distances from source to every other vertex in my_graph 
    Args : the graph, represented as defined in preprocess.hpp, a source vertex and a 1d array to receive the results
    Return value : none
    Complexity : O(nm)
*/

template<typename Tvertex, typename Tweight>
std::vector<Tweight> bellman_ford(graph<Tvertex, Tweight>& my_graph, Tvertex source){

    //to store our computed distances
    std::vector<Tweight> distances(my_graph.num_vertices_, 0);
    
    // initialization
    for(size_t i = 0; i < my_graph.num_vertices_; i++){
        if(i != source)
            distances[i] = std::numeric_limits<double>::max();            // kind of the infinity
    }

    // relax edges repeatedly
    for(size_t j = 1; j < my_graph.num_vertices_; j++){
        bool changed_something = false;                  // to check whether or not this iteration induces any changes on the graph
        for(auto edge: my_graph.edges_){
            if(distances[edge.tail_] + edge.weight_ < distances[edge.head_]){
                distances[edge.head_] = distances[edge.tail_] + edge.weight_;
                changed_something = true;
            }
        }
        if(!changed_something)                           // if no changes at all, just terminate
            break;
    }

    // checking for negative-weight cycles
    for(auto edge: my_graph.edges_){
        if(distances[edge.tail_] + edge.weight_ < distances[edge.head_]){
            std::cerr << "The graph contains a negative-weight cycle ! It is impossible to apply Bellman-Ford's algorithm on this graph" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return distances;
}


#endif