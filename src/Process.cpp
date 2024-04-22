#include "../headers/Process.h"

Process::Process(int id, float arrivalTime, float serviceTime)
{
    this->id = id;
    this->arrivalTime = arrivalTime;
    this->serviceTime = serviceTime;
};