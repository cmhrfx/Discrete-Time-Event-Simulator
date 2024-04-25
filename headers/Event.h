#ifndef EVENT_H
#define EVENT_H

#include "Process.h"
#include <string>

using std::string;

struct Event {
    Event(Process* process, float time, std::string type, int processorId);
    Event();

    Process* process;               // every event has an associated process
    float time;                     // time event occurs
    std::string type;               // type of event (arrival, departure, or poll)
    int processorId;                // id of processor on which the event is occurring


};


#endif