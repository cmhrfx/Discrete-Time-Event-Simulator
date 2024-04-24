#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    Process(int id, float arrivalTime, float serviceTime);
    
    int id;                     // every process has a uniquely-assigned id
    float arrivalTime;          // time at which processor arrived to system
    float serviceTime;          // time process needs to spend on a cpu

};


#endif