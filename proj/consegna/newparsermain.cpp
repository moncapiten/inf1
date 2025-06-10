#include "newparser.hpp"
#include "nodes.hpp"
#include "supportFunc.hpp"
#include "marginalizer.hpp"
#include "conditionalizer.hpp"

using namespace std;

int main() {
    Parser parser;
    
    parser.registerCommand("marginalize", [&parser](const vector<string>& args) {
        if (args.size() != 1 || parser.network.size() == 0) {
            cout << "Usage: marginalize\n";
            return;
        }
        parser.network = Marginalizer().marginalize(parser.network); //an enclosing-function local variable cannot be referenced in a lambda body unless it is in the capture listC/C++(1735)
        cout << "\n";
    });


    // ADD MARGINALIZATION CHECK, EVERY NODE IN THE NETWORK MUST BE MARGINALIZED, THERE COULD BE PROBLEM IF IS ASKED THE COND OF TWO ORPHAN NODES, TBD BUT VERY IMPORTRTANT, ALSO APPLIES TO JOINT PROBABILITY
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

    // Start interactive mode
    parser.runInteractiveMode();
    
    cout << "That's all folks!\n";
    return 0;
}