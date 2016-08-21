#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

/*

    Input
    Line 1: the number N of rooms in the building.
    N following lines: one line with a room number (integer), a sum of money (integer), the two numbers of the rooms which are accessible (integers or E if the door is an exit). These four values are separated by a white space.
    
    Output
    An integer representing the maximum amount of money that Bender can collect by taking a series of doors to reach the outside of the building.

    constraints : cannot go through the same room twice
    N < 10000

    My solution uses dynamic programming as well as memoization in order to significantly speed up the algorithm 

*/

typedef pair<string, string> exits;
typedef int Curr;
typedef int Pred;

int get_number(const string& room);

int get_cash(const string& room){
    stringstream ss(room);
    string token;
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    return stoi(token);
}

/*  Given an input line representing a room of the form :
        room number, amount of money in the room, first adjacent room, second adjacent room separated by spaces
        example : 0 17 1 2
                  1 14 12 E
                  13 12 E E
    returns the 2 adjacent rooms (E means exit)
*/
exits get_exits(const string& room){
    stringstream ss(room);
    exits r_exits;
    string token;
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    r_exits.first = token;
    getline(ss, token, ' ');
    r_exits.second = token;
    return r_exits;
}

/* Given a vector of rooms, returns the maximum amount of money recoverable assuming that it is not allowed to go through the same room twice */
int best_path(const vector<string>& rooms, const string curr_room, const int previous_room_number, const int curr_money, vector<bool> visited, map<pair<Curr, Pred>, int>& table){
  
    // oto get the number of our current room, will be used to index the visited vector
    Curr curr_number = get_number(curr_room);
  
    // if this room has already been visited we return 0, this is just one way to translate the fact that we cannot go through the same room twice
    if(visited[curr_number])
        return 0;

    // to create a pair given by the current room and the room that was visited just before
    pair<Curr, Pred> current_pair = make_pair(curr_number, previous_room_number);
        
    // if the amount of money is known for this pair, simply return it
    if(table[current_pair] != 0)
        return table[current_pair];
    
    // otherwise recover the amount of money available in the current room
    int cash = get_cash(curr_room);

    // to get the adjacent rooms
    exits curr_exits = get_exits(curr_room);
    
    // we add the amount of money available in the current room to the amount we have recovered so far
    int new_money = curr_money + cash;
    
    // if the first adjacent room does not exist -> is actually an exit
    if(!curr_exits.first.compare("E")){
        // if the second one is also an exit we have reached our base case, we can store the value for this pair and return
        if(!curr_exits.second.compare("E")){
            table[current_pair] = cash;
            return cash;
        }
        else {
        // if the second adjacent room is really a room and not an exit then we apply our recurrence relation and return the maximum
        // of the money we have and the money that would be recoverable going through the second adjacent room
            table[current_pair] = max(new_money, best_path(rooms, rooms[get_number(curr_exits.second)], curr_number,
                        curr_money, visited, table) + cash);
            return table[current_pair];
        }
    }
    // same as before but instead we return the maximum of the money we have already recovered and the money recoverable going through the first 
    // adjacent room
    else if(!curr_exits.second.compare("E")){
        table[current_pair] = max(new_money, best_path(rooms, rooms[get_number(curr_exits.first)], curr_number, 
                        curr_money, visited, table) + cash); 
        return table[current_pair];
    }
    else {
    // finally if both adjacent rooms are not exits we update the status of the room to "visited" and return the maximum of money 
    // recoverable going through one of the two adjacent rooms
        visited[curr_number] = true;
        table[current_pair] = max(best_path(rooms, rooms[get_number(curr_exits.first)], curr_number, curr_money, visited, table) + cash,
                   best_path(rooms, rooms[get_number(curr_exits.second)], curr_number, curr_money, visited, table) + cash);
        return table[current_pair];
    }
}
    
/*  Given an input line representing a room of the form :
        room number, amount of money in the room, first adjacent room, second adjacent room separated by spaces
        example : 0 17 1 2
                  1 14 12 E
                  13 12 E E
    returns the room number
*/
int get_number(const string& room){
    stringstream ss(room);
    string token;
    getline(ss, token, ' ');
    return stoi(token);
}
       
int main() {

    int N;
    cin >> N; cin.ignore();
    
    // to store the rooms
    vector<string> rooms;

    // to fetch the input provided by the tester
    for (int i = 0; i < N; i++) {
        string room;
        getline(cin, room);
        rooms.push_back(room);
    }
    
    // to keep track of the visited rooms, no rooms have yet been visited so the vector is filled with false values
    vector<bool> visited(N, false);
    
    // to implement memoization, to avoid computing the same value over and over again
    map<pair<Curr, Pred>, int> table;
    
    // to call the main program, the previous number is initialized to -1 and the amount of money accumulated to 0, 
    // table is empty and will be filled up as the algorithm runs
    cout << best_path(rooms, rooms[0], -1, 0, visited, table);
    
    return 0;
}

