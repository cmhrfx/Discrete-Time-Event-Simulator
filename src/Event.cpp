#include "../headers/Event.h"

Event::Event(Process* process, float time, string type)
{
    this->process = process;
    this->time = time;
    this->type = type;
};

Event::Event()
{
    this->process = nullptr;
    this->time = 0;
    this->type = "";
}