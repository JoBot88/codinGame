#ifndef HEAP_T
#define HEAP_T

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility> // swap
#include <algorithm> // min
#include <cmath>

/*
    This library provides the implementation of a min-binary-heap or a max-binary-heap

    The keys provided for the heap could have any type for which an order is explicitely defined

    When converting a regular vector to a heap, one can instanciate 
    heapify<Tkey, Tnode> with any unsigned integral type for Tnode in order to optimize some space to store indices
    
    In all complexities n denotes the size of the heap
*/

template<typename Tkey>
std::vector<Tkey> create_heap(void);
/* 
    Aim : to create a heap with one element that is not used and equal to 0 -> allow to perform bit-operations 
                                                                                like << and >> in order to improve performance
    Args : None
    Return value : an empty heap
    Complexity : O(1)
*/ 

template<typename Tkey, typename Tnode>
void bubble_up(std::vector<Tkey>& my_heap, const Tnode n); 
/* 
    Aim : swap a node with its daddy 
    Args : current heap, node that we swap
    Return value : None
    Complexity : O(1)
*/

template<typename Tnode, typename Tkey>
void percolate_up(std::vector<Tkey>& my_heap, const Tnode n_start, bool min = true);    
/*
    Aim : performs bubble_up operations until invariant is restored
    Args : current heap, node from which we start bubbling_up, bool value : true means min-heap
    Return value : None
    Complexy : O(log(n))
*/

template<typename Tkey, typename Tnode>
Tnode bubble_down(std::vector<Tkey>& my_heap, const Tnode n, bool min = true); 
/* 
    Aim : swap a node with its weakest bambino 
    Args : current heap, node that we swap, bool value : true means min-heap
    Return value : weakest bambino
    Complexity : O(1)
*/

template<typename Tkey, typename Tnode>
void percolate_down(std::vector<Tkey>& my_heap, const Tnode n_start, bool min = true); 
/*
    Aim : performs bubble_down operations until invariant is restored
    Args : current heap, node from which we start bubbling_down, bool value : true means min-heap
    Return value : None
    Complexity : O(log(n))
*/

template<typename Tkey>
void insert(std::vector<Tkey>& my_heap, const Tkey k);
/* 
    Aim : insert a new node with key k in the heap and restores the invariant if needed 
    Args : current heap, key of the new node
    Return value : None
    Complexity : O(log(n))
*/

template<typename Tkey>
Tkey pop(std::vector<Tkey>& my_heap);
/* 
    Aim : pop the min value key and restores the invariant if needed 
    Args : current heap
    Return value : min value key
    Complexity : up to O(log(n)) to restore the invariant
*/

template<typename Tkey, typename Tnode>
std::vector<Tkey> heapify(const std::vector<Tkey> & my_vector, bool min = true);
/*
    Aim : creates a binary heap from a vector
    Args : vector to be heapified, bool value : true means min-heap
    Return value : a heap that is the heapified vector
    Complexity : O(n)
*/

template<typename Tnode>
Tnode daddy(const Tnode n);
/*
    Aim : returns the daddy of a node that is not the root
    Args : a son node
    Return value : n's daddy
    Complexity : O(1)
*/

template<typename Tnode>
Tnode left_son(const Tnode n);
/*
    Aim : return the left son of n if n has one
    Args : a node
    Return value : n's left son
    Complexity : O(1)
*/

template<typename Tnode>
Tnode right_son(const Tnode n);
/*
    Aim : return the right son of n if n has one
    Args : a node
    Return value : n's right son
    Complexity : O(1)
*/

template<typename Tnode, typename Tkey>
size_t size_heap(const std::vector<Tkey> & my_heap);
/*
    Aim : return the number of elements in the heap
    Args : the heap
    Return value : size of the heap
    Complexity : O(1)
*/

template<typename Tnode, typename Tkey>
void delete_element(std::vector<Tkey> & my_heap, const Tkey key);
/*
    Aim : delete an element from the heap
    Args : the heap, the element
    Return value : none
    Complexity : O(log(n))
*/

template<typename Tnode>
Tnode daddy(const Tnode n){
    /*
    this function does not check whether or not n is the root
    */
    return n>>1;
}

template<typename Tnode>
Tnode left_son(const Tnode n){
    /*
    this function does not check whether or not n has a left child
    */
    return n<<1;
}

template<typename Tnode>
Tnode right_son(const Tnode n){
    /*
    this function does not check whether or not n has a right child
    */
    return (n<<1) + 1;
}

template<typename Tkey>
std::vector<Tkey> create_heap(){

    std::vector<Tkey> heap(1);

    return heap;
}

template<typename Tkey, typename Tnode>
void bubble_up(std::vector<Tkey>& my_heap, const Tnode n){
    /*
    This function does not check whether or not n is the root
    */
    std::swap(my_heap[n], my_heap[daddy(n)]);
}

template<typename Tnode, typename Tkey>
void percolate_up(std::vector<Tkey>& my_heap, const Tnode n_start, bool min){      // performs differently for a min-heap and a max-heap
    Tnode current_node(n_start);

    if(min){
        // min-heap
        while(current_node != 1 && my_heap[current_node] < my_heap[daddy(current_node)]){     // if we haven't reached the root 
                                                                                              // and we have to swap with daddy 
            bubble_up(my_heap, current_node);
            current_node = daddy(current_node);
        }
    }
    else{
        // max-heap
        while(current_node != 1 && my_heap[current_node] > my_heap[daddy(current_node)]){
            bubble_up(my_heap, current_node);
            current_node = daddy(current_node);
        }
    }

}

template<typename Tkey, typename Tnode>
Tnode bubble_down(std::vector<Tkey>& my_heap, const Tnode n, bool min){
    /*
    This function does not check whether or not there is a son to swap with
    */

    if(right_son(n) < my_heap.size()){
        if(min){
            Tnode min_child = (my_heap[left_son(n)] < my_heap[right_son(n)]) ? left_son(n) : right_son(n);  // we swap with the min child
            std::swap(my_heap[n], my_heap[min_child]);
            return min_child;
        }
        else{
            Tnode max_child = (my_heap[left_son(n)] > my_heap[right_son(n)]) ? left_son(n) : right_son(n);  // we swap with the max child
            std::swap(my_heap[n], my_heap[max_child]);
            return max_child;
        }
    }
    else{
        std::swap(my_heap[n], my_heap[left_son(n)]);
        return left_son(n);
    }
}

template<typename Tkey, typename Tnode>
void percolate_down(std::vector<Tkey>& my_heap, const Tnode n_start, bool min){
    Tnode current_node(n_start);
    bool have_to_swap;                               // to determine if a bubble_down operation is needed
    
    do {                                                                    // since behavior is undefined when we check outside the bounds of a vector
                                                                            // we check how many sons actually exist
        if(min){
            if(left_son(current_node) < my_heap.size()){                               // at least one son
                
                    if(right_son(current_node) < my_heap.size())                        // two sons
                            // we swap if our key is greater than one of our son's
                            have_to_swap = my_heap[current_node] > my_heap[left_son(current_node)] or my_heap[current_node] > my_heap[right_son(current_node)];
                    else                                                            // one son
                        have_to_swap = my_heap[current_node] > my_heap[left_son(current_node)];
                        
            }
            else 
                have_to_swap = false;      // no sons
        }

        else{
            if(left_son(current_node) < my_heap.size()){
                if(right_son(current_node) < my_heap.size())
                    have_to_swap = my_heap[current_node] < my_heap[left_son(current_node)] or my_heap[current_node] < my_heap[right_son(current_node)];
                else
                    have_to_swap = my_heap[current_node] < my_heap[left_son(current_node)];
            }
            else
                have_to_swap = false;       // no sons
        }
                           
                
            if(have_to_swap){
                current_node = bubble_down(my_heap, current_node, min);
            }

    } while(have_to_swap);
}

template<typename Tkey>
void insert(std::vector<Tkey>& my_heap, const Tkey k){
    my_heap.push_back(k);                            // adds at the first possible position
    percolate_up(my_heap, my_heap.size()-1);         // restores invariant if needed
}

template<typename Tkey>
Tkey pop(std::vector<Tkey>& my_heap){
    if(my_heap.size() <= 1){
        std::cerr << "This heap is empty, you cannot get pop any value" << std::endl;
        return Tkey();
    }

    Tkey returned_key = my_heap[1];                   // binary heap -> min key value is at the root
    std::swap(my_heap[1], my_heap.back());           // swap min value key and last element
    my_heap.pop_back();                              // delete last node
    percolate_down(my_heap, 1);                      // restores invariant if needed
    return returned_key;
}

template<typename Tkey, typename Tnode>
std::vector<Tkey> heapify(const std::vector<Tkey> & my_vector, bool min){
    std::vector<Tkey> my_heap = create_heap<Tkey>();
    for(size_t i = 0; i < my_vector.size(); i++)                     // create a copy of the vector in case we might want to use it again
        my_heap.push_back(my_vector[i]);

    unsigned int depth = log2(my_heap.size()-1);                    // computes the depth of the heap
    
    Tnode starting_node(0);
    
    for(size_t k = 0; k < depth; k++)                               // computes the starting node which is the last node of the (depth-1)-level 
        starting_node += pow(2.0, k);

    for(size_t j = starting_node; j > 0; j--){        // percolate at each down starting at the level depth-1, processing nodes from right to left
        percolate_down(my_heap, j, min);
    }

    return my_heap;
}

template<typename Tkey>
size_t size_heap(const std::vector<Tkey> & my_heap){
    return my_heap.size();
}

template<typename Tnode, typename Tkey>
Tkey delete_element(std::vector<Tkey> & my_heap, Tnode node){
    Tkey returned_key;
    for(size_t i = 1; i < my_heap.size(); i++){
        if(my_heap[i].pivot == node){
            returned_key = my_heap[i];
            std::swap(my_heap[my_heap.size()-1], my_heap[i]);
            my_heap.pop_back();
            percolate_down(my_heap, i);
            return returned_key;
        }
    }
    return Tkey();
}

#endif