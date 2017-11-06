#include "../src/functions.hpp"

// We put the tests in their own scope so they are independent

void testEraseWhiteSpaces()
{
    try
    {
        {
            std::string str = " 1  2  4        1             ";
            eraseWhiteSpaces( str );
            if( str != "1241" ) throw "Test 1";
        }

        {
            std::string str = "";
            eraseWhiteSpaces( str );
            if( str != "" ) throw "Test 2";
        }

        {
            std::string str = " ";
            eraseWhiteSpaces( str );
            if( str != "" ) throw "Test 3";
        }

        {
            std::string str = "       ";
            eraseWhiteSpaces( str );
            if( str != "" ) throw "Test 4";
        }

        {
            std::string str = "a";
            eraseWhiteSpaces( str );
            if( str != "a" ) throw "Test 5";
        }

        {
            std::string str = "a   b";
            eraseWhiteSpaces( str );
            if( str != "ab" ) throw "Test 6";
        }

    }
    catch( const char *msg )
    {
        std::cerr << "\neraseWhiteSpaces not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in eraseWhiteSpaces\n";
    }
}


void testParseInt()
{
    try
    {
        {
            std::string str = "123";
            int n = parseInt( str, 0, str.size()-1 );
            if( n != 123 ) throw "Test 1";
        }
        {
            std::string str = "12 3";
            int n = parseInt( str, 0, str.size()-1 );
            if( n != 12 ) throw "Test 2";
        }
        {
            std::string str = "  12 3";
            int n = parseInt( str, 0, str.size()-1 );
            if( n != 12 ) throw "Test 3";
        }
        {
            try{
                // expected behaviour here is to
                // throw an invalid_argument error
                std::string str = "asd";
                int n = parseInt( str, 0, str.size()-1 );
                // If we are here no error was thrown
                throw "Test 4";
            }
            catch( const std::invalid_argument& ia ){}
            catch( ... ){ throw "Test 4"; }
        }
        {
            try{
                // expected behaviour here is to
                // throw an invalid_argument error
                std::string str = "";
                int n = parseInt( str, 0, str.size()-1 );
                throw "Test 5";
            }
            catch( const std::invalid_argument& ia ){}
            catch( ... ){ throw "Test 5"; }
        }
        {
            std::string str = "123";
            int n = parseInt( str, 0, 0 );
            if( n != 1 ) throw "Test 6";
        }
        {
            std::string str = " asdasdasd 12 3";
            int n = parseInt( str, 0, str.size()-1 );
            if( n != 12 ) throw "Test 7";
        }
    }
    catch( const char *msg )
    {
        std::cerr << "\nparseInt not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in parseInt\n";
    }
}



int main()
{
    testEraseWhiteSpaces();
    testParseInt();

    std::cout << "\n\nFinished unit tests\n\n";
    return 0;
}
