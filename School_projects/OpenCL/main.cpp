#include "preprocess_generic.hpp"
#include "bellman-ford_generic.hpp"
#include "johnson_generic.hpp"
#include "heap_generic.hpp"

#include <iostream>

int main(){
	
    graph<long unsigned int, double> my_graph = parse_graph<long unsigned int, double>("../Tests/test1.txt");

    std::vector<double> n_weights = reweight<long unsigned int, double> (my_graph);

    for(auto & weight: n_weights)
        std::cout << weight << std::endl;

    std::vector<double> heap = create_heap<double>();

    return 0;

}