#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h> // for colored text in console

using namespace std;

// liste di adiacenza
constexpr auto MAXN = 1000; // massimo numero di nodi
vector<int> adj[MAXN];
int N, M; // numero di nodi e numero di archi

void leggiGrafo(string filename){
    std::ifstream fin(filename);
    fin >> N >> M; // prima riga
    // successive M righe
    for (int i = 0; i < M; i++){
        int u, v;
        fin >> u >> v;  // coppia non ordinata
        adj[u].push_back(v);
//        adj[v].push_back(u); // for non oriented graph, uncomment this line
    }
}

void stampaGrafo() {
    cout << "liste di adiacenza" << endl;
    for (auto u = 0; u < N; u++) {
        cout << u << ": ";
        for (auto v : adj[u])
          cout << v << " ";
        cout << endl;
    }
    cout << endl;
}

void spaceOut(int i) {
    if(i == 0) return; // if the depth is 0, return
    cout << "\t";
    for (int j = 1; j < i; j++) {
        cout << "*\t"; // print tab spaces
    }
}

void printEdge(int edgeKind, int fromNode, int toNode, int depth) {
    // 0 = tree edge, 1 = back edge, 2 = cross edge
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    switch (edgeKind) {
        case 0: // tree edge
            SetConsoleTextAttribute(hConsole, 10); // green color
            spaceOut(depth); // print the indentation based on recursion depth
            cout << "tree edge from " << fromNode << " to " << toNode << endl;
            break;
        case 1: // back edge
            SetConsoleTextAttribute(hConsole, 12); // red color
            spaceOut(depth); // print the indentation based on recursion depth
            cout << "back edge from " << fromNode << " to " << toNode << endl;
            break;
        case 2: // cross edge
            SetConsoleTextAttribute(hConsole, 14); // yellow color
            spaceOut(depth); // print the indentation based on recursion depth
            cout << "cross edge from " << fromNode << " to " << toNode << endl;
            break;
    }
    SetConsoleTextAttribute(hConsole, 15); // reset color to white
}






void dfs(int v, vector<bool>& visited, vector<bool>& allDone, vector<int>& depth, int& recursionAmount, int& loopCount) {

    if(depth[v] == -1) { // if the node is not visited
        depth[v] = recursionAmount; // set the depth of the node
        recursionAmount++; // increment the recursion depth
    }
    // DFS function to traverse the graph
    visited[v] = true; // mark the current node as visited


    for (auto i : adj[v]) { // iterate through all adjacent nodes
        if (!visited[i]) { // if the node is not visited

            printEdge(0, v, i, depth[v]); // print the tree edge

            dfs(i, visited, allDone, depth, recursionAmount, loopCount); // recursively call dfs on the adjacent node

        } else {
            if (!allDone[i]) { // if the node is already visited but not done

                printEdge(1, v, i, depth[v]); // print the back edge
                loopCount++; // increment the loop counter

            } else {

                printEdge(2, v, i, depth[v]); // print the cross edge

            }
        }
    }

    allDone[v] = true; // mark the current node as done
    recursionAmount--; // decrement the recursion depth
}


void scan(){
    cout << "DFS Traversal: " << endl << endl;
    
    vector<bool> visited(N, false); // create a visited array to keep track of visited nodes
    vector<bool> allDone(N, false); // create a done array to keep track of all nodes that are done
    vector<int> depth(N, -1); // create a depth array to keep track of the depth of each node

    int recursionAmount = 0; // variable to keep track of the recursion depth
    int loopCounter = 0; // variable to keep track of the number of loops

    for (int i = 0; i < N; i++) { // iterate through all nodes
        if (!visited[i]) { // if the node is not visited
            dfs(i, visited, allDone, depth, recursionAmount, loopCounter); // call dfs on the node
        }
    }
    cout << endl;

    cout << "DFS Traversal completed." << endl; // print the completion message
    if (loopCounter > 0) {
        cout << "There's " << loopCounter << " loops in the graph." << endl; // print the number of loops
    } else {
        cout << "There are no loops in the graph." << endl; // print the no loops message
    }
    cout << endl;
}


int main(){

    leggiGrafo("inputDiretto.txt"); // read the graph from the file

    stampaGrafo(); // print the adjacency list

    scan(); // call the scan function to perform DFS traversal

    return 0;
}

