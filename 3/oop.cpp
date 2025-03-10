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



int main() {

    return 0;
}