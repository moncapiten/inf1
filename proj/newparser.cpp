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















vector<int> reorder(vector<BayesianNode> nodesIndeces){
    vector<int> swapList;
    for( auto i:nodesIndeces){
        for( auto j : i.parents){
            if( find(swapList.begin(), swapList.end(), j) == swapList.end() ){
                swapList.push_back(j);
            }

        }
        if( find(swapList.begin(), swapList.end(), i.ID) == swapList.end() ){
            swapList.push_back(i.ID);
        }

    }
    return swapList;
}




vector<int> reorder(vector<int> nodesIndeces){
    vector<int> swapList;
    for( auto i:nodesIndeces){
        for( auto j : nodes[i].parents){
            if( find(swapList.begin(), swapList.end(), j) == swapList.end() ){
                swapList.push_back(j);
            }

        }
        swapList.push_back(i);

    }
    return swapList;
}


























void updateVector(vector<int>& parentsStates, vector<int> parentList){
    bool rollover = false;
    for( int i = 0; i < parentsStates.size(); i++){
        
        if( rollover || i==0 ) parentsStates[i]++;

        rollover = false;
        if(parentsStates[i] > nodes[parentList[i]].states.size()-1){
            parentsStates[i] = 0;
            rollover = true;
        }
    }
}













// marginalization function
// I still hate you for this Fabio

void marginalize(BayesianNode& node) {
    if (node.parents.empty()) {
        node.pureProb = node.probabilities; // assign the probabilities to the node
        return; // return if the node has no parents
    }

    vector<double> sums(node.states.size(), 0.0); // vector to hold the sums of the probabilities
    vector<int> statesOfEachParent(node.parents.size(), 0); // vector to hold the states of each parent

    // here is the calculation of marginal probabilities
    for (auto j = node.probabilities.begin(); j < node.probabilities.end(); j += node.states.size()) {

        for (auto k = j; k < j + node.states.size(); k++) {
            double toBeAdded0 = *k; // get the first probability



//            cout << statesOfEachParent << " "; // print the states of each parent
//            cout << endl;

            for (int m = 0; m < node.parents.size(); m++) {
                toBeAdded0 *= nodes[node.parents[m]].pureProb[statesOfEachParent[m]]; // multiply the probabilities
            }
            sums[k - j] += toBeAdded0; // add the probabilities to the sum
        }
        updateVector(statesOfEachParent, node.parents);
    }

    node.pureProb = sums; // assign the pure probabilities to the node
    return;
}




















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
                    nodes[substitutionIndeces[group[1]]].states = states; // assign the states to the node
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
//                    cout << *j << " "; // print the token if the parenthesis depth is 0
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
    string filename = "myothertest.bif"; // file name


    streamFile(filename); // read the file and print its content

    for(auto group : dividedGroups) { // iterate through the groups
/*        cout << "GROUP: ";
        for( auto i : group ){
            cout << '[' << i << "] "; // print the group
        }*/
        cout << endl; // print a new line
        translator(group); // call the translator function to process the group
    }

    vector<int> reorderedList(nodes.size());    // vector to hold the list reordered so that no node ever appears before its parents

    reorderedList = reorder(nodes);


    cout << "Original list: ";
    for( auto i : nodes){
        cout << i.ID << " "; // print the original list
    }
    cout << endl; // print a new line


    cout << "Reordered list: ";
    for( auto i : reorderedList){
        cout << i << " "; // print the reordered list
    }
    cout << endl; // print a new line



    for( auto& i : reorderedList ){
        marginalize(nodes[i]); // marginalize the nodes
//        cout << endl;
//        cout << i;
//        cout << i.pureProb << endl; // print the pure probabilities
//        cout << endl;
    }

    cout << "\n\n\nNETWORK:" << endl << endl; // print the nodes
    cout << nodes; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes

    cout << "That's all folks!" << endl; // print the end message

//    cout << nodes[ substitutionIndeces["LVH"]].name;
//    cout << nodes[ 11].name;


    return 0;
}