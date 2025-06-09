#include <chrono>

#include "parser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"
#include "conditionalizer.hpp"

using namespace std;

int main(){

    Parser parser; // create a new parser object
    string command;
    cout << "\n\nType 'exit' to exit the program or 'help' for help: ";
    cin >> command; // get the command from the user

    BayesianNetwork network; // create a Bayesian network object

    while (command != "exit") { // loop until the user types 'exit'
        if (command == "help") { // if the user types 'help'
            cout << "Available commands: \n"
                 << "  exit - exit the program\n"
                 << "  help - show this help message\n"
                 << "  parse <filename> - parse the file with the given name\n"
                 << "  translate - translate the parsed file into a Bayesian network\n"
                 << "  marginalize - marginalize the nodes in the Bayesian network\n"
                 << "  print - print the current Bayesian network\n"
                 << "  show - show the groups of tokens\n";
        } else if (command == "parse") {
            cout << "Enter filename to parse: ";
            cin >> command; // get the filename from the user
            parser.parse(command); // parse the file
        } else if (command == "translate") {
            parser.translate(); // translate the parsed file into a Bayesian network
            network = parser.getNetwork(); // get the Bayesian network from the parser
        } else if (command == "marginalize") {
            Marginalizer marg; // create a new marginalizer object
            network = marg.marginalize(network); // marginalize the nodes
//            cout << "Marginalization completed. Network after marginalization:\n";
//            cout << network; // print the network after marginalization
        } else if (command == "print") {
            cout << "Current Bayesian Network:\n" << network; // print the current Bayesian network
        } else if (command == "show") {
            parser.showGroups(); // show the groups of tokens
        } else {
            cout << "Unknown command: " << command << endl; // print an error message for unknown commands
        }
        cout << "\n==>  ";
        cin >> command; // get the next command from the user
    }



    return 0;
}