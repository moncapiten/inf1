#include "../include/conditionalizer.hpp"

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
                        const unordered_map<string, string>& assignment) {
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
        if( node.probabilities[idx] < EPSILON ) return 0.0; // if we have a zero probability, do not bother computing further
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





// helper to check if everything is tip top to calculate conditional/joint probabilities
/*
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
*/

void checkComputability(BayesianNetwork& net, const unordered_map<string, string>& evidence, const unordered_map<string, string>& conditions) {
    // Network-level checks
    if (net.getSubstitutionIndeces().empty()) throw invalid_argument("Network is empty.");
    if (!net.marginalized) throw runtime_error("Network is not marginalized. Please marginalize the network before computing conditional probabilities.");

    // Node and state checks
    for (const auto& [node, state] : evidence) {
        if (net.getSubstitutionIndeces().find(node) == net.getSubstitutionIndeces().end())      throw invalid_argument("Node " + node + " not found in the network.");
        if (find(net.getNode_name(node).states.begin(), net.getNode_name(node).states.end(), state) == net.getNode_name(node).states.end())     throw invalid_argument("State " + state + " not found in node " + node);
    }

    for (const auto& [node, state] : conditions) {
        if (net.getSubstitutionIndeces().find(node) == net.getSubstitutionIndeces().end())      throw invalid_argument("Node " + node + " not found in the network.");
        if (find(net.getNode_name(node).states.begin(), net.getNode_name(node).states.end(), state) == net.getNode_name(node).states.end())     throw invalid_argument("State " + state + " not found in node " + node);

        // Check if the node is in evidence
        // done here to avoid looping once more
        if (evidence.count(node))       throw invalid_argument("Node " + node + " appears in both evidence and conditions");

    }
    
}

// wrapping to maintain compatibility with the old interface
void checkComputability(BayesianNetwork& net,
                                const string& A, const string& a,
                                const string& B, const string& b) {
    unordered_map<string, string> evidence = {{A, a}};
    unordered_map<string, string> conditions = {{B, b}};

    checkComputability(net, evidence, conditions);
}








// Main function: P(A = a | B = b)
// temporal complexity is given by the fact that we have to enumerate all full assignaments
// twice,  once for the numerator, where we have A = a and B = b, and once for the denominator, where we have only B = b
// every assignaments requires O(n) to compute, and per each node i we have ki states
// so we have O( m^(n-2) ) * O(n) = O( n * m^(n-2) ) for the numerator 
// and O( m^(n-1) ) * O(n) = O( n * m^(n-1) ) for the denominator
// therefore O( n * m^(n-2) ) + O( n * m^(n-1) ) = O( n * m^(n-1) )
// ignoring the one and the O(n) factor, we have O( m^n ) as the final complexity
// with m being the average number of states per node and n the number of nodes in the network
double oldComputeConditionalProbability(BayesianNetwork& net,
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
    if (marginalB < EPSILON) return 0;
    return joint / marginalB;
}











// function with smarter enumeration
// its a bit more efficient cause we do not enumerate all assignaments twice
// complexity is still O( m^n ) but we reach it by only enumerating once
// and checking if it can be used for both numerator and denominator
/*
double computeConditionalProbability(BayesianNetwork& net,
                                     const string& A, const string& a,
                                     const string& B, const string& b) {
    // 0. Check if the network is valid, if the nodes exist and if states are valid
    checkComputability(net, A, a, B, b);

    // 1. Generate all assignments where B=b
    unordered_map<string, string> bOnly = {{B, b}};
    vector<unordered_map<string, string>> allBAssignments;
    enumerateAllAssignments(net, bOnly, allBAssignments);
    
    double joint = 0, marginalB = 0;
    for (const auto& assign : allBAssignments) {
        double prob = computeJointProb(net, assign);
        marginalB += prob;
        
        // Check if this assignment also has A=a
        if (assign.at(A) == a) {
            joint += prob;
        }
    }
    
    return marginalB < EPSILON ? 0 : joint / marginalB;
}
*/












// extension to n evidence nodes and conditions
double computeConditionalProbability(BayesianNetwork& net,
                                     const unordered_map<string, string>& evidence,
                                     const unordered_map<string, string>& conditions) {
    // 0. Basic validation
    checkComputability(net, evidence, conditions);

    // 1. Start with conditions as base assignment
    unordered_map<string, string> baseConditions = conditions;
    vector<unordered_map<string, string>> allConditionAssignments;
    enumerateAllAssignments(net, baseConditions, allConditionAssignments);
    
    double joint = 0, marginalConditions = 0;
    
    for (const auto& assign : allConditionAssignments) {
        double prob = computeJointProb(net, assign);
        marginalConditions += prob;
        
        // Check if this assignment also satisfies all evidence
        bool satisfiesEvidence = true;
        for (const auto& [evidenceNode, evidenceState] : evidence) {
            if (assign.at(evidenceNode) != evidenceState) {
                satisfiesEvidence = false;
                break;
            }
        }
        
        if (satisfiesEvidence) {
            joint += prob;
        }
    }
    
    return marginalConditions < EPSILON ? 0 : joint / marginalConditions;
}

// wrapper to work with just two nodes
double computeConditionalProbability(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b) {
    unordered_map<string, string> evidence = {{A, a}};
    unordered_map<string, string> conditions = {{B, b}};
    return computeConditionalProbability(net, evidence, conditions);
}
























