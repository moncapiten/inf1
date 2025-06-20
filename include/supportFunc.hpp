#ifndef SUPPORTFUNC_HPP
#define SUPPORTFUNC_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <map>


using namespace std;


ostream& operator<<(ostream& os, const vector<double>& vec);
ostream& operator<<(ostream& os, const vector<int>& vec);
ostream& operator<<(ostream& os, const vector<string>& vec);
ostream& operator<<(ostream& os, const map<vector<string>, double>& m);
ostream& operator<<(ostream& os, const map<string, vector<string>>& m);

std::string getCurrentTimestamp();

bool checkParenthesis(const vector<string>& v);

template <typename T>
class Stack {
    private:
        vector<T> data;

    public:
        void push(const T & x);
        T pop();
        auto empty();
        auto size();
};




#endif // SUPPORTFUNC_HPP