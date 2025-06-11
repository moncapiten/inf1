#include "newparser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"
#include "conditionalizer.hpp"

#include <iostream>
#include <chrono>

using namespace std;

int main() {
    Parser parser;
    
    parser.registerCommand("marg", [&parser](const vector<string>& args) {
        if (args.size() != 1 || parser.network.size() == 0) {
            cout << "Usage: marginalize\n";
            return;
        }
        parser.network = Marginalizer().marginalize(parser.network); //an enclosing-function local variable cannot be referenced in a lambda body unless it is in the capture listC/C++(1735)
        cout << "\n";
    });

    parser.registerCommand("cond", [&parser](const vector<string>& args) {
        if (args.size() != 5) {
            cout << "Usage: cond <A> <a> <B> <b>\n";
            return;
        }
        try {
            double result = computeConditionalProbability(parser.network, args[1], args[2], args[3], args[4]);
            cout << "P(" << args[1] << "=" << args[2] << " | " << args[3] << "=" << args[4] << ") = " << result << "\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    });

    parser.registerCommand("joint", [&parser](const vector<string>& args) {
        if (args.size() != 5) {
            cout << "Usage: joint <A> <a> <B> <b>\n";
            return;
        }
        try {
            double result = computeJointProbability(parser.network, args[1], args[2], args[3], args[4]);
            cout << "P(" << args[1] << "=" << args[2] << ", " << args[3] << "=" << args[4] << ") = " << result << "\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    });


    ///   YOU GOTTA CHANGE THE STANDARD ACONDITIONALIZER TO THE NEW VERSION YOU EEJIT
    parser.registerCommand("testCond", [&parser](const vector<string>& args) {
        if (args.size() != 1) {
            cout << "Usage: test\n";
            return;
        }
        try {
            BayesianNode A = parser.network.getNode_ID(0);
            BayesianNode B = parser.network.getNode_ID(parser.network.size() - 1);

            cout << "P(" << A.name << "=" << A.states[0] << ", " << B.name << "=" << B.states[0] << ") = ";
            auto start = chrono::high_resolution_clock::now();
            double result = computeJointProbability(parser.network, A.name, A.states[0], B.name, B.states[0]);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;
            cout << result << "\n" << "Time taken: " << elapsed.count() << " ms\n";

        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    });


    // Start interactive mode
    parser.runInteractiveMode();
    
    cout << "That's all folks!\n";
    return 0;
}