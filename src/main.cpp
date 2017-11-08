
#include "functions.hpp"
#include "program-functions.hpp"

#include <string>
#include <iostream>

int main (void) {

  // Continue looping until the end of the file.
  while(!std::cin.eof()) {

    std::string program_line_string;
    std::vector<Program> programs;

    for (int i = 0; i < 2; ++i) {

      int current_vector_pos = 0;
      Program current_program;

      // We read the line and check if it is empty.
      // If it is empty it means that it is the end of the current program.
      while (std::getline(std::cin, program_line_string) && !program_line_string.empty()) {

        // We parse the current line and then added to the vector of lines.
        Line current_program_line = parseLine(program_line_string);
        current_program.program_lines.push_back(current_program_line);

        // If the current line has label, then we introduce it of the
        // equivalence map.
        if (current_program_line.lineHasLabel) {
          current_program.labels_equivalence[current_program_line.lineLabel] = current_vector_pos;
        }

        // We increment the current position on the line vector.
        current_vector_pos++;
      }
      programs.push_back(current_program);
    }
  }

  return 0;
}
