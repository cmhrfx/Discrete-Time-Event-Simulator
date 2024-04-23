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
    // don't really populate list on default construction
    if (arrivalRate != 0 && serviceTime != 0)
    {
        float generationTime = 0;
        for (int i = 0; i < LENGTH; i++)
        {
            float interArrivalTime = genExponentialRandom(arrivalRate);
            generationTime += interArrivalTime;
            Process* newProcess = new Process(i + 1, generationTime, genExponentialRandom(1/serviceTime));
            processes.push_back(newProcess);
        }
    }
}


