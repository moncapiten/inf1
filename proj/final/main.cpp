#include <chrono>

#include "parser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"

using namespace std;






double conditionalProbs(BayesianNode A, BayesianNode B, string stateA = "", string stateB = ""){
    double result;

    if( A.parents.empty() && B.parents.empty() ){
        auto index = (find(A.states.begin(), A.states.end(), stateA) - A.states.begin());
        return A.pureProb[index];
    }

    if( find(A.parents.begin(), A.parents.end(), B.ID) != A.parents.end() && A.parents.size() == 1 ){


        auto AstateIndex = find(A.states.begin(), A.states.end(), stateA) - A.states.begin();
        auto BstateIndex = find(B.states.begin(), B.states.end(), stateB) - B.states.begin();

        int indexToBeTaken = BstateIndex * A.states.size() + AstateIndex;
        return A.probabilities[indexToBeTaken];

    }
    
    if( find(B.parents.begin(), B.parents.end(), A.ID) != B.parents.end() && B.parents.size() == 1 ){

        auto AstateIndex = find(A.states.begin(), A.states.end(), stateA) - A.states.begin();
        auto BstateIndex = find(B.states.begin(), B.states.end(), stateB) - B.states.begin();

        int indexToBeTaken = AstateIndex * B.states.size() + BstateIndex;
        return B.probabilities[indexToBeTaken] * A.pureProb[AstateIndex] / B.pureProb[BstateIndex];
    }

    return result;

}











int main(){
    string filename = "../munin.bif"; // file name


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
    cout << "\n\n\nNETWORK:" << endl << endl; // print the nodes
    cout << network; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes
    auto endPrinting = chrono::high_resolution_clock::now(); // end the timer for printing


    chrono::duration<double, milli> parsingDuration = endParsing - startParsing; // calculate the duration
    chrono::duration<double, milli> translationDuration = endTranslation - startTranslation; // calculate the duration
    chrono::duration<double, milli> duration = endMarginalization - startMarginalization; // calculate the duration
    chrono::duration<double, milli> printingDuration = endPrinting - startPrinting; // calculate the duration for printing
    cout << "Parsing took: " << parsingDuration.count() << " ms" << endl; // print the duration
    cout << "Translation took: " << translationDuration.count() << " ms" << endl; // print the duration
    cout << "Marginalization took: " << duration.count() << " ms" << endl; // print the duration
    cout << "Printing took: " << printingDuration.count() << " ms" << endl; // print the duration for printing



//    conditionalProbs(network.getNode_name("Pollution"), network.getNode_name("Smoker"), "low", "yes"); // calculate the conditional probabilities of the nodes
//    cout << conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "y", "H") << endl; // print the conditional probabilities of the nodes
//    cout << conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "y", "M") << endl; // print the conditional probabilities of the nodes
//    cout << conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "y", "L") << endl; // print the conditional probabilities of the nodes

//    cout << conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "n", "M") << endl; // print the conditional probabilities of the nodes
 //   cout << conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "m", "H") << endl; // print the conditional probabilities of the nodes

 //   cout << endl << conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "H", "y") << endl; // print the conditional probabilities of the nodes


    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}