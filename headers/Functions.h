#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <math.h>
#include "Core.h"

using std::string;
using std::cout;
using std::endl;
using std::setw;

extern Core core;   // using a global struct helps keep functions short and clean

int argChecks(int argc, char *argv[]);      // check cmdline args
void argChecktoConsole(int flag);           // output cmdline arg errors
float expRandom(float lambda);
float rand0to1();







#endif