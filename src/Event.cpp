#include "../headers/Event.h"

Event::Event(Process* process, float time, string type, int processorId)
{
    this->process = process;
    this->time = time;
    this->type = type;
    this->processorId = processorId;
};

Event::Event()
{
    this->process = nullptr;
    this->time = 0;
    this->type = "";
    this->processorId = -1;
}