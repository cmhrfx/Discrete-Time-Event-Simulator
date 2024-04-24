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
            float arrivalRate = std::stof(argv[1]);
            float serviceTime = std::stof(argv[2]);
            int scenario = std::stoi(argv[3]);
            int processors = std::stoi(argv[4]);
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

    // check that either scenario 1 or 2 is selected
    if (*argv[3] != '1' && *argv[3] != '2')
    {
        flag = 4;
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
    else if (flag == 4)
        {cout << "Must select scenario 1 or 2. Please try again.\n";}
}

// handleArrival() is a core function of the Discrete Time Event Simulator engine.
// Called when current Event is an arrival.
// Based on the cpu_status, one of two paths will be taken:
// if cpu_status is 0, the new arrival event is converted immediately into a new 
// departure event. cpu_status is also flipped to 1. 
// if cpu_status is 1, the new arrival event is converted into a process placed
// in the ready queue.
// In both cases, the next arrival event is created from the ProcessList and added
// to the event queue. 
void handleArrivalS1(Event* event)
{
    int targetProcessor = -1;
    int currentProcessor = event->processorId;

    // don't handle "dummy" events
    if (currentProcessor != -1)
    {
        core.arrivals++;
        // Path 1- idle CPU
        if (core.processors[currentProcessor]->cpu_status == 0 )
        {
            core.processors[currentProcessor]->cpu_status = 1;
            float interval = event->process->serviceTime + core.time_piece;
            Event* newDeparture = new Event(event->process, interval, "departure", currentProcessor);
            core.eq.scheduleEvent(newDeparture);
        }
        // Path 2- busy CPU
        else {
            core.queuePairs[currentProcessor]->rq->addProcess(event->process);
        }
    }

    // Both paths end in the creation of a new arrival event from ProcessList
    Process* nextProcess = core.processList.popProcess();

    // Check that ProcessList isn't empty
    if (nextProcess == nullptr)
    {
        core.processes_empty = true;
    }
    else
    {
        // randomly pick the next arrival processor
        float queueDist = rand0to1();

        if (queueDist >= 0 && queueDist <= .25)
        {
            targetProcessor = 0;
        }
        else if (queueDist >= .25 && queueDist <= .50)
        {
            targetProcessor = 1;
        }
        else if (queueDist >= .5 && queueDist <= .75)
        {
            targetProcessor = 2;
        }
        else if (queueDist >= .75 && queueDist <= 1.00)
        {
            targetProcessor = 3;
        }

        Event* newArrival = new Event(nextProcess, nextProcess->arrivalTime, "arrival", targetProcessor);
        core.eq.scheduleEvent(newArrival);
    }

}


// handleDeparture() is a core function of the Discrete Time Event Simulator engine.
// Called when the current event is a departure. 
// Based on the ready_queue, one of two paths will be taken.
// If ready_queue is empty, cpu_status is set to 0. 
// If ready_queue contains a process, it is popped and a new departure event 
// is made of that process.

void handleDepartureS1(Event* event)
{
    core.departures++;
    int currentProcessor = event->processorId;

    if (core.queuePairs[currentProcessor]->rq->isEmpty())
    {
        core.queuePairs[currentProcessor]->prc->cpu_status = 0;
    }
    else
    {
        Process* currentProcess = core.queuePairs[currentProcessor]->rq->popFront();
        float interval = currentProcess->serviceTime + core.time_piece;
        Event* newDeparture = new Event (currentProcess, interval, "departure", event->processorId);
        core.eq.scheduleEvent(newDeparture);
    }
    
    core.turnarounds += event->time - event->process->arrivalTime;

}

// handlePoll() is key to quantifying metrics of the DTES.
// Upon instantiation, the Event Queue contains a first instance of a poll event.
// When this first instance is processed, handlePoll then schedules a following
// poll event. This process repeats until the Process Queue is empty.
// Variables under the global "core" struct are used to track these metrics.
void handlePollS1(Event* event)
{
    if (!core.processes_empty)
    {
        for (int i = 0; i < core.numProcessors; i++)
        {
            core.queuePairs[i]->sample_queue += core.queuePairs[i]->rq->size();
            if (core.queuePairs[i]->prc->cpu_status == 1)
            {
                core.queuePairs[i]->active_count++;
            }
        }
        core.sample_polls++;

        Event* nextPoll = new Event(core.pollProcess, 
        core.time_piece + core.polling_interval, "poll", -1);
        core.eq.scheduleEvent(nextPoll);
    }

}

void handleArrivalS2(Event* event)
{
    core.arrivals++;
    int targetProcessor = -1;
    int currentProcessor = event->processorId;

    // Path 1- idle CPU
    if (core.processors[currentProcessor]->cpu_status == 0)
    {
        core.processors[currentProcessor]->cpu_status = 1;
        float interval = event->process->serviceTime + core.time_piece;
        Event* newDeparture = new Event(event->process, interval, "departure", currentProcessor);
        core.eq.scheduleEvent(newDeparture);
    }
    // Path 2- busy CPU
    else {
        core.rq.addProcess(event->process);
    }

    // Both paths end in the creation of a new arrival event from ProcessList
    Process* nextProcess = core.processList.popProcess();

    // Check that ProcessList isn't empty
    if (nextProcess == nullptr)
    {
        core.processes_empty = true;
    }
    else
    {
        // randomly pick next core
        float queueDist = rand0to1();
        if (queueDist >= 0 && queueDist <= .25)
        {
            targetProcessor = 0;
        }
        else if (queueDist >= .25 && queueDist <= .50)
        {
            targetProcessor = 1;
        }
        else if (queueDist >= .5 && queueDist <= .75)
        {
            targetProcessor = 2;
        }
        else if (queueDist >= .75 && queueDist <= 1.00)
        {
            targetProcessor = 3;
        }

        Event* newArrival = new Event(nextProcess, nextProcess->arrivalTime, "arrival", targetProcessor);
        core.eq.scheduleEvent(newArrival);
    }

}


// handleDeparture() is a core function of the Discrete Time Event Simulator engine.
// Called when the current event is a departure. 
// Based on the ready_queue, one of two paths will be taken.
// If ready_queue is empty, cpu_status is set to 0. 
// If ready_queue contains a process, it is popped and a new departure event 
// is made of that process.

void handleDepartureS2(Event* event)
{
    core.departures++;
    int currentProcessor = event->processorId;

    if (core.rq.isEmpty())
    {
        core.cpu_status = 0;
    }
    else
    {
        Process* currentProcess = core.rq.popFront();
        float interval = currentProcess->serviceTime + core.time_piece;
        Event* newDeparture = new Event(currentProcess, interval, "departure", event->processorId);
        core.eq.scheduleEvent(newDeparture);
    }

    core.turnarounds += event->time - event->process->arrivalTime;

}

// handlePoll() is key to quantifying metrics of the DTES.
// Upon instantiation, the Event Queue contains a first instance of a poll event.
// When this first instance is processed, handlePoll then schedules a following
// poll event. This process repeats until the Process Queue is empty.
// Variables under the global "core" struct are used to track these metrics.
void handlePollS2(Event* event)
{
    if (!core.processes_empty)
    {
        core.sample_queue += core.rq.size();
        core.sample_polls++;

        for (int i = 0; i < core.numProcessors; i++)
        {
            if (core.queuePairs[i]->prc->cpu_status == 1)
            {
                core.queuePairs[i]->active_count++;
            }
        }

        Event* nextPoll = new Event(core.pollProcess, 
        core.time_piece + core.polling_interval, "poll", -1);
        core.eq.scheduleEvent(nextPoll);
    }

}

void outputMetrics()
{
    float avg_turnaround = core.turnarounds / LENGTH;
    float throughput = LENGTH / core.time_piece;
    float cpu = core.cpu_active_count / core.sample_polls;
    float avg_rq = core.sample_queue / core.sample_polls;
    std::map<int, float>cpu_utils;
    std::map<int, float>queue_lengths;

    for (int i = 0; i < core.numProcessors; i++)
    {
        cpu_utils[i] = core.queuePairs[i]->active_count / core.sample_polls;
    }

    for (int i = 0; i < core.numProcessors; i++)
    {
        queue_lengths[i] = core.queuePairs[i]->sample_queue / core.sample_polls;
    }

    cout << std::fixed << std::setprecision(5);
    cout << "-----------------------------------------\n";
    cout << "| Metric             | Value           |\n";
    cout << "-----------------------------------------\n";
    cout << "| Arrival Rate       | " << setw(15) << core.arrivalRate << " |\n";
    cout << "| Service Time       | " << setw(15) << core.serviceTime << " |\n";
    cout << "| Average Turnaround | " << setw(15) << avg_turnaround << " |\n";
    cout << "| Throughput         | " << setw(15) << throughput << " |\n";
    cout << "| CPU Utilization    : " << setw(15) << "|\n"; 
    for (int i = 0; i < core.numProcessors; i++)
    {
        cout << "| Core " << i << ": " << setw(15) << cpu_utils[i] << "% |\n";
    }
    cout << "| Average RQ Length  : " << setw(15) << " |\n";
    for (int i = 0; i < core.numProcessors; i++)
    {
        cout << "| Core " << i << ": " << setw(15) << " |\n";
    }
    cout << "-----------------------------------------\n";

}

float expRandom(float lambda)
{
    float random = rand() / (RAND_MAX + 1.0);
    return -log(1 - random) / lambda;
}

float rand0to1()
{
    return (rand() / (RAND_MAX + 1.0));
}