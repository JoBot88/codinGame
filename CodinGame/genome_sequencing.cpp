#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool is_in(const string s1, const string s2){
    return s1.find(s2) != string::npos;
}

int reduce_strings(const string s1, string& s2){
    for(size_t i = 0; i < s2.length(); i++){
        string sub = s2.substr(0, s2.length()-i);
        size_t pos_last = s1.rfind(sub);
        if(pos_last == s1.length()-sub.length() and pos_last <= s1.length()){
            s2.erase(0, sub.length());
            return sub.length();
        }
    }
    return 0;
}

int main()
{
    int N;
    cin >> N; cin.ignore();
    vector<string> subs;
    int long_max(0);
    for (int i = 0; i < N; i++) {
        string subseq;
        cin >> subseq; cin.ignore();
        subs.push_back(subseq);
        long_max += subseq.length();
    }

    vector<int> perm;
    for(size_t i = 0; i < subs.size(); i++)
        perm.push_back(i);
    
    int best(long_max);
    string best_s;
    
    do {
        int score(long_max);
        string s(subs[perm[0]]);
        for(size_t i = 1; i < perm.size(); i++){
            string copy(subs[perm[i]]);

            if(is_in(s, subs[perm[i]]))
                score -= subs[perm[i]].length();
            else{
                score -= reduce_strings(s, copy);
                s += copy;
            }
        }
        
        if(score < best) { best = score; best_s = s; }
        
    } while(next_permutation(perm.begin(), perm.end()));

    cout << best << endl;
    
    return 0;
}