#include <chrono>

#include "parser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"
#include "conditionalizer.hpp"

using namespace std;

int main(){
    string filename;
    cout << "insert the name of the file to parse: ";
    cin >> filename;

    auto startParsing = chrono::high_resolution_clock::now(); // start the timer
    Parser parser; // create a new parser object
    parser.parse(filename); // parse the file using the parser object
    auto endParsing = chrono::high_resolution_clock::now(); // end the timer

    auto startTranslation = chrono::high_resolution_clock::now(); // start the timer for translation
    parser.translate();
    auto endTranslation = chrono::high_resolution_clock::now(); // end the timer for translation

    BayesianNetwork network = parser.getNetwork(); // get the Bayesian network from the parser

    

    auto startMarginalization = chrono::high_resolution_clock::now(); // start the timer
    Marginalizer marg; // create a new marginalizer object
    network = marg.marginalize(network); // marginalize the nodes
    auto endMarginalization = chrono::high_resolution_clock::now(); // end the timer

    auto startPrinting = chrono::high_resolution_clock::now(); // start the timer for printing
//    cout << "\n\n\nNETWORK:" << endl << endl; // print the nodes
    cout << network; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes
    auto endPrinting = chrono::high_resolution_clock::now(); // end the timer for printing


    chrono::duration<double, milli> parsingDuration = endParsing - startParsing; // calculate the duration
    chrono::duration<double, milli> translationDuration = endTranslation - startTranslation; // calculate the duration
    chrono::duration<double, milli> duration = endMarginalization - startMarginalization; // calculate the duration
    chrono::duration<double, milli> printingDuration = endPrinting - startPrinting; // calculate the duration for printing

    cout << "\n\n\n";
    cout << "Parsing took: " << parsingDuration.count() << " ms" << endl; // print the duration
    cout << "Translation took: " << translationDuration.count() << " ms" << endl; // print the duration
    cout << "Marginalization took: " << duration.count() << " ms" << endl; // print the duration
    cout << "Printing took: " << printingDuration.count() << " ms" << endl; // print the duration for printing



    // double computeConditionalProbability( network, A, a, B, b);
    // maybe add a command parser to calculate conditional probabilities?

//    cout << network.getNode_name("BirthAsphyxia").probabilities << endl; // print the node with the name "BirthAsphyxia"
//    cout << network.getNode_name("A") << endl; // print the probabilities of the node "BirthAsphyxia"

//    cout << computeConditionalProbability(network, "A", "H", "C", "L") << endl; // calculate and print the conditional probability

//    cout << network.getNode_name("A").states;

    cout << computeConditionalProbability(network, "Xray", "positive", "Cancer", "False") << endl; // calculate and print the conditional probability
    cout << computeConditionalProbability(network, "Cancer", "True", "Xray", "positive") << endl; // calculate and print the conditional probability
    cout << computeConditionalProbability(network, "Xray", "positive", "Dyspnoea", "False") << endl; // calculate and print the conditional probability
    cout << computeConditionalProbability(network, "Pollution", "high", "Xray", "negative") << endl; // calculate and print the conditional probability

//    cout << computeConditionalProbability(network, "Pollution", "high", "Cancer") << endl; // calculate and print the conditional probability
//    cout << computeConditionalProbability(network, "BirthAsphyxia", "yes", "Sick") << endl; // calculate and print the conditional probability

    cout << computeJointProbability(network, "Pollution", "high", "Cancer", "True") << endl; // calculate and print the joint probability

    string command;
    cout << "\n\nType 'exit' to exit the program or 'help' for help: ";
    cin >> command; // get the command from the user

    parser.parse(command); // parse the command
    parser.showGroups(); // show the groups

    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}