#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <windows.h> // for colored text in console
#include <string>
#include <algorithm>
#include <cctype>
#include <bitset>
#include <stdexcept>

#include "parser.hpp"
#include "parser.cpp"


using namespace std;



vector<BayesianNode> nodes; // vector of Bayesian nodes
// there is substitutionIndeces and substitutionNames in nodes.cpp, but they are used but not declared here



// I could make it so that the marginalizer is just a class that exists, and when called to marginalize
// the network is passed as an argument and will be marginalized in place, would make sense?
// does it make sense then to be a class? or just a function?
class Marginalizer{
    public:
        Marginalizer(vector<BayesianNode>& inputNetwork) : network(inputNetwork) {
            listInWorkingOrder = reorder(network); // reorder the nodes
        }

        vector<BayesianNode> marginalize() {

            for(auto& i : listInWorkingOrder) {
                marginalizeNode(network[i]); // marginalize the nodes
                cout << network[i] << endl; // print the node after marginalization
            }

            return network;
        }


    private:
        vector<BayesianNode> network; // vector of Bayesian nodes
        vector<int> listInWorkingOrder; // vector to hold the list reordered so that no node ever appears before its parents


        vector<int> reorder(vector<BayesianNode> nodesIndeces) {
            vector<BayesianNode> origin = nodesIndeces;
            vector<int> sortedIDs;
            unordered_set<int> placed;

            bool progress;

            do {
                progress = false;

                for (auto it = origin.begin(); it != origin.end(); ) {
                    bool ready = true;

                    for (int parent : it->parents) {
                        if (placed.find(parent) == placed.end()) {
                            ready = false;
                            break;
                        }
                    }

                    if (ready) {
                        sortedIDs.push_back(it->ID);
                        placed.insert(it->ID);
                        it = origin.erase(it);
                        progress = true;
                    } else {
                        ++it;
                    }
                }

                if (!progress && !origin.empty()) {
                    throw runtime_error("Cycle detected or missing parent nodes.");
                }

            } while (!origin.empty());

            return sortedIDs;
        }



        void updateVector(vector<int>& parentsStates, vector<int> parentList){
            bool rollover = false;
            for( int i = 0; i < parentsStates.size(); i++){
                
                if( rollover || i==0 ) parentsStates[i]++;

                rollover = false;
                if(parentsStates[i] > nodes[parentList[i]].states.size()-1){
                    parentsStates[i] = 0;
                    rollover = true;
                }
            }
        }







        // marginalization function
        // I still hate you for this Fabio
        void marginalizeNode(BayesianNode& node) {

            cout << "Marginalizing node: " << node.name << endl; // print the name of the node being marginalized
            cout << "Parents: ";
            for (const auto& p : node.parents) {
                cout << network[p].name << network[p].pureProb << " "; // print the names of the parents
            }
            cout << endl; // print a new line after the parents


            if (node.parents.empty()) {
                node.pureProb = node.probabilities; // assign the probabilities to the node
                return; // return if the node has no parents
            }

            vector<double> sums(node.states.size(), 0.0); // vector to hold the sums of the probabilities
            vector<int> statesOfEachParent(node.parents.size(), 0); // vector to hold the states of each parent

            // here is the calculation of marginal probabilities
            for (auto j = node.probabilities.begin(); j < node.probabilities.end(); j += node.states.size()) {

                for (auto k = j; k < j + node.states.size(); k++) {
                    double toBeAdded0 = *k; // get the first probability
                    for (int m = 0; m < node.parents.size(); m++) {
                        toBeAdded0 *= network[node.parents[m]].pureProb[statesOfEachParent[m]]; // multiply the probabilities
                    }
                    sums[k - j] += toBeAdded0; // add the probabilities to the sum
                }
            
                updateVector(statesOfEachParent, node.parents);
            }

            node.pureProb = sums; // assign the pure probabilities to the node
            return;
        }
        
};










int main(){
    string filename = "../cancer.bif"; // file name


    Parser parser(filename); // create a new parser object

    parser.translate();

    nodes = parser.getNodes();
    substitutionIndeces = parser.getIndecesArchive();
    substitutionNames = parser.getNamesArchive();


    Marginalizer marg(nodes); // create a new marginalizer object
    nodes = marg.marginalize(); // marginalize the nodes

    cout << "\n\n\nNETWORK:" << endl << endl; // print the nodes
    cout << nodes; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes

    cout << "That's all folks!" << endl; // print the end message




    return 0;
}