#include "parser.hpp"

using namespace std;


// Just functions call, from public to privaate, nothing special

void Parser::parse(const string& input) {
    filename = input; // set the filename to the input
    streamer(filename); // call the streamer function to read the file
}

void Parser::translate(){
    for(auto group : dividedGroups) { // iterate through the groups
        translator(group); // call the translator function to process the group
    }
}
BayesianNetwork& Parser::getNetwork() {
    return network; // return the Bayesian network object
}

// fileStreamer, creates from the filename it reads the file and saves it to a string
// it then calls the tokenizer to process the string into tokens
void Parser::streamer(const string& filename){
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



bool Parser::isTokenChar(char c) {
    static const array<bool, 256> tokenTable = [] {
        array<bool, 256> table{};
        for (char ch = 'a'; ch <= 'z'; ++ch) table[static_cast<unsigned char>(ch)] = true;
        for (char ch = 'A'; ch <= 'Z'; ++ch) table[static_cast<unsigned char>(ch)] = true;
        for (char ch = '0'; ch <= '9'; ++ch) table[static_cast<unsigned char>(ch)] = true;

        const char extras[] = "_-/:+=<>!?&^%#@.";
        for (char ch : extras) table[static_cast<unsigned char>(ch)] = true;

        return table;
    }();
    return tokenTable[static_cast<unsigned char>(c)];
}





// tokenizer, takes the line and splits it into tokens
// afterwards it calls the grouper to group together logic groups
void Parser::tokenizer(string line) {
    string token;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if(isTokenChar(c)){
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
void Parser::grouper( vector<vector<string>>& groups, const string& token){
    if( token == "" ){
        return; // return if the token is empty
    }
    if (token == "network" || token == "variable" || token == "probability") {
        groups.push_back({ token }); // if there's a logical block ⇒ add it to the groups
    } else {
        groups.back().push_back(token); // add the token to the last group
    }
}



// translator, it takes the groups and translates them into Bayesian nodes

// TO DO:
// check probability sums
// check probability sums per n-tuple in CPT ( n = number of states)
// check if the number of states is correct
// check if the number of probabilities is correct
// check for loops


void Parser::translator(vector<string>& group){

    for( auto i = group.begin(); i < group.end()-1; ++i){
        if( *i == "variable" ){
            string name = *next(i); // get the name of the variable

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
            vector<double> probabilities; // vector to hold the probabilities
            auto firstParenthesisIndex = find(i, group.end(), "("); // find the first parenthesis
            auto lastParenthesisIndex = find(i, group.end(), ")"); // find the last parenthesis
            string owner;



            for( auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                if( *j == "," || *j == "|" ){
                    continue;
                } else {
                    if( owner == "" ){
                        owner = *j; // get the owner of the node
                    } else {
                        network.getNode_name(owner).parents.push_back(network.getNode_name(*j).ID); // add the parent to the node
                        network.getNode_name(*j).children.push_back(network.getNode_name(owner).ID); // add the child to the parent
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
                }
            }




            if( network.getNode_name(owner).parents.size() == 0 ){
                network.getNode_name(owner).pureProb = network.getNode_name(owner).probabilities; // assign the probabilities to the node
                return; // return if the node has no parents
            }
            return;


        }



    }


}


