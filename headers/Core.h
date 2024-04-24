#ifndef CORE_H
#define CORE_H

#include "EventQueue.h"
#include "ProcessList.h"
#include "QueuePair.h"
#include <map>

struct Core {
    // essential properties
    float time_piece;           // clock
    float turnarounds;          // cumulative time all processes take from arrival to departure
    float arrivalRate;          // console arg rate of arrivals
    float serviceTime;          // console arg avg time to complete process
    int scenario;               // console arg scenario 1 or 2 (1: multiple rq, 2: single rq)
    int numProcessors;          // console arg number of processors
    int arrivals;               // for debugging, keep track of arrival events
    int departures;             // for debugging, keep track of departure events
    bool processes_empty;       // so we can tell our poll handling to stop making new polls
    
    ProcessList processList;    // holds the 10,000 processes instantiated at beginning of program
    EventQueue eq;              // global event queue to handle arrivals, departures, and polls
    ReadyQueue rq;              // global ready queue used in scenario 2

    // polling
    float sample_queue;         // cumulative number of s2 ready queue members counted in polls
    float sample_polls;         // total number of polls taken
    float polling_interval;     // interval between polls
    Process* pollProcess;       // dummy process to be used for poll events

    // scenario 1 specific properties
    std::map<int, QueuePair*> queuePairs;   // map of readyQueue->Processor pairs for scenario 1

    Core();
    ~Core();

    void initialize();          // used to populate processList and QueuePair data structs
                                // after console args have been provided

};





#endif