#include "../headers/ReadyQueue.h"

ReadyQueue::ReadyQueue()
{};

// add Process. Made when 'queue' property was private.
void ReadyQueue::addProcess(Process* newProcess)
{
    queue.push_back(newProcess);
}

// pop Front().
// Returns front-most process of ProcessList. 
// Removes front-most process from ProcessList. 
// Cleaner to have this function than to use two lines in other functions
// (eg. one for assignment and one for pop)
Process* ReadyQueue::popFront()
{
    Process* poppedProcess = queue.front();
    queue.pop_front();
    return poppedProcess;
}

// Is Empty()
// Originally made when queue property was private
bool ReadyQueue::isEmpty()
{
    return queue.empty();
}

// size()
// Originally made when queue property was private
int ReadyQueue::size()
{
    return queue.size();
}