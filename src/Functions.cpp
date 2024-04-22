#include "../headers/Functions.h"

int argChecks(int argc, char* argv[])
{
    int flag = 0;
    if (argc != 5)
    {
        // incorrect number of arguments
        flag = 1;
    }
    else
    {
        try {
            float intValue = std::stof(argv[1]);
            float floatValue = std::stof(argv[2]);
        } 
        // catch for type mismatch
        catch (const std::invalid_argument& ia)
        {
            flag = 2;
        }
        // catch for range mismatch
        catch (const std::out_of_range& oor)
        {
            flag = 3;
        }
    }
    argChecktoConsole(flag);
    return flag;
}

// function for console output from bad cmdline args
void argChecktoConsole(int flag)
{
    if (flag == 1)
        {cout << "Incorrect number of arguments. Please try again.\n";}
    else if (flag == 2)
        {cout << "Incorrect type of argument. Please try again.\n";}
    else if (flag == 3)
        {cout << "Argument out of range. Please try again.\n";}
}