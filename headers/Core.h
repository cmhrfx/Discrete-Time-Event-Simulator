#ifndef CORE_H
#define CORE_H

#include "EventQueue.h"
#include "ProcessList.h"
#include "QueuePair.h"
#include <list>

struct Core {
    // essential properties
    float time_piece;
    float turnarounds;
    int arrivals;
    int departures;
    bool processes_empty;
    bool events_empty;
    int scenario;
    ProcessList processes;
    EventQueue eq;

    // polling
    float sample_queue;
    float sample_polls;
    float polling_interval;
    Process* pollProcess;

    // scenario 1 specific properties
    float cpu_active_count;
    int cpu_status;
    ReadyQueue rq;

    // scenario 2 specific properties
    std::list<QueuePair> queues;





}





#endif