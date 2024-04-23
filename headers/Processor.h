#ifndef PROCESSOR_H
#define PROCESSOR_H

struct Processor {
    int cpu_status;
    float last_arrival;
    float last_departure;

    Processor();
};



#endif