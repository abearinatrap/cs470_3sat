#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define vvi vector<vi> 
#define pb push_back
#define ll long long
#define ull unsigned ll
#define vull vector<ull>

int check(ull state, vvi &clauses){
    int numC = 0;
    for(auto c: clauses){
        int i;

        for(i=0;i<3;++i){

            ull mask;
            if(c[i]>0){
                mask = 1 << c[i]-1;
                mask = mask & state;
                if(mask!=0){
                    numC++;
                    break;
                }
            }else {
                mask = 1 << (-c[i])-1;
                mask = mask & state;
                if(mask==0){
                    numC++;
                    break;
                }
            }
        }
        if(i==3){
            // clause failed
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

    /*
    const string DATA_START = "3-SAT";
    
    while(getline(datafile,line) && line != DATA_START){

    }

    if(line != DATA_START){
        // 3 sat data not found in data file
        return -1;
    }
    */
    getline(datafile, line);
    int i;
    for(i=0;i<line.size();++i){
        if(isnumber(line[i])) break;
    }

    string numVariablesString = line.substr(i,string::npos);
    int numVariables = stoi(numVariablesString);
    
    while(getline(datafile,line)){
        vi clause(3);
        stringstream ss(line);
        ss >> clause[0] >> clause[1] >> clause[2];
        clauses.pb(clause);
    }
    
    //max size of 64 variables 
    ull iterations = (ull)1 << numVariables;
    if(numVariables==64){
        iterations = (ull)0;
        --iterations;
    }

    int numClauses = (int)clauses.size();
    cout << numClauses << endl;
    auto start = chrono::high_resolution_clock::now();
    int maxsat = 0;
    int imax = -1;

    for(ull i=0;i<iterations;++i){
        int sat = check(i, clauses);
        
        if(sat > maxsat){
            imax=i;
            maxsat=sat;
        }
       //cout << i << sat << "\n";
        if(sat >= numClauses){
            auto end = chrono::high_resolution_clock::now();
            auto duration = duration_cast<chrono::microseconds>(end - start);
            cout << "Time: " << duration.count() << " microseconds ("<< duration_cast<chrono::milliseconds>(end - start) <<") s\n";

            cout << "Satisfied by: \n";
            for(int j=0;j<numVariables;++j){
                //cout << j+1 << ": ";
                ull mask = 1 << j;
                mask = mask & i;
                if(mask==0){
                    cout << "0 ";
                }else{
                    cout << "1 ";
                }
                //cout << (mask!=0?"true":"false") <<"\n"/*(j<numVariables-1)?", ":"\n"*/;
            }
            cout << endl;
            return 0;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = duration_cast<chrono::microseconds>(end - start);
    cout << "Time: " << duration.count() << "ms\n";
    cout << "No solution. Max: "<<maxsat << " at " << imax << endl;
    return 0;
}