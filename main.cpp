/*
Christopher Hanly
CS.3360.253 Computing Systems Fundamentals
Professor: Mina Guirguis
04/22/2024
*/

#include "main.h"

// Globals
Core core;                  // struct for global variables
const int LENGTH = 10000;   // number of processes to generate


int main(int argc, char *argv[])
{

    // It's generally not good practice to place a separate return statement, but
    // I believe this is acceptable when we're doing an early arg check.
    // This way, I don't need to nest the entire driver logic under 
    // "if (argFailure != 0)"

    int flag = argChecks(argc, argv);   // check for correct number of args
    if (flag > 0)
    {
        return 1;                   // Ends program early if a bad argument occurred.
    }

    // main driver logic //

    // assign console arguments to variables
    core.arrivalRate = std::stof(argv[1]);
    core.serviceTime = std::stof(argv[2]);
    core.scenario = std::stoi(argv[3]);
    core.numProcessors = std::stoi(argv[4]);

    // seed rand
    srand(time(NULL));

    // initialize core
    // this creates the processList and handles scenario 1 vs scenario 2
    // core variables
    core.initialize();

    // test processList population
    core.processes.listToConsole();

    while (!core.eq.events.empty())
    {
        Event* event = core.eq.getEvent();
        core.time_piece = event->time;

        if (core.scenario == 1)
        {
            if (event->type == "arrival")
                {handleArrivalS1(event);}
            else if (event->type == "departure")
                {handleDepartureS1(event);}
            else
                {handlePollS1(event);}
        } else {
            if (event->type == "arrival")
                {handleArrivalS2(event);}
            else if (event->type == "departure")
                {handleDepartureS2(event);}
            else
                {handlePollS2(event);}

        }
    }

    outputMetrics();


    
    return 0;

}