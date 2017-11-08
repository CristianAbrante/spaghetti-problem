
#ifndef PROGRAM_FUNCTIONS_HPP
#define PROGRAM_FUNCTIONS_HPP

#include <iostream>
#include <map>
#include <vector>

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

#endif // PROGRAM_FUNCTIONS_HPP
