#include "../include/marginalizer.hpp"



// Just all working together, nothing special here
BayesianNetwork& Marginalizer::marginalize(BayesianNetwork& inputNetwork) {
    outputNetwork = inputNetwork; // assign the input network to the output network
    listInWorkingOrder = reorder(outputNetwork.getNodes()); // reorder the nodes so that no node appears before its parents

    for(auto& i : listInWorkingOrder) {
        marginalizeNode(outputNetwork.getNode_ID(i)); // marginalize the nodes
    }

    outputNetwork.marginalized = true; // set the marginalized flag to true

    return outputNetwork; // return the modified network
}

// reordering function
// topological sorting - O(n^2) worst case, O(n) best case
// could be improved with Kahn Algorithm, but seems to work ok
// also O(n) spacially per each origin, sortedID and placed so total O(n)
// I still hate you for this Fabio
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
            throw runtime_error("MARGINALIZER ERROR - Cycle detected or missing parent nodes - occurred at node: " + origin.front().name);
        }

    } while (!origin.empty());

    return sortedIDs;
}


// updates the mask of parent's states, used as a vector
// it's basically mirrored binary counting
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
void Marginalizer::marginalizeNode(BayesianNode& node) {

    if (node.parents.empty()) {
        node.pureProb = node.probabilities; // assign the probabilities to the node
        return; // return if the node has no parents
    }

    vector<double> sums(node.states.size(), 0.0); // vector to hold the sums of the probabilities
    vector<int> parentsStatesMask(node.parents.size(), 0); // vector to hold the states of each parent

    // here is the calculation of marginal probabilities
    // every node gets iterated over its states and every entry in the cpt gets multipied
    // by the probabilities of its parents, then summed up
    for (auto j = node.probabilities.begin(); j < node.probabilities.end(); j += node.states.size()) {
        cout << "Changed row" << endl;

        for (auto k = j; k < j + node.states.size(); k++) {
            cout << "Evaluating the " << to_string(k - j) << "th entry of " << to_string(j - node.probabilities.begin()) << "th row : " << *k << " as part of the CPT of " << node.name << " with mask " << parentsStatesMask << endl;
            double toBeAdded = *k; // get the first probability
            for (int m = 0; m < node.parents.size(); m++) {
                toBeAdded *= outputNetwork.getNode_ID(node.parents[m]).pureProb[parentsStatesMask[m]]; // multiply the probabilities
            }
            sums[k - j] += toBeAdded; // add the probabilities to the sum
            cout << "Added " << to_string(toBeAdded) << " to the " << to_string(k - j) << "th entry of the sums vector, now equal to: " << to_string(sums[k - j]) << endl;
        }
        updateVector(parentsStatesMask, node.parents);

    }
    // Wordy explanation of the above code:
    // We iterate a "row" of the CPT at a time, as inside one row the parent's states are static
    // so we calculate the correct mask for a specific row, then we iterate over the entries of that row,
    // multypling them by the pureProbs( marginalized probabilities) of the parent's specificc states, 
    // which we obtain through the mask.
    // We use "sums[k - j]" in order to remember multiple values of the node's pureProbs, and we update each entry
    // in sums once per row, given how k increases while j is just the start of the row, so k-j is both the index of
    // the entry in the CPT's row and the index of the sums vector.
    
    // We do not need to normalize the result as the products are done in such a way as to be already normalized

    // test to see if the probs sum to 1 - there's leeway for floating point errors
    double testSum = accumulate(sums.begin(), sums.end(), 0.0); // calculate the sum of the probabilities
    if( abs(testSum - 1.0) > EPSILON ){
        throw runtime_error("MARGINALIZER ERROR - Probabilities do not sum to 1 for node: " + node.name); // throw an error if the probabilities do not sum to 1
    }

    node.pureProb = sums; // assign the pure probabilities to the node

    return;
}
