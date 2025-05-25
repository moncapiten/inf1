#include "marginalizer.hpp"




BayesianNetwork& Marginalizer::marginalize(BayesianNetwork& inputNetwork) {
    outputNetwork = inputNetwork; // assign the input network to the output network
    listInWorkingOrder = reorder(outputNetwork.getNodes()); // reorder the nodes

    for(auto& i : listInWorkingOrder) {
        marginalizeNode(outputNetwork.getNode_ID(i)); // marginalize the nodes
//                cout << outputNetwork.getNode_ID(i) << endl; // print the node after marginalization
    }

    return outputNetwork; // return the modified network
}
vector<int> Marginalizer::reorder(vector<BayesianNode> nodesIndeces) {
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



void Marginalizer::updateVector(vector<int>& statesMask, const vector<int>& parentList){
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
void Marginalizer::marginalizeNode(BayesianNode& node) {

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
