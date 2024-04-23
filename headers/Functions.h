#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <math.h>
#include "Event.h"
#include "Core.h"

using std::string;
using std::cout;
using std::endl;
using std::setw;

extern Core core;

int argChecks(int argc, char *argv[]);      // check cmdline args
void argChecktoConsole(int flag);           // output cmdline arg errors
float expRandom(float lambda);
float rand0to1();
void handleArrival(Event* event);           // logic sequence for arrival events
void handleDeparture(Event* event);         // logic sequence for departure events
void handlePoll(Event* event);              // logic sequence for poll events






#endif