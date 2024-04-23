#ifndef CORE_H
#define CORE_H

#include "EventQueue.h"
#include "ProcessList.h"
#include "QueuePair.h"
#include <map>

struct Core {
    // essential properties
    float time_piece;
    float turnarounds;
    float arrivalRate;
    float serviceTime;
    int scenario;
    int numProcessors;
    int arrivals;
    int departures;
    bool processes_empty;
    bool events_empty;
    
    ProcessList processes;
    EventQueue eq;

    // polling
    float sample_queue;
    float sample_polls;
    float polling_interval;
    Process* pollProcess;

    // scenario 1 specific properties
    std::map<int, QueuePair*> queuePairs;

    // scenario 2 specific properties
    float cpu_active_count;
    int cpu_status;
    ReadyQueue rq;
    std::map<int, Processor*> processors;

    // initialize core vars based on scenario
    Core();
    void initialize();

};





#endif