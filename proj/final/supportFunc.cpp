#include <iostream>
#include <vector>


using namespace std;


std::string toString(const std::vector<int>& bits) {
    std::string result;
    result.reserve(bits.size());
    for (int b : bits)
        result += (b ? '1' : '0');
    return result;
}

vector<int> toBitVector(uint64_t value, size_t bitCount) {
    std::vector<int> bits(bitCount);
    for (size_t i = 0; i < bitCount; ++i)
        bits[bitCount - 1 - i] = (value >> i) & 1;
    return bits;
}



std::ostream& operator<<(std::ostream& os, const std::vector<double>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i + 1 < vec.size()) os << ", ";
    }
    return os << "]";
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i + 1 < vec.size()) os << ", ";
    }
    return os << "]";
}



template <typename T>
class Stack {
    private:
        vector<T> data;

    public:
        void push(const T & x){
            data.push_back(x);
        }

        T pop(){

            if( data.empty() ){
                throw out_of_range("Stack<>::pop(): empty stack");
            } 
            T temporary_container = data.back();
            data.pop_back();

            return temporary_container;
        }

        auto empty(){
            return data.empty();
        }

        auto size(){
            return data.size();
        }


        friend ostream & operator << (ostream & os, const Stack<T> & s){
            for (const auto & i : s.data){
                os << i << " ";
            }
            return os;
        }
};

bool checkParenthesis(const vector<string>& v){
    
    string s = ""; // string to hold the concatenated tokens
    for( auto i : v ){
        s += i; // concatenate the tokens into a single string

    }
    Stack<char> stack;

    for (const auto & c : s){
//        cout << stack << endl;            // stack usage visualization, kinda useful from time to time
        if (c == '(' || c == '[' || c == '{'){
            stack.push(c);
        } else if (c == ')' || c == ']' || c == '}'){
            if (stack.empty()){
                return false;
            }
            char top = stack.pop();
            if (c == ')' && top != '('){
                return false;
            }
            if (c == ']' && top != '['){
                return false;
            }
            if (c == '}' && top != '{'){
                return false;
            }
        }
    }

    return stack.empty();
}