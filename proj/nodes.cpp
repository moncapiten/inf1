#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
#include <string>
#include <unordered_map>
//#include "supportFunc.hpp"
//#include "supportFunc.cpp"

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








vector<int> getParentStateIndices(int counter, const vector<int>& parentIDs, const vector<BayesianNode>& nodes) {
    vector<int> indices;
    int numParents = parentIDs.size();
    indices.reserve(numParents);
    
    for (int i = 0; i < numParents; ++i) {
        int subsequentStates = 1;
        for (int j = i + 1; j < numParents; ++j) {
            subsequentStates *= nodes[parentIDs[j]].states.size();
        }
        indices.push_back((counter / subsequentStates) % nodes[parentIDs[i]].states.size());
    }
    
    return indices;
}







// Implementation of the Kahn algorithm for topological sorting
// It is made to work so that no node will ever appear before its parents
// therefore the ALREAEDY WRITTEN marginalization function will work
// cause the fact that a node's CPT can be defined before its parents
// still BOGGLES MY MIND, FABIO CUZMAN YOU SHOULD BE ASHAMED OF YOURSELF FOR BUILDING SUCH A FILE DEFINITION
 


void topologicalSort(vector<BayesianNode>& nodes) {
    int n = nodes.size();
    unordered_map<int, int> inDegree;
    unordered_map<int, BayesianNode*> nodeMap;

    for (auto& node : nodes) {
        nodeMap[node.ID] = &node;
        inDegree[node.ID] = 0;
    }

    for (auto& node : nodes) {
        for (int child : node.children)
            inDegree[child]++;
    }

    queue<int> q;
    for (auto& [id, deg] : inDegree) {
        if (deg == 0)
            q.push(id);
    }

    vector<BayesianNode> sorted;
    while (!q.empty()) {
        int id = q.front();
        q.pop();
        BayesianNode* node = nodeMap[id];
        sorted.push_back(*node);

        for (int child : node->children) {
            inDegree[child]--;
            if (inDegree[child] == 0)
                q.push(child);
        }
    }

    if (sorted.size() != nodes.size()) {
        throw runtime_error("Cycle detected in graph");
    }

    // Map old ID to new index
    unordered_map<int, int> idToNewIndex;
    for (int i = 0; i < sorted.size(); ++i)
        idToNewIndex[sorted[i].ID] = i;

    // Reassign nodes, fix ID
    for (int i = 0; i < sorted.size(); ++i) {
        sorted[i].ID = i;
    }

    // Clear and rebuild parents and children with remapped indices
    for (auto& node : sorted) {
        vector<int> newParents;
        for (int pid : node.parents)
            newParents.push_back(idToNewIndex[pid]);
        node.parents = move(newParents);
    }

    // Clear all children to rebuild consistently
    for (auto& node : sorted)
        node.children.clear();

    for (int i = 0; i < sorted.size(); ++i) {
        for (int pid : sorted[i].parents) {
            sorted[pid].children.push_back(i);

        }
    }

    nodes = move(sorted);
}






/*
void topologicalSort(vector<BayesianNode>& nodes) {
    int n = nodes.size();
    unordered_map<int, int> inDegree;
    unordered_map<int, BayesianNode*> nodeMap;

    // Build in-degree map and node map
    for (auto& node : nodes) {
        nodeMap[node.ID] = &node;
        inDegree[node.ID] = 0; // initialize
    }

    for (auto& node : nodes) {
        for (int child : node.children)
            inDegree[child]++;
    }

    queue<int> q;
    for (auto& [id, deg] : inDegree) {
        if (deg == 0)
            q.push(id);
    }

    vector<BayesianNode> sorted;
    while (!q.empty()) {
        int id = q.front();
        q.pop();
        BayesianNode* node = nodeMap[id];
        sorted.push_back(*node);

        for (int child : node->children) {
            inDegree[child]--;
            if (inDegree[child] == 0)
                q.push(child);
        }
    }

    // Check for cycles
    if (sorted.size() != nodes.size()) {
        throw runtime_error("Cycle detected in graph");
    }

    nodes = move(sorted);
}
*/











/*
            vector<double> sums( nodes[substitutionIndeces[owner]].states.size(), 0.0 ); // vector to hold the sums of the probabilities
            int counterToSelectProbs = 0;


            // here is the calculation of marginal probabilities
            for( auto j = nodes[substitutionIndeces[owner]].probabilities.begin(); j < nodes[substitutionIndeces[owner]].probabilities.end(); j+=nodes[substitutionIndeces[owner]].states.size()){

                for( auto k = j; k < j + nodes[substitutionIndeces[owner]].states.size(); k++){
                    double toBeAdded0 = *k; // get the first probability

                    vector<int> bitVector = toBitVector(counterToSelectProbs, nodes[substitutionIndeces[owner]].parents.size() ); // convert the counter to a bit vector
                    reverse(bitVector.begin(), bitVector.end()); // reverse the vector

                    for( int m = 0; m < nodes[substitutionIndeces[owner]].parents.size(); m++){
                        toBeAdded0 *= nodes[ nodes[substitutionIndeces[owner]].parents[m] ].pureProb[ bitVector[m] ]; // multiply the probabilities
                    }
                    sums[k-j] += toBeAdded0; // add the probabilities to the sum
                }
                counterToSelectProbs++;
            }

            nodes[substitutionIndeces[owner]].pureProb = sums; // assign the pure probabilities to the node
*/
















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
    double sum = 0;
    for (const auto& prob : node.pureProb){ os << prob << ' '; sum += prob; }
    os << "  sum: " << sum << "\n";


    return os;
}


// Overload for a vector of nodes
ostream& operator<<(ostream& os, const vector<BayesianNode>& nodes) {
    for (const auto& node : nodes) {
        os << node << '\n';
    }
    return os;
}