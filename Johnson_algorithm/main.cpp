#include "preprocess_generic.hpp"
#include "bellman-ford_generic.hpp"
#include "johnson_generic.hpp"
#include "heap_generic.hpp"
#include "dijkstra_generic.hpp"

#include <iostream>

int main(){

    graph<long unsigned int, double> my_graph = parse_graph<long unsigned int, double>("../Tests/test1.txt");

    std::vector<std::vector<double>> distances = johnson<long unsigned int, double>(my_graph);
    
    for(size_t i = 0; i < my_graph.num_vertices_; ++i){
        std::cout << "shortest_paths from vertex " << i << " to all other vertices" << std::endl;

        for(size_t j = 0; j < my_graph.num_vertices_; ++j)
            std::cout << "distance between " << i << " and " << j << " : " << distances[i][j] << std::endl;
        std::cout << "- - - - - - - - - - - - -" << std::endl;
    }
    
    return 0;

}