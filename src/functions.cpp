#include "functions.hpp"

int parseInt( const std::string& str, int start, int end )
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
