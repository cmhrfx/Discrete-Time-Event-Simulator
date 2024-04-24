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

int argChecks(int argc, char *argv[]);      // check cmdline args
void argChecktoConsole(int flag);           // output cmdline arg errors
void handleArrivalS1(Event* event);           // logic sequence for arrival events
void handleDepartureS1(Event* event);         // logic sequence for departure events
void handlePollS1(Event* event);              // logic sequence for poll events
void handleArrivalS2(Event* event);           // logic sequence for arrival events
void handleDepartureS2(Event* event);         // logic sequence for departure events
void handlePollS2(Event* event);              // logic sequence for poll events
void outputMetricsS1();
void outputMetricsS2();
void logMetricsS1(string path);
void logMetricsS2(string path);
float rand0to1();
float expRandom(float lambda);





#endif