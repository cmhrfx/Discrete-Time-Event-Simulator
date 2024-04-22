#ifndef QUEUEPAIR_H
#define QUEUEPAIR_H

#include "ReadyQueue.h"
#include "Processor.h"

struct QueuePair {
    QueuePair(ReadyQueue rq, Processor prc);

    ReadyQueue rq;
    Processor prc;
};




#endif