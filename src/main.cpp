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

int get_mps(int, vector<int>&, vector<int>&);
int fill_table(int, int, vector<int>&, vector< vector<int> >&, vector< vector<int> >&);
int write(int, int, int, int, vector< vector<int> >&, vector< vector<int> >&);
void load_index(int, int, vector<int>&, vector< vector<int> >&, vector<int>&);

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
    int m = get_mps(sz, ep, mps_index);
    //////////// write the output file ///////////
    fout << m << endl;
    for(int i=0; i<mps_index.size(); i++) {
        fout << mps_index[i] << ' ' << ep[mps_index[i]] << endl;
    }
    fin.close();
    fout.close();
    return 0;
}

int get_mps(int sz, vector<int> &ep, vector<int> &mps_index) {
    mps_index.reserve(sz);
    vector<int> v_tmp(sz, -1);
    vector< vector<int> > M(sz, v_tmp);
    vector< vector<int> > S(M);
    int x = fill_table(0, sz-1, ep, M, S);
    load_index(0, sz-1, ep, S, mps_index);
    return x;
}

int fill_table(int i, int j, vector<int> &ep, vector< vector<int> > &M, vector< vector<int> > &S) { // return the number of chords in the subset
    if(M[i][j] != -1) {         // table filled before
        return M[i][j];
    } else if(i >= j) {         // base case
        return write(i, j, 0, -1, M, S);
    }
    int k=ep[j];
    if(k<i || k>j) {     // Condition 1: k not in (i,j)
        int m = fill_table(i, j-1, ep, M, S);
        return write(i, j, m, S[i][j-1], M, S);
    } else if(k==i) {    // Condition 2: kj = ij
        int m = 1 + fill_table(i, j-1, ep, M, S);
        return write(i, j, m, S[i][j-1]-ep.size()-1, M, S);
    } else {             // Condition 3: kj in (i,j)
        int m1 = fill_table(i, j-1, ep, M, S);   // counter used in the case (i, j-1)
        int m2 = fill_table(i, k-1, ep, M, S) + 1 + fill_table(k+1, j-1, ep, M, S);   // counter used in the case (i, k-1) kj (k+1, j-1)
        if(m1 > m2) {
            return write(i, j, m1, S[i][j-1], M, S);
        } else {
            return write(i, j, m2, j, M, S);
        }
    }
}

int write(int i, int j, int m, int s, vector< vector<int> > &M, vector< vector<int> > &S) {
    M[i][j] = m;
    S[i][j] = s;
    return m;
}

void load_index(int i, int j, vector<int> &ep, vector< vector<int> > &S, vector<int> &mps_index) {
    int s = S[i][j];
    if(s < -1) { // pass condition 2 along the way
        mps_index.push_back(i);
        s += ep.size()+1;
    }
    if(s >= 0) {
        j = s;  // go to where condition 3 happens
        int k = ep[j];
        load_index(i, k-1, ep, S, mps_index);
        mps_index.push_back(k);
        load_index(k+1, j-1, ep, S, mps_index);
    }
}
