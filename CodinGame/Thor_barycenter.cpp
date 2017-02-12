#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
    Map is 40 wide & 18 high
    Each time Thor strike all the giants in a square centered on Thor and of 9 spaces wide are destroyed
    
    Initialization input
    TX and TY indicating Thor's position
    
    Input for one game turn
    Line 1: 2 integers H & N, H denotes the number of remaining hammer strikes, N denotes the number of giants still alive
    N next lines: positions of the giants
    
    Output for one game turn:
    One single line indicating which action or movement is to be carried out this turn: WAIT, STRIKE, S, SE, SW, N, NE, NW
    
*/

typedef pair<int, int> coord;

bool in_range(const int x_thor, const int y_thor, const int x_giant, const int y_giant){
    return (abs(x_thor-x_giant) < 5 && abs(y_thor-y_giant) < 5);
}

int giants_in_range(const vector<coord> giants_positions, const int x_thor, const int y_thor){
    int number_giants_in_range(0);
    for(int i = 0; i < giants_positions.size(); i++){
        if(in_range(x_thor, y_thor, giants_positions.at(i).first, giants_positions.at(i).second))
            number_giants_in_range++;
    }
    return number_giants_in_range;
}

bool is_imminent_death(const std::vector<coord>& giants, const int x_thor, const int y_thor){  
    for(int i = 0; i < giants.size(); i++){
        if(abs(x_thor-giants.at(i).first) < 2 && abs(y_thor-giants.at(i).second) < 2)
            return true;
    }
    return false;
}

int distance_to_giant(const int x_thor, const int y_thor, const int x_giant, const int y_giant){
    return floor(sqrt(pow(abs(x_thor-x_giant), 2) + pow(abs(y_thor-y_giant), 2)));
}

/// may want to break ties by fleeing where there are the most space available
std::string closest_while_safest(const int x_thor, const int y_thor, const coord closest_giant, const coord furthest_giant, const std::vector<coord>& giants){
    std::string closest("");
    int distance_to_closest(distance_to_giant(x_thor, y_thor, closest_giant.first, closest_giant.second));
    int distance_to_furthest(distance_to_giant(x_thor, y_thor, furthest_giant.first, furthest_giant.second));

    // we check if this move :
    // - gets us any closer from the furthest giant
    // - does not get us any closer from the closest giant

    // North 
    if(!is_imminent_death(giants, x_thor, y_thor-1) && y_thor > 0) {
        
        if((distance_to_giant(x_thor, y_thor-1, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor, y_thor-1, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor, y_thor-1, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor, y_thor-1, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor, y_thor-1, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor, y_thor-1, closest_giant.first, closest_giant.second) == distance_to_closest && y_thor > 9))

        {
            distance_to_furthest = distance_to_giant(x_thor, y_thor-1, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor, y_thor-1, closest_giant.first, closest_giant.second);
            closest = "N";
        }
    }

    // North West
    if(!is_imminent_death(giants, x_thor-1, y_thor-1) && y_thor > 0 && x_thor > 0) { 
        
        if((distance_to_giant(x_thor-1, y_thor-1, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor-1, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor-1, y_thor-1, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor-1, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor-1, y_thor-1, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor-1, closest_giant.first, closest_giant.second) == distance_to_closest && (y_thor > 9 || x_thor > 19))) 
        {
            distance_to_furthest = distance_to_giant(x_thor-1, y_thor-1, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor-1, y_thor-1, closest_giant.first, closest_giant.second);
            closest = "NW";
        }
    }

    // West
    if(!is_imminent_death(giants, x_thor-1, y_thor) && x_thor > 0) {
        
        if((distance_to_giant(x_thor-1, y_thor, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor-1, y_thor, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor-1, y_thor, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor, closest_giant.first, closest_giant.second) == distance_to_closest && x_thor > 19))
        {
            distance_to_furthest = distance_to_giant(x_thor-1, y_thor, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor-1, y_thor, closest_giant.first, closest_giant.second);
            closest = "W";
        }
    }

    // South West
    if(!is_imminent_death(giants, x_thor-1, y_thor+1) && x_thor > 0 && y_thor < 17) { 
        
        if((distance_to_giant(x_thor-1, y_thor+1, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor+1, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor-1, y_thor+1, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor+1, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor-1, y_thor+1, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor-1, y_thor+1, closest_giant.first, closest_giant.second) == distance_to_closest && (y_thor < 10 || x_thor > 19)))
        {
            distance_to_furthest = distance_to_giant(x_thor-1, y_thor+1, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor-1, y_thor+1, closest_giant.first, closest_giant.second);
            closest = "SW";
        }
    }

    // South
    if(!is_imminent_death(giants, x_thor, y_thor+1) && y_thor < 17) {
        
        if((distance_to_giant(x_thor, y_thor+1, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor, y_thor+1, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor, y_thor+1, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor, y_thor+1, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor, y_thor+1, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor, y_thor+1, closest_giant.first, closest_giant.second) == distance_to_closest && y_thor < 10))
        {
            distance_to_furthest = distance_to_giant(x_thor, y_thor+1, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor, y_thor+1, closest_giant.first, closest_giant.second);
            closest = "S";
        }
    }

    // South East
    if(!is_imminent_death(giants, x_thor+1, y_thor+1) && x_thor < 39 && y_thor < 17) { 

        if((distance_to_giant(x_thor+1, y_thor+1, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor+1, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor+1, y_thor+1, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor+1, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor+1, y_thor+1, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor+1, closest_giant.first, closest_giant.second) == distance_to_closest && (y_thor < 10 || x_thor < 20)))
        {
            distance_to_furthest = distance_to_giant(x_thor+1, y_thor+1, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor+1, y_thor+1, closest_giant.first, closest_giant.second);
            closest = "SE";
        }
    }


    // EAST
    if(!is_imminent_death(giants, x_thor+1, y_thor) && x_thor < 39) { 
        if((distance_to_giant(x_thor+1, y_thor, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor+1, y_thor, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor+1, y_thor, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor, closest_giant.first, closest_giant.second) == distance_to_closest && x_thor < 20))
        {
            distance_to_furthest = distance_to_giant(x_thor+1, y_thor, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor+1, y_thor, closest_giant.first, closest_giant.second);
            closest = "E";
        }
    }

    // NORTH EAST
    if(!is_imminent_death(giants, x_thor+1, y_thor-1) && x_thor < 39 && y_thor > 0) { 
        if((distance_to_giant(x_thor+1, y_thor-1, furthest_giant.first, furthest_giant.second) < distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor-1, closest_giant.first, closest_giant.second) >= distance_to_closest) ||
          (distance_to_giant(x_thor+1, y_thor-1, furthest_giant.first, furthest_giant.second) <= distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor-1, closest_giant.first, closest_giant.second) > distance_to_closest) ||
          (distance_to_giant(x_thor+1, y_thor-1, furthest_giant.first, furthest_giant.second) == distance_to_furthest &&
           distance_to_giant(x_thor+1, y_thor-1, closest_giant.first, closest_giant.second) == distance_to_closest && (y_thor > 9 || x_thor < 20)))
        {
            distance_to_furthest = distance_to_giant(x_thor+1, y_thor-1, furthest_giant.first, furthest_giant.second);
            distance_to_closest = distance_to_giant(x_thor+1, y_thor-1, closest_giant.first, closest_giant.second);
            closest = "NE";
        }
    }

    if(closest.compare(""))
        return closest;
    // if no move improves our current position by getting us closer from the furthest giant while staying as far as possible from the closest one
    // then we wait (unless the closest giant is too close in which case we strike but this is not being taken care of here)
    else
        return "WAIT";
}

/// updates Thor's position
std::string optimal_move(const int x_thor, const int y_thor, const coord closest_giant, const coord furthest_giant, const std::vector<coord>& giants){
    return closest_while_safest(x_thor, y_thor, closest_giant, furthest_giant, giants);
}

int main()
{
    int TX;
    int TY;
    cin >> TX >> TY; cin.ignore();

    while (1) {
        int H; // the remaining number of hammer strikes.
        int N; // the number of giants which are still present on the map.
        cin >> H >> N; cin.ignore();
        vector<coord> giants;
        coord closest_giant = std::make_pair(50, 50);
        coord furthest_giant = std::make_pair(TX, TY);

        for (int i = 0; i < N; i++) {
            int X;
            int Y;
            cin >> X >> Y; cin.ignore();
            giants.push_back(make_pair(X, Y));
            if(distance_to_giant(TX, TY, X, Y) < distance_to_giant(TX, TY, closest_giant.first, closest_giant.second)){
                closest_giant.first = X;
                closest_giant.second = Y;
            } 
            if(distance_to_giant(TX, TY, X, Y) > distance_to_giant(TX, TY, furthest_giant.first, furthest_giant.second)){
                furthest_giant.first = X;
                furthest_giant.second = Y;
            }
        }

        std::string opt_move = optimal_move(TX, TY, closest_giant, furthest_giant, giants);

        /// if our position is optimal (best to kill as many giants as possible) and if a giant is about to kill us then we strike
        if(is_imminent_death(giants, TX, TY) && !opt_move.compare("WAIT"))
            std::cout << "STRIKE" << std::endl; 

        // otherwise we move or wait if our position is ideal but the giants not yet in range
        else {
            std::cout << opt_move << std::endl;  

            if(!opt_move.compare("N"))
                TY--;

            if(!opt_move.compare("NW")){
                TY--;
                TX--;
            }

            if(!opt_move.compare("W"))
                TX--;

            if(!opt_move.compare("SW")){
                TX--;
                TY++;
            }

            if(!opt_move.compare("S"))
                TY++;

            if(!opt_move.compare("SE")){
                TX++;
                TY++;
            }

            if(!opt_move.compare("E"))
                TX++;

            if(!opt_move.compare("NE")){
                TX++;
                TY--;
            }
        }
    }
    return 0;
}
