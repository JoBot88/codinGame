#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

/*
    Input
    Line 1: the width L of the map
    Line 2: the height H of the map
    H following lines: L characters # or O
    Following line: the number N of coordinates to be tested
    N following lines: the coordinates X Y to be tested, separated by a space
    
    Output
    N lines: each displaying the surface area of the lake located at the coordinates given in input (a 'O' counts for one unit of surface).

    example of a map : (# means land, O means water), two O's in diagonal are not considered parts of the same lake
        j
       
    i   ####
        ##O#
        #OO#
        ####
    
    Constraints
    0 < L < 10000
    0 < H < 10000
    0 ≤ X < L
    0 ≤ Y < H
    0 < N < 1000

    example :

    Input                                  Output
    4                                      0         because the cell (0, 0) belongs to no lake
    4                                      3         because the cell (1, 2) belongs to the lake formed by the cells (1, 2), (2, 1), (2, 2)
    ####                                   3         because the cell (2, 1) belongs to the lake formed by the cells (1, 2), (2, 1), (2, 2)
    ##O#
    #OO#
    ####
    3
    0 0
    1 2
    2 1

    My solution makes use of a graph representation for the lake and uses a breadth-first search starting at each of the coordinates given as input
    to find the surfaces required

*/


typedef pair<int, int> coord;

/*  implementation of the breadth-first search algorithm taking as input a 2D-vector filled with boolean values
    representating the status of the nodes of our graph (all pairs of coordinates on the map), false meaning that this node has not
    been visited yet and vice versa
*/
int bfs(vector<vector<bool>>& marqued, const vector<string>& graph, const coord start){
    // we initialize the size of the lake to which our piece of the map belongs 
    int size_lake(0);

    queue<coord> queue;
    // we push the starting node into the queue and set its status to visited -> true
    queue.push(start);
    marqued[start.first][start.second] = true;

    int width = graph[0].length()-1;
    int height = graph.size()-1;
    
    // if our starting node is not water we simply return 0 because it means it does not belong to a lake at all
    if(graph[start.first][start.second] == '#') return 0;

    while(!queue.empty()){
        // otherwise we pop the first node of the queue and increase the size of the lake by 1
        coord next = queue.front();
        queue.pop();
        size_lake++;

        // we check the four neighbors of our nodes (except in those rare cases where the current node is on one border of the map
        // in which case we only check the existing adjacent nodes)

        // for each neighbor if it's water we push into the queue and loop over until there is no more adjacent parts of the lake
        if(next.first > 0 and graph[next.first-1][next.second] == 'O' and !marqued[next.first-1][next.second]){
            queue.push(make_pair(next.first-1, next.second));
            marqued[next.first-1][next.second] = true;
        }
        
        if(next.second > 0 and graph[next.first][next.second-1] == 'O' and !marqued[next.first][next.second-1]){
            queue.push(make_pair(next.first, next.second-1));
            marqued[next.first][next.second-1] = true;
        }
        
        if(next.second < width and graph[next.first][next.second+1] == 'O' and !marqued[next.first][next.second+1]){
            queue.push(make_pair(next.first, next.second+1));
            marqued[next.first][next.second+1] = true;
        }
        
        if(next.first < height and graph[next.first+1][next.second] == 'O' and !marqued[next.first+1][next.second]){
            queue.push(make_pair(next.first+1, next.second));
            marqued[next.first+1][next.second] = true;
        }
    }

    // there is no more water that is adjacent to the nodes we have already visited, we can return the size of the lake computed so far
    return size_lake;
}

int main()
{
    int L;
    cin >> L; cin.ignore();
    int H;
    cin >> H; cin.ignore();
    // to fetch the map
    vector<string> map_t;
    for (int i = 0; i < H; i++) {
        string row;
        getline(cin, row);
        map_t.push_back(row);
    }
    
    int N;
    cin >> N; cin.ignore();
    // to fetch the coordinates to test
    vector<coord> to_test;
    
    for (int i = 0; i < N; i++) {
        int X;
        int Y;
        cin >> X >> Y; cin.ignore();
        to_test.push_back(make_pair(Y, X));
    }

    // for each pairs of coordinates corresponding to a position on the map, we apply our algorithm 
    for(size_t i = 0; i < N; i++) {
        vector<vector<bool>> marqued(H, vector<bool>(L, false));
        cout << bfs(marqued, map_t, to_test[i]) << endl;
    }
    
    return 0;
}
