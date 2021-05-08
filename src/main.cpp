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
int find_subset(int, int, vector<int>&, vector<int>&);

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
    return find_subset(0, sz-1, ep, mps_index);
}

int find_subset(int i, int j, vector<int> &ep, vector<int> &v) { // return the number of chords in the subset
    if(i >= j) {         // base case
        return 0;
    }
    int k=ep[j];
    if(k<i || k>j) {     // Condition 1: k not in (i,j)
        return find_subset(i, j-1, ep, v);
    } else if(k==i) {    // Condition 2: kj = ij
        v.insert(v.end(), i);
        return 1 + find_subset(i+1, j-1, ep, v);
    } else {             // Condition 3: kj in (i,j)
        vector<int> v_tmp(v);                      // vector used in the case (i, j-1)
        int c1 = find_subset(i, j-1, ep, v_tmp);   // counter used in the case (i, j-1)
        int c2 = find_subset(i, k-1, ep, v) + 1;   // counter used in the case (i, k-1) kj (k+1, j-1)
        v.insert(v.end(), k);
        c2 += find_subset(k+1, j-1, ep, v);
        if(c1 > c2) {
            v = move(v_tmp);
            return c1;
        } else {
            return c2;
        }
    }
}

