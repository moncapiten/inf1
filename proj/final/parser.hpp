#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iterator>
#include <fstream>
#include <stdexcept>

#include "supportFunc.hpp"
//#include "supportFunc.cpp"

#include "nodes.hpp"
//#include "nodes.cpp"

using namespace std;














class Parser {
public:
    Parser() {}

    void parse(const string& input);

    void translate();

    BayesianNetwork& getNetwork();

private:
    string filename; // file name
    vector<vector<string>> dividedGroups; // vector of groups
    BayesianNetwork network; // Bayesian network object to hold the nodes

    // fileStreamer, creates from the filename it reads the file and saves it to a string
    void streamer(const string& filename);

    // tokenizer, takes the line and splits it into tokens
    void tokenizer(string line);

    // grouper, it groups tokens into logical groups
    void grouper( vector<vector<string>>& groups, const string& token);

    // translator, it takes the groups and translates them into Bayesian network nodes
    void translator(vector<string>& group);

};





























#endif // PARSER_HPP