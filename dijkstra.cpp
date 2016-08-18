#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <unordered_set>
#include <cmath>

using namespace std;

/*
    List of all the stops:

    A series of lines representing the stops (one stop per line) and which contains the following fields:

    The unique identifier of the stop
    The full name of the stop, between quote marks"
    The description of the stop (not used)
    The latitude of the stop (in degrees)
    The longitude of the stop (in degrees)
    The identifier of the zone (not used)
    The url of the stop (not used)
    The type of stop
    The mother station (not used)

    These fields are separated by a comma ,

    Example:

    StopArea:ABDU,"Abel Durand",,47.22019661,-1.60337553,,,1,

    The routes between stops:

    A list of lines representing the routes between the stops (one route per line). Each line contains two stop identifiers separated by a white space. ​

    Each line represents a one-directional route running from the first identifier to the second. If two stops A and B are reciprocally accessible, then there will be two lines to represent this route:
    A B
    B A

    Example:

    StopArea:LAIL StopArea:GALH
    StopArea:GALH StopArea:LAIL

    Input

    Line 1: the identifier of the stop which is the starting point of the journey
    Line 2: the identifier of the stop which is the final point of the journey
    Line 3: the number N of stops in the TAN network
    N following lines: on each line a stop as described above
    Following line: the number M of routes in the TAN network
    M following lines: on each line a route as described above

    Output
    The list of the stops with their full names (one name per line) along which the shortest route passes from the start to the end of the journey (start and end included) The names must not be between quotes ".
    If it is not possible to find a route between the start and end points, the program should display IMPOSSIBLE.
    
    Constraints
    0 < N < 10000
    0 < M < 10000

    Example:

    Input                                                                                   Output

    StopArea:ABDU                                                                           Abel Durand                                                                        
    StopArea:ABLA                                                                           Avenue Blanche
    3
    StopArea:ABDU,"Abel Durand",,47.22019661,-1.60337553,,,1,
    StopArea:ABLA,"Avenue Blanche",,47.22973509,-1.58937990,,,1,
    StopArea:ACHA,"Angle Chaillou",,47.26979248,-1.57206627,,,1,
    2
    StopArea:ABDU StopArea:ABLA
    StopArea:ABLA StopArea:ACHA

    My solution implements the Dijkstra's algorithm using 

*/

typedef string Stop;
typedef string Route;
typedef string Name;
typedef double Distance;
typedef double Latitude;
typedef double Longitude;
typedef pair<Latitude, Longitude> Coords;
typedef string Id;
typedef map<Id, Id> Path;
typedef pair<Id, Distance> Neighbor;
typedef map<Id, vector<Neighbor>> Network;

const double pi = 3.14;

/* Compute the distance between two stops of Id A and B respectively */
Distance computeDistance(const Id A, const Id B, map<Id, Coords>& points){
    double x = (points[B].second - points[A].second) * cos((points[A].first + points[B].first)/2);
    double y = points[B].first - points[A].first;
    return sqrt(x*x + y*y);
}
    
/* the 4 next functions are simply helper functions to parse the input and store it into an appropriate data structure 
    in order to use the Dijkstra's algorithm
*/
    
const Id getId(const Stop stop){
    return stop.substr(9, stop.find(',')-9);
}

Name getName(const Stop stop){
    size_t pos1 = stop.find('"');
    return stop.substr(pos1+1, stop.substr(pos1+1).find('\"'));
}


void addName(map<Id, Name>& names, const Stop stop){
    names[getId(stop)] = getName(stop);
}

Coords getCoords(const Stop stop){
    Coords ret;
    stringstream ss(stop);
    string token;
    getline(ss, token, ',');
    getline(ss, token, ',');
    getline(ss, token, ',');
    getline(ss, token, ',');
    ret.first = (pi / 180) * stod(token);
    getline(ss, token, ',');
    ret.second = (pi / 180) * stod(token);
    return ret;
}

/* Implementation of the Dijkstra's algorithm to find the shortests paths to our starting point */
bool dijkstra(const Network& network, Path& path, const Id startingPoint, map<Id, Distance>& distances, const Id endPoint, const vector<Id>& ids){
    // to check if the distance to the end point has been updated
    bool updated(false);
    path[startingPoint] = startingPoint;
    vector<Id> taken_care_of;
    unordered_set<Id> potential_pivots;
    for(size_t i = 0; i < ids.size(); ++i){
        distances[ids[i]] = 1000000000000;
        potential_pivots.insert(ids[i]);
    }
    Id pivot = startingPoint;
    distances[startingPoint] = 0;
    
    while(taken_care_of.size() != network.size()){

        taken_care_of.push_back(pivot);
        potential_pivots.erase(pivot);
        // update la distances des voisins
        for(size_t i = 0; i < network[pivot].size(); i++){
            if(distances[network[pivot][i].first] > distances[pivot] + network[pivot][i].second){
                distances[network[pivot][i].first] = distances[pivot] + network[pivot][i].second;
                path[network[pivot][i].first] = pivot;
                if(!network[pivot][i].first.compare(endPoint))
                    updated = true;
            }
        }
        // updates the pivot
        Distance next_pivot(1000000000.0);
        for(Id id : potential_pivots){
            if(distances[id] < next_pivot){
                pivot = id;
                next_pivot = distances[id];
            }
        }
    }
    if(!updated and startingPoint.compare(endPoint)) return false;
    else         return true;
} 

int main()
{
    string startPoint;
    cin >> startPoint; cin.ignore();
    string endPoint;
    cin >> endPoint; cin.ignore();
    vector<Stop> stops;
    int N;
    cin >> N; cin.ignore();
    map<Id, Name> names;
    map<Id, Coords> points;
    vector<Id> ids;
    
    for (size_t i = 0; i < N; i++) {
        Stop stopName;
        getline(cin, stopName);
        stops.push_back(stopName);
        names[getId(stopName)] = getName(stopName);
        points[getId(stopName)] = getCoords(stopName);
        ids.push_back(getId(stopName));
    }
    
    vector<Route> routes;
    int M;
    cin >> M; cin.ignore();
    for (size_t i = 0; i < M; i++) {
        Route route;
        getline(cin, route);
        routes.push_back(route);
    }

    Network network;
    for(size_t i = 0; i < M; i++){
        const Id A = getId(routes[i].substr(0, routes[i].find(' ')));
        const Id B = getId(routes[i].substr(routes[i].find(' ')+1));
        network[A].push_back(make_pair(B, computeDistance(A, B, points)));
    }
    
    Path path;
    map<Id, Distance> distances;
    
    bool poss = dijkstra(network, path, getId(startPoint), distances, getId(endPoint), ids);
    
    if(!poss){
        cout << "IMPOSSIBLE" << endl;
        return 0;
    }

    Id curr_stop = getId(endPoint);
    
    vector<Id> stops_;

    while(curr_stop.compare(getId(startPoint))){ 
        stops_.push_back(curr_stop);
        curr_stop = path[curr_stop];
    }
    
    stops_.push_back(curr_stop);

    for(size_t i = 0; i < stops_.size(); i++)
        cout << names[stops_[stops_.size()-1-i]] << endl;
    
    return 0;
}
