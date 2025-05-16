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
                

                states.push_back( *next(firstParenthesisIndex) );
//                string firstStateName = *next(firstParenthesisIndex); // get the first state name

                if( states[0].find(",") != string::npos ){
                    states[0].erase(states[0].find(","));
                    states.push_back( *next( next(firstParenthesisIndex) ) );

                } else {
                    states.push_back( *next( next( next(firstParenthesisIndex) ) ) );
                }

                // assign the states to the node
                nodes[substitutionIndeces[group[1]]].states = states; // assign the states to the node


            }
        }
        


        //  IM PRETTY SURE IT CAN BE IMPOROVED, THE PARENTHESES EITHER CONTAIN 1 NAME, THE OWNER, OR >1, AND THE SECOND ONWARD IS JUST THE PARENTS, IT SHOULD WORK BETTER IF I CAN MANAGE TO DO THAT WAY
        if( *i == "probability" ){
//            cout << "probability" << endl; // print the probability group
            vector<double> probabilities; // vector to hold the probabilities
            auto firstParenthesisIndex = find(i, group.end(), "("); // find the first parenthesis
            auto lastParenthesisIndex = find(i, group.end(), ")"); // find the last parenthesis
            string owner;
//            vector<string> parents; // vector to hold the parents


            if( next(next(firstParenthesisIndex)) != lastParenthesisIndex) {
                
                for( auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == "," ){
                        continue;
                    } else if( *next(j) == "|" ){
                        owner = *j; // get the owner of the node
                        j++;
                    } else {
                        nodes[substitutionIndeces[owner]].parents.push_back(substitutionIndeces[*j]); // add the parent to the node
                        nodes[substitutionIndeces[*j]].children.push_back(substitutionIndeces[owner]); // add the child to the parent
//                        parents.push_back(*j); // add the parent to the vector
                    }

                }



                auto firstParenthesisIndex = find(i, group.end(), "{"); // find the first parenthesis
                auto lastParenthesisIndex = find(i, group.end(), "}"); // find the last parenthesis
                
                
               vector< vector<string>::iterator > semicolonIndeces; // vector to hold the indices of the semicolons

               semicolonIndeces.push_back(firstParenthesisIndex);
                for(auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == ";" ){
                        semicolonIndeces.push_back(j); // add the index of the semicolon to the vector
                    }
                }
                semicolonIndeces.push_back(lastParenthesisIndex); // add the index of the last parenthesis to the vector

                for( int j = 0; j < semicolonIndeces.size()-1; j++){
                    auto it = semicolonIndeces[j];

                    for(auto k = it+1; k < semicolonIndeces[j+1]; ++k){
                        if( *k == ")" ){
                            auto newCommaIndex = find(k, semicolonIndeces[j+1], ","); // find the first occurrence of ","                                                  // SHOULD BE POSSIBLE TO IMPROVE
                            nodes[substitutionIndeces[owner]].probabilities.push_back(stod(*prev(newCommaIndex))); // convert the string to double and add it to the vector
                            nodes[substitutionIndeces[owner]].probabilities.push_back(stod(*next(newCommaIndex))); // convert the string to double and add it to the vector

                        }
                         
                    }

//                    cout << endl; // print a new line
                }


                cout << owner << "\n";
                vector<double> sums = {0, 0}; // vector to hold the sums of the probabilities
                int counterToSelectProbs = 0;


                // here is the calculation of marginal probabilities
                for( auto k = nodes[substitutionIndeces[owner]].probabilities.begin(); k < nodes[substitutionIndeces[owner]].probabilities.end(); k+=nodes[substitutionIndeces[owner]].states.size()){
                    double toBeAdded0 = *k; // get the first probability
                    double toBeAdded1 = *(k+1); // get the second probability

                    cout << *k << " "; // print the probabilities
                    cout << *(k+1) << " "; // print the probabilities

                    cout << counterToSelectProbs << " "; // print the probabilities
                    vector<int> bitVector = toBitVector(counterToSelectProbs, nodes[substitutionIndeces[owner]].parents.size() ); // convert the counter to a bit vector
                    reverse(bitVector.begin(), bitVector.end()); // reverse the vector
                    string bitString = toString( bitVector );
                    cout << bitString << " "; // print the probabilities

                    

                    for( int m = 0; m < nodes[substitutionIndeces[owner]].parents.size(); m++){
//                        cout << nodes[ nodes[substitutionIndeces[owner]].parents[m] ].name << " ";
//                        cout << nodes[ nodes[substitutionIndeces[owner]].parents[m] ].pureProb[counterToSelectProbs] << " "; // print the pure probabilities
                        toBeAdded0 *= nodes[ nodes[substitutionIndeces[owner]].parents[m] ].pureProb[ bitVector[m] ]; // multiply the probabilities
                        toBeAdded1 *= nodes[ nodes[substitutionIndeces[owner]].parents[m] ].pureProb[ bitVector[m] ]; // multiply the probabilities
                    }

                    sums[0] += toBeAdded0; // add the probabilities to the sum
                    sums[1] += toBeAdded1; // add the probabilities to the sum


                    cout << "| "; // print the separator

                    counterToSelectProbs++;
                }

                nodes[substitutionIndeces[owner]].pureProb = sums; // assign the pure probabilities to the node
                cout << "\n" << sums[0] << " " << sums[1] << "\n\n"; // print the sums of the probabilities







            } else {

                // ORPHAN CASE

                owner = *next(firstParenthesisIndex); // get the owner of the node
//                cout << owner << " is an orphan" << endl; // print the owner of the node
                auto firstTableIndex = find(i, group.end(), "table"); // find the first occurrence of "table"
                auto firstSemicolonIndex = find(i, group.end(), ";"); // find the first occurrence of ";"
                auto commaIndex = find(firstTableIndex, firstSemicolonIndex, ","); // find the first occurrence of ","

                vector<double> probabilities = { stod( *prev(commaIndex) ), stod( *next(commaIndex) ) }; // convert the string to double and add it to the vector

                // check if the sum of the probabilities is equal to 1
                double sum = 0;
                for( auto j = probabilities.begin(); j < probabilities.end(); ++j){
                    sum += *j; // sum the probabilities
                }
                if( sum != 1 ){
                    cerr << "The sum of the probabilities is not equal to 1" << endl;
                    return; // return an error code
                }
                nodes[substitutionIndeces[owner]].probabilities = probabilities; // assign the probabilities to the node
                nodes[substitutionIndeces[owner]].pureProb = probabilities; // the node is an orphan ⇒ the pure probabilities are just the CPT values
                nodes[substitutionIndeces[owner]].parents = {}; // assign the parents to the node
                nodes[substitutionIndeces[owner]].children = {}; // assign the children to the node

            }


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

        if (isalpha(c)) {
            token += c;
        }
        else if (isdigit(c) || (c == '.' && i > 0 && isdigit(line[i-1]) && i+1 < line.size() && isdigit(line[i+1]))) {
            token += c; // part of a number like 0.2
        }
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
    string filename = "hailfinder.bif"; // file name


    streamFile(filename); // read the file and print its content

    for(auto group : dividedGroups) { // iterate through the groups
        cout << "GROUP: ";
        for( auto i : group ){
            cout << '[' << i << "] "; // print the group
        }
        cout << endl; // print a new line
        translator(group); // call the translator function to process the group
    }

    cout << "NETWORK:" << endl << endl; // print the nodes
    cout << nodes; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes


    return 0;
}