#include "../headers/EventQueue.h"

EventQueue::EventQueue()
{
    Process* initiationProcess = new Process(0,0,0);
    Event* initiationEvent = new Event(initiationProcess,0,"arrival", -1);
    Process* initialPollProcess = new Process(-1,0,0);
    Event* initiationPoll = new Event(initialPollProcess,0.01,"poll", -1);
    events.push_back(initiationEvent);
    events.push_back(initiationPoll);

};


// pop front event from EventQueue
Event* EventQueue::getEvent()
{
    Event* event = nullptr;
    if (!events.empty())
    {
        event = events.front();
        events.pop_front();
    }
    return event;
}

// Insert event to EventQueue based on time
// Iterates through event queue until time of event is greater
// than time of previous event. Inserts at that location.
void EventQueue::scheduleEvent(Event* newEvent)
{
    if (events.empty() || newEvent->time < events.front()->time)
    {
        events.push_front(newEvent);
    } else {
        std::list<Event*>::iterator it = events.begin();

        while (it != events.end() && newEvent->time >= (*it)->time)
        {
            ++it;
        }
        events.insert(it, newEvent);
    }
}

EventQueue::~EventQueue()
{
    for (auto& event : events)
    {
        delete event;
    }
}