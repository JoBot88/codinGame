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
    Aim : compute new weights for the graph in order to later apply Dijkstra's algorithm
    Args : the graph
    Return value : none
    Complexity : O(nm)    (reduces to one invocation of Bellman-Ford algorithm)
*/

template<typename Tvertex, typename Tweight>
std::vector<std::vector<Tweight>> johnson(graph<Tvertex, Tweight>& my_graph);
/*
    Aim : compute all shortest-paths between all pairs of vertices of the graph
    Args : a graph
    Return value : a 2D vector that contains all shortest-paths
    Complexity : O(nmlog(n))
*/

template<typename Tvertex, typename Tweight>
std::vector<Tweight> reweight(graph<Tvertex, Tweight>& my_graph){
    
    graph<Tvertex, Tweight> copy_graph = my_graph;           // so that we do not consider all fictitious edges after this function
    copy_graph.num_vertices_++;                              // we add a fictitious vertex

    copy_graph.edges_.resize(copy_graph.num_vertices_);    

    for(size_t i = 0; i < copy_graph.num_vertices_-1; i++){
        edge<Tvertex, Tweight> new_fictitious_edge = {copy_graph.num_vertices_-1, i, 0};
        copy_graph.edges_[copy_graph.num_vertices_-1].push_back(new_fictitious_edge);        // add fictitious edges from fictitious vertex
                                                                                             // to every other vertex with weight 0
    }

    std::vector<Tweight> new_weights = bellman_ford(copy_graph, copy_graph.num_vertices_-1); // apply bellman-ford on the graph using the fictitious vertex
                                                                                        // as the source

    for(size_t i = 0; i < my_graph.num_vertices_; i++){
        for(size_t j = 0; j < my_graph.edges_[i].size(); j++){
            edge<Tvertex, Tweight> current_edge = my_graph.edges_[i][j];
            my_graph.edges_[i][j].weight_ += new_weights[current_edge.tail_] - new_weights[current_edge.head_]; // update the edges
        }
    }
    
    return new_weights;
}

template<typename Tvertex, typename Tweight>
std::vector<std::vector<Tweight>> johnson(graph<Tvertex, Tweight>& my_graph){

    std::vector<std::vector<Tweight>> distances(my_graph.num_vertices_, std::vector<Tweight>(my_graph.num_vertices_, std::numeric_limits<Tweight>::max()));

    if(check_negative_weights(my_graph)){                        // there is at least one negative_weight edge and we cannot apply dijkstra directly

        std::vector<Tweight> weights = reweight(my_graph);

        for(size_t i = 0; i < my_graph.num_vertices_; ++i){
            for(size_t j = 0; j < my_graph.num_vertices_; ++j){
                std::vector<Tweight> shortest_paths_from_i = dijkstra(my_graph, i);
                distances[i][j] = shortest_paths_from_i[j] + weights[j] - weights[i];
            }
        }
    }

    // we don't need to use the reweighting technique we can just proceed
    else {
        for(size_t i = 0; i < my_graph.num_vertices_; ++i){
            for(size_t j = 0; j < my_graph.num_vertices_; ++j){
                std::vector<Tweight> shortest_paths_from_i = dijkstra(my_graph, i);
                distances[i][j] = shortest_paths_from_i[j];
            }
        }
    }
   
    return distances;
}

#endif