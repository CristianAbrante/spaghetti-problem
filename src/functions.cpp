#include "functions.hpp"

int parseInt( const std::string &str, int start, int end )
{
    return std::stoi( str.substr(start, end - start + 1) );
}


void eraseWhiteSpaces( std::string& str )
{
    auto endIterator = remove_if(
        str.begin(),
        str.end(),
        []( char c ){ return c == ' '; }
    );

    str = std::string( str.begin(), endIterator );
}


// Avoided to use regular expressions in these next functions
// because they can cause trouble if the compiler is old.
bool interpretIfStatement( Node &n )
{
    // "The language contains many other statements,
    // but only the conditional goto begins with ‘if(’ and
    // ends with ‘)goto label’"
    int ifPos = n.statement.find("if(");
    if( ifPos != 0  ||  ifPos == std::string::npos )
        return false;

    int gotoPos = n.statement.find(")goto", 3);
    if( gotoPos == std::string::npos )
        return false;

    // Reached this point this has to be a conditional goto
    // so the label must exist
    n.labelToNextNode = parseInt( n.statement, gotoPos+5, n.statement.size()-1 );
    n.statement.erase( gotoPos+5 );
    n.type = StatementType::conditional;

    return true;
}


bool interpretGotoStatement( Node &n )
{
    int gotoPos = n.statement.find("goto");
    if( gotoPos != 0  ||  gotoPos == std::string::npos )
        return false;

    n.labelToNextNode = parseInt( n.statement, gotoPos+4, n.statement.size()-1 );
    n.statement.erase( gotoPos+4 );
    n.type = StatementType::unconditional;

    return true;
}


bool interpretStopStatement( Node &n )
{
    if( n.statement != "stop" )
        return false;

    n.labelToNextNode = -1;
    n.type = StatementType::stop;

    return true;
}


void interpretOtherStatement( Node &n )
{
    n.labelToNextNode = -1;
    n.type = StatementType::other;
}


void interpretStatement( Node &n )
{
    if( !interpretStopStatement(n) )
        if( !interpretGotoStatement(n) )
            if( !interpretIfStatement(n) )
                interpretOtherStatement(n);
}


Line parseLine( std::string& str )
{
    Line line = {};
    try{
        // "The first five positions in each line are reserved
        // for an optional label, which is an integer"
        line.lineLabel = parseInt( str, 0, 4 );
        line.lineHasLabel = true;
    }
    // We reach the catch if there is no label in the line
    catch( const std::invalid_argument& ia ){
        line.lineLabel = -1;
        line.lineHasLabel = false;
    }

    // Now we keep only the statement part of the string
    // "Therefore, statements occupy positions 7 and beyond in each input line"
    line.n.statement = str.substr( 6 );

    // Erase white spaces from statement
    eraseWhiteSpaces( line.n.statement );

    // Finally, determine node type and label if it is a type of goto
    interpretStatement( line.n );

    return line;
}
