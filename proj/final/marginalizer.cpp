//#include <unordered_set>
//#include <numeric>

#include "marginalizer.hpp"

//#include "nodes.hpp"
//#include "nodes.cpp"




// Should network be a class of its own, basically a wrapper for a vector of BayesianNode
// but that also contains marginalization methods and the substitution maps?

// I could make it so that the marginalizer is just a class that exists, and when called to marginalize
// the network is passed as an argument and will be marginalized in place, would make sense?
// does it make sense then to be a class? or just a function?
class Marginalizer{
    public:
        Marginalizer() {
            
        }

        BayesianNetwork& marginalize(BayesianNetwork& inputNetwork) {
            outputNetwork = inputNetwork; // assign the input network to the output network
            listInWorkingOrder = reorder(outputNetwork.getNodes()); // reorder the nodes

            for(auto& i : listInWorkingOrder) {
                marginalizeNode(outputNetwork.getNode_ID(i)); // marginalize the nodes
//                cout << outputNetwork.getNode_ID(i) << endl; // print the node after marginalization
            }

            return outputNetwork; // return the modified network
        }
/*        vector<BayesianNode> marginalize(vector<BayesianNode> inputNetwork) {
            network = inputNetwork; // assign the input network to the network
            listInWorkingOrder = reorder(inputNetwork); // reorder the nodes

            for(auto& i : listInWorkingOrder) {
                marginalizeNode(network[i]); // marginalize the nodes
            }

            return network;
        }
*/

    private:
        //vector<BayesianNode> network; // vector of Bayesian nodes
        BayesianNetwork outputNetwork; // output network, not used in this implementation but could be useful later

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



        void updateVector(vector<int>& statesMask, const vector<int>& parentList){
            bool rollover = false;
            for( int i = 0; i < statesMask.size(); i++){

                if( rollover || i==0 ) statesMask[i]++;

                rollover = false;
                if(statesMask[i] >= outputNetwork.getNode_ID(parentList[i]).states.size()){
                    statesMask[i] = 0;
                    rollover = true;
                }
            }
        }







        // marginalization function
        // I still hate you for this Fabio
        void marginalizeNode(BayesianNode& node) {

//            cout << "Marginalizing node: " << node.name << endl; // print the name of the node being marginalized
//            cout << "Parents: " << node.parents << endl; // print the parents of the node

            if (node.parents.empty()) {
                node.pureProb = node.probabilities; // assign the probabilities to the node
                return; // return if the node has no parents
            }

            vector<double> sums(node.states.size(), 0.0); // vector to hold the sums of the probabilities
            vector<int> parentsStatesMask(node.parents.size(), 0); // vector to hold the states of each parent

            // here is the calculation of marginal probabilities
            for (auto j = node.probabilities.begin(); j < node.probabilities.end(); j += node.states.size()) {

                for (auto k = j; k < j + node.states.size(); k++) {
                    double toBeAdded0 = *k; // get the first probability
                    for (int m = 0; m < node.parents.size(); m++) {
//                        cout << "Parent: " << outputNetwork.getNode_ID(node.parents[m]).name << " State: " << parentsStatesMask[m] << endl; // print the parent and its state
                        toBeAdded0 *= outputNetwork.getNode_ID(node.parents[m]).pureProb[parentsStatesMask[m]]; // multiply the probabilities
                    }
                    sums[k - j] += toBeAdded0; // add the probabilities to the sum
                }

                updateVector(parentsStatesMask, node.parents);
            }

            double testSum = accumulate(sums.begin(), sums.end(), 0.0); // calculate the sum of the probabilities
//            if( abs(testSum - 1.0) > 1e-8 ){
//                throw runtime_error("The sum of the probabilities is not equal to 1, something went wrong!");
//            }

            node.pureProb = sums; // assign the pure probabilities to the node

            return;
        }
        
};