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
    double AHBH0, AMBH0, ALBH0,
           AHBL0, AMBL0, ALBL0,

           BHAH0, BLAH0,
           BHAM0, BLAM0,
           BHAL0, BLAL0,

           AHCH0, AMCH0, ALCH0,
           AHCM0, AMCM0, ALCM0,
           AHCL0, AMCL0, ALCL0,

           CHAH0, CMAH0, CLAH0,
           CHAM0, CMAM0, CLAM0,
           CHAL0, CMAL0, CLAL0,

           CHBH0, CMBH0, CLBH0,
           CHBL0, CMBL0, CLBL0,

           BHCH0, BLCH0,
           BHCM0, BLCM0,
           BHCL0, BLCL0;

    double AHBH1, AMBH1, ALBH1,
              AHBL1, AMBL1, ALBL1,
    
              BHAH1, BLAH1,
              BHAM1, BLAM1,
              BHAL1, BLAL1,
    
              AHCH1, AMCH1, ALCH1,
              AHCM1, AMCM1, ALCM1,
              AHCL1, AMCL1, ALCL1,
    
              CHAH1, CMAH1, CLAH1,
              CHAM1, CMAM1, CLAM1,
              CHAL1, CMAL1, CLAL1,
    
              CHBH1, CMBH1, CLBH1,
              CHBL1, CMBL1, CLBL1,
    
              BHCH1, BLCH1,
              BHCM1, BLCM1,
              BHCL1, BLCL1;

    auto startOldConditioning = chrono::high_resolution_clock::now(); // start the timer for conditional probabilities
    AHBH0 =oldComputeConditionalProbability(network, "A", "H", "B", "H");
    AMBH0 =oldComputeConditionalProbability(network, "A", "M", "B", "H");
    ALBH0 =oldComputeConditionalProbability(network, "A", "L", "B", "H");
    AHBL0 =oldComputeConditionalProbability(network, "A", "H", "B", "L");
    AMBL0 =oldComputeConditionalProbability(network, "A", "M", "B", "L");
    ALBL0 =oldComputeConditionalProbability(network, "A", "L", "B", "L");

    BHAH0 =oldComputeConditionalProbability(network, "B", "H", "A", "H");
    BLAH0 =oldComputeConditionalProbability(network, "B", "L", "A", "H");
    BHAM0 =oldComputeConditionalProbability(network, "B", "H", "A", "M");
    BLAM0 =oldComputeConditionalProbability(network, "B", "L", "A", "M");
    BHAL0 =oldComputeConditionalProbability(network, "B", "H", "A", "L");
    BLAL0 =oldComputeConditionalProbability(network, "B", "L", "A", "L");

    AHCH0 =oldComputeConditionalProbability(network, "A", "H", "C", "H");
    AMCH0 =oldComputeConditionalProbability(network, "A", "M", "C", "H");
    ALCH0 =oldComputeConditionalProbability(network, "A", "L", "C", "H");
    AHCM0 =oldComputeConditionalProbability(network, "A", "H", "C", "M");
    AMCM0 =oldComputeConditionalProbability(network, "A", "M", "C", "M");
    ALCM0 =oldComputeConditionalProbability(network, "A", "L", "C", "M");
    AHCL0 =oldComputeConditionalProbability(network, "A", "H", "C", "L");
    AMCL0 =oldComputeConditionalProbability(network, "A", "M", "C", "L");
    ALCL0 =oldComputeConditionalProbability(network, "A", "L", "C", "L");

    CHAH0 =oldComputeConditionalProbability(network, "C", "H", "A", "H");
    CHAM0 =oldComputeConditionalProbability(network, "C", "H", "A", "M");
    CHAL0 =oldComputeConditionalProbability(network, "C", "H", "A", "L");
    CMAH0 =oldComputeConditionalProbability(network, "C", "M", "A", "H");
    CMAM0 =oldComputeConditionalProbability(network, "C", "M", "A", "M");
    CMAL0 =oldComputeConditionalProbability(network, "C", "M", "A", "L");
    CLAH0 =oldComputeConditionalProbability(network, "C", "L", "A", "H");
    CLAM0 =oldComputeConditionalProbability(network, "C", "L", "A", "M");
    CLAL0 =oldComputeConditionalProbability(network, "C", "L", "A", "L");

    CHBH0 =oldComputeConditionalProbability(network, "C", "H", "B", "H");
    CMBH0 =oldComputeConditionalProbability(network, "C", "M", "B", "H");
    CLBH0 =oldComputeConditionalProbability(network, "C", "L", "B", "H");
    CHBL0 =oldComputeConditionalProbability(network, "C", "H", "B", "L");
    CMBL0 =oldComputeConditionalProbability(network, "C", "M", "B", "L");
    CLBL0 =oldComputeConditionalProbability(network, "C", "L", "B", "L");

    BHCH0 =oldComputeConditionalProbability(network, "B", "H", "C", "H");
    BLCH0 =oldComputeConditionalProbability(network, "B", "L", "C", "H");
    BHCM0 =oldComputeConditionalProbability(network, "B", "H", "C", "M");
    BLCM0 =oldComputeConditionalProbability(network, "B", "L", "C", "M");
    BHCL0 =oldComputeConditionalProbability(network, "B", "H", "C", "L");
    BLCL0 =oldComputeConditionalProbability(network, "B", "L", "C", "L");

    auto stopOldConditioning = chrono::high_resolution_clock::now(); // end the timer for conditional probabilities


    auto startNewConditioning = chrono::high_resolution_clock::now(); // start the timer for conditional probabilities
    AHBH1 = computeConditionalProbability(network, "A", "H", "B", "H");
    AMBH1 = computeConditionalProbability(network, "A", "M", "B", "H");
    ALBH1 = computeConditionalProbability(network, "A", "L", "B", "H");
    AHBL1 = computeConditionalProbability(network, "A", "H", "B", "L");
    AMBL1 = computeConditionalProbability(network, "A", "M", "B", "L");
    ALBL1 = computeConditionalProbability(network, "A", "L", "B", "L");

    BHAH1 = computeConditionalProbability(network, "B", "H", "A", "H");
    BLAH1 = computeConditionalProbability(network, "B", "L", "A", "H");
    BHAM1 = computeConditionalProbability(network, "B", "H", "A", "M");
    BLAM1 = computeConditionalProbability(network, "B", "L", "A", "M");
    BHAL1 = computeConditionalProbability(network, "B", "H", "A", "L");
    BLAL1 = computeConditionalProbability(network, "B", "L", "A", "L");

    AHCH1 = computeConditionalProbability(network, "A", "H", "C", "H");
    AMCH1 = computeConditionalProbability(network, "A", "M", "C", "H");
    ALCH1 = computeConditionalProbability(network, "A", "L", "C", "H");
    AHCM1 = computeConditionalProbability(network, "A", "H", "C", "M");
    AMCM1 = computeConditionalProbability(network, "A", "M", "C", "M");
    ALCM1 = computeConditionalProbability(network, "A", "L", "C", "M");
    AHCL1 = computeConditionalProbability(network, "A", "H", "C", "L");
    AMCL1 = computeConditionalProbability(network, "A", "M", "C", "L");
    ALCL1 = computeConditionalProbability(network, "A", "L", "C", "L");

    CHAH1 = computeConditionalProbability(network, "C", "H", "A", "H");
    CHAM1 = computeConditionalProbability(network, "C", "H", "A", "M");
    CHAL1 = computeConditionalProbability(network, "C", "H", "A", "L");
    CMAH1 = computeConditionalProbability(network, "C", "M", "A", "H");
    CMAM1 = computeConditionalProbability(network, "C", "M", "A", "M");
    CMAL1 = computeConditionalProbability(network, "C", "M", "A", "L");
    CLAH1 = computeConditionalProbability(network, "C", "L", "A", "H");
    CLAM1 = computeConditionalProbability(network, "C", "L", "A", "M");
    CLAL1 = computeConditionalProbability(network, "C", "L", "A", "L");

    CHBH1 = computeConditionalProbability(network, "C", "H", "B", "H");
    CMBH1 = computeConditionalProbability(network, "C", "M", "B", "H");
    CLBH1 = computeConditionalProbability(network, "C", "L", "B", "H");
    CHBL1 = computeConditionalProbability(network, "C", "H", "B", "L");
    CMBL1 = computeConditionalProbability(network, "C", "M", "B", "L");
    CLBL1 = computeConditionalProbability(network, "C", "L", "B", "L");

    BHCH1 = computeConditionalProbability(network, "B", "H", "C", "H");
    BLCH1 = computeConditionalProbability(network, "B", "L", "C", "H");
    BHCM1 = computeConditionalProbability(network, "B", "H", "C", "M");
    BLCM1 = computeConditionalProbability(network, "B", "L", "C", "M");
    BHCL1 = computeConditionalProbability(network, "B", "H", "C", "L");
    BLCL1 = computeConditionalProbability(network, "B", "L", "C", "L");

    auto stopNewConditioning = chrono::high_resolution_clock::now(); // end the timer for conditional probabilities



    auto startPrinting = chrono::high_resolution_clock::now(); // start the timer for printing
    cout << "\n\n\nNETWORK:" << endl << endl; // print the nodes
    cout << network; // print the nodes using the overloaded operator<<
    cout << endl; // print a new line after the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(A | B):" << endl; // print the conditional probabilities of the nodes
    cout << "P(A=H, B=H) = " << AHBH0 << "\t" << AHBH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M, B=H) = " << AMBH0 << "\t" << AMBH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L, B=H) = " << ALBH0 << "\t" << ALBH1 << endl; // print the conditional probabilities of the nodes

    cout << "P(A=H, B=L) = " << AHBL0 << "\t" << AHBL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M, B=L) = " << AMBL0 << "\t" << AMBL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L, B=L) = " << ALBL0 << "\t" << ALBL1 << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(B | A):" << endl; // print the conditional probabilities of the nodes
    cout << "P(B=H, A=H) = " << BHAH0 << "\t" << BHAH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L, A=H) = " << BLAH0 << "\t" << BLAH1 << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H, A=M) = " << BHAM0 << "\t" << BHAM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L, A=M) = " << BLAM0 << "\t" << BLAM1 << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H, A=L) = " << BHAL0 << "\t" << BHAL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L, A=L) = " << BLAL0 << "\t" << BLAL1 << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(A | C):" << endl; // print the conditional probabilities of the nodes
    cout << "P(A=H,C=H) = " << AHCH0 << "\t" << AHCH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M,C=H) = " << AMCH0 << "\t" << AMCH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L,C=H) = " << ALCH0 << "\t" << ALCH1 << endl; // print the conditional probabilities of the nodes

    cout << "P(A=H,C=M) = " << AHCM0 << "\t" << AHCM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M,C=M) = " << AMCM0 << "\t" << AMCM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L,C=M) = " << ALCM0 << "\t" << ALCM1 << endl; // print the conditional probabilities of the nodes

    cout << "P(A=H,C=L) = " << AHCL0 << "\t" << AHCL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=M,C=L) = " << AMCL0 << "\t" << AMCL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(A=L,C=L) = " << ALCL0 << "\t" << ALCL1 << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(C | A):" << endl; // print the conditional probabilities of the nodes
    cout << "P(C=H|A=H) = " << CHAH0 << "\t" << CHAH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|A=H) = " << CHAM0 << "\t" << CHAM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|A=H) = " << CHAL0 << "\t" << CHAL1 << endl; // print the conditional probabilities of the nodes

    cout << "P(C=H|A=M) = " << CMAH0 << "\t" << CMAH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|A=M) = " << CMAM0 << "\t" << CMAM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|A=M) = " << CMAL0 << "\t" << CMAL1 << endl; // print the conditional probabilities of the nodes

    cout << "P(C=H|A=L) = " << CLAH0 << "\t" << CLAH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|A=L) = " << CLAM0 << "\t" << CLAM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|A=L) = " << CLAL0 << "\t" << CLAL1 << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(C | B):" << endl; // print the conditional probabilities of the nodes
    cout << "P(C=H|B=H) = " << CHBH0 << "\t" << CHBH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|B=H) = " << CMBH0 << "\t" << CMBH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|B=H) = " << CLBH0 << "\t" << CLBH1 << endl; // print the conditional probabilities of the nodes

    cout << "P(C=H|B=L) = " << CHBL0 << "\t" << CHBL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=M|B=L) = " << CMBL0 << "\t" << CMBL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(C=L|B=L) = " << CLBL0 << "\t" << CLBL1 << endl; // print the conditional probabilities of the nodes

    cout << "\n\nCONDITIONAL PROBABILITIES P(B | C):" << endl; // print the conditional probabilities of the nodes
    cout << "P(B=H|C=H) = " << BHCH0 << "\t" << BHCH1 << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L|C=H) = " << BLCH0 << "\t" << BLCH1 << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H|C=M) = " << BHCM0 << "\t" << BHCM1 << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L|C=M) = " << BLCM0 << "\t" << BLCM1 << endl; // print the conditional probabilities of the nodes

    cout << "P(B=H|C=L) = " << BHCL0 << "\t" << BHCL1 << endl; // print the conditional probabilities of the nodes
    cout << "P(B=L|C=L) = " << BLCL0 << "\t" << BLCL1 << endl; // print the conditional probabilities of the nodes

    auto endPrinting = chrono::high_resolution_clock::now(); // end the timer for printing




    chrono::duration<double, milli> parsingDuration = endParsing - startParsing; // calculate the duration
    chrono::duration<double, milli> translationDuration = endTranslation - startTranslation; // calculate the duration
    chrono::duration<double, milli> duration = endMarginalization - startMarginalization; // calculate the duration
    chrono::duration<double, milli> oldConditioningDuration = stopOldConditioning - startOldConditioning; // calculate the duration for conditional probabilities
    chrono::duration<double, milli> conditioningDuration = stopNewConditioning - startNewConditioning; // calculate the duration for conditional probabilities
    chrono::duration<double, milli> printingDuration = endPrinting - startPrinting; // calculate the duration for printing
    cout << "\n\n\n"; // print a new line
    cout << "Parsing took: " << parsingDuration.count() << " ms" << endl; // print the duration
    cout << "Translation took: " << translationDuration.count() << " ms" << endl; // print the duration
    cout << "Marginalization took: " << duration.count() << " ms" << endl; // print the duration
    cout << "Old Conditioner took: " << oldConditioningDuration.count() << " ms" << endl; // print the duration for conditional probabilities
    cout << "New Conditioner took: " << conditioningDuration.count() << " ms" << endl; // print the duration for conditional probabilities
    cout << "Printing took: " << printingDuration.count() << " ms" << endl; // print the duration for printing

    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}