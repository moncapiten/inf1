#include <iostream>
#include <vector>

using namespace std;


class queue {
    private:
        vector<int> data;
        int head;
        int tail;
    public:
        queue() {
            head = 0;
            tail = 0;
        }

        void push(int x) {
            data.push_back(x);
            tail++;
        }

        int pop() {
            return data[head];
            head++;
        }
/*
        int front() {
            return data[head];
        }
*/
        int back() {
            return data[tail - 1];
        }

        size_t size() {
            return tail - head;
        }

        bool empty() {
            return size() == 0;
        }
};



template <typename T>
class Stack {
    private:
        vector<T> data;

    public:
//        Stack(){
//        data = {};

                        // uso & per permettermi di passare in maniera più veloce tramite riferimento
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






bool is_balanced(const string & s){
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


/*
inline bool sameKind( const char & c1, const char & c2){
    return (c1 == '(' && c2 == ')') || (c1 == '[' && c2 == ']') || (c1 == '{' && c2 == '}');
}
*/



int main() {

    vector<int> v = {1, 2, 3, 4, 5};

    Stack<int> s;

    for (auto i : v){
        s.push(i);
    }

    cout << s << endl;
    while (!s.empty()){
        cout << s.pop() << " ";
    }
    cout << endl;



    string valid = "{[()]}[][()(){{{}}}]";
    string invalid = "{[()]}[][()(){{}}}]";

    cout << is_balanced(valid) << endl;
    cout << is_balanced(invalid) << endl;
    return 0;
}