#include "../headers/Functions.h"

// verify correct console arguments were provided
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
            std::cout << "arrivalRate: " << std::stof(argv[1]) << std::endl;
            std::cout << "serviceTime: " << std::stof(argv[2]) << std::endl;
            std::cout << "scenario: " << std::stoi(argv[3]) << std::endl;
            std::cout << "processors: " << std::stoi(argv[4]) << std::endl;

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

// handleArrivalS1() is a core function of the Discrete Time Event Simulator engine.
// In Scenario 1, each processor has it's own ready queue.
// Therefore, we look at the processor the arrival is headed for and queue if necessary.
// Departures are scheduled only after the process is placed onto a cpu.
// In all cases, the next process is pulled from the processList and an arrival event is
// placed onto the event queue. 
void handleArrivalS1(Event* event)
{
    int targetProcessor = -1;
    int currentProcessor = event->processorId;

    // don't handle "dummy" events, like 'initiation' arrival event
    if (currentProcessor != -1)
    {
        core.arrivals++;
        // Path 1- idle CPU
        if (core.queuePairs[currentProcessor]->prc->cpu_status == 0)
        {
            core.queuePairs[currentProcessor]->prc->cpu_status = 1;
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


// handleDepartureS1() is a core function of the Discrete Time Event Simulator engine.
// Called when the current event is a departure. 
// One of two paths will be taken, depending on whether the processor from which the departure
// occurs has an occupancy in its ready queue. 
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

// handlePollS1() is key to quantifying metrics of the DTES.
// Upon instantiation, the Event Queue contains a first instance of a poll event.
// When this first instance is processed, handlePoll then schedules a following
// poll event. This process repeats until the Process Queue is empty.
// In handlePollS1(), the length of each readyQueue is checked an the cumulative
// sum of lengths for a given QueuePair is stored within the QueuePair object. 
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

// handleArrivalS2() is in many ways similar to its S1 counterpart, but bears the largest
// distinction in the S1 vs S2 implementations.
// In contrast with handleArrivalS1, handleArrivalS2 does not randomly associate new processes
// with a given processor. Instead, it seeks for an idle processor to assign the new arrival to.
// If all processors are busy, the new arrival is placed in a central ready queue. 
void handleArrivalS2(Event* event)
{
    int targetProcessor = -1;
    int currentProcessor = event->processorId;

    // Path 1- idle CPU
    if (currentProcessor != -1)
    {
        core.arrivals++;
        // check if there are occupants in the ready queue
        if (core.rq.isEmpty())
        {
            int openProcessor = -1;
            int count = 0;
            int rand_shift = (rand() % 4);
            // seek for an idle processor
            while (openProcessor == -1 && count < core.numProcessors)
            {
                rand_shift = rand_shift%core.numProcessors;
                if (core.queuePairs[rand_shift]->prc->cpu_status == 0)
                {
                    openProcessor = rand_shift;
                    core.queuePairs[rand_shift]->prc->cpu_status = 1;
                    float interval = event->process->serviceTime + core.time_piece;
                    Event* newDeparture = new Event(event->process, interval, "departure", rand_shift);
                    core.eq.scheduleEvent(newDeparture);
                }
                count++;
                rand_shift++;
            }
            // if none are found, push to the back of the ready queue
            if (openProcessor == -1)
            {
                core.rq.addProcess(event->process);
            }
        }
        // if ready queue already has occupants, place process in rq
        else
        {
            core.rq.addProcess(event->process);
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
        // temporary processor assignment of '-2' until process really arrives
        Event* newArrival = new Event(nextProcess, nextProcess->arrivalTime, "arrival", -2);
        core.eq.scheduleEvent(newArrival);
    }

}


// handleDepartureS2() is very similar to its S1 counterpart
// however, because S2 has a central ready queue, we must check that
// instead of the ready queue of a given processor
void handleDepartureS2(Event* event)
{
    core.departures++;
    int currentProcessor = event->processorId;

    if (core.rq.isEmpty())
    {
        core.queuePairs[currentProcessor]->prc->cpu_status = 0;
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

// handlePollS2() is similar to its S1 counterpart.
// However, it checks the global ready queue's length instead of
// the ready queue for each processor. 
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

// output metrics to console for multiple cores and multiple queues
void outputMetricsS1()
{
    float avg_turnaround = core.turnarounds / LENGTH;
    float throughput = LENGTH / core.time_piece;
    float avg_rq = core.sample_queue / core.sample_polls;
    std::map<int, float>cpu_utils;
    std::map<int, float>queue_lengths;

    for (int i = 0; i < core.numProcessors; i++)
    {
        cpu_utils[i] = (core.queuePairs[i]->active_count / core.sample_polls) * 100;
    }

    for (int i = 0; i < core.numProcessors; i++)
    {
        queue_lengths[i] = core.queuePairs[i]->sample_queue / core.sample_polls;
    }

    cout << std::fixed << std::setprecision(5);
    cout << "---------------------------------------------\n";
    cout << "| Metric                  | Value           |\n";
    cout << "---------------------------------------------\n";
    cout << "| Arrival Rate            | " << setw(15) << core.arrivalRate << " |\n";
    cout << "| Service Time            | " << setw(15) << core.serviceTime << " |\n";
    cout << "| Total Run Time:         | " << setw(15) << core.time_piece << " |\n";
    cout << "| Average Turnaround      | " << setw(15) << avg_turnaround << " |\n";
    cout << "| Throughput (prc/second) | " << setw(15) << throughput << " |\n";
    cout << "| CPU Utilization              " << setw(15) << "|\n"; 
    for (int i = 0; i < core.numProcessors; i++)
    {
        cout << "| Core " << i << ":                  " << setw(15) << cpu_utils[i] << "% |\n";
    }
    cout << "| Average RQ Length            " << setw(15) << " |\n";
    for (int i = 0; i < core.numProcessors; i++)
    {
        cout << "| Core " << i << ":                   " << setw(15) << queue_lengths[i] << " |\n";
    }
    cout << "---------------------------------------------\n";

}

// output metrics to console for multiple cores and a single ready queue
void outputMetricsS2()
{
    float avg_turnaround = core.turnarounds / LENGTH;
    float throughput = LENGTH / core.time_piece;
    float avg_rq = core.sample_queue / core.sample_polls;
    std::map<int, float>cpu_utils;

    for (int i = 0; i < core.numProcessors; i++)
    {
        cpu_utils[i] = (core.queuePairs[i]->active_count / core.sample_polls) * 100;
    }

    cout << std::fixed << std::setprecision(5);
    cout << "---------------------------------------------\n";
    cout << "| Metric                  | Value           |\n";
    cout << "---------------------------------------------\n";
    cout << "| Arrival Rate            | " << setw(15) << core.arrivalRate << " |\n";
    cout << "| Service Time            | " << setw(15) << core.serviceTime << " |\n";
    cout << "| Total Run Time:         | " << setw(15) << core.time_piece << " |\n";
    cout << "| Average Turnaround      | " << setw(15) << avg_turnaround << " |\n";
    cout << "| Throughput (prc/second) | " << setw(15) << throughput << " |\n";
    cout << "| CPU Utilization              " << setw(15) << "|\n"; 
    for (int i = 0; i < core.numProcessors; i++)
    {
        cout << "| Core " << i << ":                  " << setw(15) << cpu_utils[i] << "% |\n";
    }
    cout << "| Average RQ Length       | " << setw(15) << avg_rq << " |\n";
    cout << "---------------------------------------------\n";

}

// output metrics to file for multiple cores and multiple queues
void logMetricsS1(string path)
{
    cout << "log path: " << path << endl;
    std::ofstream outFile(path, std::ios::app);
    if (!outFile) {
        std::cerr << "Error opening log file path." << std::endl;
    }
    else
    {
        outFile << core.arrivalRate << "," << core.serviceTime << "," << (core.turnarounds / LENGTH)
        << "," << (LENGTH / core.time_piece) << "," 
        << core.queuePairs[0]->active_count / core.sample_polls << ","
        << core.queuePairs[1]->active_count / core.sample_polls << ","
        << core.queuePairs[2]->active_count / core.sample_polls << ","
        << core.queuePairs[3]->active_count / core.sample_polls << ","
        << core.queuePairs[0]->sample_queue / core.sample_polls << ","
        << core.queuePairs[1]->sample_queue / core.sample_polls << ","
        << core.queuePairs[2]->sample_queue / core.sample_polls << ","
        << core.queuePairs[3]->sample_queue / core.sample_polls << endl;
        outFile.close();
        cout << "Results written to logs" << endl;
    }
}

// output metrics to log for multiple cores and a single ready queue
void logMetricsS2(string path)
{
    cout << "log path: " << path << endl;
    std::ofstream outFile(path, std::ios::app);
    if (!outFile) {
        std::cerr << "Error opening log file path." << std::endl;
    }
    else
    {
        outFile << core.arrivalRate << "," << core.serviceTime << "," << (core.turnarounds / LENGTH)
        << "," << (LENGTH / core.time_piece) << "," 
        << core.queuePairs[0]->active_count / core.sample_polls << ","
        << core.queuePairs[1]->active_count / core.sample_polls << ","
        << core.queuePairs[2]->active_count / core.sample_polls << ","
        << core.queuePairs[3]->active_count / core.sample_polls << ","
        << (core.sample_queue / core.sample_polls) << endl;
        outFile.close();
        cout << "Results written to logs" << endl;
    }
}

// lambda is the rate of instances over a fixed length of time.
// Thus, for arrivals, we can use the flat value (e.g. lambda = 10)
// However, for service times, we are given the unit of 0.04s service time.
// By dividing 1/ avg service time, we can transform the service time to a
// poisson lambda (ie a rate). 
float expRandom(float lambda)
{
    float random = rand() / (RAND_MAX + 1.0);
    return -log(1 - random) / lambda;
}

// generate a random number between 0 and 1
// primarily used to decide which processor and ready queue receive
// a new process in S1. 
float rand0to1()
{
    return (rand() / (RAND_MAX + 1.0));
}