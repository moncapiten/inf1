#ifndef MARGINALIZER_HPP
#define MARGINALIZER_HPP

#include <unordered_set>
#include <numeric>

#include "nodes.hpp"
#include "supportFunc.hpp"

using namespace std;


class Marginalizer{
    public:
        Marginalizer() {}
        BayesianNetwork& marginalize(BayesianNetwork& inputNetwork);

    private:
        BayesianNetwork outputNetwork; // output network, not used in this implementation but could be useful later
        vector<int> listInWorkingOrder; // vector to hold the list reordered so that no node ever appears before its parents

        vector<int> reorder(vector<BayesianNode> nodesIndeces);
        void updateVector(vector<int>& statesMask, const vector<int>& parentList);
        void marginalizeNode(BayesianNode& node);

};



#endif // MARGINALIZER_HPP