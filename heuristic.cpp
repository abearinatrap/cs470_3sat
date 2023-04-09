#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define vvi vector<vi> 
#define pb push_back
#define ll long long
#define ull unsigned ll
#define vull vector<ull>

void ppsz_3sat(const std::vector<std::vector<int>>& clauses, int num_vars) {
    std::bitset<100> truth_assignment;
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());  // Random number generator
    int num_iterations = 4 * num_vars * num_vars;  // Number of iterations to run the algorithm
    for (int iteration = 1; iteration <= num_iterations; iteration++) {
        int unsatisfied_clause = -1;
        for (int i = 0; i < clauses.size(); i++) {
            bool clause_satisfied = false;
            for (int literal : clauses[i]) {
                int var = abs(literal) - 1;
                bool value = truth_assignment[var] ^ (literal < 0);  // Flip the value of the variable if the literal is negative
                if (value) {
                    clause_satisfied = true;
                    break;
                }
            }
            if (!clause_satisfied) {
                unsatisfied_clause = i;
                break;
            }
        }
        if (unsatisfied_clause == -1) {
            // All clauses are satisfied
            cout << "Solution: "; 
            for(int i=0;i<num_vars;++i){
                cout<< truth_assignment[i] << (i==(num_vars-1)?'\n':' ');
            }
            return;
        } else {
            // Choose a variable to flip randomly
            int var_to_flip = abs(clauses[unsatisfied_clause][rng() % 3]) - 1;
            truth_assignment[var_to_flip].flip();
        }
    }
    cout << "No solution found in heuristic" << endl;
}

int main (int argc, char* argv[]) {
    if (argc != 2){
        // need input file
        return -1;
    }

    ifstream datafile;
    datafile.open(argv[1]);
    
    if(!datafile.is_open()){
        // error opening file
        return -1;
    }

    string line;
    getline(datafile, line);
    int i;
    for(i=0;i<line.size();++i){
        if(isnumber(line[i])) break;
    }

    string numVariablesString = line.substr(i,string::npos);
    int numVariables = stoi(numVariablesString);

    map<int,int> count;

    vvi clauses;

    while(getline(datafile,line)){
        vi clause(3);
        stringstream ss(line);
        ss >> clause[0] >> clause[1] >> clause[2];
        if(clause[0]==-clause[1] || clause[0]==-clause[2] || -clause[1]==clause[2]) continue;
        for(auto &c:clause){
            count[(c<0?-c-1:c-1)]++;
        }
        clauses.pb(clause);
    }

    for(int i=0;i<numVariables;++i){
        cout << i << ": " << count[i] << endl;
    }
    auto start = chrono::high_resolution_clock::now();
    ppsz_3sat(clauses, numVariables);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
    cout << "Time: " << duration.count() << " microseconds ("<< minidur.count() <<") s\n";

    return 0;
}