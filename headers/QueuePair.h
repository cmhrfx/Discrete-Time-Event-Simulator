#ifndef QUEUEPAIR_H
#define QUEUEPAIR_H

#include "ReadyQueue.h"
#include "Processor.h"

struct QueuePair {
    QueuePair(ReadyQueue* rq, Processor* prc);
    ~QueuePair();

    ReadyQueue* rq;
    Processor* prc;
    float active_count;
    float sample_queue;
};




#endif