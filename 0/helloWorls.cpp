// make an hello world program that writes hello world in the aproximate shape of a christmas tree

#include <iostream>
#include <string>

#define TREE_HEIGHT 10

int main() {
    std::string tree[TREE_HEIGHT] = {
        "         *",
        "        ***",
        "       *****",
        "      *******",
        "     *********",
        "    ***********",
        "   *************",
        "  ***************",
        " *****************",
        "*******************"
    };

    for (int i = 0; i < TREE_HEIGHT; i++) {
        std::cout << tree[i] << std::endl;
    }

    return 0;
}




