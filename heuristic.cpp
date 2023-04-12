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
#define all(a) a.begin(),a.end()

const uint64_t MAX_WAIT_TIME = 10; //what is the maximum time we will wait for the algorithm to run before giving up. In seconds
// set to 0 if wait until find solution

const uint32_t STATE_BIT_SIZE = 128; // number of bits in state - change if number of variables increase

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    cout << "No solution found in heuristic." << endl;
    // cleanup and close up stuff here  
    // terminate program  

    exit(signum);  
}

void heuristic_3sat(const vvi& clauses, int num_vars, vector<int> setb) {
    // implementation of 
    auto start = chrono::high_resolution_clock::now();

    // slower than using int64 as indexing but allows for more variables
    bitset<STATE_BIT_SIZE> state_bits; //start with all false. allow for up to 128 variables
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count()); // rng
    /*
    for(int i=0;i<num_vars;++i){
        state_bits[i] = rng() % 2;
    }
    */

    int num_iterations = 4 * num_vars * num_vars;  // Number of iterations to run the algorithm before checking if over time
    int numClauses = sz(clauses);
    int maxSat = 0;
    for (int iteration = 1; iteration <= num_iterations; iteration++) {
        int unsat_clause = -1;
        int satClauses = 0;
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
                unsat_clause = i;
            }else{
                ++satClauses;
            }
        }

        maxSat = max(maxSat, satClauses);


        if (unsat_clause == -1) {
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
            int var_to_flip = abs(clauses[unsat_clause][rng() % 3]) - 1;
            state_bits[var_to_flip].flip();
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
    cout << "No solution found in heuristic. Max sat: "<< maxSat << " out of " << numClauses << endl;
    cout << "Time: " << duration.count() << " microseconds ("<< minidur.count() <<"ms)\n";
}

tuple<int,int,int> additem(vector<int> ar)
{   
    sort(all(ar));
    return make_tuple(ar[0],ar[1],ar[2]);
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
    if(numVariables>STATE_BIT_SIZE){
        cout << "Need to recompile with larger state size (STATE_BIT_SIZE), current size in bits is " << STATE_BIT_SIZE << endl;
        return -1;
    }

    vvi clauses;

    map<tuple<int,int,int>,int> duplicates;

    vector<int> setbits;

    while(getline(datafile,line)){
        vi clause(3);
        stringstream ss(line);
        ss >> clause[0] >> clause[1] >> clause[2];
        // this will always be true, don't add to clauses
        if(clause[0]==-clause[1] || clause[0]==-clause[2] || -clause[1]==clause[2]) continue;
        // if all three are same, bit must be set
        if(clause[0]==clause[1] && clause[1]==clause[2]){
            setbits.pb(clause[0]-1);
        }
        duplicates[additem(clause)]++;
    }

    for(auto it : duplicates){ // go through duplicates and only add once
        clauses.pb({get<0>(it.first),get<1>(it.first),get<2>(it.first)});
    }

    for(int i=0;i<numVariables;++i){
        //cout << i << ": " << count[i] << endl;
    }
    heuristic_3sat(clauses, numVariables, setbits);

    return 0;
}