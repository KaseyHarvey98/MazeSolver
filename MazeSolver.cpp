//
//  MazeSolver.cpp
//  
//
//  Created by Kasey Harvey on 11/2/18.
//

#include "MazeSolver.h"
#include <iostream>// cerr = error output stream
#include <cstdlib> //exit
#include <string>//string
#include <sstream>
#include <cctype> // isspace
#include <fstream> // file stream
using namespace std;

    /*constructor
    pre: input file is in correct format with first two values being integers
          followed by valid maze characters in {'_', '*','$'}
    post: if inuput file cannot be read outputs "Cannot read from input_file"
          otherwise sets maze_rows_ and maze_columns_ from the first two values in input file
          and allocates two 2-dimesional array of size maze_rows_ and maze_columns_
          both maze_ and solution_ are initialized with characters read from input */
MazeSolver::MazeSolver(std::string input_file){
    ifstream in_stream;
    in_stream.open (input_file);
    if(in_stream.fail())
    {
        cout << "Cannot read from input_file_name\n";
        in_stream.close();
        exit(1);
    }

    maze_ready= true;
    
    in_stream >> maze_rows_ >> maze_columns_;

    initializeMaze(maze_rows_, maze_columns_);
    fillMaze(in_stream);
    initializeSolution();
}
    
   /*  destructor
    post: deletes maze_ and solution_ */
MazeSolver::~MazeSolver(){
    if (maze_ready) {
        
        maze_ = nullptr;
        solution_ = nullptr;
        
        for( int i = 0 ; i < maze_rows_ ; i++ ){
            delete [] maze_[i] ;
            delete [] maze_ ;
        }
        for( int i = 0 ; i < maze_rows_ ; i++ ){
            delete [] solution_[i] ;
            delete [] solution_ ;
        }
    }
}

    //return: true if maze has been initialized, false otherwise
bool MazeSolver::mazeIsReady(){
    return maze_ready;
}
    
    /*pre: rows and columns are positive integers
    post: allocates maze_ with rows and columns
    called by constructor */
void MazeSolver::initializeMaze(int rows, int columns){
    
    //memory allocated for elements of rows.
    maze_ = new char* [rows] ;

    //memory allocated for  elements of each column.

    for( int i = 0 ; i < rows ; i++ ){
            maze_[i] = new char[columns];
    }
}
                             /* Private functions*/

    /*pre: maze_ has been allocated with the correct number of rows and columns read from input file
    post: fills in maze_ with characters read from input file
    called by constructor*/
void MazeSolver::fillMaze(std::ifstream& input_stream){
    for (int rows = 0; rows < maze_rows_; rows ++) {
        for (int columns = 0; columns <maze_columns_; columns++) {
            input_stream >> maze_[rows][columns];
        }// end for col
    }
}

/*   pre: maze_ has been initialized with valid character values in {'_', '*','$'}
         start position is always [0][0]
    post: initializes solution_ with a copy of maze_
          initializes backtrack_stack_  with all viable paths from position [0][0]
          and mark the current position as visited ( '>' ) on solution_
    called by constructor */
void MazeSolver::initializeSolution(){
    copyMazetoSolution();
    Position load;
    backtrack_stack_.push(load);
}

    /*pre: maze_ has been properly initialized
    post: allocates solution_ to the correct number of rows and columns
          and copies the contents of maze_ into solution_
    called by initializeSolution() */
void  MazeSolver::copyMazetoSolution(){
    solution_ = new char*[maze_rows_]; // memory allocated for elements of rows
    for (int i = 0; i < maze_rows_; i++) {
        solution_[i] = new char [maze_columns_];
    }
    for (int rows = 0; rows < maze_rows_; rows ++) {
        for (int columns = 0; columns <maze_columns_; columns++) {
            solution_[rows][columns] = maze_[rows][columns];
        }// end for col
    }
}// end copy maze to solution

 // move back to public section

/*pre: maze_ has been initialized with valid character values in {'_', '*','$'}
 post: solution_ has been marked with '>' signs along the path to the exit
 prints "Found the exit!!!" if exit is found
 or "This maze has no solution." if no exit could be found
 return: true if exit is found, false otherwise*/
bool MazeSolver::solveMaze(){
    Position current_position;
    current_position.row = 0;
    current_position.column = 0;
    while (!backtrack_stack_.empty()) {
        if (maze_[current_position.row][current_position.column] == '$') {
            cout << "Found the exit!!!" << endl;
            return true;
        } else if(extendPath(current_position)) {
            solution_[current_position.row][current_position.column] = '>';
            current_position = backtrack_stack_.top();
        }
        else if(!extendPath(current_position)){
            maze_[current_position.row][current_position.column] = 'X';
            solution_[current_position.row][current_position.column] = '@';
            backtrack_stack_.pop();
        }
         if (!backtrack_stack_.empty()) current_position = backtrack_stack_.top();
    }
    cout << "This maze has no solution."<< endl;
    return false;
}



    /*pre: current_position is a valid position on the maze_
    post: adds all positions extensible from current_position to backtrack_stack_
    return: true if path was extended, false otherwise
    called by solveMaze()*/
bool MazeSolver::extendPath(Position current_position){
    bool extended = false;
    if (isExtensible(current_position, SOUTH)) {
        backtrack_stack_.push(getNewPosition(current_position, SOUTH));
        extended = true;
    }
    if (isExtensible(current_position, EAST)) {
        backtrack_stack_.push(getNewPosition(current_position, EAST));
        extended = true;
    }
    return extended;
}

    /*pre: old_position is a Position initialized with rows and column to valid positions in maze_ and it is extensible in direction dir
    return: a new Position on the maze moving in direction dir from old_position
    called by extendPath() */
Position MazeSolver::getNewPosition(Position old_position, direction dir){
    Position new_position;
    if (dir == SOUTH){
        new_position.row    = old_position.row+1;
        new_position.column = old_position.column;
    }
    if (dir == EAST){
        new_position.row    = old_position.row;
        new_position.column = old_position.column+1;
    }
    return new_position;
}

    /*checks if the path can be extended in maze_ from position current_position in direction dir
    return: true if path can be extended given current_position and dir, false otherwise
    called by extendPath*/
bool MazeSolver::isExtensible(Position current_position, direction dir){
    if ((current_position.row < maze_rows_) && (current_position.column < maze_columns_)) {
        if (dir == SOUTH){
            if ((current_position.row+1 < maze_rows_) && (current_position.column < maze_columns_)){
                
                if ((maze_[current_position.row+1][current_position.column] == '_')|| (maze_[current_position.row+1][current_position.column] == '$')) return true;
            }
        }
        
        if (dir == EAST)  {
            if ((current_position.row < maze_rows_) && (current_position.column+1 < maze_columns_)){
                
                if ((maze_[current_position.row][current_position.column+1] == '_')|| (maze_[current_position.row][current_position.column+1] == '$')) return true;
            }
        }
        return false;
    }
    return false;
}

/*post: prints the solution to the standard output stream
       with single space character between each maze character
       and each maze rows on a new line */
void MazeSolver::printSolution(){
    cout << "The solution to this maze is:" << endl;
    for (int rows = 0; rows < maze_rows_; rows ++) {
        for (int columns = 0; columns <maze_columns_; columns++) {
            cout << solution_[rows][columns] << " " ;
        }// end for col
        cout << endl;
    }
}
