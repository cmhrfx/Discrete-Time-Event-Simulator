#include "../headers/ProcessList.h"

ProcessList::ProcessList(float arrivalRate, float serviceTime)
{
    populateList(arrivalRate, serviceTime);
}

ProcessList::ProcessList()
{
    populateList(0,0);
}

void ProcessList::populateList(float arrivalRate, float serviceTime)
{
    if (!this->processes.empty()){
        this->processes.clear();
    }
    
    // don't really populate list on default construction
    if (arrivalRate != 0 && serviceTime != 0)
    {
        float generationTime = 0;
        for (int i = 0; i < LENGTH; i++)
        {
            float interArrivalTime = expRandom(arrivalRate);
            generationTime += interArrivalTime;
            Process* newProcess = new Process(i + 1, generationTime, expRandom(1/serviceTime));
            processes.push_back(newProcess);
        }
    }
}


void ProcessList::listToConsole()
{
    std::cout   << std::left << std::setw(12) << "Process ID"
                << std::setw(15) << "Arrival Time"
                << std::setw(15) << "Service Time"
                << std::endl;
    std::cout   << std::string(42, '-') << std::endl;

    for (const auto& process : processes)
    {
        std::cout << std::left << std::setw(12) << process->id
                  << std::setw(15) << process->arrivalTime
                  << std::setw(15) << process->serviceTime
                  << std::endl;
    }
}

// popProcess() cleanly returns the front element of a ProcessList and 
// then removes it from that list. 
Process* ProcessList::popProcess()
{
    Process* process = nullptr;
    if (!processes.empty())
    {
        process = processes.front();
        processes.pop_front();
    }
    return process;
}

// Return if the process list is empty 
// originally made when 'processes' property was private
bool ProcessList::isEmpty()
{
    return processes.empty();
}

// Default Destructor
ProcessList::~ProcessList()
{
    for (Process* process : processes) {
        delete process;
    }
    processes.clear();
}

float ProcessList::expRandom(float lambda)
{
    float random = rand() / (RAND_MAX + 1.0);
    return -log(1 - random) / lambda;
}

float ProcessList::rand0to1()
{
    return (rand() / (RAND_MAX + 1.0));
}