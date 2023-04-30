#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define vvi vector<vi> 
#define pb push_back
#define ll long long
#define ull unsigned ll
#define vull vector<ull>

int check(vvi &clauses, ll state){
    // while bitset allows for more variables, is ~15% slower than tested with
    int numC = 0;
    for(auto c: clauses){
        int i;
        for(i=0;i<3;++i){
            ll mask;
            
            int bar = abs(c[i]) - 1;
            mask = 1 << bar;
            mask = mask & state;
            // if(mask){
            //     numC++;
            //     break;
            // }
            if(c[i]>0){
                if(mask!=0){
                    numC++;
                    break;
                }
            }else {
                if(mask==0){
                    numC++;
                    break;
                }
            }
        }
    }

    return numC;
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

    vvi clauses;
    vvi special_clauses;

    string line;
    getline(datafile, line);
    
    int i; //get number of variables
    for(i=0;i<line.size();++i){
        if(isnumber(line[i])) break;
    }
    string numVariablesString = line.substr(i,string::npos);
    int numVariables = stoi(numVariablesString);
    ll iterations;
    
    if(numVariables>sizeof(iterations)*8){
        cout << "Need to recompile with larger state datatype, current size in bits is " << sizeof(iterations)*8 << endl;
        return -1;
    }
    
    while(getline(datafile,line)){ // read clauses in
        vi clause(3);
        if(line[0]=='$') break;
        if(isalpha(line[0])) continue;
        stringstream ss(line);
        ss >> clause[0] >> clause[1] >> clause[2];
        clauses.pb(clause);
    }
    datafile.close();
    
    //max size of 128 variables 
    iterations = (ll)1 << numVariables;

    if(numVariables==64){
        iterations = (ll)0;
        --iterations;
    }

    int numClauses = (int)clauses.size();
    //cout << numClauses << endl;
    auto start = chrono::high_resolution_clock::now();
    int maxsat = 0;
    int imax = -1;

    for(ll i=0;i<iterations;++i){
        int sat = check(clauses, i);
        
        maxsat = max(maxsat,sat);
       //cout << i << sat << "\n";
        if(sat >= numClauses){
            auto end = chrono::high_resolution_clock::now();
            auto duration = duration_cast<chrono::microseconds>(end - start);

            cout << "Satisfied by: \n";
            for(int j=0;j<numVariables;++j){
                ull mask = 1 << j;
                mask = mask & i;
                if(mask==0){
                    cout << "0 ";
                }else{
                    cout << "1 ";
                }
            }
            cout << endl << "Time: " << duration.count() << " microseconds ("<< duration_cast<chrono::milliseconds>(end - start).count() <<"ms)\n";
            return 0;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = duration_cast<chrono::microseconds>(end - start);
    cout << "No solution. Max sat: "<<maxsat << " out of " << numClauses << endl;
    cout << "Time: " << duration.count() << "microseconds\n";
    return 0;
}