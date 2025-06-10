#include <chrono>

#include "newparser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"
#include "conditionalizer.hpp"

using namespace std;

int main(){
    string filename = "../Data/conditionaltest3.bif"; // file name


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


    // variable for conditional probabilities and
    // condProbs calculations with timing cause its fun
    double AHBH, AMBH, ALBH,
           AHBL, AMBL, ALBL,

           BHAH, BLAH,
           BHAM, BLAM,
           BHAL, BLAL,

           AHCH, AMCH, ALCH,
           AHCM, AMCM, ALCM,
           AHCL, AMCL, ALCL,

           CHAH, CMAH, CLAH,
           CHAM, CMAM, CLAM,
           CHAL, CMAL, CLAL,

           CHBH, CMBH, CLBH,
           CHBL, CMBL, CLBL,

           BHCH, BLCH,
           BHCM, BLCM,
           BHCL, BLCL;

    auto startConditioning = chrono::high_resolution_clock::now(); // start the timer for conditional probabilities
    AHBH = computeConditionalProbability(network, "A", "H", "B", "H");
    AMBH = computeConditionalProbability(network, "A", "M", "B", "H");
    ALBH = computeConditionalProbability(network, "A", "L", "B", "H");
    AHBL = computeConditionalProbability(network, "A", "H", "B", "L");
    AMBL = computeConditionalProbability(network, "A", "M", "B", "L");
    ALBL = computeConditionalProbability(network, "A", "L", "B", "L");

    BHAH = computeConditionalProbability(network, "B", "H", "A", "H");
    BLAH = computeConditionalProbability(network, "B", "L", "A", "H");
    BHAM = computeConditionalProbability(network, "B", "H", "A", "M");
    BLAM = computeConditionalProbability(network, "B", "L", "A", "M");
    BHAL = computeConditionalProbability(network, "B", "H", "A", "L");
    BLAL = computeConditionalProbability(network, "B", "L", "A", "L");

    AHCH = computeConditionalProbability(network, "A", "H", "C", "H");
    AMCH = computeConditionalProbability(network, "A", "M", "C", "H");
    ALCH = computeConditionalProbability(network, "A", "L", "C", "H");
    AHCM = computeConditionalProbability(network, "A", "H", "C", "M");
    AMCM = computeConditionalProbability(network, "A", "M", "C", "M");
    ALCM = computeConditionalProbability(network, "A", "L", "C", "M");
    AHCL = computeConditionalProbability(network, "A", "H", "C", "L");
    AMCL = computeConditionalProbability(network, "A", "M", "C", "L");
    ALCL = computeConditionalProbability(network, "A", "L", "C", "L");

    CHAH = computeConditionalProbability(network, "C", "H", "A", "H");
    CHAM = computeConditionalProbability(network, "C", "H", "A", "M");
    CHAL = computeConditionalProbability(network, "C", "H", "A", "L");
    CMAH = computeConditionalProbability(network, "C", "M", "A", "H");
    CMAM = computeConditionalProbability(network, "C", "M", "A", "M");
    CMAL = computeConditionalProbability(network, "C", "M", "A", "L");
    CLAH = computeConditionalProbability(network, "C", "L", "A", "H");
    CLAM = computeConditionalProbability(network, "C", "L", "A", "M");
    CLAL = computeConditionalProbability(network, "C", "L", "A", "L");

    CHBH = computeConditionalProbability(network, "C", "H", "B", "H");
    CMBH = computeConditionalProbability(network, "C", "M", "B", "H");
    CLBH = computeConditionalProbability(network, "C", "L", "B", "H");
    CHBL = computeConditionalProbability(network, "C", "H", "B", "L");
    CMBL = computeConditionalProbability(network, "C", "M", "B", "L");
    CLBL = computeConditionalProbability(network, "C", "L", "B", "L");

    BHCH = computeConditionalProbability(network, "B", "H", "C", "H");
    BLCH = computeConditionalProbability(network, "B", "L", "C", "H");
    BHCM = computeConditionalProbability(network, "B", "H", "C", "M");
    BLCM = computeConditionalProbability(network, "B", "L", "C", "M");
    BHCL = computeConditionalProbability(network, "B", "H", "C", "L");
    BLCL = computeConditionalProbability(network, "B", "L", "C", "L");

    auto stopConditioning = chrono::high_resolution_clock::now(); // end the timer for conditional probabilities



    auto startPrinting = chrono::high_resolution_clock::now(); // start the timer for printing
    cout << "\n\n\nNETWORK:" << endl << endl; // print the nodes
    cout << network; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes

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

    auto endPrinting = chrono::high_resolution_clock::now(); // end the timer for printing




    chrono::duration<double, milli> parsingDuration = endParsing - startParsing; // calculate the duration
    chrono::duration<double, milli> translationDuration = endTranslation - startTranslation; // calculate the duration
    chrono::duration<double, milli> duration = endMarginalization - startMarginalization; // calculate the duration
    chrono::duration<double, milli> conditioningDuration = stopConditioning - startConditioning; // calculate the duration for conditional probabilities
    chrono::duration<double, milli> printingDuration = endPrinting - startPrinting; // calculate the duration for printing
    cout << "\n\n\n"; // print a new line
    cout << "Parsing took: " << parsingDuration.count() << " ms" << endl; // print the duration
    cout << "Translation took: " << translationDuration.count() << " ms" << endl; // print the duration
    cout << "Marginalization took: " << duration.count() << " ms" << endl; // print the duration
    cout << "Conditioning took: " << conditioningDuration.count() << " ms" << endl; // print the duration for conditional probabilities
    cout << "Printing took: " << printingDuration.count() << " ms" << endl; // print the duration for printing

    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}