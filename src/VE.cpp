// ACTUAL Variable Elimination
#include "../include/VE.hpp"

using namespace std;

    // Create factor from a node's CPT
Factor::Factor(const BayesianNode& node, const BayesianNetwork& net) {
    // Add node itself first, then parents (to match CPT ordering)
    vars.push_back(node.name);
    for (int parentID : node.parents) {
        vars.push_back(net.getNode_ID(parentID).name);
    }
    
    // Generate all combinations and fill table
    vector<vector<string>> allStates;
    allStates.push_back(node.states);
    for (int parentID : node.parents) {
        allStates.push_back(net.getNode_ID(parentID).states);
    }
    
    // generate a table with all combinations of states and the respective probabilities
    // by searching through the node's cpt recursively
    function<void(vector<string>&, int)> fillTable = [&](vector<string>& assignment, int depth) {
        if (depth == vars.size()) {
            int idx = 0;
            int multiplier = 1;
            for (int i = 1; i < vars.size(); ++i) {  // Skip first var (the node itself)
                auto it = find(allStates[i].begin(), allStates[i].end(), assignment[i]);
                int stateIdx = distance(allStates[i].begin(), it);
                idx += stateIdx * multiplier;
                multiplier *= allStates[i].size();
            }
            // Add node's own state
            auto it = find(allStates[0].begin(), allStates[0].end(), assignment[0]);
            int nodeStateIdx = distance(allStates[0].begin(), it);
            idx = idx * allStates[0].size() + nodeStateIdx;
            
            if (node.parents.empty()) {
                table[assignment] = node.pureProb[nodeStateIdx];
            } else {
                table[assignment] = node.probabilities[idx];
            }
            return;
        }
        
        for (const string& state : allStates[depth]) {
            assignment[depth] = state;
            fillTable(assignment, depth + 1);
        }
    };
    
    vector<string> assignment(vars.size());
    fillTable(assignment, 0);

//    cout << "Factor created for node: " << node.name << endl;
//    cout << table << endl;
}

double Factor::getValue(const vector<string>& assignment) const {
    auto it = table.find(assignment);
        return it != table.end() ? it->second : 0.0;
}

// Multiply two factors
Factor multiply(const Factor& f1, const Factor& f2) {
    Factor result;
    
    // Union of variables
    set<string> varSet(f1.vars.begin(), f1.vars.end());
    varSet.insert(f2.vars.begin(), f2.vars.end());
    result.vars.assign(varSet.begin(), varSet.end()); // result.vars now contains all unique variables from both factors
    
    // Get state spaces for each variable
    map<string, vector<string>> stateSpaces;
    for (const auto& [assignment, prob] : f1.table) {
        for (int i = 0; i < f1.vars.size(); ++i) {
            if (stateSpaces[f1.vars[i]].empty()) {
                // Find all states for this variable in f1
                set<string> states;
                for (const auto& [ass, p] : f1.table) {
                    states.insert(ass[i]);
                }
                stateSpaces[f1.vars[i]].assign(states.begin(), states.end());
            }
        }
    }
    for (const auto& [assignment, prob] : f2.table) {
        for (int i = 0; i < f2.vars.size(); ++i) {
            if (stateSpaces[f2.vars[i]].empty()) {
                set<string> states;
                for (const auto& [ass, p] : f2.table) {
                    states.insert(ass[i]);
                }
                stateSpaces[f2.vars[i]].assign(states.begin(), states.end());
            }
        }
    }
    // basically stateSpaces contains all possible values for all the variables that appear in the factor multiplication
//    cout << stateSpaces << endl;


    // for every possible combination of states in the factor multiplication
    // the stateSpaces map provides the possible values for each variable
    // those are used to calculate the joint probability
    // Generate all assignments for result variables
    function<void(vector<string>&, int)> generateAssignments = [&](vector<string>& assignment, int depth) {
        if (depth == result.vars.size()) {
            // Get values from f1 and f2
            vector<string> f1Assignment, f2Assignment;
            
            // Extract f1's subassignment
            for (const string& var : f1.vars) {
                auto it = find(result.vars.begin(), result.vars.end(), var);
                int idx = distance(result.vars.begin(), it);
                f1Assignment.push_back(assignment[idx]);
            }
            
            // Extract f2's subassignment
            for (const string& var : f2.vars) {
                auto it = find(result.vars.begin(), result.vars.end(), var);
                int idx = distance(result.vars.begin(), it);
                f2Assignment.push_back(assignment[idx]);
            }
            
            double val1 = f1.getValue(f1Assignment);
            double val2 = f2.getValue(f2Assignment);
            result.table[assignment] = val1 * val2;
            return;
        }
        
        for (const string& state : stateSpaces[result.vars[depth]]) {
            assignment[depth] = state;
            generateAssignments(assignment, depth + 1);
        }
    };
    
    vector<string> assignment(result.vars.size());
    generateAssignments(assignment, 0);
    
    return result;
}

// Eliminate (sum out) a variable from a factor
Factor eliminate(const Factor& factor, const string& varToElim) {
    Factor result;
    
    // Find variable to eliminate
    auto it = find(factor.vars.begin(), factor.vars.end(), varToElim);
    if (it == factor.vars.end()) return factor;  // Variable not in factor
    
    int elimIdx = distance(factor.vars.begin(), it);
    
    // Result has all variables except the eliminated one
    for (int i = 0; i < factor.vars.size(); ++i) {
        if (i != elimIdx) {
            result.vars.push_back(factor.vars[i]);
        }
    }
    
    // If no variables left, sum everything
    if (result.vars.empty()) {
        double sum = 0.0;
        for (const auto& [assignment, prob] : factor.table) {
            sum += prob;
        }
        result.table[vector<string>()] = sum;
        return result;
    }
    
    // loop over every assignment in the factor's table
    // get a vector containing all variables in the assignment
    // except the one we are eliminating
    // sum the probabilities for all assignaments that match
    // the reduced assignment
    for (const auto& [assignment, prob] : factor.table) {
        vector<string> reducedAssignment;
        for (int i = 0; i < assignment.size(); ++i) {
            if (i != elimIdx) {
                reducedAssignment.push_back(assignment[i]);
            }
        }
        result.table[reducedAssignment] += prob; // the probability does not need to be scaled since, with it being a conditional probability to begin with, it's as if the scaling was "already included"
    }
    
    return result;
}

double computeConditionalProbabilityVE(BayesianNetwork& net,
                                          const unordered_map<string, string>& evidence,
                                          const unordered_map<string, string>& conditions) {
    checkComputability(net, evidence, conditions);
    
    // Create factors from CPTs
    vector<Factor> factors;
    for (const auto& node : net.getNodes()) {
        factors.emplace_back(node, net);
    }
    
    // Variables in query
    set<string> queryVars;
    for (const auto& [var, state] : evidence) queryVars.insert(var);
    for (const auto& [var, state] : conditions) queryVars.insert(var);
    
    // Elimination order: all variables not in query
    vector<string> elimOrder;
    for (const auto& node : net.getNodes()) {
        if (queryVars.find(node.name) == queryVars.end()) {
            elimOrder.push_back(node.name);
        }
    }
    
    // Variable elimination loop
    for (const string& var : elimOrder) {
        // Find factors containing this variable
        vector<Factor> relevant, remaining;
        for (const auto& factor : factors) {
            if (find(factor.vars.begin(), factor.vars.end(), var) != factor.vars.end()) {
                relevant.push_back(factor);
            } else {
                remaining.push_back(factor);
            }
        }
        
        // Multiply relevant factors
        Factor combined = relevant[0];
        for (int i = 1; i < relevant.size(); ++i) {
            combined = multiply(combined, relevant[i]);
        }
        
        // Eliminate the variable
        Factor eliminated = eliminate(combined, var);
        
        // Update factor list
        factors = remaining;
        if (!eliminated.vars.empty() || !eliminated.table.empty()) {
            factors.push_back(eliminated);
        }
    }
    
    // Multiply remaining factors to get final joint distribution
    Factor final = factors[0];
    for (int i = 1; i < factors.size(); ++i) {
        final = multiply(final, factors[i]);
    }
    
    // Extract probabilities
    vector<string> jointAssignment, conditionAssignment;
    for (const string& var : final.vars) {
        if (evidence.count(var)) {
            jointAssignment.push_back(evidence.at(var));
            conditionAssignment.push_back(evidence.at(var));
        } else if (conditions.count(var)) {
            jointAssignment.push_back(conditions.at(var));
            conditionAssignment.push_back(conditions.at(var));
        }
    }
    
    double joint = final.getValue(jointAssignment);
    
    // For marginal, sum over evidence variables
    double marginal = 0.0;
    if (evidence.empty()) {
        marginal = final.getValue(conditionAssignment);
    } else {
        // Need to sum over evidence variables
        Factor conditionFactor = final;
        for (const auto& [evidenceVar, evidenceState] : evidence) {
            conditionFactor = eliminate(conditionFactor, evidenceVar);
        }
        // assignements with only the condition variables
        vector<string> condOnlyAssignment;
        for (const string& var : conditionFactor.vars) {
            condOnlyAssignment.push_back(conditions.at(var));
        }
        marginal = conditionFactor.getValue(condOnlyAssignment); // marginal probability is the value of the condi
    }
    
    return marginal < EPSILON ? 0.0 : joint / marginal;
}

// Wrapper
double computeConditionalProbabilityVE(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b) {
    unordered_map<string, string> evidence = {{A, a}};
    unordered_map<string, string> conditions = {{B, b}};
    return computeConditionalProbabilityVE(net, evidence, conditions);
}





// Compute joint probability P(X1=x1, X2=x2, ..., Xn=xn) using Variable Elimination
double computeJointProbabilityVE(BayesianNetwork& net, const unordered_map<string, string>& assignment) {
    // Create factors from CPTs
    vector<Factor> factors;
    for (const auto& node : net.getNodes()) {
        factors.emplace_back(node, net);
    }
    
    // Variables in the joint assignment
    set<string> queryVars;
    for (const auto& [var, state] : assignment) {
        queryVars.insert(var);
    }
    
    // Elimination order: all variables not in the assignment
    vector<string> elimOrder;
    for (const auto& node : net.getNodes()) {
        if (queryVars.find(node.name) == queryVars.end()) {
            elimOrder.push_back(node.name);
        }
    }
    
    // Variable elimination loop
    for (const string& var : elimOrder) {
        // Find factors containing this variable
        vector<Factor> relevant, remaining;
        for (const auto& factor : factors) {
            if (find(factor.vars.begin(), factor.vars.end(), var) != factor.vars.end()) {
                relevant.push_back(factor);
            } else {
                remaining.push_back(factor);
            }
        }
        
        // Multiply relevant factors
        Factor combined = relevant[0];
        for (int i = 1; i < relevant.size(); ++i) {
            combined = multiply(combined, relevant[i]);
        }
        
        // Eliminate the variable
        Factor eliminated = eliminate(combined, var);
        
        // Update factor list
        factors = remaining;
        if (!eliminated.vars.empty() || !eliminated.table.empty()) {
            factors.push_back(eliminated);
        }
    }
    
    // Multiply remaining factors to get final joint distribution
    Factor final = factors[0];
    for (int i = 1; i < factors.size(); ++i) {
        final = multiply(final, factors[i]);
    }
    
    // Extract the joint probability for the given assignment
    vector<string> jointAssignment;
    for (const string& var : final.vars) {
        if (assignment.count(var)) {
            jointAssignment.push_back(assignment.at(var));
        }
    }
    
    return final.getValue(jointAssignment);
}

// Convenience wrapper for two variables
double computeJointProbabilityVE(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b) {
    unordered_map<string, string> assignment = {{A, a}, {B, b}};
    return computeJointProbabilityVE(net, assignment);
}





