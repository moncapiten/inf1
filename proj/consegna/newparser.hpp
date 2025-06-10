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
#include <array>
#include <map>
#include <functional>

#include "supportFunc.hpp"
#include "nodes.hpp"


using namespace std;




class Parser {
public:
    Parser() {}
    BayesianNetwork network; // Bayesian network object to hold the nodes


    // parse the input file
    void parse(const string& input);

    void showGroups() const {
        for (const auto& group : dividedGroups) {
            cout << "Group: ";
            for (const auto& token : group) {
                cout << token << " ";
            }
            cout << endl;
        }
    }

    // translate from raw groups to Bayesian network nodes
    void translate();

    // tokenizer, takes the line and splits it into tokens
    void tokenizer(string line);

    void flush() {
        dividedGroups.clear(); // clear the vector of groups
        network = BayesianNetwork(); // reset the Bayesian network object
    }

    // outputs the Bayesian network object
    BayesianNetwork& getNetwork();







    void interpret(const std::string& input);
    void registerCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> handler);
    void runInteractiveMode();

private:
    string filename; // file name
    vector<vector<string>> dividedGroups; // vector of groups

    // fileStreamer, creates from the filename it reads the file and saves it to a string
    void streamer(const string& filename);

    // helper function to the tokenizer, checks if the character is a valid token character
    bool isTokenChar(char c);



    // grouper, it groups tokens into logical groups
    void grouper( vector<vector<string>>& groups, const string& token);

    // translator, it takes the groups and translates them into Bayesian network nodes
    void translator(vector<string>& group);



    map<std::string, std::function<void(const std::vector<std::string>&)>> commands;
    vector<std::vector<std::string>> commandGroups; // separate from dividedGroups
    
    void commandGrouper(std::vector<std::vector<std::string>>& groups, const std::string& token);
    void executeCommand(const std::vector<std::string>& commandTokens);


};






    























#endif // PARSER_HPP