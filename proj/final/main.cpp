#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <windows.h> // for colored text in console
#include <string>
#include <algorithm>
#include <cctype>
#include <bitset>
#include <stdexcept>
//#include <functional>
#include <numeric>

#include <chrono>


#include "parser.hpp"
//#include "parser.cpp"

#include "marginalizer.hpp"
//#include "marginalizer.cpp"

#include "nodes.hpp"
#include "supportFunc.hpp"



using namespace std;







int main(){
    string filename = "../cancer.bif"; // file name


    auto startParsing = chrono::high_resolution_clock::now(); // start the timer
    Parser parser; // create a new parser object
    parser.parse(filename); // parse the file using the parser object
    auto endParsing = chrono::high_resolution_clock::now(); // end the timer

    auto startTranslation = chrono::high_resolution_clock::now(); // start the timer for translation
    parser.translate();
    auto endTranslation = chrono::high_resolution_clock::now(); // end the timer for translation

    BayesianNetwork network = parser.getNetwork(); // get the Bayesian network from the parser
//    nodes = parser.getNodes();
//    substitutionIndeces = parser.getIndecesArchive();
//    substitutionNames = parser.getNamesArchive();


    

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


    cout << "\n\nThat's all folks!" << endl; // print the end message



    return 0;
}