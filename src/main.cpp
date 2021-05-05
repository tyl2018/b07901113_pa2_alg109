//
//  main.cpp
//  Main program. compile to get mps
//
//  Created by Tania Liao on 2021/5/3.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void get_mps(int, vector<int>, vector<int>&);
int fill_table(int, vector<int>, vector< vector<int> >&, vector< vector<int> >&);
void find_subset(int, int, vector<int>, vector< vector<int> >, vector<int>&);

void help_message() {
    cout << "usage: mps <input_file> <output_file>" << endl;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        help_message();
        return 0;
    }
    
    //////////// read the input file /////////////
    
    int sz;
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    fin >> sz;
    int num1, num2;
    //vector<int> ep;
    vector<int> ep(sz,-1); // i and data[i] share the same chord
    while (fin >> num1 >> num2) {
        //ep.push_back(num1);
        //ep.push_back(num2);
        ep[num1] = num2;
        ep[num2] = num1;
    }
    //////////// find mps ///////////
    vector<int> mps_index;
    get_mps(sz, ep, mps_index);
    //////////// write the output file ///////////
    fout << mps_index.size() << endl;
    for(int i=0; i<mps_index.size(); i++) {
        fout << mps_index[i] << ' ' << ep[mps_index[i]] << endl;
    }
    fin.close();
    fout.close();
    return 0;
}



void get_mps(int sz, vector<int> ep, vector<int> &mps_index) {
    vector<int> tmp_vec;
    tmp_vec.assign(sz, 0);
    vector< vector<int> > M(sz, tmp_vec);
    tmp_vec.assign(sz, -1);
    vector< vector<int> > Cond(sz, tmp_vec);
    int m = fill_table(sz, ep, M, Cond);
    find_subset(0, sz-1, ep, Cond, mps_index);
}

int fill_table(int sz, vector<int> ep, vector< vector<int> > &M, vector< vector<int> > &Cond) {
    cout << "Filling table" << endl;
    for(int d=1; d<sz; d++) {
        for(int i=0; i<sz-d; i++) {
            int j=i+d;
            int k=ep[j];
            if(k<i || k>j) {    // k not in (i,j)
                M[i][j] = M[i][j-1];
                Cond[i][j] = 0;
            } else if(k==i) {   // kj = ij
                M[i][j] = 1 + M[i+1][j-1];
                Cond[i][j] = 1;
            } else {            // kj in (i,j)
                M[i][j] = M[i][k-1] + 1 + M[k+1][j-1];
                if(M[i][j-1] > M[i][j]) {
                    M[i][j] = M[i][j-1];
                    Cond[i][j] = 0;
                } else {
                    Cond[i][j] = 2;
                }
            }
        }
    }
    return M[0][sz-1];
    cout << "done." << endl;
}

void find_subset(int i, int j, vector<int> ep, vector< vector<int> > Cond, vector<int> &v) {
    cout << ".";
    if(Cond[i][j] != -1) {
        while(Cond[i][j] == 0) {
            j--;
        }
        if(Cond[i][j] == 1) {   // ij is in mps
            v.push_back(i);
            find_subset(i+1, j-1, ep, Cond, v);
        } else if(Cond[i][j] == 2) {
            int k = ep[j];
            find_subset(i, k-1, ep, Cond, v);
            v.push_back(k);
            find_subset(k+1, j-1, ep, Cond, v);
        }
    }
}

