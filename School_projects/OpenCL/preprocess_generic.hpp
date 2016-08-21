#ifndef PREPROCESS
#define PREPROCESS

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

/*
    This module provides a couple function to parse data from a file in order 
    to later apply Bellman's or Dijkstra's algorithm
*/

template<typename Tvertex, typename Tweight>
struct edge {
    Tvertex tail_;
    Tvertex head_;
    Tweight weight_;
} ;

template<typename Tvertex, typename Tweight>
struct graph {
    Tvertex num_vertices_;
    std::vector<edge<Tvertex, Tweight>> edges_;
} ;

template<typename Tvertex, typename Tweight>
graph<Tvertex, Tweight> parse_graph(const char* filename);
/*
    Aim : create a graph representation from a file
    Args : a string that is the file name
    Return value : a vector of edges and a vector of vertices, that is, a graph
    Complexity : O(m)
*/

template<typename Tvertex, typename Tweight>
bool check_negative_weights(std::vector<Tvertex, Tweight>& edges_);
/*
    Aim : check whether or not there are negative-weight edges in the graph
    Args : a collection of edges
    Return value : true if there is at least one negative-weight edge, false otherwise
    Complexity : O(m)
*/




template<typename Tvertex, typename Tweight>
graph<Tvertex, Tweight> parse_graph(const char* filename){
    /*
        This function assumes the format of the file to be as followed :

        num_vertices num_edges
        tail_1 head_1 weight_1
        tail_2 head_2 weight_2
        .
        .
        .
        tail_m head_m weight_m

        It also assumes that vertices are represented as positive integers from 0 to num_vertices-1
    */
    graph<Tvertex, Tweight> new_graph;

    std::ifstream file(filename, std::ios::in);

    if(file){                                       // file opened correctly

        std::string num_vertices, num_edges_;
        std::string line;
        getline(file, line);
        std::istringstream isstream(line);

        isstream >> num_vertices >> num_edges_;       // initialize the number of vertices
        new_graph.num_vertices_ = atoi(num_vertices.c_str());
        new_graph.edges_.resize(atoi(num_edges_.c_str()));          // changes the dimension of the vector of edges so every edge fits into it
        
        
        size_t i(0);

        while(getline(file, line)){
            std::istringstream sstream(line);
            edge<Tvertex, Tweight> new_edge;
            sstream >> new_edge.tail_ >> new_edge.head_ >> new_edge.weight_;
            new_graph.edges_[i] = new_edge;
            i++;
        }
    }
    else{
        std::cerr << "Sorry the file " << filename << " cannot be opened" << std::endl;
        exit(EXIT_FAILURE);
    }

    return new_graph;

}

template<typename Tvertex, typename Tweight>
bool check_negative_weights(std::vector<Tvertex, Tweight>& edges_){
    for(auto & edge: edges_){
        if(edge.weight_ < 0)
            return true;
    }
    return false;
}

#endif
