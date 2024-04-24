#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include "Process.h"
#include "Event.h"
#include <cstdlib>
#include <list>
#include <iostream>
#include <iomanip>

extern const int LENGTH;

struct ProcessList {
    std::list<Process*> processes;

    ProcessList(float arrivalRate, float serviceTime);
    ProcessList();
    ~ProcessList();

    void listToConsole();
    Process* popProcess();
    bool isEmpty();
    void populateList(float arrivalRate, float serviceTime);
    float expRandom(float lambda);
    float rand0to1();
    
};



#endif