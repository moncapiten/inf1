#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <windows.h> // for colored text in console
#include <string>
#include <algorithm>
#include <cctype>


using namespace std;




struct BayesianNode {
    string name; // name of the node
    vector<string> parents; // parents of the node
    vector<string> children; // children of the node
    vector<string> states; // states of the node
    vector<double> probabilities; // probabilities of the node
    bool state;

    BayesianNode(string n, vector<string> p, vector<string> c, vector<string> s, vector<double> prob) {
        name = n; // initialize the name of the node
        parents = p; // initialize the parents of the node
        children = c; // initialize the children of the node
        states = s; // initialize the states of the node
        probabilities = prob; // initialize the probabilities of the node
    }

    BayesianNode(string n) {
        name = n; // initialize the name of the node
    }

};








// Overload for a single node
ostream& operator<<(ostream& os, const BayesianNode& node) {
    os << "Node: " << node.name << '\n';

    os << "  Parents: ";
    for (const auto& p : node.parents) os << p << ' ';
    os << '\n';

    os << "  Children: ";
    for (const auto& c : node.children) os << c << ' ';
    os << '\n';

    os << "  States: ";
    for (const auto& s : node.states) os << s << ' ';
    os << " [0, 1]";
    os << '\n';

    os << "  Probabilities: ";
    for (const auto& prob : node.probabilities) os << prob << ' ';
    os << "\n";

    return os;
}

// Overload for a vector of nodes
ostream& operator<<(ostream& os, const vector<BayesianNode>& nodes) {
    for (const auto& node : nodes) {
        os << node << '\n';
    }
    return os;
}











vector<BayesianNode> nodes; // vector of Bayesian nodes
vector<vector<string>> dividedGroups; // vector of groups
unordered_map<string, int> substitutionNames; // map to hold the names of the nodes and their indices




template <typename T>
class Stack {
    private:
        vector<T> data;

    public:
//        Stack(){
//        data = {};

                        // uso & per permettermi di passare in maniera più veloce tramite riferimento
        void push(const T & x){
            data.push_back(x);
        }

        T pop(){

            if( data.empty() ){
                throw out_of_range("Stack<>::pop(): empty stack");
            } 
            T temporary_container = data.back();
            data.pop_back();

            return temporary_container;
        }

        auto empty(){
            return data.empty();
        }

        auto size(){
            return data.size();
        }


        friend ostream & operator << (ostream & os, const Stack<T> & s){
            for (const auto & i : s.data){
                os << i << " ";
            }
            return os;
        }
};

bool checkParenthesis(const vector<string>& v){
    
    string s = ""; // string to hold the concatenated tokens
    for( auto i : v ){
        s += i; // concatenate the tokens into a single string

    }
    Stack<char> stack;

    for (const auto & c : s){
//        cout << stack << endl;            // stack usage visualization, kinda useful from time to time
        if (c == '(' || c == '[' || c == '{'){
            stack.push(c);
        } else if (c == ')' || c == ']' || c == '}'){
            if (stack.empty()){
                return false;
            }
            char top = stack.pop();
            if (c == ')' && top != '('){
                return false;
            }
            if (c == ']' && top != '['){
                return false;
            }
            if (c == '}' && top != '{'){
                return false;
            }
        }
    }

    return stack.empty();
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

            substitutionNames[name] = nodes.size(); // add the name to the map with the index of the node
            nodes.push_back(BayesianNode(name)); // create a new Bayesian node with the name
            
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
                nodes[substitutionNames[group[1]]].states = states; // assign the states to the node


            }
        }
        


        /****************************************************************** WRITTEN BY COPILOT - TO BE CHECKED ****************************************************/

        if( *i == "probability" ){
            cout << "probability" << endl; // print the probability group
            vector<double> probabilities; // vector to hold the probabilities
            auto firstParenthesisIndex = find(i, group.end(), "("); // find the first parenthesis
            auto lastParenthesisIndex = find(i, group.end(), ")"); // find the last parenthesis
            string owner;
            vector<string> parents; // vector to hold the parents


            if( next(next(firstParenthesisIndex)) != lastParenthesisIndex) {
                
                for( auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    if( *j == "," ){
                        continue;
                    } else if( *next(j) == "|" ){
                        owner = *j; // get the owner of the node
                        j++;
                    } else {
                        nodes[substitutionNames[owner]].parents.push_back(*j); // add the parent to the node
                        nodes[substitutionNames[*j]].children.push_back(owner); // add the child to the parent
                        parents.push_back(*j); // add the parent to the vector
                    }

                }
/*                cout << "Owner: " << owner  << " is child of " ; // print the owner of the node
                for( auto j = parents.begin(); j < parents.end(); ++j){
                    cout << *j << " "; // print the parent of the node
                }*/
                cout << endl; // print a new line


                auto firstParenthesisIndex = find(i, group.end(), "{"); // find the first parenthesis
                auto lastParenthesisIndex = find(i, group.end(), "}"); // find the last parenthesis
                
                /*
                // print all that is between the first and last parenthesis
                for(auto j = next(firstParenthesisIndex); j < lastParenthesisIndex; ++j){
                    cout << *j << " "; // print the token
                }
                */
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

//                    vector<double> numbers;

                    for(auto k = it+1; k < semicolonIndeces[j+1]; ++k){
                        if( *k == ")" ){
                            auto newCommaIndex = find(k, semicolonIndeces[j+1], ","); // find the first occurrence of ","                                                  // SHOULD BE POSSIBLE TO IMPROVE
                            nodes[substitutionNames[owner]].probabilities.push_back(stod(*prev(newCommaIndex))); // convert the string to double and add it to the vector
                            nodes[substitutionNames[owner]].probabilities.push_back(stod(*next(newCommaIndex))); // convert the string to double and add it to the vector
//                            numbers.push_back(stod(*prev(newCommaIndex))); // convert the string to double and add it to the vector
//                            numbers.push_back(stod(*next(newCommaIndex))); // convert the string to double and add it to the vector
                        }
                         
                    }

                    

                    



                    cout << endl; // print a new line
                }




            } else {

                // ORPHAN CASE

                owner = *next(firstParenthesisIndex); // get the owner of the node
                cout << owner << " is an orphan" << endl; // print the owner of the node
                auto firstTableIndex = find(i, group.end(), "table"); // find the first occurrence of "table"
                auto firstSemicolonIndex = find(i, group.end(), ";"); // find the first occurrence of ";"
                auto commaIndex = find(firstTableIndex, firstSemicolonIndex, ","); // find the first occurrence of ","

                vector<double> probabilities = { stod( *prev(commaIndex) ), stod( *next(commaIndex) ) }; // convert the string to double and add it to the vector
                
                /*vector<double> probabilities;
                for( auto j = next(firstTableIndex); j < next(next(next(firstTableIndex))); ++j){
                    if( *j == "," ){
                        continue;
                    } else if( *j == ";" ){
                        break;
                    } else {
                        cout << *j << endl; // print the token
                        probabilities.push_back(stod(*j)); // convert the string to double and add it to the vector
                    }
                }*/

                // check if the sum of the probabilities is equal to 1
                double sum = 0;
                for( auto j = probabilities.begin(); j < probabilities.end(); ++j){
                    sum += *j; // sum the probabilities
                }
                if( sum != 1 ){
                    cerr << "The sum of the probabilities is not equal to 1" << endl;
                    return; // return an error code
                }
                nodes[substitutionNames[owner]].probabilities = probabilities; // assign the probabilities to the node
                nodes[substitutionNames[owner]].parents = {}; // assign the parents to the node
                nodes[substitutionNames[owner]].children = {}; // assign the children to the node

            }

            /*


            if( *next(firstParenthesisIndex) == "|" ){
                for( auto j = next(firstParenthesisIndex); j < group.end(); ++j){
                    if( *j == "|" ){
                        continue;
                    } else if( *j == ")" ){
                        break;
                    } else {
                        nodes[substitutionNames[group[1]]].parents.push_back(*j); // add the parent to the node
                    }
                }
            }

            for( auto j = next(firstParenthesisIndex); j < group.end(); ++j){
                if( *j == ")" ){
                    break;
                } else {
                    probabilities.push_back(stod(*j)); // convert the string to double and add it to the vector
                }
            }

            nodes[substitutionNames[group[1]]].probabilities = probabilities; // assign the probabilities to the node


            */


        }

        /*******************************************************************************************************************************************************/


    }


}

void cleaner(vector<vector<string>>& groups){
    for (auto& group : groups){
        for (auto& token : group){
            if (token == ""){
                token.erase(); // erase the token if it is empty
            }
        }
    }

}


void grouper( vector<vector<string>>& groups, string token){


    if (token == "network" || token == "variable" || token == "probability") {
//        if (!groups.empty()) { // if there is already a group
//            translator(groups.back()); // call the translator function to process the last group
//        }
        groups.push_back({ token }); // add the network group
    } else {
        if( token != "" ){

 /*           if( token.find(",") != string::npos){
                groups.back().push_back(token.substr(0, token.find(","))); // erase the comma from the token
                groups.back().push_back(token.substr(token.find(",")+1)); // add the token after the comma
            }
            else if( token.find(";") != string::npos){
                groups.back().push_back(token.substr(0, token.find(";"))); // erase the semicolon from the token
                groups.back().push_back(token.substr(token.find(";")+1)); // add the token after the semicolon
            }
            else if( token.find("{") != string::npos){
                groups.back().push_back(token.substr(0, token.find("{"))); // erase the parenthesis from the token
                groups.back().push_back(token.substr(token.find("{")+1)); // add the token after the parenthesis
            }
            else if( token.find("}") != string::npos){
                groups.back().push_back(token.substr(0, token.find("}"))); // erase the parenthesis from the token
                groups.back().push_back(token.substr(token.find("}")+1)); // add the token after the parenthesis
            }
            else {
*/                groups.back().push_back(token); // add the token to the last group
//            }



        }
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






// old tokenizer function
/*
void tokenizer(string line){
    string token;
    size_t pos = 0;
    while ((pos = line.find(" ")) != string::npos) {
        token = line.substr(0, pos);
//        cout << token << endl; // print the token
        grouper(dividedGroups, token); // call the grouper function to process the token
        line.erase(0, pos+1); // erase the token from the line
    }
    if(!line.empty()) { // if there is any remaining token in the line
        grouper(dividedGroups, line); // call the grouper function to process the remaining token
    }

}
*/

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
    string filename = "cancer.bif"; // file name


    streamFile(filename); // read the file and print its content

    for(auto group : dividedGroups) { // iterate through the groups
        translator(group); // call the translator function to process the group
    }

    cout << "Divided groups:" << endl; // print the divided groups
    for (const auto& group : dividedGroups) {
        for (const auto& token : group) {
            cout << '[';
            cout << token; // print the tokens in the group
            cout << "], ";
        }
        cout << endl; // print a new line after each group
    }


    cout << "Nodes:" << endl; // print the nodes
    cout << nodes; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes


    return 0;
}