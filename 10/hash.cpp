/*
da frequenza lunga ➔ numero piccolo

fornisco trasformazione che mi da da U ➔ h appartenente a [0, m-1]
esempio modulo, ma ho problema di collisione poiché spazio di partenza è molto più grande di quello di arrivo

si veda hash universale
h(x) = ((ax + b) mod p) mod m con a, b scelti casualmente da [1, p-1] e p appartenente a [m, 2m-1] primo

con a e b corretti si ottiene distribuzione uniforme anche con distribuzioni iniziale elevate, e probabilità di collisione viene ridotta a 1/m


se un file molto lungo e voglio fare hash su stringhe, posso fare hash su blocchi di stringhe e XOR dei risultati finche non ho un solo risultato, poi hash finale


*/

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class HashMap {
    private:
        vector<vector<int>> table;
        int m;
        int p;
        int a,b;

        int randomInt(int min, int max){
            return min + rand() % (max - min + 1);
        }

        int h(int x) {
            return ((static_cast<long>(a)*x + b) % p) % m;      // static_cast<long> necessary to avoid overflow from 32*32 bit into 32 bit, long is 64 bit and module brings it back to 32 bit
        }

    public:
        
        HashMap(int m): m(m), table(m) {
            p = 131;                    // 131 is prime, to be removed and replaced with a prime number in [m, 2m-1] calculated in the constructor programmatically
            a = randomInt(1, p-1);
            b = randomInt(1, p-1);
        }


        bool searchValue(int x) { 
            return find(table[h(x)].begin(), table[h(x)].end(), x) != table[h(x)].end();        // find returns an iterator to the element if found, otherwise to the end of the vector
        }

        bool searchWithKey(int x, int i) {
            return find(table[i].begin(), table[i].end(), x) != table[i].end();                 // same as above but avoids calculating h(x) again
        }


        void insert(int x) {
            int position = h(x);
            if(!searchWithKey(x, position)) table[position].push_back(x);
        }

        void remove(int x) {
            int position = h(x);
            auto it = find(table[position].begin(), table[position].end(), x);
            if(it != table[position].end()) table[position].erase(it);
        }

        friend ostream& operator<<(ostream& os, const HashMap& map) {
            for(int i = 0; i < map.m; i++) {
                os << i << ": ";
                for(int x : map.table[i]) os << x << " ";
                os << endl;
            }
            return os;
        }
};


int main(){
    HashMap map(10);

    map.insert(1);
    map.insert(2);
    map.insert(3);
    map.insert(4);
    map.insert(5);
    map.insert(6);
    map.insert(7);
    map.insert(8);
    map.insert(9);
    map.insert(10);
    map.insert(11);
    map.insert(12);
    map.insert(13);
    map.insert(14);
    map.insert(15);
//    map.remove(2);


    cout << map << endl;

    map.remove(8);

    cout << map << endl;


    return 0;
}

