#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/*

    Input
    Line 1: the number N of performance measures carried out on the same program.
    N following lines: one performance measure per line. Each line contains two values: an integer num representing the number of items that the program has processed and an integer t representing the execution time measured to process these items in microseconds. These values are separated by a space. Values of n are unique and sorted in ascending order.
    Output
    The most probable computational complexity among the following possibilities: O(1), O(log n), O(n), O(n log n), O(n^2), O(n^2 log n), O(n^3), O(2^n)
    Constraints

    5 < N < 1000

    5 < num < 15000

    0 < t < 10000000

    Example

    Input                             Output

    10                                O(n)
    5 341
    1005 26324
    2005 52585
    3005 78877
    4005 104925
    4805 125920
    6105 159156
    7205 188017
    8105 211417
    9905 258991
    
    My solution implements a linear regression analysis 

*/

using namespace std;

void product_tab(const vector<double>& tab1, const vector<double>& tab2, vector<double>& res){
    for(size_t i = 0; i < tab1.size(); i++)
        res[i] = tab1[i] * tab2[i];
}

vector<double> log_tab(const vector<double>& tab){
    vector<double> res;
    for(size_t i = 0; i < tab.size(); i++)
        res.push_back(log(tab[i]));
    return res;       
}

vector<double> lin_log_tab(const vector<double>& tab){
    vector<double> res;
    for(size_t i = 0; i < tab.size(); i++)
        res.push_back(tab[i] * log(tab[i]));
    return res;
}

vector<double> square_tab(const vector<double>& tab){
    vector<double> res;
    for(size_t i = 0; i < tab.size(); i++)
        res.push_back(tab[i] * tab[i]);
    return res;
}

vector<double> square_log_tab(const vector<double>& tab){
    vector<double> res;
    for(size_t i = 0; i < tab.size(); i++)
        res.push_back(tab[i] * tab[i] * log(tab[i]));
    return res;
}

vector<double> cube_tab(const vector<double>& tab){
    vector<double> res;
    for(size_t i = 0; i < tab.size(); i++)
        res.push_back(tab[i] * tab[i] * tab[i]);
    return res;
}

double average(const vector<double>& tab){
    double average(0.0);
    for(size_t i = 0; i < tab.size(); i++)
        average += (double) tab[i];
    return average / tab.size();
}

double covariance(const vector<double>& tab1, const vector<double>& tab2){
    vector<double> tmp(tab1.size());
    product_tab(tab1, tab2, tmp);
    return average(tmp) - average(tab1) * average(tab2);   
}

double variance(const vector<double>& tab){
    double _average = average(tab);
    vector<double> tmp(tab.size(), 0);
    product_tab(tab, tab, tmp);
    return average(tmp) - _average * _average;
}

double standard_deviation(const vector<double>& tab){
    return sqrt(variance(tab));
}

double corr(const vector<double>& tab1, const vector<double>& tab2){
    return covariance(tab1, tab2) / ( standard_deviation(tab1) * standard_deviation(tab2) );
}

int main()
{
    int N;
    cin >> N; cin.ignore();
    vector<double> nums;
    vector<double> ts;

    /*
        0 -> O(1)
        1 -> O(log n)
        2 -> O(n)
        3 -> O(n log n)
        4 -> O(n^2)
        5 -> O(n^2 log n)
        6 -> O(n^3)
        7 -> O(2 ^n)
                            */

    for (int i = 0; i < N; i++) {
        double num;
        double t;
        cin >> num >> t; cin.ignore();
        nums.push_back(num);
        ts.push_back(t);
    }
    
    vector<double> coeffs;
    
    double coeff_log = abs(corr(log_tab(nums), ts));
    coeffs.push_back(coeff_log);
    
    double coeff_lin = abs(corr(nums, ts));
    coeffs.push_back(coeff_lin);
  
    double coeff_lin_log = abs(corr(lin_log_tab(nums), ts));
    coeffs.push_back(coeff_lin_log);
    
    double coeff_square = abs(corr(square_tab(nums), ts));
    coeffs.push_back(coeff_square);
    
    double coeff_square_log = abs(corr(square_log_tab(nums), ts));
    coeffs.push_back(coeff_square_log);
    
    double coeff_cube = abs(corr(cube_tab(nums), ts));
    coeffs.push_back(coeff_cube);
    
    double coeff_expo = abs(corr(nums, log_tab(ts)));
    coeffs.push_back(coeff_expo);
    
    int complexity(0);
    
    // if no coefficient is > 0.95 it means the constant function is the best we have
    double best_r(0.95); 

    for(size_t i = 0; i < coeffs.size(); i++){
        if(coeffs[i] > best_r){
            complexity = i + 1;
            best_r = coeffs[i];
        }
    }

    switch(complexity){
        case 0: cout << "O(1)" << endl; break;
        case 1: cout << "O(log n)" << endl; break;
        case 2: cout << "O(n)" << endl; break;
        case 3: cout << "O(n log n)" << endl; break;
        case 4: cout << "O(n^2)" << endl; break;
        case 5: cout << "O(n^2 log n)" << endl; break;
        case 6: cout << "O(n^3)" << endl; break;
        case 7: cout << "O(2^n)" << endl; break;
        default: cout << "Je suis dans le default" << endl; break;
    }
    
    return 0;
}
            
            
                
                
