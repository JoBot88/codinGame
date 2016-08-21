#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <queue>
#include <map>

using namespace std;

/*
    Solution to the Skynet Revolution: 2 problem 
*/

void bfs(vector<bool>& marqued, const vector<list<int>>& graph, const int sommet, vector<int>& parents, vector<int>& distances){
    queue<int> queue;
    queue.push(sommet);
    distances[sommet] = 0;
    while(!queue.empty()){
        int next = queue.front();
        queue.pop();
        if(!marqued[next]){
            marqued[next] = true;
            for(auto it = graph[next].cbegin(); it != graph[next].cend(); ++it){
                if((distances[next]+1) < distances[*it])
                    parents[*it] = next;
                distances[*it] = min(distances[*it], distances[next]+1);
                queue.push(*it);
            }
        }
    }
}

int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();
    vector<list<int>> graph(N, list<int>());
    vector<int> gateways;
    
    for (int i = 0; i < L; i++) {
        unsigned int N1; // N1 and N2 defines a link between these nodes
        unsigned int N2;
        cin >> N1 >> N2; cin.ignore();
        graph[N1].push_back(N2);
        graph[N2].push_back(N1);
    }
    for (int i = 0; i < E; i++) {
        unsigned int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
        gateways.push_back(EI);
    }

    pair<int, int> link;

    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();

        // to call the bfs
        vector<bool> marqued(N, false);
        vector<int> distances(N, 1000);
        vector<int> parents(N, -1);
   
        bfs(marqued, graph, SI, parents, distances);

        bool danger_is_imminent(false);
        // if there is a gateway that's only one step away from the agent we just kill the right link
        for(size_t i = 0; i < gateways.size(); i++){
            if(distances[gateways[i]] == 1){
                link.first = gateways[i];
                link.second = parents[gateways[i]];
                danger_is_imminent = true;
                break;
            }
        }

        if(!danger_is_imminent){
            // for each gateway we determine which node is the most interesting one (the one whose link with the gateway is to be taken care of first)
            // it's the one that has a link with the greatest # of gateways

            // first element : the node, second one : number of adjacent gateways
            vector<pair<int, unsigned int>> best_nodes(N, make_pair(-1, 0));
            // # of connections of each gate
            // a connection is actually a link between an adjacent node to the gateway and any gateway
            vector<int> num_connections(N, 0);
            
            // so for each gateway...
            for(size_t i = 0; i < gateways.size(); i++){

                // where store the number of connections
                int nb_gateways_touched(0); 
                // for each neighbor of the gateway being considered 
                for(auto it = graph[gateways[i]].cbegin(); it != graph[gateways[i]].cend(); ++it){
                    
                    // where we store the # of gateways adjacent to the neighbor being considered
                    // this is necessary to establish a hierarchy between the nodes adjacent of a gateway in case this is
                    // the one we gonna have to take care of this turn
                    unsigned int nb_gateways_around(0);
                    // for each gateway...
                    for(size_t j = 0; j < gateways.size(); j++){
                        // we determine if the node being considered share a link with it
                        for(auto it_gate = graph[gateways[j]].cbegin(); it_gate != graph[gateways[j]].cend(); ++it_gate){
                            if((*it_gate) == (*it)){ 
                                // if yes then I increment the # of connections of the gateway as well as the # of adjacent gateways of the node
                                nb_gateways_around++;
                                nb_gateways_touched++;
                            }
                        }
                    } 
                    // we update the best node of the gateway i being the one which has the greatest # of adjacent gateways
                    // to break ties we keep the one that's the father of the gateway in the path given by the bfs (might be redundant with 
                    // the work done to determine the link a little below)
                    if(nb_gateways_around > best_nodes[gateways[i]].second or (nb_gateways_around == best_nodes[gateways[i]].second and parents[gateways[i]] == (*it))){
                        best_nodes[gateways[i]].second = nb_gateways_around;
                        best_nodes[gateways[i]].first = *it;
                    }
                }
                // we store the # of connections of the current gateway
                if(nb_gateways_touched > num_connections[gateways[i]])
                    num_connections[gateways[i]] = nb_gateways_touched;
            }

            float best_score(0);
            
            map<int, int> interesting_neighbors;
            // we compute here the # of neighbors of each best node that share the same gateway(to break ties in the case the #_connections / distances_to_agent ratios 
            // of two gateways are the same)
            for(size_t i = 0; i < gateways.size(); i++){
                int num_neighbors(0);
                // for each neighbor of our gateway we are going to check if it's also a neighbor of our best node
                for(auto it = graph[gateways[i]].cbegin(); it != graph[gateways[i]].cend(); ++it){
                    // for each neighbor of the best node of the current gateway
                    for(auto it_best = graph[best_nodes[gateways[i]].first].cbegin(); it_best != graph[best_nodes[gateways[i]].first].cend(); ++it_best){
                        if((*it_best) == (*it))
                            num_neighbors++;
                    }
                }
                interesting_neighbors[gateways[i]] = num_neighbors;
            }
                    
            
            // alright here is the magic, we gonna try to establish the correct order for the links by considering each gateway
            for(size_t i = 0; i < gateways.size(); i++){
                
                // so far we decide that a gateway is to be taken care of first if its ratio #_connections / distances_to_the_agent is maximum
                // to break ties we need to prioritize the gateway whose adjacent nodes are the more linked together
                // basically we need to count the sum of the links between two nodes that are adjacent to the gateway being considered
                if((float)num_connections[gateways[i]] / distances[gateways[i]] > best_score or 
                ((float)num_connections[gateways[i]] / distances[gateways[i]] == best_score and interesting_neighbors[gateways[i]] > interesting_neighbors[link.first])){
                    // once a gateway is chosen we have one end of our winner
                    // the other end is our gateway's best node according the criterium defined above
                    best_score = (float)num_connections[gateways[i]] / distances[gateways[i]];
                    link.first = gateways[i];
                    link.second = best_nodes[gateways[i]].first;
                }
            }
        }
        // don't forget to update the graph by removing the link we are killing
        if(!graph[link.first].empty())graph[link.first].remove(link.second);
        if(!graph[link.second].empty())graph[link.second].remove(link.first);
    
        cout << link.first <<  " " << link.second << endl;
    }
    return 0;
}