#ifndef PREPROCESS
#define PREPROCESS

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

/*
    This module provides a couple function to parse data from a file in order 
    to later apply Bellman's or Dijkstra's algorithm

    Tnode must be an unsigned type
    Tweight must implement operator<
*/

template<typename Tnode, typename Tweight>
struct edge {
    Tnode tail_;
    Tnode head_;
    Tweight weight_;
} ;

template<typename Tnode, typename Tweight>
std::ostream& operator<<(std::ostream& out, edge<Tnode, Tweight>& edge_){
    out << edge_.tail_ << " - " << edge_.head_ << " : " << edge_.weight_ << std::endl;
    return out;
}


template<typename Tnode, typename Tweight>
struct graph {
    Tnode num_vertices_;
    std::vector<std::vector<edge<Tnode, Tweight>>> edges_;
} ;

template<typename Tnode, typename Tweight>
void affiche(graph<Tnode, Tweight>& my_graph, std::ostream& out){
    for(size_t i= 0;i < my_graph.num_vertices_;i++){
        for(size_t j=0;j<my_graph.edges_[i].size();j++){
            std::cout << my_graph.edges_[i][j] << std::endl;
        }
    }
}

template<typename Tnode, typename Tweight>
std::ostream& operator<<(std::ostream& out, graph<Tnode, Tweight>& graph_){
    affiche(graph_, out);
    return out;
}

template<typename Tnode, typename Tweight>
graph<Tnode, Tweight> parse_graph(const char* filename);
/*
    Aim : create a graph representation from a file
    Args : a string that is the file name
    Return value : a vector of edges and a vector of vertices, that is, a graph
    Complexity : O(m)
*/

template<typename Tnode, typename Tweight>
bool check_negative_weights(graph<Tnode, Tweight>& my_graph);
/*
    Aim : check whether or not there are negative-weight edges in the graph
    Args : a graph
    Return value : true if there is at least one negative-weight edge, false otherwise
    Complexity : O(m)
*/




template<typename Tnode, typename Tweight>
graph<Tnode, Tweight> parse_graph(const char* filename){
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
    graph<Tnode, Tweight> new_graph;

    std::ifstream file(filename, std::ios::in);

    if(file){                                       // file opened correctly

        std::string num_vertices, num_edges_;
        std::string line;
        getline(file, line);
        std::istringstream isstream(line);

        isstream >> num_vertices >> num_edges_;       // initialize the number of vertices
        new_graph.num_vertices_ = atoi(num_vertices.c_str());
        new_graph.edges_.resize(new_graph.num_vertices_);       // changes the dimension of the vector of edges so every edge fits into it

        while(getline(file, line)){
            std::istringstream sstream(line);
            edge<Tnode, Tweight> new_edge;
            sstream >> new_edge.tail_ >> new_edge.head_ >> new_edge.weight_;
            new_graph.edges_[new_edge.tail_].push_back(new_edge);
        }
    }
    else{
        std::cerr << "Sorry the file " << filename << " cannot be opened" << std::endl;
        exit(EXIT_FAILURE);
    }

    return new_graph;

}

template<typename Tnode, typename Tweight>
bool check_negative_weights(const graph<Tnode, Tweight>& my_graph){
    for(size_t i = 0; i < my_graph.num_vertices_; i++){
        for(auto & edge: my_graph.edges_[i])
        if(edge.weight_ < 0)
            return true;
    }
    return false;
}

#endif
