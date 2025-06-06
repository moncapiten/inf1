#include <unordered_map>
#include <numeric>
#include <stdexcept>
#include <vector>
#include "nodes.hpp"
#include <algorithm>








































// Helper to get index of a state
int stateIndex(const BayesianNode& node, const string& state) {
    auto it = find(node.states.begin(), node.states.end(), state);
    if (it == node.states.end()) throw invalid_argument("State " + state + " not found in node: " + node.name);
    return distance(node.states.begin(), it);
}











int computeCPTIndex(const BayesianNode& node,
                    const BayesianNetwork& net,
                    const unordered_map<string, string>& assignment) {
    const int numStates = node.states.size();
    int index = 0;
    int multiplier = 1;

    // For each parent in **left-to-right** order
    for (int i = node.parents.size() - 1; i >= 0; --i) {
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
                        unordered_map<string, int> cache = {}) {
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
//        int multiplier = 1;
//        for (int i = node.parents.size() - 1; i >= 0; --i) {
//            const BayesianNode& parent = net.getNode_ID(node.parents[i]);
//            int pIdx = stateIndex(parent, assignment.at(parent.name));
//            idx += pIdx * multiplier;
//            multiplier *= parent.states.size();
//        }
//        idx = idx * node.states.size() + valIndex;
//        prob *= node.probabilities[idx];
    }
    return prob;
}

// Generate all consistent full assignments extending a partial one
void enumerateAllAssignments(const BayesianNetwork& net,
                             unordered_map<string, string>& partial,
                             vector<unordered_map<string, string>>& out,
                             size_t i = 0) {
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

// Main function: P(A = a | B = b)
double computeConditionalProbability(const BayesianNetwork& net,
                                     const string& A, const string& a,
                                     const string& B, const string& b) {
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









