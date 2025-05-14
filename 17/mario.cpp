#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream file("cancer.bif");
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << '\n';
        }
        file.close();
    } else {
        cerr << "Unable to open file\n";
    }

    return 0;
}
