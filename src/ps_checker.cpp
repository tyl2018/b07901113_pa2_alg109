//
//  ps_checker.cpp
//  Used to check the output file. Do not check maximum or not
//
//  Created by Tania Liao on 2021/5/5.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void help_message() {
    cout << "usage: ps_checker <input_file>" << endl;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        help_message();
        return 0;
    }
    
    // read
    
    int n;
    fstream fin(argv[1]);
    fin >> n;
    vector<int> temp(2, -1);
    vector< vector<int> > C;
    while (fin >> temp[0] >> temp[1]) {
        C.push_back(temp);
    }
    
    // check
    if(n != C.size()) {
        cout << "Invalid: number of chords doesn't match" << endl;
        return 0;
    }
    int last_index = -1;
    for(int i=0; i<n; i++) {
        if(C[i][0] >= C[i][1]) {
            cout << "Invalid: chord " << i << " has reversed endpoints" << endl;
            return 0;
        }
        
        if(C[i][0] <= last_index) {
            cout << "Invalid: chord " << i << " doesn't come in order" << endl;
            return 0;
        }
        last_index = C[i][0];
        
        for(int j=0; j<i; j++) {
            if( (C[j][1]-C[i][0]) * (C[j][1]-C[i][1]) < 0) {
                cout << "Invalid: chord " << j << " and chord " << i << " cross" << endl;
                return 0;
            }
        }
    }
    cout << "Valid file" << endl;
    return 0;
}
