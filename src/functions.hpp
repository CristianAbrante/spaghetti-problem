#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <algorithm>

// Returns the number found in the range [start, end]
// Throws an invalid_argument exception if no number is found
int parseInt( const std::string& str, int start, int end );

// Removes every white space from a string
void eraseWhiteSpaces( std::string& str );

#endif
