//
//  main.cpp
//  
//
//  Created by Kasey Harvey on 11/2/18.
//

#include "MazeSolver.cpp"
#include <iostream>
#include "MazeSolver.h"
int main() {
    MazeSolver solver("input4.txt");
    if(solver.mazeIsReady())
    {
        solver.solveMaze();
        solver.printSolution();
    }
    return 0;
}




