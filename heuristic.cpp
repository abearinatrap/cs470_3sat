// Andrew Renninger, 2023, CS 570
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

const uint32_t STATE_BIT_SIZE = 128; // number of bits in state - change if number of variables increase

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    cout << "No solution found in heuristic." << endl;
    // cleanup and close up stuff here  
    // terminate program  

    exit(signum);  
}

int MASS_RUN=1;


//returns maximum saturation and number of microseconds taken
//ignore on normal execution
pair<int,ll> heuristic_3sat(const vvi& clauses, int num_vars, vector<int> setb) {
    auto start = chrono::high_resolution_clock::now();

    // slower than using int64 as indexing but allows for more variables
    bitset<STATE_BIT_SIZE> state_bits; //start with all false. allow for up to 128 variables
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count()); // rng
    /*
    //randomize initialization
    for(int i=0;i<num_vars;++i){
        state_bits[i] = rng() % 2;
    }
    */
    int numClauses = sz(clauses);
    ll num_iterations =  numClauses * num_vars * num_vars * num_vars;  
    // ^ Number of iterations to run the algorithm
    
    int maxSat = 0;
    for (ll iteration = 1; iteration <= num_iterations; iteration++) {
        int unsat_clause = -1;
        int satClauses = 0;
        // check clauses to see if all satisfied
        for (int i = 0; i < numClauses; i++) {
            bool clause_satisfied = false;
            for (int lit : clauses[i]) {
                int var = abs(lit) - 1;
                //see if literal is evaluated to be true using bitwise XOR
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

        // if all clauses are satisfied
        if (unsat_clause == -1) {
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
            if(MASS_RUN==1 or rng()%25==0){
                cout << "Solution:\n"; 
                for(int i=0;i<num_vars;++i){
                    cout << state_bits[i] << (i==(num_vars-1)?'\n':' ');
                }

                cout << "Time: " << duration.count() << " microseconds ("<< minidur.count() <<"ms)\n";
                
            }
            
            return {numClauses,duration.count()};
        } else {
            // else randomly choose variable to flip :/
            int var_to_flip = abs(clauses[unsat_clause][rng() % 3]) - 1;
            state_bits[var_to_flip].flip();
        }
    }

    // no solution has been found, return max sat and time taken
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
    if(MASS_RUN == 1 || rng()%25==0){
        cout << "No solution found in heuristic. Max sat: "<< maxSat << " out of " << numClauses << endl;
        cout << "Time: " << duration.count() << " microseconds ("<< minidur.count() <<"ms)\n";
    }
    //cout << duration.count() <<"\n";
    return {maxSat,duration.count()};
}

tuple<int,int,int> additem(vector<int> ar)
{   
    sort(all(ar));
    return make_tuple(ar[0],ar[1],ar[2]);
}

int main (int argc, char* argv[]) {
    signal(SIGINT, signalHandler);  
    if (argc < 2){
        // need input file
        cout << "./h.out <INPUT_FILE>" << endl;
        return -1;
    }
    if(argc==3){
        MASS_RUN = stoi(argv[2]);
    }

    ifstream datafile;
    datafile.open(argv[1]);
    
    if(!datafile.is_open()){
        // error opening file
        cout << "error opening file" << endl;
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
    // read clauses
    while(getline(datafile,line)){
        vi clause(3);
        if(line[0]=='$') break;
        if(isalpha(line[0])) continue;
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
    datafile.close();

    for(auto it : duplicates){ // go through duplicates and only add once
        clauses.pb({get<0>(it.first),get<1>(it.first),get<2>(it.first)});
    }

    for(int i=0;i<numVariables;++i){
        //cout << i << ": " << count[i] << endl;
    }

    // support for running the heuristic many times.
    int num=0;
    ll count=0;
    ll bcount=0;
    int iters = MASS_RUN;
    int cs = sz(clauses);

    
    
    
    if(iters == 1 ) {
        // normal execution, running heuristic once
        heuristic_3sat(clauses, numVariables, setbits);
    }
    else {
        // running heuristic many times
        for(int i=0; i< iters; ++i){
            auto d = heuristic_3sat(clauses, numVariables, setbits);
            if(d.first==cs){
                num++;
                count+=d.second;
            }else{
                bcount+=d.second;
            }
            /*
            if(heuristic_3sat(clauses, numVariables, setbits)==clauses.size()){
                ++num;
            }
            */
        }   
        cout << num << "/" << iters << " found solution.\n";
        cout << "Avg running time of solution find: "<<(double)count/(num) << "\n";
        //don't divide by zero
        if(iters!=num) {
            cout << "Avg running time of solution miss: " << ((double)bcount)/(iters-num) << "\n";
            cout << "Avg combined: " << ((double)count+bcount)/(num) << "\n";
        }
    }
    return 0;
}