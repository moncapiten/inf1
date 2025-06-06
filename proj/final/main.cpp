#include <chrono>

#include "parser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"

#include "conditionalizer.cpp"

using namespace std;





double getConditionalProbability(BayesianNetwork& net, const string& targetName, const string& givenName, 
                                     const string& targetState, const string& givenState) {
    BayesianNode& A = net.getNode_name(targetName); // P(A | B)
    BayesianNode& B = net.getNode_name(givenName);

    // Find index of desired states
    auto itA = find(A.states.begin(), A.states.end(), targetState);
    auto itB = find(B.states.begin(), B.states.end(), givenState);
    if (itA == A.states.end() || itB == B.states.end()) {
        throw invalid_argument("State not found in target or given node.");
    }
    int indexA = distance(A.states.begin(), itA);
    int indexB = distance(B.states.begin(), itB);

    // Build joint P(A = a, B = b) by iterating over A's CPT
    // NOTE: this only works if B is a parent of A (i.e., P(A|B) is in A.probabilities)
    if (find(A.parents.begin(), A.parents.end(), B.ID) == A.parents.end()) {
        throw logic_error("B must be a parent of A for conditional lookup.");
    }

    // For binary states and single parent
    int numStatesA = A.states.size();
    int numStatesB = B.states.size();
    
    // Probabilities stored as: P(A | B): [A0|B0, A1|B0, A0|B1, A1|B1]
    // So index = indexA + indexB * numStatesA
    int idx = indexA + indexB * numStatesA;

    double pAB = A.probabilities[idx];           // P(A = a | B = b)
    double pB = B.pureProb[indexB];              // P(B = b)

    if (pB == 0) return 0; // Avoid division by zero

    return pAB * pB / pB; // Simplifies to pAB
}





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
    string filename = "../conditionaltest3.bif"; // file name


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




    cout << "\n\nCONDITIONAL PROBABILITIES P(A | B):" << endl; // print the conditional probabilities of the nodes
    cout << "P(A= H | B = H)" << computeConditionalProbability(network, "A", "H", "B", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= M | B = H)" << computeConditionalProbability(network, "A", "M", "B", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= L | B = H)" << computeConditionalProbability(network, "A", "L", "B", "H") << endl; // print the conditional probabilities of the nodes")

    cout << "P(A= H | B = L)" << computeConditionalProbability(network, "A", "H", "B", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= M | B = L)" << computeConditionalProbability(network, "A", "M", "B", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= L | B = L)" << computeConditionalProbability(network, "A", "L", "B", "L") << endl; // print the conditional probabilities of the nodes")

    cout << "\n\nCONDITIONAL PROBABILITIES P(B | A):" << endl; // print the conditional probabilities of the nodes
    cout << "P(B= H | A = H)" << computeConditionalProbability(network, "B", "H", "A", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(B= L | A = H)" << computeConditionalProbability(network, "B", "L", "A", "H") << endl; // print the conditional probabilities of the nodes")

    cout << "P(B= H | A = M)" << computeConditionalProbability(network, "B", "H", "A", "M") << endl; // print the conditional probabilities of the nodes")
    cout << "P(B= L | A = M)" << computeConditionalProbability(network, "B", "L", "A", "M") << endl; // print the conditional probabilities of the nodes")

    cout << "P(B= H | A = L)" << computeConditionalProbability(network, "B", "H", "A", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(B= L | A = L)" << computeConditionalProbability(network, "B", "L", "A", "L") << endl; // print the conditional probabilities of the nodes")

    cout << "\n\nCONDITIONAL PROBABILITIES P(A | C):" << endl; // print the conditional probabilities of the nodes
    cout << "P(A= H | C = High)" << computeConditionalProbability(network, "A", "H", "C", "High") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= M | C = High)" << computeConditionalProbability(network, "A", "M", "C", "High") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= L | C = High)" << computeConditionalProbability(network, "A", "L", "C", "High") << endl; // print the conditional probabilities of the nodes")

    cout << "P(A= H | C = Medium)" << computeConditionalProbability(network, "A", "H", "C", "Medium") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= M | C = Medium)" << computeConditionalProbability(network, "A", "M", "C", "Medium") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= L | C = Medium)" << computeConditionalProbability(network, "A", "L", "C", "Medium") << endl; // print the conditional probabilities of the nodes")

    cout << "P(A= H | C = Low)" << computeConditionalProbability(network, "A", "H", "C", "Low") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= M | C = Low)" << computeConditionalProbability(network, "A", "M", "C", "Low") << endl; // print the conditional probabilities of the nodes")
    cout << "P(A= L | C = Low)" << computeConditionalProbability(network, "A", "L", "C", "Low") << endl; // print the conditional probabilities of the nodes")

    cout << "\n\nCONDITIONAL PROBABILITIES P(C | A):" << endl; // print the conditional probabilities of the nodes
    cout << "P(C= High | A = H)" << computeConditionalProbability(network, "C", "High", "A", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Medium | A = H)" << computeConditionalProbability(network, "C", "Medium", "A", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Low | A = H)" << computeConditionalProbability(network, "C", "Low", "A", "H") << endl; // print the conditional probabilities of the nodes")

    cout << "P(C= High | A = M)" << computeConditionalProbability(network, "C", "High", "A", "M") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Medium | A = M)" << computeConditionalProbability(network, "C", "Medium", "A", "M") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Low | A = M)" << computeConditionalProbability(network, "C", "Low", "A", "M") << endl; // print the conditional probabilities of the nodes")

    cout << "P(C= High | A = L)" << computeConditionalProbability(network, "C", "High", "A", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Medium | A = L)" << computeConditionalProbability(network, "C", "Medium", "A", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Low | A = L)" << computeConditionalProbability(network, "C", "Low", "A", "L") << endl; // print the conditional probabilities of the nodes")

    cout << "\n\nCONDITIONAL PROBABILITIES P(C | B):" << endl; // print the conditional probabilities of the nodes
    cout << "P(C= High | B = H)" << computeConditionalProbability(network, "C", "High", "B", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Medium | B = H)" << computeConditionalProbability(network, "C", "Medium", "B", "H") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Low | B = H)" << computeConditionalProbability(network, "C", "Low", "B", "H") << endl; // print the conditional probabilities of the nodes")

    cout << "P(C= High | B = L)" << computeConditionalProbability(network, "C", "High", "B", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Medium | B = L)" << computeConditionalProbability(network, "C", "Medium", "B", "L") << endl; // print the conditional probabilities of the nodes")
    cout << "P(C= Low | B = L)" << computeConditionalProbability(network, "C", "Low", "B", "L") << endl; // print the conditional probabilities of the nodes")

    cout << "\n\nCONDITIONAL PROBABILITIES P(B | C):" << endl; // print the conditional probabilities of the nodes
    cout << "P(B= H | C = High)" << computeConditionalProbability(network, "B", "H", "C", "High") << endl; // print the conditional probabilities of the nodes")
    cout << "P(B= L | C = High)" << computeConditionalProbability(network, "B", "L", "C", "High") << endl; // print the conditional probabilities of the nodes")

    cout << "P(B= H | C = Medium)" << computeConditionalProbability(network, "B", "H", "C", "Medium") << endl; // print the conditional probabilities of the nodes")
    cout << "P(B= L | C = Medium)" << computeConditionalProbability(network, "B", "L", "C", "Medium") << endl; // print the conditional probabilities of the nodes")

    cout << "P(B= H | C = Low)" << computeConditionalProbability(network, "B", "H", "C", "Low") << endl; // print the conditional probabilities of the nodes")
    cout << "P(B= L | C = Low)" << computeConditionalProbability(network, "B", "L", "C", "Low") << endl; // print the conditional probabilities of the nodes")


    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}