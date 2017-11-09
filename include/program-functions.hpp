
#ifndef PROGRAM_FUNCTIONS_HPP
#define PROGRAM_FUNCTIONS_HPP

#include <iostream>
#include <map>
#include <vector>
#include <stack>

#include "functions.hpp"
// Program struct contains the vector of nodes
// used to represent the lines of the program,
// and the execution flow.

struct Program {

  std::vector<Line> program_lines;

  // Hash that gets the label of the node
  // as the key and return this node position
  // in the program_line vector.
  std::map<int, int> labels_equivalence;
};

  Program readProgram(std::ifstream &is);

//This function checks if there is a unconditional goto statements loop
//in program starting from the line currentPLine. currentPLine contains
//an unconditional goto statement
void loopChecker(Program program, int& currentPLine);
  
//This function determines if two Fortran IV programs (represented as graphs)
//are equivalent or not, with equivalent we mean that for all possible inputs,
//they execute exactly the same sequence of statements, ignoring unconditional
//gotos and labels
bool comparePrograms(Program firstProgram, Program secondProgram);

#endif // PROGRAM_FUNCTIONS_HPP
