#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <algorithm>

// Returns the first number found in the range [start, end]
// Throws an invalid_argument exception if no number is found
int parseInt( const std::string &str, int start, int end );

// Removes every white space from a string
void eraseWhiteSpaces( std::string &str );

// There are 3 type of statements special for us
// we want to distinguish between them and the rest
enum StatementType {
    conditional,
    unconditional,
    stop,
    other
};

// This defines the structure of the statements that will
// be the node of our graph
struct Node{
    std::string statement;
    StatementType type;
    int labelToNextNode; // If the node is not of the type conditional
                         // or unconditional this is -1
};

// These are "private" functions. They are used through the
// "interpretStatement" function defined below
// These functions are use to fill the type and labelToNextNode
// of the node. In addition to that, the interpretIf and Goto remove
// the label from the statement.
// The node has to have the statement string with the
// white spaces already removed.
bool interpretIfStatement( Node &n );
bool interpretGotoStatement( Node &n );
bool interpretStopStatement( Node &n );
void interpretOtherStatement( Node &n );

// This uses the 4 interpret "private" functions above to determine
// the type and labelToNextNode of the node. It also removes the
// label in conditional and unconditional types from the statement.
// As the functions above it needs the node to have the statement string
// with no white spaces.
void interpretStatement( Node &n );

#endif
