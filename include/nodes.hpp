#ifndef NODES_HPP
#define NODES_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <iterator>
#include <numeric>
#include <sstream>

using namespace std;


constexpr double EPSILON = 1e-6; // small value to compare floating point numbers


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





class BayesianNetwork {
    public:
        string name; // name of the Bayesian network
        bool marginalized = false; // flag to indicate if the network is marginalized

        BayesianNetwork() = default; // default constructor for the BayesianNetwork class
        
        BayesianNetwork(string name) : name(name) {
            // Constructor for the BayesianNetwork class
        }

        void addNode(const BayesianNode& node);
        
        BayesianNode& getNode_ID(int ID);
        const BayesianNode& getNode_ID(int ID) const;

        BayesianNode& getNode_name(const string& name);
        
        int size() const;
        
        vector<BayesianNode> getNodes() const;
        
        void updateMaps(string name, int index);

        unordered_map<string, int> getSubstitutionIndeces() const {
            return substitutionIndeces; // return the map of names and their indices
        }
        unordered_map<int, string> getSubstitutionNames() const {
            return substitutionNames; // return the map of indices and their names
        }

        private:
        vector<BayesianNode> nodes; // vector to hold the Bayesian nodes
        unordered_map<string, int> substitutionIndeces; // map to hold the names of the nodes and their indices
        unordered_map<int, string> substitutionNames; // map to hold the indices of the nodes and their names
};


ostream& operator<<(ostream& os, const BayesianNode& node);

ostream& operator<<(ostream& os, const BayesianNetwork& network);

std::string to_string(const BayesianNode& node);

std::string to_string(const BayesianNetwork& network);



#endif // NODES_HPP