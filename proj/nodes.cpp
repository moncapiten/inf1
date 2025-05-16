#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;



unordered_map<string, int> substitutionIndeces; // map to hold the names of the nodes and their indices
unordered_map<int, string> substitutionNames; // map to hold the indices of the nodes and their names

void updateMaps(string name, int index){
    substitutionIndeces[name] = index; // add the name to the map with the index of the node
    substitutionNames[index] = name; // add the index to the map with the name of the node
}



struct BayesianNode {
    string name; // name of the node
    int ID; // ID of the node
    vector<int> parents; // parents of the node
    vector<int> children; // children of the node
    vector<string> states; // states of the node
    vector<double> probabilities; // probabilities of the node
    vector<double> pureProb;
    bool state;

    BayesianNode(string n, vector<int> p, vector<int> c, vector<string> s, vector<double> prob) {
        name = n; // initialize the name of the node
        parents = p; // initialize the parents of the node
        children = c; // initialize the children of the node
        states = s; // initialize the states of the node
        probabilities = prob; // initialize the probabilities of the node
    }

    BayesianNode(string n) {
        name = n; // initialize the name of the node
    }

};




// Overload for a single node
ostream& operator<<(ostream& os, const BayesianNode& node) {
    os << "Node: " << node.name << " alias " << node.ID << '\n';

    os << "  Parents: ";
    for (const auto& p : node.parents) os << substitutionNames[p] << ' ';
//    for (const auto& p : node.parents) os << p << ' ';
    os << '\n';

    os << "  Children: ";
    for (const auto& c : node.children) os << substitutionNames[c] << ' ';
//    for (const auto& c : node.children) os << c << ' ';
    os << '\n';

    os << "  States: ";
    for (const auto& s : node.states) os << s << ' ';
    os << " [0, 1]";
    os << '\n';

    os << "  Probabilities: ";
    for (const auto& prob : node.probabilities) os << prob << ' ';
    os << "\n";

    os << "  Pure Probabilities: ";
    for (const auto& prob : node.pureProb) os << prob << ' ';
    os << "\n";

    return os;
}


// Overload for a vector of nodes
ostream& operator<<(ostream& os, const vector<BayesianNode>& nodes) {
    for (const auto& node : nodes) {
        os << node << '\n';
    }
    return os;
}