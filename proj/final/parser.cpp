#include "parser.hpp"

using namespace std;


// Just functions call, from public to privaate, nothing special

void Parser::parse(const string& input) {
    filename = input; // set the filename to the input
    streamer(filename); // call the streamer function to read the file
}

void Parser::translate(){
    for(auto group : dividedGroups) { // iterate through the groups
        if( checkParenthesis(group) == false ){
            throw runtime_error("PARSER ERROR - Unbalanced parentheses in group"); // throw an error if the parentheses are unbalanced
        }
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
// X - check probability sums
// X - check probability sums per n-tuple in CPT ( n = number of states)
// X - check if the number of states is correct
// X - check if the number of probabilities is correct
// check for loops

// --> parenthesis check is done in grouper before the translator is called


void Parser::translator(vector<string>& group){

    for( auto i = group.begin(); i < group.end()-1; ++i){

        if( *i == "network" ){
            string name = *next(i); // get the name of the network
            network = BayesianNetwork(name); // set the name of the network
            return; // return if the group is a network
        }
        else {
            // if we are trying to assign a variable to a non initialized network throw and error
            if( network.name == "" ){
                throw runtime_error("PARSER ERROR - Network was not initialized before being used"); // throw an error if the network name is not set
            }
        

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

                        auto firstParenthesisIndex = find(j, group.end(), "["); // find the first parenthesis
                        auto lastParenthesisIndex = find(j, group.end(), "]"); // find the last parenthesis

                        int declaredStatesCount = stoi(*next(firstParenthesisIndex)); // get the number of declared states


                        firstParenthesisIndex = find(j, group.end(), "{"); // find the first parenthesis
                        lastParenthesisIndex = find(j, group.end(), "}"); // find the last parenthesis

                        for( auto k = next(firstParenthesisIndex); k < lastParenthesisIndex; ++k){
                            if( *k == "," ){
                                continue;
                            } else {
                                states.push_back(*k); // add the state to the vector
                            }
                        }

                        if( states.size() != declaredStatesCount ){
                            throw runtime_error("PARSER ERROR - Number of states does not match the declared number of states for node: " + name); // throw an error if the number of states does not match the declared number
                        }

                        network.getNode_name(name).states = states; // assign the states to the node
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

                int possibleParentsStates = 1;


                for( auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == "," || *j == "|" ){
                        continue;
                    } else {
                        if( owner == "" ){
                            owner = *j; // get the owner of the node
                        } else {
                            try{
                                network.getNode_name(owner).parents.push_back(network.getNode_name(*j).ID); // add the parent to the node
                                network.getNode_name(*j).children.push_back(network.getNode_name(owner).ID); // add the child to the parent
                            } catch (const out_of_range& e) {
                                throw runtime_error("PARSER ERROR - Node " + *j + " not found in the network while parsing probabilities for node: " + owner); // throw an error if the node is not found
                            }
                            possibleParentsStates *= network.getNode_name(*j).states.size(); // calculate the number of possible states for the parents
                        }
                    }
                }


                firstParenthesisIndex = find(lastParenthesisIndex, group.end(), "{"); // find the first parenthesis
                lastParenthesisIndex = find(i, group.end(), "}"); // find the last parenthesis
                int parenDepth = 0; // variable to hold the parenthesis depth
                int CPTEntries = 0; // variable to hold the counter for the probabilities

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
                        CPTEntries++;
                    }
                }



                for(auto j = network.getNode_name(owner).probabilities.begin(); j < network.getNode_name(owner).probabilities.end(); j += network.getNode_name(owner).states.size()){

                    double testSum = accumulate(j, j + network.getNode_name(owner).states.size(), 0.0); // calculate the sum of the probabilities for each state
                    if( abs(testSum - 1.0) > 1e-7 ){
                        throw runtime_error("PARSER ERROR - Line in CPT does not sum to 1 for node: " + owner); // throw an error if the probabilities do not sum to 1
                    }
                }

                if( CPTEntries != network.getNode_name(owner).states.size() * possibleParentsStates ){
                    throw runtime_error("PARSER ERROR - Incorrect number of probabilities in the CPT for node: " + owner); // throw an error if the number of probabilities is incorrect
                }

                if( network.getNode_name(owner).parents.size() == 0 ){
                    network.getNode_name(owner).pureProb = network.getNode_name(owner).probabilities; // assign the probabilities to the node
                    return; // return if the node has no parents
                }
                return;


            }


        }

    }


}


