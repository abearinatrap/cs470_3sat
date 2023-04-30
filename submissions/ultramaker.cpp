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

int MASS_RUN=1000;

typedef std::shared_mutex Lock;
typedef std::unique_lock< Lock >  WriteLock;
typedef std::shared_lock< Lock >  ReadLock;

vvi bestClauses;
int minNum=MASS_RUN;

int rmin(){
    ReadLock r_lock(Lock);
    return minNum;
}

void wmin(int min,vvi &c){
    WriteLock w_lock(Lock);
    minNum=min;
    bestClauses = vvi(c);
}

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    cout << "No solution found in heuristic." << endl;
    // cleanup and close up stuff here  
    // terminate program  
    for(auto c: bestClauses){
        for(auto n:c){
            cout << n << " ";
        }
        cout << endl;
    }
    exit(signum);  
}

int heuristic_3sat(const vvi& clauses, int num_vars, vector<int> setb) {
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

    ll num_iterations =  10 * num_vars * num_vars;  // Number of iterations to run the algorithm before checking if over time
    int numClauses = sz(clauses);
    int maxSat = 0;
    for (ll iteration = 1; iteration <= num_iterations; iteration++) {
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
            if(MASS_RUN==1){
                cout << "Solution:\n"; 
                for(int i=0;i<num_vars;++i){
                    cout << state_bits[i] << (i==(num_vars-1)?'\n':' ');
                }

                cout << "Time: " << duration.count() << " microseconds ("<< minidur.count() <<"ms)\n";
                
            }
            
            return numClauses;
        } else {
            // randomly choose variable to flip :/
            int var_to_flip = abs(clauses[unsat_clause][rng() % 3]) - 1;
            state_bits[var_to_flip].flip();
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    auto minidur = chrono::duration_cast<chrono::milliseconds> (end - start);
    //cout << "No solution found in heuristic. Max sat: "<< maxSat << " out of " << numClauses << endl;
    //cout << duration.count() <<"\n";
    return maxSat;
}




int getNum(int numClauses,int numVariables, int numHard){
    vvi clauses(numClauses,vi(3));
    vector<int> setbits{0};
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    for(int i=0;i<numClauses;++i){
        int v,f;
        for(int j=0;j<2;++j){
            v=rng() % numVariables;
            f=rng() % 2;
            if(f){
                v=-v;
            }
            clauses[i][j] = v;
        }
        int v2 = rng() % numHard;
        if (v2!=0){
            v=rng() % numVariables;
            f=rng() % 2;
            if(f){
                v=-v;
            }
            clauses[i][2] = v;
        }else{
            clauses[i][2]=v;
        }
    }

    int iters = MASS_RUN;
    int cs = numClauses;
    int num=0;
    ll count=0;
    ll bcount=0;
    for(int j=0; j< iters; ++j){
        auto d = heuristic_3sat(clauses, numVariables, setbits);
        if(d==cs){
            num++;
        }
        /*
        if(heuristic_3sat(clauses, numVariables, setbits)==clauses.size()){
            ++num;
        }
        */
    }
    int minNum = rmin();
    if(num!= 0 && num<minNum){
        wmin(num,clauses);
        cout << num << endl;
        return num;
    }
    return 0;
}

int main() {
    signal(SIGINT, signalHandler);  
    int numVariables = 20;
    int numClauses = 70;
    int numHard = 14;
    double best = 1;
    srand(time(0));

    for(int a=0;a<10'000'000;++a){
        vector<thread> n(10,thread(getNum, numClauses,numVariables,numHard));
        for(int i=0;i<10;++i ){
            n[i].join();
        }
    }
    
    for(auto c: bestClauses){
        for(auto n:c){
            cout << n << " ";
        }
        cout << endl;
    }
    
}