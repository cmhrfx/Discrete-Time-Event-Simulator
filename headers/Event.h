#ifndef EVENT_H
#define EVENT_H

#include "Process.h"
#include <string>

using std::string;

struct Event {
    Event(Process* process, float time, std::string type);
    Event();

    Process* process;
    float time;
    std::string type;


};


#endif