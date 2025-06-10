#include "newparser.hpp"

using namespace std;


// Just functions call, from public to private, nothing special
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


void Parser::streamer(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("PARSER ERROR - Unable to open file: " + filename);
    }

    string line;
    size_t line_num = 1;
    while (getline(file, line)) {
        tokenizer(line); // line by line processing
    }

    // Handle read errors
    if (file.bad()) {
        throw runtime_error("PARSER ERROR - I/O error reading: " + filename);
    }
    if (file.fail() && !file.eof()) {
        throw runtime_error("PARSER ERROR - Format error in: " + filename);
    }
    // the closing is donw automatically by the destructor of ifstream
}


// helper function to check if the character is a valid token character
// uses a static array for performance, it just checks if the place in the array coincident
// with the character in check is true or false and returns that
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
// maybe add check for comments?

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
                throw runtime_error("PARSER ERROR - Network was not initialized before trying to add elements to it"); // throw an error if the network name is not set
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
//                            if (net.getSubstitutionIndeces().find(A) == net.getSubstitutionIndeces().end()) throw invalid_argument("Node " + A + " not found in the network."); check if the owner exists? 
//                            as it stands now i am afrad that if i had a bif file with CPT of a node that never was created the parser would error in incorrect ways
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
                    if( abs(testSum - 1.0) > 1e-6 ){
                        throw runtime_error("PARSER ERROR - Line " + to_string(distance(network.getNode_name(owner).probabilities.begin(), j) / network.getNode_name(owner).states.size() ) + " in CPT does not sum to 1 for node: " + owner + "  ( sum = " + to_string(testSum) + " )"); // throw an error if the probabilities do not sum to 1
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













void Parser::interpret(const string& input) {
    commandGroups.clear(); // Clear previous command groups
    
    // Reuse your existing tokenizer, but with commandGroups
    string token;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if(isTokenChar(c)){
            token += c;
        } else {
            if (!token.empty()) {
                commandGrouper(commandGroups, token);
                token.clear();
            }
            if (!isspace(c)) {
                string sym(1, c);
                commandGrouper(commandGroups, sym);
            }
        }
    }
    if (!token.empty()) {
        commandGrouper(commandGroups, token);
    }
    
    // Execute the command
    if (!commandGroups.empty() && !commandGroups[0].empty()) {
        executeCommand(commandGroups[0]);
    }
}

// Modified grouper for commands (simpler - just one group per command)
void Parser::commandGrouper(vector<vector<string>>& groups, const string& token) {
    if (token.empty()) return;
    
    if (groups.empty()) {
        groups.push_back({token});
    } else {
        groups[0].push_back(token);
    }
}

// Execute the command based on the first token
void Parser::executeCommand(const vector<string>& commandTokens) {
    if (commandTokens.empty()) return;
    
    string command = commandTokens[0];
    auto it = commands.find(command);
    
    // if the command is found, execute it with the rest of the tokens as arguments( could do rest - the first, which is just the command name, changes little in the grand scheme)
    if (it != commands.end()) {
        it->second(commandTokens);
    } else {
        cout << "Unknown command: " << command << endl;
        cout << "Type 'help' for available commands." << endl;
    }
}

// just adds the commands to the map with their handlers( handler have to match the signature: void(const vector<string>&) )
void Parser::registerCommand(const string& name, function<void(const vector<string>&)> handler) {
    commands[name] = handler;
}

void Parser::runInteractiveMode() {
    // Register default commands
    registerCommand("help", [this](const vector<string>& args) {
        cout << "Available commands:\n";
        for (const auto& cmd : commands) {
            cout << "  " << cmd.first << "\n";
        }
    });
    
    registerCommand("quit", [](const vector<string>& args) {
        cout << "Goodbye Crocodile!\n";
        exit(0);
    });
    
    registerCommand("load", [this](const vector<string>& args) {
        if (args.size() != 2) {
            cout << "Usage: load <filename>\n";
            return;
        }
        try {
            parse(args[1]); // Reuse existing parse function
            translate();    // Reuse existing translate function
            cout << "Successfully loaded network: " << network.name << "\n";
        } catch (const exception& e) {
            cout << "Error loading file: " << e.what() << "\n";
        }
    });
    
    registerCommand("network", [this](const vector<string>& args) {
        if (network.name.empty()) {
            cout << "No network loaded.\n";
        } else {
            if (args.size() > 1 && args[1] == "-v" ) {
                cout << network;
            }
            else {
                cout << "Current network: " << network.name << "\n";
                cout << "Nodes: " << network.size() << "\n";
            }
        }
    });
    
    registerCommand("node", [this](const vector<string>& args) {
        if (network.name.empty()) {
            cout << "No network loaded.\n";
            return;
        }
        if (args.size() < 2) {
            cout << "Usage: node <node_name> or node <node_ID>\n";
            return;
        }
        try {
            if (isdigit(args[1][0])) {
                int id = stoi(args[1]);
                cout << network.getNode_ID(id);
            } else {
                cout << network.getNode_name(args[1]);
            }
        } catch (const out_of_range& e) {
            cout << "Node not found: " << args[1] << "\n";
        } catch (const exception& e) {
            cout << "Error retrieving node: " << e.what() << "\n";
        }
    });
    
    // Interactive loop
    string input;
    cout << "Bayesian Network Parser - Interactive Mode\n";
    cout << "Type 'help' for commands, 'quit' to exit\n";
    
    while (true) {
        // arrow cause it's way cooler than just ">>"
        cout << "==> ";
        getline(cin, input);
        if (!input.empty()) {
            interpret(input);
        }
    }
}


