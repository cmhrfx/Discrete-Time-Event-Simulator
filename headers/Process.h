#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    Process(int id, float arrivalTime, float serviceTime);
    
    int id;
    float arrivalTime;
    float serviceTime;

};


#endif