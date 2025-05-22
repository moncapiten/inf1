#include <iostream>
#include <vector>

using namespace std;

vector<int> allZeroes(const vector<int>& vec) {
    vector<int> zeroes(vec.size(), 0);
    return zeroes;
}


int maxStates(int i) {
    switch(i) {
        case 0: return 3;
        case 1: return 2;
        case 2: return 4;
        default: return 1; // Default case if i is out of range
    }
}



int main() {

    int parentSize = 3;
    vector<int> vec(parentSize);



    do{



        cout << "Current vector: ";
        for( int i = 0; i < parentSize; i++){
            cout << vec[i] << " ";
        }
        cout << endl;



        bool rollover = false;
        for( int i = 0; i < parentSize; i++){
            
            if( rollover || i==0 ) vec[i]++;

            rollover = false;
            if(vec[i] > maxStates(i)-1){
                vec[i] = 0;
                rollover = true;
            }
        }


        

    }while( vec != allZeroes(vec));


    cout << "Hello, World!" << endl;
    return 0;
}