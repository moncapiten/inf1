#include <chrono>

#include "parser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"

//#include "conditionalizer.cpp"

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




    double AHBH, AMBH, ALBH,
           AHBL, AMBL, ALBL,
           BHAH, BLAH,
           BHAM, BLAM,
           BHAL, BLAL,
           AHCH, AMCH, ALCH,
           AHCM, AMCM, ALCM,
           AHCL, AMCL, ALCL,
           CHAH, CHAM, CHAL,
           CMAH, CMAM, CMAL,
           CLAH, CLAM, CLAL,
           CHBH, CMBH, CLBH,
           CHBL, CMBL, CLBL,
           BHCH, BLCH,
           BHCM, BLCM,
           BHCL, BLCL;

    auto startConditioning = chrono::high_resolution_clock::now(); // start the timer for conditional probabilities
    AHBH = conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "H", "H");
    AMBH = conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "M", "H");
    ALBH = conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "L", "H");
    AHBL = conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "H", "L");
    AMBL = conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "M", "L");
    ALBL = conditionalProbs(network.getNode_name("A"), network.getNode_name("B"), "L", "L");

    BHAH = conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "H", "H");
    BLAH = conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "L", "H");
    BHAM = conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "H", "M");
    BLAM = conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "L", "M");
    BHAL = conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "H", "L");
    BLAL = conditionalProbs(network.getNode_name("B"), network.getNode_name("A"), "L", "L");

    AHCH = conditionalProbs(network.getNode_name("A"), network.getNode_name("C"), "H", "H");
    AMCH = conditionalProbs(network.getNode_name("A"), network.getNode_name("C"), "M", "H");
    ALCH = conditionalProbs(network.getNode_name("A"), network.getNode_name("C"), "L", "H");
    AHCM = conditionalProbs(network.getNode_name("A"), network.getNode_name("C"), "H", "M");
    AMCM = conditionalProbs(network.getNode_name("A"), network.getNode_name("C"), "M", "M");
    ALCM = conditionalProbs(network.getNode_name("A"), network.getNode_name("C"), "L", "M");
    auto stopConditioning = chrono::high_resolution_clock::now(); // end the timer for conditional probabilities






    cout << "\n\nCONDITIONAL PROBABILITIES P(A | B):" << endl; // print the conditional probabilities of the nodes
    cout << "P(A=H, B=H) = " << AHBH << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M, B=H) = " << AMBH << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L, B=H) = " << ALBH << endl; // print the conditional probabilities of the nodes

    cout << "P(A=H, B=L) = " << AHBL << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M, B=L) = " << AMBL << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L, B=L) = " << ALBL << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(B | A):" << endl; // print the conditional probabilities of the nodes
    cout << "P(B=H, A=H) = " << BHAH << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L, A=H) = " << BLAH << endl; // print the conditional probabilities of the nodes
    
    cout << "P(B=H, A=M) = " << BHAM << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L, A=M) = " << BLAM << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H, A=L) = " << BHAL << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L, A=L) = " << BLAL << endl; // print the conditional probabilities of the nodes
    
    cout << "\n\nCONDITIONAL PROBABILITIES P(A | C):" << endl; // print the conditional probabilities of the nodes
    cout << "P(A=H,C=H) = " << AHCH << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M,C=H) = " << AMCH << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L,C=H) = " << ALCH << endl; // print the conditional probabilities of the nodes

    cout << "P(A=H,C=M) = " << AHCM << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M,C=M) = " << AMCM << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L,C=M) = " << ALCM << endl; // print the conditional probabilities of the nodes

    cout << "P(A=H,C=L) = " << AHCL << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M,C=L) = " << AMCL << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L,C=L) = " << ALCL << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(C | A):" << endl; // print the conditional probabilities of the nodes
    cout << "P(C=H|A=H) = " << CHAH << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|A=H) = " << CHAM << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|A=H) = " << CHAL << endl; // print the conditional probabilities of the nodes

    cout << "P(C=H|A=M) = " << CMAH << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|A=M) = " << CMAM << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|A=M) = " << CMAL << endl; // print the conditional probabilities of the nodes

    cout << "P(C=H|A=L) = " << CLAH << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|A=L) = " << CLAM << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|A=L) = " << CLAL << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(C | B):" << endl; // print the conditional probabilities of the nodes
    cout << "P(C=H|B=H) = " << CHBH << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|B=H) = " << CMBH << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|B=H) = " << CLBH << endl; // print the conditional probabilities of the nodes

    cout << "P(C=H|B=L) = " << CHBL << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|B=L) = " << CMBL << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|B=L) = " << CLBL << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(B | C):" << endl; // print the conditional probabilities of the nodes
    cout << "P(B=H|C=H) = " << BHCH << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L|C=H) = " << BLCH << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H|C=M) = " << BHCM << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L|C=M) = " << BLCM << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H|C=L) = " << BHCL << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L|C=L) = " << BLCL << endl; // print the conditional probabilities of the nodes



    chrono::duration<double, milli> parsingDuration = endParsing - startParsing; // calculate the duration
    chrono::duration<double, milli> translationDuration = endTranslation - startTranslation; // calculate the duration
    chrono::duration<double, milli> duration = endMarginalization - startMarginalization; // calculate the duration
    chrono::duration<double, milli> printingDuration = endPrinting - startPrinting; // calculate the duration for printing
//    chrono::duration<double, milli> conditioningDuration = stopConditioning - startConditioning; // calculate the duration for conditional probabilities
    cout << "\n\n\n"; // print a new line
    cout << "Parsing took: " << parsingDuration.count() << " ms" << endl; // print the duration
    cout << "Translation took: " << translationDuration.count() << " ms" << endl; // print the duration
    cout << "Marginalization took: " << duration.count() << " ms" << endl; // print the duration
    cout << "Printing took: " << printingDuration.count() << " ms" << endl; // print the duration for printing
//    cout << "Conditioning took: " << conditioningDuration.count() << " ms" << endl; // print the duration for conditional probabilities





/*
//    cout << network.getNode_ID(0) << endl; // print the first node of the network
    auto startConditioning = chrono::high_resolution_clock::now(); // start the timer for conditional probabilities
    double p = conditionalProbs(network.getNode_name("Dyspnoea"), network.getNode_name("Pollution"), "True", "high");
    double p2 = conditionalProbs(network.getNode_name("Dyspnoea"), network.getNode_name("Pollution"), "False", "high");
    double p3 = conditionalProbs(network.getNode_name("Dyspnoea"), network.getNode_name("Pollution"), "True", "low");
    double p4 = conditionalProbs(network.getNode_name("Dyspnoea"), network.getNode_name("Pollution"), "False", "low");
    auto stopConditioning = chrono::high_resolution_clock::now(); // end the timer for conditional probabilities

    chrono::duration<double, milli> conditioningDuration = stopConditioning - startConditioning; // calculate the duration for conditional probabilities
    cout << "\n\nCONDITIONAL PROBABILITIES P(L_SUR_CV_CA | R_LNLW_MED_SEV):" << endl; // print the conditional probabilities of the nodes
    cout << "P(dyspnoea=True | pollution=high) = " << p << endl; // print the conditional probabilities of the nodes
    cout << "P(dyspnoea=False | pollution=high) = " << p2 << endl; // print the conditional probabilities of the nodes
    cout << "P(dyspnoea=True | pollution=low) = " << p3 << endl; // print the conditional probabilities of the nodes
    cout << "P(dyspnoea=False | pollution=low) = " << p4 << endl; // print the conditional probabilities of the nodes
    cout << "Conditioning took: " << conditioningDuration.count() << " ms" << endl; // print the duration for conditional probabilities
    cout << "\n\n\n"; // print a new line
*/


    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}