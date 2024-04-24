#ifndef QUEUEPAIR_H
#define QUEUEPAIR_H

#include "ReadyQueue.h"
#include "Processor.h"

struct QueuePair {
    QueuePair(ReadyQueue* rq, Processor* prc);
    ~QueuePair();

    ReadyQueue* rq;
    Processor* prc;
    int active_count;
    int sample_queue;
};




#endif