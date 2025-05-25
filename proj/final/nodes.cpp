#include "nodes.hpp"

using namespace std;




void BayesianNetwork::addNode(const BayesianNode& node) {
    nodes.push_back(node); // add the node to the vector of nodes
    updateMaps(node.name, node.ID); // update the maps with the name and index of the node
}

BayesianNode& BayesianNetwork::getNode_ID(int ID) {
    if (ID < 0 || ID >= nodes.size()) {
        throw out_of_range("Node ID is out of range"); // throw an error if the ID is out of range
    }
    return nodes[ID]; // return the node with the given ID
}

BayesianNode& BayesianNetwork::getNode_name(const string& name) {
    auto it = substitutionIndeces.find(name); // find the index of the node with the given name
    if (it == substitutionIndeces.end()) {
        throw out_of_range("Node name not found"); // throw an error if the name is not found
    }
    return nodes[it->second]; // return the node with the given name
}

int BayesianNetwork::size() const {
    return nodes.size(); // return the size of the vector of nodes
}

vector<BayesianNode> BayesianNetwork::getNodes() const {
    return nodes; // return the vector of nodes
}

void BayesianNetwork::updateMaps(string name, int index) {
    substitutionIndeces[name] = index; // add the name to the map with the index of the node
    substitutionNames[index] = name; // add the index to the map with the name of the node
}










vector<int> getParentStateIndices(int counter, const vector<int>& parentIDs, const vector<BayesianNode>& nodes) {
    vector<int> indices;
    for (int i = parentIDs.size() - 1; i >= 0; --i) { // Reverse iteration
        int parentID = parentIDs[i];
        int subsequentStates = 1;
        for (int j = i + 1; j < parentIDs.size(); ++j) {
            subsequentStates *= nodes[parentIDs[j]].states.size();
        }
        int parentState = (counter / subsequentStates) % nodes[parentID].states.size();
        indices.insert(indices.begin(), parentState); // Maintain original order
    }
    return indices;
}








// Overload for a single node
ostream& operator<<(ostream& os, const BayesianNode& node) {
    os << "Node: " << node.name << " alias " << node.ID << '\n';

    os << "  Parents: ";
    for (const auto& p : node.parents) os << p; //substitutionNames[p] << ' ';
//    for (const auto& p : node.parents) os << p << ' ';
    os << '\n';

    os << "  Children: ";
    for (const auto& c : node.children) os << c; //substitutionNames[c] << ' ';
//    for (const auto& c : node.children) os << c << ' ';
    os << '\n';

    os << "  States: ";
    for (const auto& s : node.states) os << s << ' ';
    os << " [0, 1]";
    os << '\n';

    os << "  Flattened CPT: ";
    for (const auto& prob : node.probabilities) os << prob << ' ';
    os << "\n";

    os << "  Pure Probabilities: ";
//    double sum = 0;
    for (const auto& prob : node.pureProb){ os << prob << ' ';/* sum += prob;*/ }
    auto sum = accumulate(node.pureProb.begin(), node.pureProb.end(), 0.0); // calculate the sum of the pure probabilities
    os << "  sum: " << sum << "\n";


    return os;
}


// Overload for a vector of nodes
ostream& operator<<(ostream& os, const BayesianNetwork& network) {
    for (const auto& node : network.getNodes()) {
        os << node << '\n';
    }
    return os;
}