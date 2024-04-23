#include "../headers/QueuePair.h"
#include "../headers/Processor.h"
#include "../headers/ReadyQueue.h"

QueuePair::QueuePair(ReadyQueue* rq, Processor* prc) {
    this->rq = rq;
    this->prc = prc;
    active_count = 0;
}