#ifndef CONDITIONALIZER_HPP
#define CONDITIONALIZER_HPP

#include <unordered_map>
#include <numeric>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "nodes.hpp"

using namespace std;


// Helper to get index of a state
int stateIndex(const BayesianNode& node, const string& state);

// Compute the index in the CPT for a given node and assignment
int computeCPTIndex(const BayesianNode& node, const BayesianNetwork& net, const unordered_map<string, string>& assignment);

// Recursively compute joint probability for an assignment
double computeJointProb(const BayesianNetwork& net, const unordered_map<string, string>& assignment);
double computeJointProbability(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b);

// Generate all consistent full assignments extending a partial one
void enumerateAllAssignments(const BayesianNetwork& net, unordered_map<string, string>& partial, vector<unordered_map<string, string>>& out, size_t i = 0);

// helper to check if all is good and computations can start
void checkComputability(BayesianNetwork& net, const unordered_map<string, string>& evidence, const unordered_map<string, string>& conditions);
void checkComputability(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b);

// Main function: P(A = a | B = b)
double oldComputeConditionalProbability(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b);

// Main function redone slightly more efficient
double computeConditionalProbability(BayesianNetwork& net, const unordered_map<string, string>& evidence, const unordered_map<string, string>& conditions);
double computeConditionalProbability(BayesianNetwork& net, const string& A, const string& a, const string& B, const string& b);

//double computeConditionalProbability(BayesianNetwork& net, const string& A, const string& a, const string& B);


#endif // CONDITIONALIZER_HPP