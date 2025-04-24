#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// liste di adiacenza
constexpr auto MAXN = 1000; // massimo numero di nodi
vector<int> adj[MAXN];
int N, M; // numero di nodi e numero di archi

/*
8 11
0 1
1 2
0 6
6 5
1 7
2 7
7 6
2 3
3 4
4 5
7 4
*/


void leggiGrafoNonOrdinato(string filename){
    std::ifstream fin(filename);
    fin >> N >> M; // prima riga
    // successive M righe
    for (int i = 0; i < M; i++){
        int u, v;
        fin >> u >> v;  // coppia non ordinata
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // nota: deg(u) = adj[u].size()
}

void stampaGrafo() {
    cout << "GRAFO (non orientato)\n\n";
    cout << "liste di adiacenza" << endl;
    for (auto u = 0; u < N; u++) {
        cout << u << ": ";
        for (auto v : adj[u])
          cout << v << " ";
        cout << endl;
    }
    cout << endl;
}

int main(){
    leggiGrafoNonOrdinato("input.txt");

    stampaGrafo();
    return 0;
}
 