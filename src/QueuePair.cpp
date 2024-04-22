#include "../headers/QueuePair.h"
#include "../headers/Processor.h"
#include "../headers/ReadyQueue.h"

struct QueuePair {
    Processor processor;
    ReadyQueue readyqueue;
}