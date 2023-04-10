#include <bits/stdc++.h>

using namespace std;

// code reductions I normally use
#define vi vector<int>
#define vvi vector<vi> 
#define pb push_back
#define ll long long
#define ull unsigned ll
#define vull vector<ull>
#define sz(a) (int)a.size()

const uint64_t MAX_WAIT_TIME = 0; //what is the maximum time we will wait for the algorithm to run before giving up. In seconds
// set to 0 if wait until find solution

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    cout << "No solution found in heuristic." << endl;
    // cleanup and close up stuff here  
    // terminate program  

    exit(signum);  
}

void ppsz_3sat(const vvi& clauses, int num_vars) {
    // implementation of 
    auto start = chrono::high_resolution_clock::now();

    // slower than using int64 as indexing but allows for more variables
    bitset<128> state_bits; //start with all false. allow for up to 128 variables
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    int num_iterations = 10 * num_vars * num_vars;  // Number of iterations to run the algorithm before checking if over time
    int numClauses = sz(clauses);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    while(duration.count() < MAX_WAIT_TIME || MAX_WAIT_TIME == 0){
        for (int iteration = 1; iteration <= num_iterations; iteration++) {
            int unsatisfied_clause = -1;

            for (int i = 0; i < numClauses; i++) {
                bool clause_satisfied = false;
                for (int lit : clauses[i]) {
                    int var = abs(lit) - 1;
                    bool value = state_bits[var] ^ (lit < 0);
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
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
                cout << "Solution:\n"; 
                for(int i=0;i<num_vars;++i){
                    cout << state_bits[i] << (i==(num_vars-1)?'\n':' ');
                }

                cout << "Time: " << duration.count() << " microseconds ("<< minidur.count() <<"ms)\n";
                return;
            } else {
                // randomly choose variable to flip :/
                int var_to_flip = abs(clauses[unsatisfied_clause][rng() % 3]) - 1;
                state_bits[var_to_flip].flip();
            }
        }
        auto end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::seconds>(end - start);
        auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
    }
    cout << "No solution found in heuristic" << endl;
}

int main (int argc, char* argv[]) {
    signal(SIGINT, signalHandler);  
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
        //cout << i << ": " << count[i] << endl;
    }
    ppsz_3sat(clauses, numVariables);

    return 0;
}