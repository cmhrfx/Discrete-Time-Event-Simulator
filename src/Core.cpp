#include "../headers/Core.h"

Core::Core() {
    // essential properties
    float time_piece = 0;
    float turnarounds = 0;
    float arrivalRate = 0;
    float serviceTime = 0;
    int scenario = 0;
    int numProcessors = 0;
    int arrivals = 0;
    int departures = 0;
    bool processes_empty = false;
    bool events_empty = false;
    
    ProcessList processes;
    EventQueue eq;

    // polling
    float sample_queue = 0;
    float sample_polls = 0;
    float polling_interval = 0.1;
    Process* pollProcess;

    // scenario 1 specific properties
    std::map<int, QueuePair*> queuePairs;

    // scenario 2 specific properties
    float cpu_active_count = 0;
    int cpu_status = 0;
    ReadyQueue rq;
    std::map<int, Processor*> processors;

};

void Core::initialize()
{
    // populate process list
    processes.populateList(arrivalRate, serviceTime);

    // handle scenario 1 instantiations
    if (scenario == 1)
    {
        for (int i = 0; i < numProcessors; i++)
        {
            Processor* prc = new Processor();
            ReadyQueue* rq = new ReadyQueue();
            QueuePair* qp = new QueuePair(rq, prc);
            queuePairs[i] = qp;
        }
    }
    else {
        for (int i = 0; i < numProcessors; i++)
        {
            Processor* prc = new Processor();
            processors[i] = prc;
        }

    }
}