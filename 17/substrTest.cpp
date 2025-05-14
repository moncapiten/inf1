#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>


using namespace std;

int main() {
    string str = "Hello, World!";


    vector<string> tokens;
    if(str.find(",") != string::npos) {
        size_t pos = str.find(",");
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + 1);
    }


    // print the tokens
    for (const auto& token : tokens) {
        cout << token << endl;
    }


    vector<string> groups = {{"network"}, {"variable"}, {"probability"}};

    for( auto i = groups.begin(); i < groups.end(); i++ ) {

    

        cout << "Group: " << *i << endl;
    }


    auto varIndex = find(groups.begin(), groups.end(), "variable");

    cout  << '\t' << *varIndex << endl;


    string nums = "3.5,";
    double num = stod(nums);
    cout << "Converted number: " << num << endl;



    return 0;
}