#ifndef SUPPORTFUNC_HPP
#define SUPPORTFUNC_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>



using namespace std;


std::ostream& operator<<(std::ostream& os, const std::vector<double>& vec);
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec);
ostream& operator<<(ostream& os, const vector<std::string>& vec);

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
//        friend ostream & operator << <>(ostream & os, const Stack<T> & s);
};




#endif // SUPPORTFUNC_HPP