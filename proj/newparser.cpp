#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <windows.h> // for colored text in console
#include <string>
#include <algorithm>
#include <cctype>
#include <bitset>


#include "supportFunc.hpp"
#include "supportFunc.cpp"

#include "nodes.hpp"
#include "nodes.cpp"


using namespace std;



vector<BayesianNode> nodes; // vector of Bayesian nodes
vector<vector<string>> dividedGroups; // vector of groups
// there is substitutionIndeces and substitutionNames in nodes.cpp, but they are used but not declared here







void translator(vector<string>& group){
    if( checkParenthesis(group) ){
//        cout << "The group is valid" << endl; // print the group if it is valid                           MAYBE REINSTATE? TBD
    } else {
//        cout << "The group is invalid" << endl; // print the group if it is invalid                       MAYBE REINSTATE? TBD
        return; // return an error code
    }

    for( auto i = group.begin(); i < group.end()-1; ++i){
        if( *i == "variable" ){
            string name = *next(i); // get the name of the variable

            updateMaps(name, nodes.size()); // update the maps with the name and index of the node
//            substitutionIndeces[name] = nodes.size(); // add the name to the map with the index of the node
            nodes.push_back(BayesianNode(name)); // create a new Bayesian node with the name
            nodes[substitutionIndeces[name]].ID = substitutionIndeces[name]; // set the state of the node to false
            
        }

        if ( *i == "type"){
            if( *next(i, 1) == "discrete" ){
                vector<string> states; // vector to hold the states

                auto firstParenthesisIndex = find(i, group.end(), "{"); // find the first parenthesis
                auto lastParenthesisIndex = find(i, group.end(), "}"); // find the last parenthesis

                for( auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == "," ){
                        continue;
                    } else {
                        states.push_back(*j); // add the state to the vector
                    }
                }
                /*
                states.push_back( *next(firstParenthesisIndex) );
//                string firstStateName = *next(firstParenthesisIndex); // get the first state name

                if( states[0].find(",") != string::npos ){
                    states[0].erase(states[0].find(","));
                    states.push_back( *next( next(firstParenthesisIndex) ) );

                } else {
                    states.push_back( *next( next( next(firstParenthesisIndex) ) ) );
                }

                */

                // assign the states to the node
                nodes[substitutionIndeces[group[1]]].states = states; // assign the states to the node


            }
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
                        nodes[substitutionIndeces[owner]].parents.push_back(substitutionIndeces[*j]); // add the parent to the node
                        nodes[substitutionIndeces[*j]].children.push_back(substitutionIndeces[owner]); // add the child to the parent
                    }
                }
            }


            firstParenthesisIndex = find(i, group.end(), "{"); // find the first parenthesis
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
                    cout << *j << " "; // print the token if the parenthesis depth is 0
                    nodes[substitutionIndeces[owner]].probabilities.push_back(stod(*j)); // convert the string to double and add it to the vector
                }
            }



            if( nodes[substitutionIndeces[owner]].parents.size() == 0 ){
                nodes[substitutionIndeces[owner]].pureProb = nodes[substitutionIndeces[owner]].probabilities; // assign the probabilities to the node
                return; // return if the node has no parents
            }
            return;

/*
            vector<double> sums( nodes[substitutionIndeces[owner]].states.size(), 0.0 ); // vector to hold the sums of the probabilities
            int counterToSelectProbs = 0;


            // here is the calculation of marginal probabilities
            for( auto j = nodes[substitutionIndeces[owner]].probabilities.begin(); j < nodes[substitutionIndeces[owner]].probabilities.end(); j+=nodes[substitutionIndeces[owner]].states.size()){

                for( auto k = j; k < j + nodes[substitutionIndeces[owner]].states.size(); k++){
                    double toBeAdded0 = *k; // get the first probability

                    vector<int> bitVector = toBitVector(counterToSelectProbs, nodes[substitutionIndeces[owner]].parents.size() ); // convert the counter to a bit vector
                    reverse(bitVector.begin(), bitVector.end()); // reverse the vector

                    for( int m = 0; m < nodes[substitutionIndeces[owner]].parents.size(); m++){
                        toBeAdded0 *= nodes[ nodes[substitutionIndeces[owner]].parents[m] ].pureProb[ bitVector[m] ]; // multiply the probabilities
                    }
                    sums[k-j] += toBeAdded0; // add the probabilities to the sum
                }
                counterToSelectProbs++;
            }

            nodes[substitutionIndeces[owner]].pureProb = sums; // assign the pure probabilities to the node
*/

            return;


        }



    }


}




void grouper( vector<vector<string>>& groups, string token){

    if( token == "" ){
        return; // return if the token is empty
    }
    if (token == "network" || token == "variable" || token == "probability") {

        groups.push_back({ token }); // add the network group
    } else {
//        if( token != "" ){
                groups.back().push_back(token); // add the token to the last group
//        }
    }
}





// new tokenizer function
void tokenizer(string line) {
    string token;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '/' || c == ':' || c == '+' || c == '=' || c == '<' || c == '>' || c == '!' || c == '?' || c == '&' || c == '^' || c == '%' || c == '#' || c == '@' || c == '.') { 
            token += c;
        }
//        else if (c == '.' && i > 0 && isdigit(line[i - 1]) && i + 1 < line.size() && isdigit(line[i + 1])) {
//            token += c; // part of a number
//        }
        else {
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



void streamFile(string filename){
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
//            cout << line << '\n';
            tokenizer(line); // call the tokenizer function to process the line
        }
        file.close();
        return; // return success code
    } else {
        cerr << "Unable to open file\n";
        return; // return an error code
    }
}



int main(){
    string filename = "child.bif"; // file name


    streamFile(filename); // read the file and print its content

    for(auto group : dividedGroups) { // iterate through the groups
/*        cout << "GROUP: ";
        for( auto i : group ){
            cout << '[' << i << "] "; // print the group
        }*/
        cout << endl; // print a new line
        translator(group); // call the translator function to process the group
    }

    cout << "NETWORK:" << endl << endl; // print the nodes
    cout << nodes; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes


    return 0;
}