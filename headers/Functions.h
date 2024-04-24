#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <math.h>
#include <map>
#include "Event.h"
#include "Core.h"

using std::string;
using std::cout;
using std::endl;
using std::setw;

extern Core core;

int argChecks(int argc, char *argv[]);          // check cmdline args
void argChecktoConsole(int flag);               // output cmdline arg errors
void handleArrivalS1(Event* event);             // logic sequence for S1 arrival events
void handleDepartureS1(Event* event);           // logic sequence for S1 departure events
void handlePollS1(Event* event);                // logic sequence for S1 poll events
void handleArrivalS2(Event* event);             // logic sequence for S2 arrival events
void handleDepartureS2(Event* event);           // logic sequence for S2 departure events
void handlePollS2(Event* event);                // logic sequence for S2 poll events
void outputMetricsS1();                         // output metrics for S1 to console
void outputMetricsS2();                         // output metrics for S2 to console
void logMetricsS1(string path);                 // log metrics for S1 to text file
void logMetricsS2(string path);                 // log metrics for S2 to text file
float rand0to1();                               // generate random float from 0 to 1
float expRandom(float lambda);                  // generate exponential random

#endif