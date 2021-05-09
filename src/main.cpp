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

class tree_node; // use tree nodes to get the subsets
int get_mps(int, vector<int>&, vector<int>&);
int fill_table(int, int, vector<int>&, vector< vector<int> >&, vector< vector<tree_node*> >&, vector<tree_node>&);
int write(int, int, int, tree_node*, vector< vector<int> >&, vector< vector<tree_node*> >&);
void traverse(tree_node*, vector<tree_node>&, vector<int>&);

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

class tree_node {
public:
    tree_node *lc;
    tree_node *rc;
    //tree_node *p;
    int key; // i
    tree_node():lc(NULL),rc(NULL),key(-1){};    // empty node constructor
    tree_node(int k):lc(NULL),rc(NULL),key(k){}; // non-empty node constructor
};


int get_mps(int sz, vector<int> &ep, vector<int> &mps_index) {
    mps_index.reserve(sz);
    vector<int> M_tmp(sz, -1);
    vector< vector<int> > M(sz, M_tmp);
    vector<tree_node*> N_tmp(sz, NULL);
    vector< vector<tree_node*> > N(sz, N_tmp);
    vector<tree_node> C;       // a container of the nodes
    C.reserve(sz);
    
    int x = fill_table(0, sz-1, ep, M, N, C);
    traverse(N[0][sz-1], C, mps_index);
    return x;
}

int fill_table(int i, int j, vector<int> &ep, vector< vector<int> > &M, vector< vector<tree_node*> > &N, vector<tree_node> &C) { // return the number of chords in the subset
    if(M[i][j] != -1) {         // table filled before
        return M[i][j];
    } else if(i >= j) {         // base case
        //cout << "base case " << i << ", " << j << endl;
        //tree_node n();
        //C.push_back(n);
        return write(i, j, 0, NULL, M, N);
    }
    int k=ep[j];
    if(k<i || k>j) {     // Condition 1: k not in (i,j)
        //cout << "(1) " << i << ", " << j << endl;
        int m = fill_table(i, j-1, ep, M, N, C);
        return write(i, j, m, N[i][j-1], M, N);
    } else if(k==i) {    // Condition 2: kj = ij
        //cout << "(2) " << i << ", " << j << endl;
        tree_node n(i);
        n.rc = N[i+1][j-1];
        C.push_back(n);
        int m = 1 + fill_table(i+1, j-1, ep, M, N, C);
        return write(i, j, m, &C.back(), M, N);
    } else {             // Condition 3: kj in (i,j)
        int m1 = fill_table(i, j-1, ep, M, N, C);   // counter used in the case (i, j-1)
        int m2 = fill_table(i, k-1, ep, M, N, C) + 1 + fill_table(k+1, j-1, ep, M, N, C);   // counter used in the case (i, k-1) kj (k+1, j-1)
        if(m1 > m2) {
            //cout << "(3a) " << i << ", " << j << endl;
            return write(i, j, m1, N[i][j-1], M, N);
        } else {
            //cout << "(3b) " << i << ", " << k << ", " << j << endl;
            tree_node n(k);
            n.lc = N[i][k-1];
            n.rc = N[k+1][j-1];
            C.push_back(n);
            return write(i, j, m2, &C.back(), M, N);
        }
    }
}

int write(int i, int j, int m, tree_node *t, vector< vector<int> > &M, vector< vector<tree_node*> > &N) {
    M[i][j] = m;
    N[i][j] = t;
    return m;
}

void traverse(tree_node *r, vector<tree_node> &C, vector<int> &list) {
    if(r!=NULL) {
        traverse(r->lc, C, list);
        list.push_back(r->key);
        traverse(r->rc, C, list);
    }
}
