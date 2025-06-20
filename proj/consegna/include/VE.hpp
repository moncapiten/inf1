#ifndef VE_HPP
#define VE_HPP

// ACTUAL Variable Elimination - not your method with fancy wrapping
#include <set>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "nodes.hpp"
#include "conditionalizer.hpp"
#include "supportFunc.hpp"

using namespace std;

// Real factor that stores a probability table
struct Factor {
    vector<string> vars;           // Variables in this factor
    map<vector<string>, double> table; 

    Factor() = default;
    
    // Create factor from a node's CPT
    Factor(const BayesianNode& node, const BayesianNetwork& net);

    double getValue(const vector<string>& assignment) const;
};

// Multiply two factors
Factor multiply(const Factor& f1, const Factor& f2);

// Eliminate a variable from a factor
Factor eliminate(const Factor& factor, const string& varToElim);

// ACTUAL Variable Elimination
double computeConditionalProbabilityVE(BayesianNetwork& net,
                                          const unordered_map<string, string>& evidence,
                                          const unordered_map<string, string>& conditions);

// Wrapper
double computeConditionalProbabilityVE(BayesianNetwork& net,
                                          const string& A, const string& a,
                                          const string& B, const string& b);

double computeJointProbabilityVE(BayesianNetwork& net, const unordered_map<string, string>& assignment);

double computeJointProbabilityVE(BayesianNetwork& net,
                                          const string& A, const string& a,
                                          const string& B, const string& b);



















#endif // VE_HPP