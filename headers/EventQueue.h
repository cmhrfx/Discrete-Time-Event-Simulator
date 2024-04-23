#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "Event.h"
#include <list>

struct EventQueue {
    EventQueue();
    ~EventQueue();
    Event* getEvent();
    void scheduleEvent(Event* newEvent);
    std::list<Event*> events;

};


#endif