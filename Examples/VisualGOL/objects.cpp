#include <iostream>
#include <vector>
// necessary for coloured text
#include <windows.h>
#include "header.hpp"

using namespace std;


class Grid{
    private:
        vector<vector<bool>> grid;
        const int rows;
        const int cols;
        int stepCounter = 0;

    public:
        Grid(int r, int c): rows(r), cols(c){
            grid = vector<vector<bool>>(rows, vector<bool>(cols, 0));
        }


        // the grid gets generated and either initialized at 0 or in random values
        void generateGrid(bool random){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    if(random){
                        grid[i][j] = rand() % 2;
                    } else {
                        grid[i][j] = 0;
                    }

                }
            }
        }



        void set(int i, int j, bool val){
            grid[i][j] = val;
        }


        bool get(int i, int j){
            return grid[i][j];
        }

        vector<int> size(){
            return vector<int>(rows, cols);
        }


        //                                 DA RULES
        // every generation the board gets recreated anew
        // if a cell has 2 living neighbour it remains in the state it was
        // if any cell has 3 living neighbours, it becomes alive( or stays)

        void step(){
            vector<vector<bool>> newGrid = vector<vector<bool>>(rows, vector<bool>(cols, 0));

            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    int aliveNeighbourCount = 0;

                    // simple debug reporting, as later
                    if(debugMode && verbose){
                        cout << "Analyzing cell " << i << j << endl;
                        cout << "\tAlive:\t" << grid[i][j] << endl;
                        cout << "\tNeighbours:" << endl;
                    }

                    // checking all cells around the one in exam, min and max are necessary to avoid out of range problems
                    for(int k = max(0, i-1); k < min(rows, i+2); k++){
                        for(int h = max(0, j-1); h < min(cols, j+2); h++){
                            // silly way to avoid counting oneselves as one of the neighbours, i think k!=i || k!=j should be the same but havent tried yet
                            if(k==i && h==j){}
                            else {
                                if(debugMode && verbose){
                                    cout << "\t\t" << k << h << " " << grid[k][h] << endl;
                                }
                                if(grid[k][h]){
                                    aliveNeighbourCount++;
                                }
                            }
                        }
                    }

                    // implementarion of the rules from above
                    if((aliveNeighbourCount == 2)){
                        newGrid[i][j] = grid[i][j];
                    }
                    if(aliveNeighbourCount == 3){
                        newGrid[i][j] = true;
                    }


                    if(debugMode){
                        cout << "Cell in position " << i << " " << j ;
                        if( grid[i][j]){
                            cout << " is alive ";
                        } else {
                            cout << " is dead ";
                        }
                        cout << "and has " << aliveNeighbourCount << " living neightbours" << endl;
                    }
                    


                }
                if(debugMode) cout << endl;
            }

            grid = newGrid;
            stepCounter++;
        }

        // << overload to simplify the printing process
        friend ostream & operator<<(ostream & os, const Grid & g){
            
            // necessary to control the text's color
            HANDLE hConsole;
            int k;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


            cout << "Step: " << g.stepCounter << endl << endl;
            for(int i = 0; i < g.rows; i++){
                for(int j = 0; j < g.cols; j++){
                    // set text to green if cell is alive
                    if(g.grid[i][j]){
                        SetConsoleTextAttribute(hConsole, 10);
                    }
                    os << g.grid[i][j] << " ";
                    // text back to white for normal reporting
                    SetConsoleTextAttribute(hConsole, 15);
                }
                os << endl;
            }
            return os;
        }

};



/*****************************************************************************************DONT WORK - OFFSET DEPENDENTS ON GRID SIZE AND TO BE DONE ********************************************************/
void standardStarting(int startingIndex, Grid & g){
    vector<vector<bool>>  standardStartings;
    standardStartings.push_back({0, 1, 0, 0, 0, 0, 0, 1, 0, 0,  1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    standardStartings.push_back({0,0,0,0,1,1,1,1,0,0,0,0,
                                 0,0,1,1,1,1,1,1,1,1,0,0,
                                 1,1,1,1,1,1,1,1,1,1,1,1,
                                 0,0,1,1,1,1,1,1,1,1,0,0,
                                 0,0,0,0,1,1,1,1,0,0,0,0});

/*    for(int i = 0; i < standardStartings[startingIndex].size(); i++){
        g.set(i, g.)
    }
*/
    vector<int> size = g.size();
    int rows = size[0];
    int cols = size[1];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(j + rows*i < standardStartings[startingIndex].size()){
                g.set(i, j, standardStartings[startingIndex][j + rows*i]);
            } else {
                g.set(i, j, false);
            }
        }
    }
}
