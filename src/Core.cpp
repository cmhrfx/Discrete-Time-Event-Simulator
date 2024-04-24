#include "../headers/Core.h"

Core::Core() {
    // essential properties
    time_piece = 0;
    turnarounds = 0;
    arrivalRate = 0;
    serviceTime = 0;
    scenario = 0;
    numProcessors = 0;
    arrivals = 0;
    departures = 0;
    processes_empty = false;
    events_empty = false;

    // polling
    sample_queue = 0;
    sample_polls = 0;
    polling_interval = 0.1;
    Process* pollProcess = new Process(-1,0,0);

    // scenario 2 specific
    cpu_active_count = 0;
    cpu_status = 0;

};

void Core::initialize()
{
    // populate process list
    processList.populateList(arrivalRate, serviceTime);

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

Core::~Core()
{
    queuePairs.clear();
    processors.clear();
    delete pollProcess;
}