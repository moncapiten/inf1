#include <iostream>
#include <vector>

using namespace std;

vector<int> sum1(vector<int> & A){
    int max = 0;
    int init = 0;
    int end = 0;
    for(int i = 0; i < A.size(); i++){
        for(int j = i; j < A.size(); j++){
            int sommaParziale = 0;
            for(int k = i; k <= j; k++){
                sommaParziale += A[k];
            }
            if(sommaParziale > max){
                max = sommaParziale;
                init = i;
                end = j;
            }
        }
    }
//    return somma;
    return vector<int>{init, end, max};
}

int sum2(vector<int> & A){
    int somma = 0;
    for(int i = 0; i < A.size(); i++){
        for(int j = i; j < A.size(); j++){
            int sommaParziale = 0;
            sommaParziale += A[j];
            if(sommaParziale > somma){
                somma = sommaParziale;
            }
        }
    }
    return somma;
}

int random(int min, int max){
    return min + rand() % (max - min + 1);
}


int main() {
    constexpr auto N = 5;
    constexpr auto MIN = -10;
    constexpr auto MAX = 10;

    vector<int> A;
    for(auto i = 0; i < N; i++){
        A.push_back(random(MIN, MAX));
    }

//    vector<int> B = sum1(A);
//    for( auto x : B){
//        cout << x << " ";
//    }

    cout << "Il subarray di somma massima inizia in posizione " << sum1(A)[0] << " e finisce in posizione " << sum1(A)[1] << " con somma " << sum1(A)[2] << endl;
    return 0;
}