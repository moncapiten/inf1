#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include "header.hpp"
#include "objects.cpp"

// necessary for sleeping between steps
using namespace std::chrono;
using namespace std::this_thread;

int numrows = 5;
int numcols = 5;
int maxStepping = 7;

bool randomStart = true;
int startingIndex = -1;


void userInteraction(){
    cout << "Hello\nthis simple program runs Conway's Game of Life and outputs a matrix that represent the state of the board\n\nHow many rows you want your board to have? ";
    cin >> numrows;
    cout << "And how many columns? ";
    cin >> numcols;

    cout << "How many generations should the game last? ";
    cin >> maxStepping;


/************************************************************************************* NOT YET IMPLEMENTED *********************************************************************************
    cout << "Should the board be initialized to a random state? y/n ";
    string choice;
    cin >> choice;
    if(choice == "y"){
        return;
    } else {
        randomStart = false;
        cout << randomStart;
    }
    cout << "Which standard starting point do you want?\n1- Glider\n2- Glider generator(requires 12x12 grid)\n";
    cin >> startingIndex;
//    cout << "Would you like then to provide a custom start vector? y/n ";
//    return;
*********************************************************************************************************************************************************************************************/


}









int main(){
    srand(time(NULL));

    debugMode = false;
    verbose = false;

    if(debugMode){
        cout << "WARNING - Debug Mode is on";
        if(verbose) cout << " in Verbose setting";
        cout << "\nThe output may not be representative of the actual program workings" << endl << endl << endl;
    }

    userInteraction();

    Grid g(numrows, numcols);

    g.generateGrid(randomStart);

//******************************************************************************************************TO BE FIXED**********************************************************************/
//    if(!randomStart){
//        standardStarting(startingIndex, g);
//    }


    
    cout << g << endl;
    for(int i = 0; i < maxStepping; i++){
        g.step();
        cout << g << endl;
        sleep_for(milliseconds(500));
    }


    return 0;
}