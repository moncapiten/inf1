#include "conditionalizer.hpp"

// Helper to get index of a state
// just returns the index of the state in the node's states vector
int stateIndex(const BayesianNode& node, const string& state) {
    auto it = find(node.states.begin(), node.states.end(), state);
    if (it == node.states.end()) throw invalid_argument("State " + state + " not found in node: " + node.name);
    return distance(node.states.begin(), it);
}

// Compute the index in the CPT for a given node and assignment
int computeCPTIndex(const BayesianNode& node,
                    const BayesianNetwork& net,
                    const unordered_map<string, string>& assignment) {
    const int numStates = node.states.size();
    int index = 0; // index that will be returned
    int multiplier = 1; // how much each parent state contributes to the index

    // For each parent in **left-to-right** order the index is computed by
    // finding the index of the state the parent is in, multiplying it by the
    // multiplier, and adding it to the index.
    // it is basically like counting in a number system where every digit has a different base
    for (int i = 0; i < node.parents.size(); ++i){
//    for (int i = node.parents.size() - 1; i >= 0; --i) {
        const BayesianNode& parent = net.getNode_ID(node.parents[i]);
        int parentStateIndex = stateIndex(parent, assignment.at(parent.name));
        index += parentStateIndex * multiplier;
        multiplier *= parent.states.size();
    }

    // Now scale for node's own state
    int nodeStateIndex = stateIndex(node, assignment.at(node.name));
    return index * numStates + nodeStateIndex;
}

// Recursively compute joint probability for an assignment
// example Rain -> Sprinkler -> Grass
// P( Rain = true, Sprinkler = false, Grass = wet ) = 
// 1 * P(Rain = true) * P(Sprinkler = false | Rain = true) * P(Grass = wet | Rain = true, Sprinkler = false)
double computeJointProb(const BayesianNetwork& net,
                        const unordered_map<string, string>& assignment,
                        unordered_map<string, int> cache) {
    double prob = 1.0;
    for (const auto& node : net.getNodes()) {
        const string& name = node.name;
        int valIndex = stateIndex(node, assignment.at(name));

        // If node has no parents
        if (node.parents.empty()) {
            prob *= node.pureProb[valIndex];
            continue;
        }

        // Compute CPT index

        int idx = computeCPTIndex(node, net, assignment);
        prob *= node.probabilities[idx];
    }
    return prob;
}

// Generate all consistent full assignments extending a partial one
void enumerateAllAssignments(const BayesianNetwork& net,
                             unordered_map<string, string>& partial,
                             vector<unordered_map<string, string>>& out,
                             size_t i) {
    const vector<BayesianNode>& nodes = net.getNodes();
    if (i == nodes.size()) {
        out.push_back(partial);
        return;
    }

    const string& nodeName = nodes[i].name;
    if (partial.count(nodeName)) { // partial.count(nodeName) = 1 if nodeName is in partial, 0 otherwise
        enumerateAllAssignments(net, partial, out, i + 1);
    } else {
        for (const auto& state : nodes[i].states) {
            partial[nodeName] = state;
            enumerateAllAssignments(net, partial, out, i + 1);
        }
        partial.erase(nodeName);
    }
}



// helper to check if everything is tip top to calculate conditional/joint probabilities
void checkComputability(BayesianNetwork& net,
                                const string& A, const string& a,
                                const string& B, const string& b) {
    // Network-level checks
    if (net.getSubstitutionIndeces().empty()) throw invalid_argument("Network is empty.");
    if (!net.marginalized) throw runtime_error("Network is not marginalized. Please marginalize the network before computing conditional probabilities.");

    // Node and state checks
    if (net.getSubstitutionIndeces().find(A) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + A + " not found in the network.");
    if (net.getSubstitutionIndeces().find(B) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + B + " not found in the network.");

    if (find(net.getNode_name(A).states.begin(), net.getNode_name(A).states.end(), a) == net.getNode_name(A).states.end()) throw invalid_argument("State " + a + " not found in node " + A);
    if (find(net.getNode_name(B).states.begin(), net.getNode_name(B).states.end(), b) == net.getNode_name(B).states.end()) throw invalid_argument("State " + b + " not found in node " + B);
}







double computeJointProbability(BayesianNetwork& net,
                                const string& A, const string& a,
                                const string& B, const string& b) {
    // 0. Check if nodes exist and if states are valid
    checkComputability(net, A, a, B, b);

    // 1. Create a base assignment with A = a and B = b
    unordered_map<string, string> base = {{A, a}, {B, b}};
    vector<unordered_map<string, string>> fullAssignments;
    enumerateAllAssignments(net, base, fullAssignments);

    double jointProb = 0.0;
    for (const auto& assign : fullAssignments) {
        jointProb += computeJointProb(net, assign);
    }
    return jointProb;
}



// Main function: P(A = a | B = b)
double computeConditionalProbability(BayesianNetwork& net,
                                     const string& A, const string& a,
                                     const string& B, const string& b) {
    // 0. Check if the network is valid, if the nodes exist and if states are valid
    checkComputability(net, A, a, B, b);

    // 1. Numerator: P(A = a, B = b)
    // calculated by enumerating all full assignments that include A = a and B = b
    // and summing their joint probabilities
    unordered_map<string, string> base = {{A, a}, {B, b}};
    vector<unordered_map<string, string>> fullAssignments;
    enumerateAllAssignments(net, base, fullAssignments);

    double joint = 0;
    for (const auto& assign : fullAssignments)
        joint += computeJointProb(net, assign);

    // 2. Denominator: P(B = b)
    // calculated by enumerating all assignments that include B = b
    // and summing their joint probabilities
    // ...again
    unordered_map<string, string> bOnly = {{B, b}};
    vector<unordered_map<string, string>> bAssignments;
    enumerateAllAssignments(net, bOnly, bAssignments);

    double marginalB = 0;
    for (const auto& assign : bAssignments)
        marginalB += computeJointProb(net, assign);

    // 3. Return the conditional probability P(A = a | B = b) = P(A = a, B = b) / P(B = b)
    // just Bayes rule with joint instead of P(B|A) * P(A)
    if (marginalB < 1e-6) return 0;
    return joint / marginalB;
}


