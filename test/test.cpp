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
            try{
                // expected behaviour here is to
                // throw an invalid_argument error
                std::string str = " asdasdasd 12 3";
                int n = parseInt( str, 0, str.size()-1 );
                throw "Test 7";
            }
            catch( const std::invalid_argument& ia ){}
            catch( ... ){ throw "Test 7"; }
        }
        {
            std::string str = "-123";
            int n = parseInt( str, 0, str.size()-1 );
            if( n != -123 ) throw "Test 8";
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




// Remember that this function assumes that the statement is free
// from white spaces. Remember also that the problem promises us
// that after "if(...)goto" comes the label
void testInterpretIfStatement()
{
    try
    {
        {
            Node n = {};
            n.statement = "if()goto123";
            interpretIfStatement(n);

            if( n.statement != "if()goto"
            ||  n.type != StatementType::conditional
            ||  n.labelToNextNode != 123 )
                throw "Test 1";
        }

        {
            Node n = {};
            n.statement = "if(i.lt.j)goto99999";
            interpretIfStatement(n);

            if( n.statement != "if(i.lt.j)goto"
            ||  n.type != StatementType::conditional
            ||  n.labelToNextNode != 99999 )
                throw "Test 2";
        }

        {
            Node n = {};
            n.statement = "if(";

            if( interpretIfStatement(n) )
                throw "Test 3";
        }

        {
            Node n = {};
            n.statement = "goto1";

            if( interpretIfStatement(n) )
                throw "Test 4";
        }

        {
            Node n = {};
            n.statement = "stop";

            if( interpretIfStatement(n) )
                throw "Test 5";
        }

        {
            Node n = {};
            n.statement = "ifgoto123";

            if( interpretIfStatement(n) )
                throw "Test 6";
        }
    }
    catch( const char *msg )
    {
        std::cerr << "\ntestInterpretIfStatement not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in testInterpretIfStatement\n";
    }

}

// Remember that this function assumes that the statement is free
// from white spaces. Remember also that the problem promises us
// that after "goto" comes the label
void testInterpretGotoStatement()
{
    try
    {
        {
            Node n = {};
            n.statement = "goto123";
            interpretGotoStatement(n);

            if( n.statement != "goto"
            ||  n.type != StatementType::unconditional
            ||  n.labelToNextNode != 123 )
                throw "Test 1";
        }

        {
            Node n = {};
            n.statement = "goto99999";
            interpretGotoStatement(n);

            if( n.statement != "goto"
            ||  n.type != StatementType::unconditional
            ||  n.labelToNextNode != 99999 )
                throw "Test 2";
        }

        {
            Node n = {};
            n.statement = "if()goto999";

            if( interpretGotoStatement(n) )
                throw "Test 3";
        }

        {
            Node n = {};
            n.statement = "asdgoto999";

            if( interpretGotoStatement(n) )
                throw "Test 4";
        }
    }
    catch( const char *msg )
    {
        std::cerr << "\ninterpretGotoStatement not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in interpretGotoStatement\n";
    }

}


void testInterpretStopStatement()
{
    try
    {
        {
            Node n = {};
            n.statement = "if(stop)goto1";

            if( interpretStopStatement(n) )
                throw "Test 1";
        }

        {
            Node n = {};
            n.statement = "stop";
            interpretStopStatement(n);

            if( n.statement != "stop"
            ||  n.type != StatementType::stop
            ||  n.labelToNextNode != -1 )
                throw "Test 2";
        }

        {
            Node n = {};
            n.statement = "stops";

            if( interpretStopStatement(n) )
                throw "Test 3";
        }

        {
            Node n = {};
            n.statement = "sstop";

            if( interpretStopStatement(n) )
                throw "Test 4";
        }
    }
    catch( const char *msg )
    {
        std::cerr << "\ninterpretStopStatement not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in interpretStopStatement\n";
    }

}



void testInterpretOtherStatement()
{
    try
    {
        {
            Node n = {};
            n.statement = "asdasd";
            interpretOtherStatement(n);

            if( n.statement != "asdasd"
            ||  n.type != StatementType::other
            ||  n.labelToNextNode != -1 )
                throw "Test 1";
        }

    }
    catch( const char *msg )
    {
        std::cerr << "\ninterpretOtherStatement not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in interpretOtherStatement\n";
    }

}


void testInterpretStatement()
{
    try
    {
        {
            Node n = {};
            n.statement = "asdasd";
            interpretStatement(n);

            if( n.statement != "asdasd"
            ||  n.type != StatementType::other
            ||  n.labelToNextNode != -1 )
                throw "Test 1";
        }

        {
            Node n = {};
            n.statement = "stop";
            interpretStatement(n);

            if( n.statement != "stop"
            ||  n.type != StatementType::stop
            ||  n.labelToNextNode != -1 )
                throw "Test 2";
        }

        {
            Node n = {};
            n.statement = "goto1";
            interpretStatement(n);

            if( n.statement != "goto"
            ||  n.type != StatementType::unconditional
            ||  n.labelToNextNode != 1 )
                throw "Test 3";
        }

        {
            Node n = {};
            n.statement = "go_to1";
            interpretStatement(n);

            if( n.statement != "go_to1"
            ||  n.type != StatementType::other
            ||  n.labelToNextNode != -1 )
                throw "Test 4";
        }

        {
            Node n = {};
            n.statement = "if()goto1";
            interpretStatement(n);

            if( n.statement != "if()goto"
            ||  n.type != StatementType::conditional
            ||  n.labelToNextNode != 1 )
                throw "Test 5";
        }

        {
            Node n = {};
            n.statement = "if(goto)goto1";
            interpretStatement(n);

            if( n.statement != "if(goto)goto"
            ||  n.type != StatementType::conditional
            ||  n.labelToNextNode != 1 )
                throw "Test 6";
        }
    }
    catch( const char *msg )
    {
        std::cerr << "\ninterpretStatement not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in interpretStatement\n";
    }

}



void testParseLine()
{
    try{
        {
            std::string str = "      if(something)goto123";
            Line l = parseLine( str );

            if(
                l.n.statement != "if(something)goto"
            ||  l.n.type != StatementType::conditional
            ||  l.n.labelToNextNode != 123
            ||  l.lineLabel != -1
            ||  l.lineHasLabel != false
            ) throw "Test 1";
        }

        {
            std::string str = "      if ( something ) goto 123";
            Line l = parseLine( str );

            if(
                l.n.statement != "if(something)goto"
            ||  l.n.type != StatementType::conditional
            ||  l.n.labelToNextNode != 123
            ||  l.lineLabel != -1
            ||  l.lineHasLabel != false
            ) throw "Test 2";
        }

        {
            std::string str = "      if ( some t hing ) goto 123";
            Line l = parseLine( str );

            if(
                l.n.statement != "if(something)goto"
            ||  l.n.type != StatementType::conditional
            ||  l.n.labelToNextNode != 123
            ||  l.lineLabel != -1
            ||  l.lineHasLabel != false
            ) throw "Test 3";
        }

        {
            std::string str = "12345 if(something)goto123";
            Line l = parseLine( str );

            if(
                l.n.statement != "if(something)goto"
            ||  l.n.type != StatementType::conditional
            ||  l.n.labelToNextNode != 123
            ||  l.lineLabel != 12345
            ||  l.lineHasLabel != true
            ) throw "Test 4";
        }

        {
            std::string str = "99999 if( something )goto 123";
            Line l = parseLine( str );

            if(
                l.n.statement != "if(something)goto"
            ||  l.n.type != StatementType::conditional
            ||  l.n.labelToNextNode != 123
            ||  l.lineLabel != 99999
            ||  l.lineHasLabel != true
            ) throw "Test 5";
        }

        {
            std::string str = "12345$if(something)goto123";
            Line l = parseLine( str );

            if(
                l.n.statement != "if(something)goto"
            ||  l.n.type != StatementType::conditional
            ||  l.n.labelToNextNode != 123
            ||  l.lineLabel != 12345
            ||  l.lineHasLabel != true
            ) throw "Test 6";
        }

        {
            std::string str = "12345 stop";
            Line l = parseLine( str );

            if(
                l.n.statement != "stop"
            ||  l.n.type != StatementType::stop
            ||  l.n.labelToNextNode != -1
            ||  l.lineLabel != 12345
            ||  l.lineHasLabel != true
            ) throw "Test 7";
        }

        {
            std::string str = "      stop";
            Line l = parseLine( str );

            if(
                l.n.statement != "stop"
            ||  l.n.type != StatementType::stop
            ||  l.n.labelToNextNode != -1
            ||  l.lineLabel != -1
            ||  l.lineHasLabel != false
            ) throw "Test 8";
        }

        {
            std::string str = "12345 read 6, i, j, k";
            Line l = parseLine( str );

            if(
                l.n.statement != "read6,i,j,k"
            ||  l.n.type != StatementType::other
            ||  l.n.labelToNextNode != -1
            ||  l.lineLabel != 12345
            ||  l.lineHasLabel != true
            ) throw "Test 9";
        }

        {
            std::string str = "   33 i = j";
            Line l = parseLine( str );

            if(
                l.n.statement != "i=j"
            ||  l.n.type != StatementType::other
            ||  l.n.labelToNextNode != -1
            ||  l.lineLabel != 33
            ||  l.lineHasLabel != true
            ) throw "Test 10";
        }

        {
            std::string str = "   33 goto   1";
            Line l = parseLine( str );

            if(
                l.n.statement != "goto"
            ||  l.n.type != StatementType::unconditional
            ||  l.n.labelToNextNode != 1
            ||  l.lineLabel != 33
            ||  l.lineHasLabel != true
            ) throw "Test 11";
        }

        {
            // The continuation marker doesn't have to be a white space
            std::string str = "33   $goto 33";
            Line l = parseLine( str );

            if(
                l.n.statement != "goto"
            ||  l.n.type != StatementType::unconditional
            ||  l.n.labelToNextNode != 33
            ||  l.lineLabel != 33
            ||  l.lineHasLabel != true
            ) throw "Test 12";
        }
    }
    catch( const char *msg )
    {
        std::cerr << "\ntestParseLine not working as expected. Failed " << msg << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nUnknown error in testParseLine\n";
    }

}


int main()
{
    testEraseWhiteSpaces();
    testParseInt();
    testInterpretIfStatement();
    testInterpretGotoStatement();
    testInterpretStopStatement();
    testInterpretOtherStatement();
    testInterpretStatement();
    testParseLine();

    std::cout << "\n\nFinished unit tests\n\n";
    return 0;
}
