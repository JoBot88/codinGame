#ifndef DIJKSTRA
#define DIJKSTRA

#include <iostream>
#include <vector>
#include "heap_generic.hpp"
#include "preprocess_generic.hpp"
#include <limits>

template<typename Tnode, typename Tweight>
struct dijkstra_pair {
    Tnode pivot;
    Tweight key;

    bool operator<(dijkstra_pair &other_pair){
        return key < other_pair.key;
    }

    bool operator>(dijkstra_pair &other_pair){
        return key > other_pair.key; 
    }
};

template<typename Tnode, typename Tweight>
std::ostream& operator<<(std::ostream& out, dijkstra_pair<Tnode, Tweight>& pair){
    out << "Vertex : " << pair.pivot << ", key : " << pair.key << std::endl;
    return out;
}

template<typename Tnode, typename Tweight>
std::vector<Tweight> dijkstra(graph<Tnode, Tweight>& my_graph, const Tnode node, std::vector<bool> in_heap, std::vector<Tweight>& distances);
/*
    Aim : compute the shortest distances from source to every other vertex
    Args : the graph, represented as defined in preprocess.hpp and a source vertex
    Return value : a 1D array that contains the shortest distances
    Complexity : O(mlog(n))
*/

template<typename Tnode, typename Tweight>
std::vector<Tweight> initialize_distances(graph<Tnode, Tweight>& my_graph, const Tnode source);
/*
    Aim : initialize the shortest paths, a shortest path is equal to the value of the edge between source and vertex if it exists, 0 between source
          and source, infinity represented as numeric_limits<Tweight>::max() otherwise
    Args : a graph and a source node
    Return value : a 1D array containing all initialized distances
    Complexity : O(n)
*/

template<typename Tnode, typename Tweight>
Tweight compute_key(graph<Tnode, Tweight>& my_graph, const Tnode node);
/*
    Aim : compute the key to insert node in a heap
    Args : a graph and a node
    Return value : the value of the key
    Complexity : O(m)
*/

template<typename Tnode, typename Tweight>
std::vector<Tweight> dijkstra(graph<Tnode, Tweight>& my_graph, Tnode source){

    //to store our computed distances
    std::vector<Tweight> distances = initialize_distances(my_graph, source);

    std::vector<bool> in_heap(my_graph.num_vertices_, false);                  // to keep track of nodes in the heap
    std::vector<bool> processed(my_graph.num_vertices_, false);                // to keep track of processed nodes

    std::vector<dijkstra_pair<Tnode, Tweight>> heap = create_heap<dijkstra_pair<Tnode, Tweight>>();        // our amazing heap for BLAZINGLY FAST
                                                                                                           // implementation

    insert(heap, {source, 0});             // insert the source and mark it as "in the heap"
    in_heap[source] = true;

    while(size_heap(heap) != 1){

        dijkstra_pair<Tnode, Tweight> new_pair = pop(heap);
        in_heap[new_pair.pivot] = false;                             // old min is not in the heap anymore
        processed[new_pair.pivot] = true;                            // old min is now processed           
        distances[new_pair.pivot] = new_pair.key;                    // the old key is the permanent distance between source and associated node

        for(size_t i = 0; i < my_graph.edges_[new_pair.pivot].size(); i++){          // we check all outgoing edges of the new added vertex

            edge<Tnode, Tweight> current_edge = my_graph.edges_[new_pair.pivot][i];
            Tnode current_node = current_edge.head_;                                 // the head of an outgoing edge whose tail is the new added vertex
             
             // if already in the heap we update its key
            if(in_heap[current_node]){                                              

                Tweight old_key = delete_element(heap, current_node).key;
                Tweight new_key = std::min(old_key, distances[new_pair.pivot] + current_edge.weight_);
                insert(heap, {current_node, new_key});
            }

            else if(!processed[current_node]){   
          
                // otherwise we compute its key and insert it
                Tweight new_key = compute_key(my_graph, current_node, in_heap, distances);
                insert(heap, {current_node, new_key});
                
                in_heap[current_node] = true;
            }
        }
    }
    return distances;
}

template<typename Tnode, typename Tweight>
Tweight compute_key(graph<Tnode, Tweight>& my_graph, const Tnode node, std::vector<bool>& in_heap, std::vector<Tweight>& distances){

    Tweight current_key = distances[node];
    for(size_t i = 0; i < my_graph.num_vertices_; i++){
        for(size_t j = 0; j < my_graph.edges_[i].size(); j++){
            edge<Tnode, Tweight> current_edge = my_graph.edges_[i][j];
            if(current_edge.head_ == node and !in_heap[current_edge.tail_]){
                if(current_edge.weight_ + distances[current_edge.tail_] < current_key){
                    current_key = current_edge.weight_ + distances[current_edge.tail_];
                }
            }
        }
    }
    return current_key;
}

template<typename Tnode, typename Tweight>
std::vector<Tweight> initialize_distances(graph<Tnode, Tweight>& my_graph, const Tnode source){
    std::vector<Tweight> distances(my_graph.num_vertices_, std::numeric_limits<Tweight>::max());
    
    distances[source] = 0;
    for(size_t i = 0; i < my_graph.edges_[source].size(); i++)
        distances[my_graph.edges_[source][i].head_] = my_graph.edges_[source][i].weight_;
    
    return distances;
}

#endif
