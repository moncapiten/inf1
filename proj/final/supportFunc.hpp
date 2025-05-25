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

#endif // SUPPORTFUNC_HPP