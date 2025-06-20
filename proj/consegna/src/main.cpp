#include "../include/newparser.hpp"
#include "../include/nodes.hpp"
#include "../include/supportFunc.hpp"
#include "../include/marginalizer.hpp"
#include "../include/conditionalizer.hpp"
#include "../include/VE.hpp"

#include <iostream>
#include <chrono>

using namespace std;

int main() {
    Parser parser;
    
    parser.registerCommand("marg", [&parser](const vector<string>& args) { // the parser itself is captured by reference so that it may be used in the lambda
        if (args.size() != 1 || parser.network.size() == 0) {
            parser.respond("Usage: marg\n");
            return;
        }
        parser.network = Marginalizer().marginalize(parser.network);
    }, {}, "Marginalize the currently loaded Bayesian network, every node will contain in the pureProb vector its own marginal probabilities.\nNecessary before computing conditional/joint probabilities." );

    parser.registerCommand("cond", [&parser](const vector<string>& args) {
        if (args.size() != 5) {
            parser.respond("Usage: cond <A> <a> <B> <b>\n");
            return;
        }
        try {
            double result = computeConditionalProbabilityVE(parser.network, args[1], args[2], args[3], args[4]);
            parser.respond("P(" + args[1] + "=" + args[2] + " | " + args[3] + "=" + args[4] + ") = " + to_string(result) + "\n");
        } catch (const exception& e) {
            parser.respond("Error: " + string(e.what()) + "\n");
        }
    }, {"<A> <a> <B> <b>"}, "Compute the conditional probability P(A=a | B=b) using variable elimination. It is relatively fast, but still requires a non-trivial amount of time for larger networks( >= 20 nodes).\nRequires the network to be marginalized first, use 'marg' command to do so.");

    parser.registerCommand("joint", [&parser](const vector<string>& args) {
        if (args.size() != 5) {
            parser.respond("Usage: joint <A> <a> <B> <b>\n");
            return;
        }
        try {
            double result = computeJointProbabilityVE(parser.network, args[1], args[2], args[3], args[4]);
            parser.respond("P(" + args[1] + "=" + args[2] + ", " + args[3] + "=" + args[4] + ") = " + to_string(result) + "\n");
        } catch (const exception& e) {
            parser.respond("Error: " + string(e.what()) + "\n");
        }
    }, {"<A> <a> <B> <b>"}, "Compute the joint probability P(A=a, B=b) using variable elimination. It is relatively fast, but still requires a non-trivial amount of time for larger networks( >= 20 nodes).\nRequires the network to be marginalized first, use 'marg' command to do so.");



    parser.registerCommand("testCond", [&parser](const vector<string>& args) {
        if (args.size() != 1) {
            parser.respond("Usage: test\n");
            return;
        }
        try {
            BayesianNode A = parser.network.getNode_ID(0);
            BayesianNode B = parser.network.getNode_ID(parser.network.size() - 1);

            parser.respond("P(" + A.name + "=" + A.states[0] + ", " + B.name + "=" + B.states[0] + ") = \n");
            auto start = chrono::high_resolution_clock::now();
            double result = computeConditionalProbabilityVE(parser.network, A.name, A.states[0], B.name, B.states[0]);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;
            auto timeTaken = elapsed.count();
            if (timeTaken > 1000) {
                parser.respond( to_string(result) + " (took " + to_string(timeTaken/1000) + " seconds)\n");
            } else {
                parser.respond( to_string(result) + " (took " + to_string(timeTaken) + " ms)\n");
            }

        } catch (const exception& e) {
            parser.respond("Error: " + string(e.what()) + "\n");
        }
    }, {}, "Test the conditional probability computation with a specific pair of nodes( first and last defined in the bif file).\nUseful to gauge the workability of the network.");


    // Start interactive mode
    parser.runInteractiveMode();
    
    cout << "That's all folks!\n";
    return 0;
}