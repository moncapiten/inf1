#include "parser.hpp"
/*
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
#include "supportFunc.cpp"

#include "nodes.hpp"
#include "nodes.cpp"
*/


using namespace std;









class Parser {
public:
    Parser(const string& input) : filename(input) {
        streamer(filename); // autopopulate tokens on creation
    }


    void translate(){
        for(auto group : dividedGroups) { // iterate through the groups
            translator(group); // call the translator function to process the group
        }
    }
    BayesianNetwork& getNetwork() {
        return network; // return the Bayesian network object
    }
/*
    vector<BayesianNode> getNodes() {
        return nodes; // return the nodes
    }

    unordered_map<string, int> getIndecesArchive() {
        return indecesArchive; // return the indeces archive
    }
    unordered_map<int, string> getNamesArchive() {
        return namesArchive; // return the names archive
    }
*/

private:
    string filename; // file name
    vector<vector<string>> dividedGroups; // vector of groups
//    vector<BayesianNode> nodes; // vector of Bayesian nodes
    BayesianNetwork network; // Bayesian network object to hold the nodes
//    unordered_map<string, int> indecesArchive; // map to hold the names of the nodes and their indices
//    unordered_map<int, string> namesArchive; // map to hold the indices of the nodes and their names





    // fileStreamer, creates from the filename it reads the file and saves it to a string
    // it then calls the tokenizer to process the string into tokens
    void streamer(const string& filename){
        ifstream file(filename);
        string line;

        if (file.is_open()) {
            while (getline(file, line)) {
                tokenizer(line); // call the tokenizer function to process the line
            }
            file.close();
            return; // return success code
        } else {
            throw runtime_error("Unable to open file"); // throw an error if the file cannot be opened
        }
    }




    // tokenizer, takes the line and splits it into tokens
    // afterwards it calls the grouper to group together logic groups
    void tokenizer(string line) {
        string token;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];

            if (isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '/' || c == ':' || c == '+' || c == '=' || c == '<' || c == '>' || c == '!' || c == '?' || c == '&' || c == '^' || c == '%' || c == '#' || c == '@' || c == '.') { 
                token += c;
            } else {
                if (!token.empty()) {
                    grouper(dividedGroups, token);
                    token.clear();
                }
                if (!isspace(c)) {
                    string sym(1, c);
                    grouper(dividedGroups, sym);
                }
            }
        }
        if (!token.empty()) {
            grouper(dividedGroups, token);
        }
    }




    // grouper, it groups tokens into logical groups
    // it also eliminates empty tokens
    void grouper( vector<vector<string>>& groups, const string& token){
        if( token == "" ){
            return; // return if the token is empty
        }
        if (token == "network" || token == "variable" || token == "probability") {
            groups.push_back({ token }); // if there's a logical block ⇒ add it to the groups
        } else {
            groups.back().push_back(token); // add the token to the last group
        }
    }






/*    void updateMaps(string name, int index){
        indecesArchive[name] = index; // add the name to the map with the index of the node
        namesArchive[index] = name; // add the index to the map with the name of the node
    }*/







    // translator, it takes the groups and translates them into Bayesian nodes

    // TO DO:
    // check probability sums
    // check probability sums per n-tuple in CPT ( n = number of states)
    // check if the number of states is correct
    // check if the number of probabilities is correct
    // check for loops


    void translator(vector<string>& group){
        

        for( auto i = group.begin(); i < group.end()-1; ++i){
            if( *i == "variable" ){
                string name = *next(i); // get the name of the variable

//                network.updateMaps(name, network.size()); // update the maps with the name and index of the node

//                nodes.push_back(BayesianNode(name)); // create a new Bayesian node with the name
//                nodes[indecesArchive[name]].ID = indecesArchive[name]; // set the state of the node to false
                BayesianNode node(name); // create a new Bayesian node with the name
                node.ID = network.size(); // set the ID of the node to the size of the network
                network.addNode(node); // add the node to the network

                
                
                for( auto j = next(i); j < group.end(); ++j){

                    if( *j == "type" ){
                        if( *next(j, 1) != "discrete" ){
                            return; // return an error code
                        }
                        vector<string> states; // vector to hold the states

                        auto firstParenthesisIndex = find(j, group.end(), "{"); // find the first parenthesis
                        auto lastParenthesisIndex = find(j, group.end(), "}"); // find the last parenthesis

                        for( auto k = next(firstParenthesisIndex); k < lastParenthesisIndex; ++k){
                            if( *k == "," ){
                                continue;
                            } else {
                                states.push_back(*k); // add the state to the vector
                            }
                        }
                        network.getNode_name(name).states = states; // assign the states to the node
//                        nodes[indecesArchive[group[1]]].states = states; // assign the states to the node
                        return;

                    }

                }


                return; // return if the group is a variable
                
            }




            if( *i == "probability" ){
    //            cout << "probability" << endl; // print the probability group
                vector<double> probabilities; // vector to hold the probabilities
                auto firstParenthesisIndex = find(i, group.end(), "("); // find the first parenthesis
                auto lastParenthesisIndex = find(i, group.end(), ")"); // find the last parenthesis
                string owner;
    //            vector<string> parents; // vector to hold the parents



                for( auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == "," || *j == "|" ){
                        continue;
                    } else {
                        if( owner == "" ){
                            owner = *j; // get the owner of the node
                        } else {
                            network.getNode_name(owner).parents.push_back(network.getNode_name(*j).ID); // add the parent to the node
                            network.getNode_name(*j).children.push_back(network.getNode_name(owner).ID); // add the child to the parent
//                            nodes[indecesArchive[owner]].parents.push_back(indecesArchive[*j]); // add the parent to the node
//                            nodes[indecesArchive[*j]].children.push_back(indecesArchive[owner]); // add the child to the parent
                        }
                    }
                }


                firstParenthesisIndex = find(lastParenthesisIndex, group.end(), "{"); // find the first parenthesis
                lastParenthesisIndex = find(i, group.end(), "}"); // find the last parenthesis
                int parenDepth = 0; // variable to hold the parenthesis depth


                for(auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == ";" ){
                        if(parenDepth != 0){
                            return; // return an error code
                        } else{
                            continue; // continue if the parenthesis is not closed
                        }
                    } else if( *j == "(" ){
                        parenDepth++; // increase the parenthesis depth
                    } else if( *j == ")" ){
                        parenDepth--; // decrease the parenthesis depth
                    } else if( *j == "," || *j == "table" ){
                        continue; // continue if the token is a comma
                    } else if (parenDepth == 0){
                        network.getNode_name(owner).probabilities.push_back(stod(*j)); // convert the string to double and add it to the vector
//                        nodes[indecesArchive[owner]].probabilities.push_back(stod(*j)); // convert the string to double and add it to the vector
                    }
                }




//                if( nodes[indecesArchive[owner]].parents.size() == 0 ){
//                    nodes[indecesArchive[owner]].pureProb = nodes[indecesArchive[owner]].probabilities; // assign the probabilities to the node
//                    return; // return if the node has no parents
//                }
                if( network.getNode_name(owner).parents.size() == 0 ){
                    network.getNode_name(owner).pureProb = network.getNode_name(owner).probabilities; // assign the probabilities to the node
                    return; // return if the node has no parents
                }
                return;


            }



        }


    }














};

















































































