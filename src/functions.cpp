#include "functions.hpp"

int parseInt( const std::string &str, int start, int end )
{
    int num = 0;
    bool thereIsNoNumber = true;

    while( start <= end )
    {
        if( str[start] <= '9'  &&  str[start] >= '0' )
        {
            thereIsNoNumber = false;
            while( str[start] <= '9'  &&  str[start] >= '0' && start <= end )
            {
                // 48 is the ascii value for '0'
                num += str[start] - 48;
                num *= 10;
                ++start;
            }
            break;
        }
        ++start;
    }

    if( thereIsNoNumber )
        throw std::invalid_argument("There is no number in the string range");

    return( num / 10 );
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