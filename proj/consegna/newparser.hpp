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


    //COMMAND INTERPRETER

    bool logging = false; // flag to indicate if logging is enabled
    ofstream log; // log file stream

    // interpreter to process commands
    void interpret(const string& input);

    // register a command with its handler
    void registerCommand(const string& name, function<void(const vector<string>&)> handler, const vector<string>& args = {}, const string& description = "");

    // function to talk to the user and also log the response if so enabled
    void respond(const string& response, bool toConsole = true);

    // run in order to use the program interactively
    // not necessary but works mighty fine
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


    // command map to hold commands and their handlers
    map<string, function<void(const vector<string>&)>> commands;
    map<string, vector<string>> commandArgs; // to hold arguments for commands
    map<string, string> commandDescription; // to hold descriptions for commands

    vector<vector<string>> commandGroups; // separate from dividedGroups

    // commandGrouper, groups tokens into a single command group
    void commandGrouper(vector<vector<string>>& groups, const string& token);

    // executeCommand, executes the command based on the first token
    void executeCommand(const vector<string>& commandTokens);


};

#endif // PARSER_HPP