#ifndef READYQUEUE_H
#define READYQUEUE_H

#include "Process.h"
#include <list>

struct ReadyQueue {
    ReadyQueue();
    void addProcess(Process* newProcess);
    Process* popFront();
    bool isEmpty();
    int size();
    std::list<Process*> queue; 

};




#endif