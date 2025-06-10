#include "conditionalizer.hpp"

// Helper to get index of a state
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
    int index = 0;
    int multiplier = 1;

    // For each parent in **left-to-right** order
    for (int i = 0; i < node.parents.size(); ++i){
//    for (int i = node.parents.size() - 1; i >= 0; --i) {
        const BayesianNode& parent = net.getNode_ID(node.parents[i]);
        int parentStateIndex = stateIndex(parent, assignment.at(parent.name));
        index += parentStateIndex * multiplier;
        multiplier *= parent.states.size();
    }

    // Now scale for node's own state (like interleaving)
    int nodeStateIndex = stateIndex(node, assignment.at(node.name));
    return index * numStates + nodeStateIndex;
}

// Recursively compute joint probability for an assignment
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
    if (partial.count(nodeName)) {
        enumerateAllAssignments(net, partial, out, i + 1);
    } else {
        for (const auto& state : nodes[i].states) {
            partial[nodeName] = state;
            enumerateAllAssignments(net, partial, out, i + 1);
        }
        partial.erase(nodeName);
    }
}



double computeJointProbability(BayesianNetwork& net,
                                const string& A, const string& a,
                                const string& B, const string& b) {
    // 0. Check if nodes exist and if states are valid
    if (net.getSubstitutionIndeces().find(A) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + A + " not found in the network.");
    if (net.getSubstitutionIndeces().find(B) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + A + " or " + B + " not found in the network.");

    if (find(net.getNode_name(A).states.begin(), net.getNode_name(A).states.end(), a) == net.getNode_name(A).states.end()) throw invalid_argument("State " + a + " not found in node " + A);
    if (find(net.getNode_name(B).states.begin(), net.getNode_name(B).states.end(), b) == net.getNode_name(B).states.end()) throw invalid_argument("State " + b + " not found in node " + B);

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
    if (net.getSubstitutionIndeces().empty()) throw invalid_argument("Network is empty.");

    if (net.getSubstitutionIndeces().find(A) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + A + " not found in the network.");
    if (net.getSubstitutionIndeces().find(B) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + A + " or " + B + " not found in the network.");

    if (find(net.getNode_name(A).states.begin(), net.getNode_name(A).states.end(), a) == net.getNode_name(A).states.end()) throw invalid_argument("State " + a + " not found in node " + A);
    if (find(net.getNode_name(B).states.begin(), net.getNode_name(B).states.end(), b) == net.getNode_name(B).states.end()) throw invalid_argument("State " + b + " not found in node " + B);

    

    // 1. Numerator: P(A = a, B = b)
    unordered_map<string, string> base = {{A, a}, {B, b}};
    vector<unordered_map<string, string>> fullAssignments;
    enumerateAllAssignments(net, base, fullAssignments);

    double joint = 0;
    for (const auto& assign : fullAssignments)
        joint += computeJointProb(net, assign);

    // 2. Denominator: P(B = b)
    unordered_map<string, string> bOnly = {{B, b}};
    vector<unordered_map<string, string>> bAssignments;
    enumerateAllAssignments(net, bOnly, bAssignments);

    double marginalB = 0;
    for (const auto& assign : bAssignments)
        marginalB += computeJointProb(net, assign);

    if (marginalB == 0) return 0;
    return joint / marginalB;
}
/*
double computeConditionalProbability(BayesianNetwork& net, const string& A, const string& a, const string& B){
    double prob = 0;
    for(const auto& state : net.getNode_name(B).states){
        cout << "Conditional probability P(" << A << "=" << a << " | " << B << "=" << state;
        double conditionalProb = computeConditionalProbability(net, A, a, B, state);
        cout << ") = " << conditionalProb << endl;
//        prob += conditionalProb * net.getNode_name(B).pureProb[stateIndex(net.getNode_name(B), state)];
    }
    return prob;
}
*/







