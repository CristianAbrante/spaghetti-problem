#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <stack>

enum StatementType {
    conditional,
    unconditional,
    stop,
    other
};

struct Node{
    std::string statement;
    StatementType type;
    int labelToNextNode;
};

struct Line {
    Node n;
    bool lineHasLabel;
    int lineLabel;
};


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

struct Program {

  std::vector<Line> program_lines;

  // Hash that gets the label of the node
  // as the key and return this node position
  // in the program_line vector.
  std::map<int, int> labels_equivalence;
};


void loopChecker(Program program, int& currentPLine)
{
    int auxiliar = currentPLine;

    std::vector<bool> visited( program.program_lines.size(), false);

    bool unconditionalGoto = true;

    //Until a non unconditional goto statement is reached or an already visited
    //unconditional goto statement is reached, unconditional goto statements are ignored
    while(unconditionalGoto && !visited[currentPLine])
    {
        visited[currentPLine] = true;
        currentPLine = program.labels_equivalence[program.program_lines[currentPLine].n.labelToNextNode];

        if(program.program_lines[currentPLine].n.type != unconditional)
            unconditionalGoto = false;
    }

    //If an already visited unconditional goto statement was reached, the function returns
    //the same currentPLine value
    if(unconditionalGoto)
        currentPLine = auxiliar;

    //If not, currentPLine will contain the position of a non unconditional goto statement

}



bool comparePrograms(Program firstProgram, Program secondProgram)
{
    //This vectors will indicate if a specific node have been visited or not
    std::vector<bool> firstProgramVisited( firstProgram.program_lines.size(), false), secondProgramVisited( secondProgram.program_lines.size(), false);

    //These stacks will contain the positions(within the vectors of nodes of each program) of the first node of each bifurcation of a program
    std::stack<int> firstProgramBifurcations, secondProgramBifurcations;

    //The first nodes to be evaluated are the first ones of the vector of nodes of each program
    int currentFPLine = 0, currentSPLine = 0;

    //At the end of the algorithm, this variable will indicate if the programs are equivalent or not
    bool equivalents = false;

    //This variable indicates when the algorithm must stop
    bool stopIterating = false;

    //This variable will indicate if the bifurcations must be explored
    bool exploreBifurcations = false;

    while(!stopIterating)
    {
        //The algorithm must check if there is some unconditional goto statements loop
        if(firstProgram.program_lines[currentFPLine].n.type == unconditional)
            loopChecker(firstProgram, currentFPLine);

        if(secondProgram.program_lines[currentSPLine].n.type == unconditional)
            loopChecker(secondProgram, currentSPLine);


        if(firstProgram.program_lines[currentFPLine].n.type == secondProgram.program_lines[currentSPLine].n.type)     //If the nodes are of the same type
        {
            if(firstProgram.program_lines[currentFPLine].n.statement == secondProgram.program_lines[currentSPLine].n.statement)      //And they contain the same statement
            {
                if( !firstProgramVisited[currentFPLine] || !secondProgramVisited[currentSPLine])     //And some of them has not been visited
                {
                    switch(firstProgram.program_lines[currentFPLine].n.type)            //The algorithm checks the statement type to act in consequence
                    {
                        case conditional:           //The algorithm adds the new bifurcations and continues evaluating the programs
                            firstProgramVisited[currentFPLine] = true;
                            secondProgramVisited[currentSPLine] = true;

                            firstProgramBifurcations.push(currentFPLine + 1);
                            secondProgramBifurcations.push(currentSPLine + 1);

                            currentFPLine = firstProgram.labels_equivalence[firstProgram.program_lines[currentFPLine].n.labelToNextNode];
                            currentSPLine = secondProgram.labels_equivalence[secondProgram.program_lines[currentSPLine].n.labelToNextNode];
                            break;

                        case unconditional:         //Both programs are in an unconditional goto statements infinite loop, the algorithm must evaluate the existing bifurcations
                            exploreBifurcations = true;
                            firstProgramVisited[currentFPLine] = true;
                            secondProgramVisited[currentSPLine] = true;
                            break;

                        case stop:                      //The programs are equivalents, the algorithm must evaluate if there is some bifurcation to be explored
                            exploreBifurcations = true;
                            firstProgramVisited[currentFPLine] = true;
                            secondProgramVisited[currentSPLine] = true;
                            break;

                        case other:                     //Same statements, the node is marked as visited and the algorithm evaluates the next nodes
                            firstProgramVisited[currentFPLine] = true;
                            secondProgramVisited[currentSPLine] = true;

                            currentFPLine++;
                            currentSPLine++;
                            break;
                    }
                }else//If both nodes are already visited the programs are equivalent, the algorithm must evaluate if there is some bifurcation to be explored
                {
                    exploreBifurcations = true;
                }
            }else//If the nodes don't contain the same statement, the programs are not equivalent
            {
                stopIterating = true;
            }
        }else//If the nodes are not of the same type, the programs are not equivalent
        {
            stopIterating = true;
        }


        if(exploreBifurcations)
        {
            exploreBifurcations = false;

            if(firstProgramBifurcations.empty() && secondProgramBifurcations.empty())   //There isn't bifurcations to be explored, the programs are equivalent!
            {
                equivalents = true;
                stopIterating = true;
            }
            else//Some bifurcation must be explored
            {
                if(!firstProgramBifurcations.empty() && !secondProgramBifurcations.empty())     //Both programs have some bifurcation that must be explored
                {
                    currentFPLine = firstProgramBifurcations.top();
                    firstProgramBifurcations.pop();

                    currentSPLine = secondProgramBifurcations.top();
                    secondProgramBifurcations.pop();
                }
                else        //Only one program have some bifurcation that must be explored, the programs are not equivalent
                {
                    stopIterating = true;
                }
            }
        }


    }


    return equivalents;
}


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

    if (comparePrograms(programs[0], programs[1])) {
      std::cout << "The programs are equivalent." << '\n';
    } else {
      std::cout << "The programs are not equivalent." << '\n';
    }

  }

  return 0;
}
