/*
Christopher Hanly
CS.3360.253 Computing Systems Fundamentals
Homework 5
Professor: Mina Guirguis
04/22/2024

In Homework 5, we expand on Homework 4's Discrete Time Event Simulator.
Now we present a Discrete Time Event Simulator that offers two designs:
1. The "Grocery Store" design that uses an individual ready queue for each processor
2. The "Airport" design that uses a central ready queue for all processors

Based on the console arguments provided, one of either scenario will be ran by the program.
Once complete, metrics will be displayed to the console and exported to either "cmh171s1.txt"
or "cmh171s2.txt", depending on the scenario selected.

Furthermore, certain parts of this implementation are designed to handle a variable number of processors.
However, it appears that fully implementing "n" processors is extraneous to design specifications.
Thus, the design is implemented specifically for 4 processors and will exhibit problems if any other
number of processors is used. 

Finally, the repo is complete with multirun1.sh and multirun2.sh which have been written to support
iterating through the requested console argument sets. They will write to the aforementioned .txt files.
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


    // CORE FUNCTIONAL LOOP
    while (!core.eq.events.empty())
    {
        Event* event = core.eq.getEvent();
        core.time_piece = event->time;

        if (core.scenario == 1)
        {
            // S1 BRANCH
            if (event->type == "arrival")
                {handleArrivalS1(event);}
            else if (event->type == "departure")
                {handleDepartureS1(event);}
            else
                {handlePollS1(event);}
        } else {

            // S2 BRANCH
            if (event->type == "arrival")
                {handleArrivalS2(event);}
            else if (event->type == "departure")
                {handleDepartureS2(event);}
            else
                {handlePollS2(event);}
        }
    }

    // OUTPUT TO CONSOLE AND LOG
    if (core.scenario == 1)
    {
        outputMetricsS1();
        logMetricsS1("cmh171s1.txt");
    } else {
        outputMetricsS2();
        logMetricsS2("cmh171s2.txt");
    }
    
    return 0;

}