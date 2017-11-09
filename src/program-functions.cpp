
#include "program-functions.hpp"

  Program readProgram(std::ifstream &is) {

    

  }
  
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
