#include <chrono>

#include "../include/newparser.hpp"
#include "../include/nodes.hpp"
#include "../include/supportFunc.hpp"
#include "../include/marginalizer.hpp"
#include "../include/conditionalizer.hpp"
#include "../include/VE.hpp"

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

    double AHBH2, AMBH2, ALBH2,
              AHBL2, AMBL2, ALBL2,
    
              BHAH2, BLAH2,
              BHAM2, BLAM2,
              BHAL2, BLAL2,
    
              AHCH2, AMCH2, ALCH2,
              AHCM2, AMCM2, ALCM2,
              AHCL2, AMCL2, ALCL2,
    
              CHAH2, CMAH2, CLAH2,
              CHAM2, CMAM2, CLAM2,
              CHAL2, CMAL2, CLAL2,
    
              CHBH2, CMBH2, CLBH2,
              CHBL2, CMBL2, CLBL2,
    
              BHCH2, BLCH2,
              BHCM2, BLCM2,
              BHCL2, BLCL2;

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

        auto startVEConditioning = chrono::high_resolution_clock::now(); // start the timer for conditional probabilities
    AHBH2 = computeConditionalProbabilityVE(network, "A", "H", "B", "H");
    AMBH2 = computeConditionalProbabilityVE(network, "A", "M", "B", "H");
    ALBH2 = computeConditionalProbabilityVE(network, "A", "L", "B", "H");
    AHBL2 = computeConditionalProbabilityVE(network, "A", "H", "B", "L");
    AMBL2 = computeConditionalProbabilityVE(network, "A", "M", "B", "L");
    ALBL2 = computeConditionalProbabilityVE(network, "A", "L", "B", "L");

    BHAH2 = computeConditionalProbabilityVE(network, "B", "H", "A", "H");
    BLAH2 = computeConditionalProbabilityVE(network, "B", "L", "A", "H");
    BHAM2 = computeConditionalProbabilityVE(network, "B", "H", "A", "M");
    BLAM2 = computeConditionalProbabilityVE(network, "B", "L", "A", "M");
    BHAL2 = computeConditionalProbabilityVE(network, "B", "H", "A", "L");
    BLAL2 = computeConditionalProbabilityVE(network, "B", "L", "A", "L");

    AHCH2 = computeConditionalProbabilityVE(network, "A", "H", "C", "H");
    AMCH2 = computeConditionalProbabilityVE(network, "A", "M", "C", "H");
    ALCH2 = computeConditionalProbabilityVE(network, "A", "L", "C", "H");
    AHCM2 = computeConditionalProbabilityVE(network, "A", "H", "C", "M");
    AMCM2 = computeConditionalProbabilityVE(network, "A", "M", "C", "M");
    ALCM2 = computeConditionalProbabilityVE(network, "A", "L", "C", "M");
    AHCL2 = computeConditionalProbabilityVE(network, "A", "H", "C", "L");
    AMCL2 = computeConditionalProbabilityVE(network, "A", "M", "C", "L");
    ALCL2 = computeConditionalProbabilityVE(network, "A", "L", "C", "L");

    CHAH2 = computeConditionalProbabilityVE(network, "C", "H", "A", "H");
    CHAM2 = computeConditionalProbabilityVE(network, "C", "H", "A", "M");
    CHAL2 = computeConditionalProbabilityVE(network, "C", "H", "A", "L");
    CMAH2 = computeConditionalProbabilityVE(network, "C", "M", "A", "H");
    CMAM2 = computeConditionalProbabilityVE(network, "C", "M", "A", "M");
    CMAL2 = computeConditionalProbabilityVE(network, "C", "M", "A", "L");
    CLAH2 = computeConditionalProbabilityVE(network, "C", "L", "A", "H");
    CLAM2 = computeConditionalProbabilityVE(network, "C", "L", "A", "M");
    CLAL2 = computeConditionalProbabilityVE(network, "C", "L", "A", "L");

    CHBH2 = computeConditionalProbabilityVE(network, "C", "H", "B", "H");
    CMBH2 = computeConditionalProbabilityVE(network, "C", "M", "B", "H");
    CLBH2 = computeConditionalProbabilityVE(network, "C", "L", "B", "H");
    CHBL2 = computeConditionalProbabilityVE(network, "C", "H", "B", "L");
    CMBL2 = computeConditionalProbabilityVE(network, "C", "M", "B", "L");
    CLBL2 = computeConditionalProbabilityVE(network, "C", "L", "B", "L");

    BHCH2 = computeConditionalProbabilityVE(network, "B", "H", "C", "H");
    BLCH2 = computeConditionalProbabilityVE(network, "B", "L", "C", "H");
    BHCM2 = computeConditionalProbabilityVE(network, "B", "H", "C", "M");
    BLCM2 = computeConditionalProbabilityVE(network, "B", "L", "C", "M");
    BHCL2 = computeConditionalProbabilityVE(network, "B", "H", "C", "L");
    BLCL2 = computeConditionalProbabilityVE(network, "B", "L", "C", "L");
    auto stopVEConditioning = chrono::high_resolution_clock::now(); // end the timer for conditional probabilities



    parser.logging = true;
    parser.log = ofstream("log/conditionalizerTest.log"); // open the log file
    parser.respond("Conditionalizer Test\n\n"); // print the header

    auto startPrinting = chrono::high_resolution_clock::now(); // start the timer for printing
    parser.respond("NETWORK:\n\n" + to_string(network) + "\n\n", true); // print the network using the overloaded operator<<

    parser.respond("\n\nCONDITIONAL PROBABILITIES P(A | B):\n"); 
    parser.respond("P(A=H, B=H) = " + to_string(AHBH0) + "\t" + to_string(AHBH1) + "\t" + to_string(AHBH2) + "\n"); 
    parser.respond("P(A=M, B=H) = " + to_string(AMBH0) + "\t" + to_string(AMBH1) + "\t" + to_string(AMBH2) + "\n");
    parser.respond("P(A=L, B=H) = " + to_string(ALBH0) + "\t" + to_string(ALBH1) + "\t" + to_string(ALBH2) + "\n");

    parser.respond("P(A=H, B=L) = " + to_string(AHBL0) + "\t" + to_string(AHBL1) + "\t" + to_string(AHBL2) + "\n");
    parser.respond("P(A=M, B=L) = " + to_string(AMBL0) + "\t" + to_string(AMBL1) + "\t" + to_string(AMBL2) + "\n");
    parser.respond("P(A=L, B=L) = " + to_string(ALBL0) + "\t" + to_string(ALBL1) + "\t" + to_string(ALBL2) + "\n");

    parser.respond("\n\nCONDITIONAL PROBABILITIES P(B | A):\n"); 
    parser.respond("P(B=H, A=H) = " + to_string(BHAH0) + "\t" + to_string(BHAH1) + "\t" + to_string(BHAH2) + "\n");
    parser.respond("P(B=L, A=H) = " + to_string(BLAH0) + "\t" + to_string(BLAH1) + "\t" + to_string(BLAH2) + "\n");

    parser.respond("P(B=H, A=M) = " + to_string(BHAM0) + "\t" + to_string(BHAM1) + "\t" + to_string(BHAM2) + "\n");
    parser.respond("P(B=L, A=M) = " + to_string(BLAM0) + "\t" + to_string(BLAM1) + "\t" + to_string(BLAM2) + "\n");

    parser.respond("P(B=H, A=L) = " + to_string(BHAL0) + "\t" + to_string(BHAL1) + "\t" + to_string(BHAL2) + "\n");
    parser.respond("P(B=L, A=L) = " + to_string(BLAL0) + "\t" + to_string(BLAL1) + "\t" + to_string(BLAL2) + "\n");

    parser.respond("\n\nCONDITIONAL PROBABILITIES P(A | C):\n"); 
    parser.respond("P(A=H, C=H) = " + to_string(AHCH0) + "\t" + to_string(AHCH1) + "\t" + to_string(AHCH2) + "\n");
    parser.respond("P(A=M, C=H) = " + to_string(AMCH0) + "\t" + to_string(AMCH1) + "\t" + to_string(AMCH2) + "\n");
    parser.respond("P(A=L, C=H) = " + to_string(ALCH0) + "\t" + to_string(ALCH1) + "\t" + to_string(ALCH2) + "\n");

    parser.respond("P(A=H, C=M) = " + to_string(AHCM0) + "\t" + to_string(AHCM1) + "\t" + to_string(AHCM2) + "\n");
    parser.respond("P(A=M, C=M) = " + to_string(AMCM0) + "\t" + to_string(AMCM1) + "\t" + to_string(AMCM2) + "\n");
    parser.respond("P(A=L, C=M) = " + to_string(ALCM0) + "\t" + to_string(ALCM1) + "\t" + to_string(ALCM2) + "\n");

    parser.respond("P(A=H, C=L) = " + to_string(AHCL0) + "\t" + to_string(AHCL1) + "\t" + to_string(AHCL2) + "\n");
    parser.respond("P(A=M, C=L) = " + to_string(AMCL0) + "\t" + to_string(AMCL1) + "\t" + to_string(AMCL2) + "\n");
    parser.respond("P(A=L, C=L) = " + to_string(ALCL0) + "\t" + to_string(ALCL1) + "\t" + to_string(ALCL2) + "\n");

    parser.respond("\n\nCONDITIONAL PROBABILITIES P(C | A):\n"); 
    parser.respond("P(C=H, A=H) = " + to_string(CHAH0) + "\t" + to_string(CHAH1) + "\t" + to_string(CHAH2) + "\n"); 
    parser.respond("P(C=M, A=H) = " + to_string(CHAM0) + "\t" + to_string(CHAM1) + "\t" + to_string(CHAM2) + "\n"); 
    parser.respond("P(C=L, A=H) = " + to_string(CHAL0) + "\t" + to_string(CHAL1) + "\t" + to_string(CHAL2) + "\n"); 

    parser.respond("P(C=H, A=M) = " + to_string(CMAH0) + "\t" + to_string(CMAH1) + "\t" + to_string(CMAH2) + "\n"); 
    parser.respond("P(C=M, A=M) = " + to_string(CMAM0) + "\t" + to_string(CMAM1) + "\t" + to_string(CMAM2) + "\n"); 
    parser.respond("P(C=L, A=M) = " + to_string(CMAL0) + "\t" + to_string(CMAL1) + "\t" + to_string(CMAL2) + "\n"); 

    parser.respond("P(C=H, A=L) = " + to_string(CLAH0) + "\t" + to_string(CLAH1) + "\t" + to_string(CLAH2) + "\n"); 
    parser.respond("P(C=M, A=L) = " + to_string(CLAM0) + "\t" + to_string(CLAM1) + "\t" + to_string(CLAM2) + "\n"); 
    parser.respond("P(C=L, A=L) = " + to_string(CLAL0) + "\t" + to_string(CLAL1) + "\t" + to_string(CLAL2) + "\n"); 

    parser.respond("\n\nCONDITIONAL PROBABILITIES P(C | B):\n"); 
    parser.respond("P(C=H, B=H) = " + to_string(CHBH0) + "\t" + to_string(CHBH1) + "\t" + to_string(CHBH2) + "\n");
    parser.respond("P(C=M, B=H) = " + to_string(CMBH0) + "\t" + to_string(CMBH1) + "\t" + to_string(CMBH2) + "\n"); 
    parser.respond("P(C=L, B=H) = " + to_string(CLBH0) + "\t" + to_string(CLBH1) + "\t" + to_string(CLBH2) + "\n"); 

    parser.respond("P(C=H, B=L) = " + to_string(CHBL0) + "\t" + to_string(CHBL1) + "\t" + to_string(CHBL2) + "\n"); 
    parser.respond("P(C=M, B=L) = " + to_string(CMBL0) + "\t" + to_string(CMBL1) + "\t" + to_string(CMBL2) + "\n"); 
    parser.respond("P(C=L, B=L) = " + to_string(CLBL0) + "\t" + to_string(CLBL1) + "\t" + to_string(CLBL2) + "\n"); 

    parser.respond("\n\nCONDITIONAL PROBABILITIES P(B | C):\n"); 
    parser.respond("P(B=H, C=H) = " + to_string(BHCH0) + "\t" + to_string(BHCH1) + "\t" + to_string(BHCH2) + "\n");
    parser.respond("P(B=L, C=H) = " + to_string(BLCH0) + "\t" + to_string(BLCH1) + "\t" + to_string(BLCH2) + "\n");

    parser.respond("P(B=H, C=M) = " + to_string(BHCM0) + "\t" + to_string(BHCM1) + "\t" + to_string(BHCM2) + "\n");
    parser.respond("P(B=L, C=M) = " + to_string(BLCM0) + "\t" + to_string(BLCM1) + "\t" + to_string(BLCM2) + "\n");

    parser.respond("P(B=H, C=L) = " + to_string(BHCL0) + "\t" + to_string(BHCL1) + "\t" + to_string(BHCL2) + "\n");
    parser.respond("P(B=L, C=L) = " + to_string(BLCL0) + "\t" + to_string(BLCL1) + "\t" + to_string(BLCL2) + "\n");

    auto endPrinting = chrono::high_resolution_clock::now(); // end the timer for printing




    chrono::duration<double, milli> parsingDuration = endParsing - startParsing; 
    chrono::duration<double, milli> translationDuration = endTranslation - startTranslation; 
    chrono::duration<double, milli> duration = endMarginalization - startMarginalization; 
    chrono::duration<double, milli> oldConditioningDuration = stopOldConditioning - startOldConditioning;
    chrono::duration<double, milli> newConditioningDuration = stopNewConditioning - startNewConditioning;
    chrono::duration<double, milli> veConditioningDuration = stopVEConditioning - startVEConditioning;
    chrono::duration<double, milli> printingDuration = endPrinting - startPrinting;

    parser.respond("\n\n\n");
    parser.respond("Parsing took:\t" + to_string(parsingDuration.count()) + " ms\n");
    parser.respond("Translation took:\t" + to_string(translationDuration.count()) + " ms\n");
    parser.respond("Marginalization took:\t" + to_string(duration.count()) + " ms\n");
    parser.respond("Old Conditioner took:\t" + to_string(oldConditioningDuration.count()) + " ms\n");
    parser.respond("New Conditioner took:\t" + to_string(newConditioningDuration.count()) + " ms\n");
    parser.respond("VE Conditioner took:\t" + to_string(veConditioningDuration.count()) + " ms\n");
    parser.respond("Printing took:\t" + to_string(printingDuration.count()) + " ms\n");

    parser.log.close(); // close the log file

    parser.respond("\n\nThat's all folks!\n");

    return 0;
}